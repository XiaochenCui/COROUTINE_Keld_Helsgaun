#include "coroutine.h"
#include <stdlib.h>
#include <iostream.h>

class Player : public Coroutine {
public: 
    int Id;
    Player *Next;
    Player(int id) : Id(id) {}

    void Routine() {
        int Sum = 0;
        while ((Sum += rand()%6+1) < 100)
            Resume(Next);
        cout << "The winner is player " << Id << endl;
    }
};

void DiceGame(int Players) {
    Player *FirstPlayer = new Player(1), *p = FirstPlayer;
    for (int i = 2; i <= Players; i++, p = p->Next) 
        p->Next = new Player(i);
    p->Next = FirstPlayer;
    Resume(FirstPlayer);		
}

int main() Sequencing(DiceGame(4))