#define Synchronize // {jmp_buf E; if (!setjmp(E)) longjmp(E,1);}

#include "coroutine.h"
#include <iostream.h>
#include <stdlib.h>
#include <string.h>

char *StackBottom;

void NoClean() {} 
void (*CleanUp) () = NoClean;

class State;
class Notification;

State *TopState;
size_t NotifiedSpace;
Notification *FirstNotification, *N;
unsigned long LastChoice, Alternatives;
long Merit;

#define Terminated(C) (!(C)->StackBuffer && (C)->BufferSize)
static Coroutine *Current = 0, *Next;

static void Error(const char *Message) {
    cerr << "Error: " << Message << endl; 
    exit(0);
}

Coroutine *Coroutine::ToBeResumed = 0;

static class MainCoroutine : public Coroutine { 
public:
    MainCoroutine() { Current = this; }
    void Routine() {}
} Main;

Coroutine::Coroutine() {
    char X;
    if (StackBottom)
        if (&X < StackBottom ? 
            &X <= (char*) this && (char*) this <= StackBottom :
            &X >= (char*) this && (char*) this >= StackBottom)    
            Error("Attempt to allocate a Coroutine on the stack");
    StackBuffer = 0; Low = High = 0; BufferSize = 0;
    Callee = Caller = 0; TopState = 0;
    LastChoice = Alternatives = 0;
    Merit = 0;
}

Coroutine::~Coroutine() { CleanUp(); delete StackBuffer; StackBuffer = 0; }

inline void Coroutine::RestoreStack() {
    Synchronize;
    char X;
    if (&X >= Low && &X <= High) 
        RestoreStack();
    memcpy(Low, StackBuffer, High - Low);
    longjmp(Environment, 1);
}

inline void Coroutine::StoreStack() {
    if (!Low) {
        if (!StackBottom)
            Error("StackBottom is not initialized");
        Low = High = StackBottom;
    }
    char X;
    if (&X > StackBottom) 
        High = &X; 
    else 
        Low = &X;
    if (High - Low > BufferSize) {
        delete StackBuffer;
        BufferSize = High - Low;
        if (!(StackBuffer = new char[BufferSize]))
            Error("No more space available");
    }
    Synchronize;
    memcpy(StackBuffer, Low, High - Low);
}

inline void Coroutine::Enter() {
    if (!Terminated(Current)) {
        Current->StoreStack();
        if (setjmp(Current->Environment)) 
            return; 
    }
    Current->TopState = ::TopState;
    Current->LastChoice = ::LastChoice;
    Current->Alternatives = ::Alternatives;
    Current->Merit = ::Merit;
    ::TopState = TopState;
    ::LastChoice = LastChoice;
    ::Alternatives = Alternatives;
    ::Merit = Merit;
    Current = this;
    if (!StackBuffer) {
        Routine();
        CleanUp();
        delete Current->StackBuffer;
        Current->StackBuffer = 0;
        if (ToBeResumed) {
            Next = ToBeResumed;
            ToBeResumed = 0;
            Resume(Next);
        }
        Detach();
    }
    RestoreStack();
}

void Resume(Coroutine *Next) {
    if (!Next)
        Error("Attempt to Resume a non-existing Coroutine");
    if (Next == Current) 
        return;
    if (Terminated(Next))
        Error("Attempt to Resume a terminated Coroutine");
    if (Next->Caller) 
        Error("Attempt to Resume an attached Coroutine");
    while (Next->Callee) 
         Next = Next->Callee;
    Next->Enter();
}

void Call(Coroutine *Next) {
    if (!Next) 
        Error("Attempt to Call a non-existing Coroutine");
    if (Terminated(Next))
        Error("Attempt to Call a terminated Coroutine");
    if (Next->Caller) 
        Error("Attempt to Call an attached Coroutine");
    Current->Callee = Next;
    Next->Caller = Current;
    while (Next->Callee) 
        Next = Next->Callee;
    if (Next == Current) 
        Error("Attempt to Call an operating Coroutine");
    Next->Enter();
}

void Detach() {
    Next = Current->Caller;
    if (Next)  
        Current->Caller = Next->Callee = 0;
    else {
        Next = &Main; 
        while (Next->Callee)
            Next = Next->Callee;
    }
    Next->Enter(); 
}

Coroutine *CurrentCoroutine() { return Current; }

Coroutine *MainCoroutine() { return &Main; }