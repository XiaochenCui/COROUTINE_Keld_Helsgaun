#include "coroutine.h"
#include <iostream>

int A[] = {1,5,6,8,10,12,15,17};
int B[] = {2,4,7,9,11,13,14,18,20,30}; 
int *C;
int m = 8, n = 10, CIndex;
int Count = 0;

class Traverser : public Coroutine {
public:
    Traverser(int A[], int L) : Array(A), Limit(L), Index(0) {} 
    void Routine() {
        while (Index < Limit) {
            if (Partner->Array[Partner->Index] < Array[Index]) 
                Resume(Partner);
            C[CIndex++] = Array[Index++];
        }
        while (CIndex < m+n)
            C[CIndex++] = Partner->Array[Partner->Index++];
    }
public:
    int *Array;
    int Limit;
    Traverser *Partner;
    int Index;
};

void MergeArrays() {
    Traverser *X = new Traverser(A,m);
    Traverser *Y = new Traverser(B,n);
    X->Partner = Y; Y->Partner = X; 
    C = new int[m+n]; CIndex = 0; 
    Resume(X);
    for (int j = 0; j < m+n; j++) 
        std::cout << C[j] << " ";
    std::cout << std::endl;
}

int main() Sequencing(MergeArrays())