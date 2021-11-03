#include "CBack.h"
#include <stdlib.h>
#include <iostream>
#include <setjmp.h>

void (*Fiasco) () = 0;

static void Error(const char *Msg) {
    std::cerr << "Error: " << Msg << std::endl; 
    exit(0);
}

class State : public Coroutine {
public:
    unsigned long LastChoice;
    State *Previous;
private:
    void Routine() {};
};

static State *TopState = 0, *Previous;

unsigned long Choice(long N) {
    if (N <= 0)
        Backtrack();
    if (N == 1)
        return 1;
    Previous = TopState;
    TopState = new State;
    if (!TopState) 
        Error("No more space for choice\n");
    TopState->Previous = Previous;
    TopState->LastChoice = 0;
    TopState->StoreStack();
    setjmp(TopState->Environment);
    if (++TopState->LastChoice < N)  
        return TopState->LastChoice; 
    Previous = TopState->Previous;
    delete TopState;
    TopState = Previous;
    return N;
}

void Backtrack() {
    if (!TopState) {
        if (Fiasco)
            Fiasco();
        exit(0);
    }
    TopState->RestoreStack();;
}