#include "CBack.h"
#include <iostream.h>
#include <setjmp.h>

void (*Fiasco) () = 0;
int BreadthFirst = 0;

static void Error(const char *Message) {
    cerr << "Error: " << Message << endl; 
    exit(0);
}

class State;
class Notification;

extern State *TopState, *FirstFree = 0;
extern unsigned long LastChoice, Alternatives;
extern size_t NotifiedSpace;
extern Notification *FirstNotification, *N; 

void Clean() {
    State *OldTopState = TopState;
    TopState = CurrentCoroutine()->TopState;
    ClearChoices();
    if (CurrentCoroutine() != MainCoroutine()) 
        TopState = OldTopState;
    CurrentCoroutine()->TopState = 0;
}

class State : public Coroutine {
    void Routine() {};
public:
    State() { CleanUp = Clean; }
    State *Link(State *B) {
        B->Next = Son; 
        B->Previous = this;
        if (Son) 
            Son->Previous = B;
        Son = B;
        return this;
    }
    State *Merge(State *B) {
        if (!BreadthFirst)
            return Merit >= B->Merit ? Link(B) : B->Link(this);
        return Merit > B->Merit ? Link(B) : B->Link(this);
    }
    void Insert() {
        Previous = Next = Son = 0; 
        ::TopState = !::TopState ? this : Merge(::TopState);
    }
    State *Previous, *Next, *Son;
};

class Notification {
public:
    void *Base;
    size_t Size;
    Notification *Next;
};

void inline DeleteMax() {
    State *Max = TopState, *Prev, *A, *B, *C;
    if (!Max) 
        return;
    Prev = TopState = 0;
    for (A = Max->Son; A && (B = A->Next); A = C) {
         C = B->Next;
        A->Next = B->Next = A->Previous = B->Previous = 0;
        TopState = B->Merge(A);
        TopState->Previous = Prev;
        Prev = TopState;
    }
    if (A) {
        A->Previous = Prev; 
        TopState = A;
    }
    if (TopState) {
        for (A = TopState->Previous; A; A = B) {
	          B = A->Previous;
	          TopState = A->Merge(TopState);
        }
        TopState->Previous = 0;
    }
    Max->Next = FirstFree;
    FirstFree = Max;
}

void PopState() { DeleteMax(); }

unsigned long Choice(long N) {
    if (N <= 0)
        Backtrack();
    if (N == 1 && (!TopState || TopState->Merit <= Merit))
        return (LastChoice = Alternatives = 1);
    State *NewState;
    if (FirstFree)  {
        NewState = FirstFree;
        FirstFree = NewState->Next;
    } 
    else if (!(NewState = new State)) 
        Error("No more space for Choice\n");
    NewState->LastChoice = NewState->Alternatives = 0;
    NewState->Merit = Merit;
    NewState->Previous = NewState->Next = NewState->Son = 0;
    static Notification *Ntf;
    static char *B;
    for (Ntf = FirstNotification, B = (char *) NotifiedSpace; 
         Ntf;	
         B += Ntf->Size, Ntf = Ntf->Next)
        memcpy(B, Ntf->Base, Ntf->Size);
    NewState->StoreStack();
    setjmp(NewState->Environment);
    if (!NewState->Alternatives) {
        NewState->Alternatives = N;
        NewState->Insert();
        TopState->RestoreStack();
    }
    else {
       for (Ntf = FirstNotification, B = (char*) NotifiedSpace; 
            Ntf;	
            B += Ntf->Size, Ntf = Ntf->Next) 
	         memcpy(Ntf->Base, B, Ntf->Size);
    }
    Alternatives = TopState->Alternatives; 
    Merit = TopState->Merit;
    LastChoice = ++TopState->LastChoice;
    if (LastChoice == Alternatives)
        PopState();
    return LastChoice;
}

void Backtrack() { 
    if (!TopState) {
        if (Fiasco)
	          Fiasco();
        Detach();
    }
    TopState->RestoreStack();
}

unsigned long NextChoice() {
    if (++LastChoice > Alternatives) 
        Backtrack();
    if (LastChoice == Alternatives) 
        PopState();
    else 
        TopState->LastChoice = LastChoice;
    return LastChoice;
}

void Cut() {
    if (LastChoice < Alternatives)
        PopState();
    Backtrack();
}

void *NotifyStorage(void *Base, size_t Size) {
    if (TopState) 
        Error("Notification (unfinished Choice calls)");
    for (Notification *N = FirstNotification; N; N = N->Next)
        if (N->Base == Base)
            return 0;
    N = new Notification;
    if (!N) 
        Error("No more space for notification");
    N->Base = Base; 
    N->Size = Size; 
    NotifiedSpace += Size; 
    N->Next = FirstNotification; 
    FirstNotification = N;
    CleanUp = Clean; 
    return Base;
}

void RemoveNotification(void *Base) {
    if (TopState) 
        Error("RemoveNotification (unfinished Choice calls)");
    for (Notification *N = FirstNotification, *Prev = 0; 
         N; 
         Prev = N, N = N->Next) {
        if (N->Base == Base) {
            NotifiedSpace -= N->Size;
            if (!Prev) 
                FirstNotification = N->Next; 
            else 
                Prev->Next = N->Next;
            delete N;
            return;
        }         
    }
}

void ClearChoices() {
    while (TopState)
        PopState();
    LastChoice = Alternatives = 0;
}

void ClearNotifications() {
    while (FirstNotification)
        RemoveNotification(FirstNotification->Base);            
}