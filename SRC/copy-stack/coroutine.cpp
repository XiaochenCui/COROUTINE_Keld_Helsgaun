#define Synchronize //

#include "coroutine.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

char *StackBottom;

// return true if the StackBuffer is NULL and BufferSize is not 0
#define Terminated(C) (!(C)->StackBuffer && (C)->BufferSize)

// initailized to the `Main` coroutine at the start of the program
//
// changed at the `RestoreStack` function
static Coroutine *Current = 0;

// changed in the Call / Detach / Resume functions
static Coroutine *Next;

static void Error(const char *Message)
{
    std::cerr << "Error: " << Message << std::endl;
    exit(0);
}

Coroutine *Coroutine::ToBeResumed = 0;

static class MainCoroutine : public Coroutine
{
public:
    MainCoroutine() { Current = this; }
    void Routine() {}
} Main;

Coroutine::Coroutine(size_t Dummy)
{
    char X;
    if (StackBottom)
        if (&X < StackBottom
                ? &X <= (char *)this && (char *)this <= StackBottom
                : &X >= (char *)this && (char *)this >= StackBottom)
            Error("Attempt to allocate a Coroutine on the stack");
    StackBuffer = 0;
    Low = High = 0;
    BufferSize = Dummy = 0;
    Callee = Caller = 0;
}

Coroutine::~Coroutine()
{
    delete StackBuffer;
    StackBuffer = 0;
}

// steps:
// 1. set the `Current` to the `this` coroutine
// 2. copy the stack content stored in the `StackBuffer`
//  to the stack
// 3. restore registers content saved previously
inline void Coroutine::RestoreStack()
{
    Synchronize;
    char X = 0;
    if (&X >= Low && &X <= High)
    {

        // make sure the destination memory is not used concurrently
        //
        // Under what conditions will this happen?
        // the length of current used stack is less than the length
        // of the stack buffer saved previously
        //
        // Why we have to make sure the destination memory is not used concurrently?
        // I don't know.
        //
        RestoreStack();
    }
    Current = this;
    memcpy(Low, StackBuffer, High - Low);

    // why?
    // becase this is the last step in the procedure of Coroutine::Enter
    // (i.e, we are going to exit). So we restore the registers and
    // jmp to the setjmp, return directly.
    longjmp(Current->Environment, 1);
}

inline void Coroutine::StoreStack()
{
    if (!Low)
    {
        if (!StackBottom)
            Error("StackBottom is not initialized");
        Low = High = StackBottom;
    }
    char X;
    if (&X > StackBottom)
        High = &X;
    else
        Low = &X;
    if (High - Low > BufferSize)
    {
        delete StackBuffer;
        BufferSize = High - Low;
        if (!(StackBuffer = new char[BufferSize]))
            Error("No more space available");
    }
    Synchronize;
    memcpy(StackBuffer, Low, High - Low);
}

inline void Coroutine::Enter()
{
    if (!Terminated(Current))
    {
        Current->StoreStack();
        if (setjmp(Current->Environment))
            return;
    }
    Current = this;
    if (!StackBuffer)
    {
        Routine();
        delete Current->StackBuffer;
        Current->StackBuffer = 0;
        if (ToBeResumed)
        {
            Next = ToBeResumed;
            ToBeResumed = 0;
            Resume(Next);
        }
        Detach();
    }
    RestoreStack();
}

void Resume(Coroutine *Next)
{
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

void Call(Coroutine *Next)
{
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

void Detach()
{
    Next = Current->Caller;
    if (Next)
        Current->Caller = Next->Callee = 0;
    else
    {
        Next = &Main;
        while (Next->Callee)
            Next = Next->Callee;
    }
    Next->Enter();
}

Coroutine *CurrentCoroutine() { return Current; }

Coroutine *MainCoroutine() { return &Main; }
