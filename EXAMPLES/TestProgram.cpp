#include "coroutine.h"
#include <iostream>
#include <stdlib.h>

class A : public Coroutine {
  void Routine();
};
class B : public Coroutine {
  void Routine();
};
class C : public Coroutine {
  void Routine();
};
Coroutine *a1, *b1, *c1;

void A::Routine() {
  std::cout << "a1";
  Detach();
  std::cout << "a2";
  Call(c1 = new C);
  std::cout << "a3";
  Call(b1);
  std::cout << "a4";
  Detach();
}

void B::Routine() {
  std::cout << "b1";
  Detach();
  std::cout << "b2";
  Resume(c1);
  std::cout << "b3";
};

void C::Routine() {
  std::cout << "c1";
  Detach();
  std::cout << "c2" << std::endl << "==> ";
  flush(std::cout);
  char command;
  std::cin >> command;
  if (command == 'r')
    Resume(a1);
  else if (command == 'c')
    Call(a1);
  else
    Detach();
  std::cout << "c3";
  Detach();
  std::cout << "c4";
}

void TestProgram() {
  std::cout << "m1";
  Call(a1 = new A);
  std::cout << "m2";
  Call(b1 = new B);
  std::cout << "m3";
  Resume(a1);
  std::cout << "m4";
  Resume(c1);
  std::cout << "m5" << std::endl;
}

int main() Sequencing(TestProgram())
