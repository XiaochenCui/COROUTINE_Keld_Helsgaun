#include "CBack.h"
#include <iostream>
#include <limits.h>

const int NodeMax = 100, DegreeMax = 5;
unsigned int Deg[NodeMax];               // degree of each node
unsigned int Wt[NodeMax];                // weight of each node 
unsigned int Desc[NodeMax][DegreeMax];   // immed. desc. of nodes
unsigned long Best = ULONG_MAX;          // current minimum

class Shortest : public Coroutine {
public:	
    Shortest(unsigned int R) : Root(R) {}
private:
    unsigned int Root;

    void Routine() {
        unsigned long Sum;               // current path sum
        unsigned int NodeNow;	           // current node pointer

        Sum = Wt[NodeNow = Root];
        while (Sum < Best) {
            if (Deg[NodeNow] > 0) {      // pick descendant node
                NodeNow = Desc[NodeNow][Choice(Deg[NodeNow])];
                Sum += Wt[NodeNow];
            }
            else {                       // this node is terminal
                Best = Sum;
                Detach();                // let colleague try to beat it 
                Backtrack();             // look for alternate path
            }
        }
        Backtrack();                     // current path already too large
    }
};

void Treewalk() {
    Deg[1] = 3;  Wt[1] = 2;  Desc[1][1] = 2; Desc[1][2] = 3; Desc[1][3] = 4;
    Deg[2] = 2;  Wt[2] = 5;  Desc[2][1] = 5; Desc[2][2] = 6; 
    Deg[3] = 0;  Wt[3] = 7;  
    Deg[4] = 1;  Wt[4] = 4;  Desc[4][1] = 7;
    Deg[5] = 0;  Wt[5] = 1; 
    Deg[6] = 0;  Wt[6] = 2; 
    Deg[7] = 0;  Wt[7] = 0;
    Deg[11] = 3; Wt[11] = 0; Desc[11][1] = 12; Desc[11][2] = 13; Desc[11][3] = 14;
    Deg[12] = 2; Wt[12] = 5; Desc[12][1] = 15; Desc[12][2] = 16; 
    Deg[13] = 0; Wt[13] = 6;  
    Deg[14] = 1; Wt[14] = 3; Desc[14][1] = 17;
    Deg[15] = 0; Wt[15] = 2; 
    Deg[16] = 0; Wt[16] = 1; 
    Deg[17] = 0; Wt[17] = 2;
    Shortest *T[] = {new Shortest(1), new Shortest(11)};
    unsigned long OldBest;
    int i = 0;                          // let system 0 try first
    do {
        OldBest = Best;                 // save current best path weight
        Call(T[i]);                     // call current subsystem
        i = 1 - i;                      // switch to other subsystem
    } while (Best < OldBest);
    std::cout << "Tree " << i << " has minimum terminal path" << std::endl;  	
}

int main() Backtracking(Treewalk())