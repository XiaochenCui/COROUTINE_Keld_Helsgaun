#include "coroutine.h"
#include <iostream>

class Rand : public Coroutine {
public:
    Rand(int Seed = 1) : U(Seed) {}
    unsigned int Next() {
        Call(this);
        return (unsigned int) (U/65536) % 32768; 
    }
private:
    void Routine() {
        for (;;) {
            U = U*1103515245 + 12345;
            Detach();
        }
    }
    unsigned long U;
};

void RandTest() {
    Rand *R = new Rand;
    for (int i = 1; i <= 20; i++) 
    std::cout << R->Next() << std::endl;
}

int main() Sequencing(RandTest())