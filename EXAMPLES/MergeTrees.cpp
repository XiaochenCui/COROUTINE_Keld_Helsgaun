#include "coroutine.h"
#include <iostream>
#include <limits.h>

class Tree {
public:
    Tree(int V, Tree *L, Tree *R) : Value(V), Left(L), Right(R) {} 
    int Value; 
    Tree *Left, *Right;
};

class Traverser : public Coroutine {
public:
    Traverser(Tree *T) : MyTree(T) {}
    int Current;
private:
    Tree *MyTree;

    void Routine() {
        Traverse(MyTree);
        Current = INT_MAX;
    } 
    
    void Traverse(Tree *T) {
        if (T != NULL) {
            Traverse(T->Left);
		    Current = T->Value;
            Detach();
            Traverse(T->Right); 
        }
    }
};

void MergeTrees() {
    Tree *Tree1 = new Tree(8,
                      new Tree(5,
                          new Tree(1, NULL, NULL),
                          new Tree(6, NULL, NULL)),
                      new Tree(10,
                          NULL,
                          new Tree(12,
                              NULL,
                              new Tree(15,
                                  NULL,
                                  new Tree(17, NULL, NULL)))));
    Tree *Tree2 = new Tree(13, 
                      new Tree(4, 
                          new Tree(2, NULL, NULL),
                          new Tree(9,
                              new Tree(7, NULL, NULL),
                              new Tree(11, NULL, NULL))),
                      new Tree(20,
                          new Tree(14,
                              NULL,
                              new Tree(18, NULL, NULL)),
                              new Tree(30, NULL, NULL)));
    Traverser *T1 = new Traverser(Tree1);
    Traverser *T2 = new Traverser(Tree2);
    Call(T1);
    Call(T2);
    for (;;) {
        int min = T1->Current;
        if (T2->Current < min) {
            min = T2->Current;
            Call(T2);
        } else {
        	if (min == INT_MAX)
            	break;
            Call(T1);
		}
        std::cout << min << " ";
    }
    std::cout << std::endl; 
}

int main() Sequencing(MergeTrees())