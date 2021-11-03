#include "coroutine.h"
#include <iostream.h>

const int CardLength = 8, LineLength = 12;
char Card[CardLength], Line[LineLength], c1, c2;
Coroutine *theReader, *theDisassembler, *theSquasher, *theAssembler, *thePrinter; 

class Reader : public Coroutine {
    void Routine() {
        for (;;) {
            for (int i = 0; i < CardLength; i++)
                cin >> Card[i];
        Resume(theDisassembler);
    }
}
};

class Disassembler : public Coroutine {
    void Routine() {
        for (;;) {
            Resume(theReader);
            for (int i = 0; i < CardLength; i++) {
                c1 = Card[i];
                Resume(theSquasher);
            }
            c1 = ' ';
            Resume(theSquasher);
        }
    }
};


class Squasher : public Coroutine {
    void Routine() {
        for (;;) {
            if (c1 == '*') {
                Resume(theDisassembler);
                if (c1 == '*') {
                    c2 = '^';
                Resume(theAssembler);
            }
            else {
                c2 = '*';
                Resume(theAssembler);
                c2 = c1;
            }
        }
        else
            c2 = c1;
            Resume(theAssembler);
            Resume(theDisassembler); 
        }
    }
};

class Assembler : public Coroutine {
    void Routine() {
        for (;;) {
            for (int i = 0; i < LineLength; i++) {
                Line[i] = c2;
            if (c2 == '€') {
                while (++i < LineLength)
                    Line[i] = ' ';
                Resume(thePrinter);
                Detach();    // back to main program
            }
            Resume(theSquasher);
        }
        Resume(thePrinter);
    }
}	
};

class Printer : public Coroutine {
    void Routine() {
        for (;;) {
            for (int i = 0; i < LineLength; i++) 
                cout << Line[i];
            cout << endl;
            Resume(theAssembler);
        }
    }	
};

void TextTransformation() {
    theReader = new Reader();
    theDisassembler = new Disassembler();
    theSquasher = new Squasher();
    theAssembler = new Assembler();
    thePrinter = new Printer();
    Resume(theDisassembler);
}

int main() Sequencing(TextTransformation())