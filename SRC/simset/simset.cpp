#include "simset.h"

Linkage::Linkage() { SUC = PRED = 0; }
Link* Linkage::Pred() const { return PRED ? PRED->LINK() : 0; }
Link* Linkage::Suc() const { return SUC ? SUC->LINK() : 0; }
Linkage* Linkage::Prev() const { return PRED; }

Head::Head() { SUC = PRED = this; }
Link* Head::First() const { return Suc(); }
Link* Head::Last() const { return Pred(); }
int Head::Empty() const { return SUC == (Linkage*) this; }
int Head::Cardinal() const {
    int i = 0; 
    for (Link *L = First(); L; L = L->Suc()) i++;
        return i; 
}
void Head::Clear() { while (First()) First()->Out(); }
Link* Head::LINK() { return 0; }

void Link::Out() { 
    if (SUC) { SUC->PRED = PRED; PRED->SUC = SUC; PRED = SUC = 0; }
}
void Link::Into(Head *H) { Precede(H); }
void Link::Precede(Linkage *L) { 
    Out();
    if (L && L->SUC) { SUC = L; PRED = L->PRED; L->PRED = PRED->SUC = this; }
}
void Link::Follow(Linkage *L) { if (L) Precede(L->SUC); else Out(); }
Link* Link::LINK() { return this; }