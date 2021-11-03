#ifndef Sequencing
#define Sequencing(S)     \
  {                       \
    char Dummy;           \
    StackBottom = &Dummy; \
    S;                    \
  }
#include <setjmp.h>
#include <stddef.h>

extern char *StackBottom;

class Coroutine
{
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

  // Points to the lowest and highest address of stack used currently.
  // In the cases when stack grow down (which is the most cases):
  // - High was assigned the value of StackBottom in the first time of
  //  StoreStack, and never changed afterwards.
  // - Low was assigned the lowest address of stack in each time of
  //  StoreStack.
  char *Low, *High;

  // Store memory content of the Coroutine's stack, store and restore is achieved by ordinary memcpy
  // When is it initialized? When a coroutine hangs for the first time
  // When was it expands? When the Buffer Size of a coroutine is less than the size of the current used stack
  // When was it deleted? 1. When a coroutine is deconstructed; 2. When a coroutine's routine function exits
  char *StackBuffer;

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