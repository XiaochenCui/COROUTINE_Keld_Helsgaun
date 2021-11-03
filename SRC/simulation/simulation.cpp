#include "simulation.h"
#include <iostream>

static void Error(const char *Msg) {
    std::cerr << "Error: " << Msg << std::endl; 
    exit(0);
}

class SQS_Process : public Process {
public:
    SQS_Process() { EVTIME = -1; PRED = SUC = this; }
    void Actions() {}
    inline static void SCHEDULE(Process *Q, Process *P) {
        Q->PRED = P; Q->SUC = P->SUC;
        P->SUC = Q; Q->SUC->PRED = Q;
    }
    inline static void UNSCHEDULE(Process *P) {
        P->PRED->SUC = P->SUC;
        P->SUC->PRED =P->PRED;
        P->PRED = P->SUC = 0;
    }
} SQS;

class Main_Program : public Process {
public:
    Main_Program() { EVTIME = 0; SQS.SCHEDULE(this,&SQS); }
    void Actions() { while (1) Detach(); }
} MainProgram;

Process::Process(size_t stack_size) : Coroutine(stack_size) 
    { TERMINATED = 0; PRED = SUC = 0; }

void Process::Routine() { 
    Actions(); 
    TERMINATED = 1; 
    SQS.UNSCHEDULE(this);
    if (SQS.SUC == &SQS)
        Error("SQS is empty"); 
    ToBeResumed = SQS.SUC; 
}
int Process::Idle() const { return SUC == 0; }
int Process::Terminated() const { return TERMINATED; }
double Process::EvTime() const {
    if (SUC == 0)
        Error("No EvTime for Idle Process"); 
    return EVTIME; 
}
Process* Process::NextEv() const { return SUC == &SQS ? 0 : SUC; }

Process *Main() { return &MainProgram; }
Process *Current() { return SQS.SUC; } 
double Time() { return SQS.SUC->EVTIME; }

void Hold(double T) { 
    Process *Q = SQS.SUC;
    if (T > 0) 
        Q->EVTIME += T;
    T = Q->EVTIME;
    if (Q->SUC != &SQS && Q->SUC->EVTIME <= T) {
        SQS.UNSCHEDULE(Q);
        Process *P = SQS.PRED;
        while (P->EVTIME > T) 
            P = P->PRED;
        SQS.SCHEDULE(Q,P);
        Resume(SQS.SUC);
    }
}

void Passivate() {
    Process *CURRENT = SQS.SUC;
    SQS.UNSCHEDULE(CURRENT);
    if (SQS.SUC == &SQS) 
        Error("SQS is empty");
    Resume(SQS.SUC);
} 

void Wait(Head *Q) {
    Process *CURRENT = SQS.SUC;
    CURRENT->Into(Q);
    SQS.UNSCHEDULE(CURRENT);
    if (SQS.SUC == &SQS) 
        Error("SQS is empty");
    Resume(SQS.SUC);
}

void Cancel(Process *P) {
    if (!P || !P->SUC) 
        return;
    Process *CURRENT = SQS.SUC;
    SQS.UNSCHEDULE(P);
    if (SQS.SUC != CURRENT) 
        return;
    if (SQS.SUC == &SQS) 
        Error("SQS is empty");
    Resume(SQS.SUC);
}

enum {direct = 0};

void Activat(int Reac, Process *X, int Code, 
    double T, Process *Y, int Prio) {
    if (!X || X->TERMINATED || (!Reac && X->SUC)) 
        return;
    Process *CURRENT = SQS.SUC, *P = 0;
    double NOW = CURRENT->EVTIME;
    switch(Code) {
    case direct:
        if (X == CURRENT) 
            return; 
        T = NOW; P = &SQS;
        break;
    case delay:
        T += NOW;
    case at:
        if (T <= NOW) {
            if (Prio && X == CURRENT) 
                return;
            T = NOW;
        } 
        break;
    case before:
    case after:
        if (!Y || !Y->SUC) {
            SQS.UNSCHEDULE(X); 
            if (SQS.SUC == &SQS) 
                Error("SQS is empty");
            return;
        }
        if (X == Y) 
            return;
        T = Y->EVTIME;
        P = Code == before ? Y->PRED : Y;
    }
    if (X->SUC)
        SQS.UNSCHEDULE(X);
    if (!P) {
        for (P = SQS.PRED; P->EVTIME > T; P = P->PRED)
            ;
        if (Prio) 
            while (P->EVTIME == T) 
                P = P->PRED;  
    }
    X->EVTIME = T;
    SQS.SCHEDULE(X,P);
    if (SQS.SUC != CURRENT) 
        Resume(SQS.SUC);
}

void Activate(Process *P) { Activat(0,P,direct,0,0,0); }
void Activate(Process *P, Haste H, double T) { Activat(0,P,H,T,0,0); }
void Activate(Process *P, Haste H, double T, Prior Pri) { Activat(0,P,H,T,0,Pri); }
void Activate(Process *P1, Ranking Rank, Process *P2) { Activat(0,P1,Rank,0,P2,0); }

void Reactivate(Process *P) { Activat(1,P,direct,0,0,0); }
void Reactivate(Process *P, Haste H, double T) { Activat(1,P,H,T,0,0); }
void Reactivate(Process *P, Haste H, double T, Prior Pri)  { Activat(1,P,H,T,0,Pri); }
void Reactivate(Process *P1, Ranking Rank, Process *P2) { Activat(1,P1,Rank,0,P2,0); }  

void Accum(double &A, double &B, double &C, double D) {
    A += C*(Time() - B); B = Time(); C += D;
}
