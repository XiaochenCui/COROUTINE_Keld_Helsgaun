#include "coroutine.h"
#include <iostream.h>
#include <fstream.h>
#include <string.h>

char Word[20], Letter;
ifstream *TelegramFile;
Coroutine *theLetterProducer, *theWordProducer, *thePrinter;

class LetterProducer : public Coroutine {
    void Routine() {
        for (;;) {
            TelegramFile->get(Letter);
            if (Letter == '\n') 
                Letter = ' ';
            Resume(theWordProducer);
        }
    }
};

class WordProducer : public Coroutine {
    void Routine() {
        for (;;) {
            while (Letter == ' ') 
                Resume(theLetterProducer);
            char NextWord[21];
            NextWord[0] = '\0';	
            do {
                char NextLetter[1];
                NextLetter[0] = Letter;
                strncat(NextWord, NextLetter, 1);
                Resume(theLetterProducer);
            } while (Letter != ' ');
            if (!strcmp(NextWord, "STOP")) 
                strcat(Word, ".");
            else {
                if (strlen(Word))
                    Resume(thePrinter);
                if (!strcmp(Word, "ZZZZ") && !strcmp(NextWord, "ZZZZ"))
                    Detach();
                strcpy(Word, NextWord);
            }
        }
    }
};

class Printer : public Coroutine {
    void Routine() {
        int LineLength = 0;
        for (;;) {
            while (strcmp(Word, "ZZZZ")) {
                if (LineLength + strlen(Word) > 20) {
                    cout << endl;
                    LineLength = 0;
                }
                cout << Word << " ";
                LineLength += strlen(Word) + 1;
                Resume(theWordProducer);
            }
            cout << endl << endl;
            LineLength = 0;
            Resume(theWordProducer);
        }
    }
};

void TelegramFilter() {
    do {
        cout << "Enter file name: ";
        char FileName[80];
        cin >> FileName;
        delete TelegramFile;
        TelegramFile = new ifstream(FileName, ios::in);
    } while (!TelegramFile->is_open());

    theLetterProducer = new LetterProducer;
    theWordProducer = new WordProducer;
    thePrinter = new Printer;
    Resume(theLetterProducer);	
}

int main() Sequencing(TelegramFilter())