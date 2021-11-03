#ifndef Backtracking
#define Backtracking Sequencing

#include "coroutine.h"
#include <stddef.h>

#define Notify(V) NotifyStorage(&V, sizeof(V))
#define Nmalloc(Size) NotifyStorage(malloc(Size), Size)
#define Ncalloc(N, Size) NotifyStorage(calloc(N,Size), (N)*Size)
#define Nrealloc(P, Size)\
        (RemoveNotification(P), NotifyStorage(realloc(P, Size), Size))
#define Nfree(P) (RemoveNotification(P), free(P))
#define ClearAll() (ClearChoices(), ClearNotifications())

unsigned long Choice(long);
void Backtrack();      
unsigned long NextChoice(void);
void Cut(void);
void ClearChoices(void);

void *NotifyStorage(void *Base, size_t Size);
void RemoveNotification(void *Base);
void ClearNotifications(void);

extern void (*Fiasco)();
extern long Merit;
extern int BreadthFirst;
  
#endif