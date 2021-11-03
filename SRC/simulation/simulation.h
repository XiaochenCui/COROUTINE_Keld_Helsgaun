#ifndef Simulation
#define Simulation Sequencing

#include "coroutine.h"
#include "simset.h"
#include "random.h"

class Process : public Link, public Coroutine {
friend Process *Current();
friend double Time();
friend void Activat(int Reac, Process *X, 
int Code, double T, 
Process *Y, int Prio);
friend void Hold(double T);
friend void Passivate();
friend void Wait(Head *Q);
friend void Cancel(Process *P);
friend class Main_Program;
friend class SQS_Process;
public:
    virtual void Actions() = 0;
    Process(size_t stack_size = DEFAULT_STACK_SIZE);
    int Idle() const;
    int Terminated() const;
    double EvTime() const;
    Process *NextEv() const;
private:
    void Routine();
    int TERMINATED;
    Process *PRED, *SUC;
    double EVTIME;
};

Process *Main();
Process *Current();
double Time();

void Hold(double T);
void Passivate();
void Wait(Head *Q);
void Cancel(Process *P);

enum Haste {at = 1, delay = 2};
enum Ranking {before = 3, after = 4};
enum Prior {prior = 5};

void Activate(Process *P);
void Activate(Process *P, Haste H, double T);
void Activate(Process *P, Haste H, double T, Prior Prio);
void Activate(Process *P1, Ranking Rank, Process *P2);

void Reactivate(Process *P);
void Reactivate(Process *P, Haste H, double T);
void Reactivate(Process *P, Haste H, double T, Prior Prio);
void Reactivate(Process *P1, Ranking Rank, Process *P2);

void Accum(double &A, double &B, double &C, double D);

#endif

