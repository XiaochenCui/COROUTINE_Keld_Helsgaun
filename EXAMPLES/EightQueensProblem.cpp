#include "CBack.h"
#include <iostream.h>

int Q[9];

void EightQueensProblem() {
    for (int r = 1; r <= 8; r++) {
        int c = Choice(8);
        for (int i = 1; i < r; i++) 
            if (c == Q[i] || abs(c - Q[i]) == r - i) 
                Backtrack();
        Q[r] = c; 
    }
    for (int r = 1; r <= 8; r++) 
        cout << Q[r] << " "; 
    cout << endl;
    Backtrack();
}

int main() Backtracking(EightQueensProblem())