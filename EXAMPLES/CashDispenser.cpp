#include "coroutine.h"
#include "simset.h"
#include <iostream>

int Amount;

inline int min(int a, int b) { return a < b ? a : b; }

class Coin : public Link, public Coroutine {
public:
    Coin(int d, int n) : Denomination(d), Number(n) {} 
    int Denomination, Number, Used;

    void PrintSolution() {
        if (Pred() != NULL)
            ((Coin*) Pred())->PrintSolution();
        if (Used > 0)
            std::cout << Used << " of " << Denomination << std::endl;
    }

    void Routine() {
        for (;;) { 
            for (Used = min(Amount/Denomination, Number); Used >= 0; Used--) {
                Number -= Used;
                Amount -= Used*Denomination;
                if (Amount == 0) {
                    PrintSolution();
                    Detach();
                }
                if (Suc() != NULL)  
                    Resume((Coin*) Suc());
                Amount += Used*Denomination;
                Number += Used;
            }
            if (Pred() == NULL) {
                std::cout << "No solution" << std::endl;
                Detach(); 
            }
            Resume((Coin*) Pred());
        }			
    }
};


void CashDispensor() {
    std::cout << "Amount to be paid: ";
    std::cin >> Amount;	

    Head *List = new Head;
    (new Coin(1000,19))->Into(List);
    (new Coin(500,9))->Into(List);
    (new Coin(100,11))->Into(List);
    (new Coin(50,10))->Into(List);
    (new Coin(20,32))->Into(List);
    (new Coin(10,0))->Into(List);
    (new Coin(5,1))->Into(List);
    (new Coin(1,7))->Into(List);
    Resume((Coin*) List->First());
}

int main() Sequencing(CashDispensor())