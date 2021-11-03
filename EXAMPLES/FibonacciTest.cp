#include "coroutine.h"
#include <iostream.h>

class Fibonacci : public Coroutine {
public:
    int Next() {
        Call(this);
        return F2; 
    }
private:
    void Routine() {
        F1 = 0; F2 = 1;
        for (;;) {
            Detach();
            F2 += F1; F1 = F2 - F1;
       }
    }
    int F1, F2;
};

void FibonacciTest() {
    Fibonacci *F = new Fibonacci;
    for (int i = 1; i <= 20; i++) 
        cout << F->Next() << endl;
}

int main() Sequencing(FibonacciTest())