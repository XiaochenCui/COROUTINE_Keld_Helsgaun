#include "coroutine.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int V;

class Tree : public Coroutine
{
    int Value;
    Tree *Left, *Right;

    void Routine()
    {
        if (V == -1)
        {
            Detach();
            V = -1;
            return;
        }
        Value = V;
        Tree *Left = new Tree, *Right = new Tree;
        for (;;)
        {
            Detach();
            if (V == -1)
                break;
            if (V <= Value)
                Left->Send(V);
            else
                Right->Send(V);
        }
        Left->Send(-1);
        Right->Send(-1);
        Detach();
        for (;;)
        {
            V = Left->Receive();
            if (V == -1)
                break;
            Detach();
        }
        V = Value;
        Detach();
        for (;;)
        {
            V = Right->Receive();
            if (V == -1)
                break;
            Detach();
        }
        V = -1;
    }

public:
    void Send(int InputValue)
    {
        V = InputValue;
        Call(this);
    }

    int Receive()
    {
        Call(this);
        return V;
    }
};

void BinaryInsertionSort()
{
    Tree *BST = new Tree;
    for (int i = 0; i < 100; i++)
        BST->Send(rand() % 100);
    BST->Send(-1);
    int i;
    while ((i = BST->Receive()) != -1)
        std::cout << i << " ";
    std::cout << std::endl;
}

int main() Sequencing(BinaryInsertionSort())
