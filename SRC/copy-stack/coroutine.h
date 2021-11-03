#ifndef Sequencing
#define Sequencing(S) {char Dummy; StackBottom = &Dummy; S;}
#include <stddef.h>
#include <setjmp.h>

extern char *StackBottom;

class Coroutine {
friend void Resume(Coroutine *);
friend void Call(Coroutine *);
friend void Detach();
friend class Process;
friend unsigned long Choice(long);
friend void Backtrack();
protected:
    Coroutine(size_t Dummy = 0);
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
};

void Resume(Coroutine *);
void Call(Coroutine *);
void Detach();

Coroutine *CurrentCoroutine();
Coroutine *MainCoroutine();

#define DEFAULT_STACK_SIZE 0
#endif