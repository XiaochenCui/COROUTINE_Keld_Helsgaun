#include "coroutine.h"
#include <iostream>
#include <stdlib.h>
#include <cstring>

class A : public Coroutine
{
    void Routine();
};
class B : public Coroutine
{
    void Routine();
};
Coroutine *a1, *b1;

void A::Routine()
{
    std::cout << "a1" << std::endl;
    Call(b1 = new B);
    std::cout << "a2" << std::endl;
}

void B::Routine() { std::cout << "b1" << std::endl; };

void TestProgram()
{
    std::cout << "start" << std::endl;
    Call(a1 = new A);
    std::cout << "create a2" << std::endl;
    Coroutine *a2 = new A;
    Call(a2);
    std::cout << "end" << std::endl;
}

class C : public Coroutine
{
    void Routine();
};
void C::Routine()
{
    int c1 = 1;
    std::cout << "c1: " << c1 << std::endl;
    int c2 = 2;
    std::cout << "c2: " << c2 << std::endl;
    int c3 = 3;
    std::cout << "c3: " << c3 << std::endl;
    int c4 = 4;
    std::cout << "c4: " << c4 << std::endl;
    int c5 = 5;
    std::cout << "c5: " << c5 << std::endl;
    int c6 = 6;
    std::cout << "c6: " << c6 << std::endl;

    char buf[0x50000] = "";
    std::cout << "szieof(buf): " << sizeof(buf) << std::endl;
}

void RunSingleRoutine()
{
    std::cout << "start" << std::endl;
    Coroutine *c = new C;
    Call(c);
    std::cout << "end" << std::endl;
}

static int nestLevel = 0;

class Nest : public Coroutine
{
    void Routine();
};
void Nest::Routine()
{
    std::cout << "nest start" << std::endl;
    nestLevel++;
    if (nestLevel < 5)
    {
        Call(new Nest);
    }
    else
    {
        std::cout << "level: " << nestLevel << std::endl;
    }
    std::cout << "nest end" << std::endl;
}

void NestedRoutine()
{
    std::cout << "start" << std::endl;
    Coroutine *n = new Nest;
    Call(n);
    std::cout << "end" << std::endl;
}

// int main() Sequencing(TestProgram())
// int main() Sequencing(RunSingleRoutine())
int main() Sequencing(NestedRoutine())
