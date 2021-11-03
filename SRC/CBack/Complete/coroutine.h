#ifndef Sequencing
#define Sequencing(S) {char Dummy; StackBottom = &Dummy; S;}

#include <stddef.h>
#include <setjmp.h>

extern char *StackBottom;
extern void (*CleanUp) ();

class Coroutine {
friend void Resume(Coroutine *);
friend void Call(Coroutine *);
friend void Detach();
friend unsigned long Choice(long);
friend void Backtrack();
friend unsigned long NextChoice();
friend class Process;
friend class State;
friend class Notification;
friend void Clean();
protected:
    Coroutine();
    ~Coroutine();
    virtual void Routine() = 0;
private:
    void Enter();
    void StoreStack();
    void RestoreStack();
    char *StackBuffer, *Low, *High;
    size_t BufferSize;
    jmp_buf Environment;
    Coroutine *Caller, *Callee;
    static Coroutine *ToBeResumed;
    State *TopState;
    unsigned long LastChoice, Alternatives;
    long Merit;
};

void Resume(Coroutine *);
void Call(Coroutine *);
void Detach();

Coroutine *CurrentCoroutine();
Coroutine *MainCoroutine();

#endif