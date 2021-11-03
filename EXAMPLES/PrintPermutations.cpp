#include "coroutine.h"
#include <iostream.h>

class Permuter : public Coroutine {
public: 
    int N, *p, More;
    Permuter(int n) : N(n) { p = new int[N+1]; }

    void Permute(int k) {
        if (k == 1)
            Detach();
         else {
             Permute(k-1);
             for (int i = 1; i < k; i++) {
                 int q = p[i]; p[i] = p[k]; p[k] = q; 
                 Permute(k-1);
                 q = p[i]; p[i] = p[k]; p[k] = q; 
             }
         }
    }

    void Routine() {	
        for (int i = 1; i <= N; i++) 
            p[i] = i;
        More = 1;
        Permute(N);
        More = 0;
    }
};

void PrintPermutations(int n) {
    Permuter *P = new Permuter(n);
    Call(P);
    while (P->More) {
        for (int i = 1; i <= n; i++)
            cout << P->p[i] << " ";
        cout << endl; 
        Call(P);
    } 
}

int main() Sequencing(PrintPermutations(5))