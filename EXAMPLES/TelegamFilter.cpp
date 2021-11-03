#include "coroutine.h"
#include <iostream>
#include <fstream>
#include <string.h>

const int MAX_WORD_LENGTH = 100;

char Word[MAX_WORD_LENGTH], Letter;
std::ifstream *TelegramFile;
Coroutine *theLetterProducer, *theWordProducer, *thePrinter;

class LetterProducer : public Coroutine
{
    void Routine()
    {
        for (;;)
        {
            TelegramFile->get(Letter);
            if (Letter == '\n')
                Letter = ' ';
            if (Letter == '\t')
                Letter = ' ';
            Resume(theWordProducer);
        }
    }
};

class WordProducer : public Coroutine
{
    void Routine()
    {
        for (;;)
        {
            while (Letter == ' ')
                Resume(theLetterProducer);
            char NextWord[MAX_WORD_LENGTH];
            NextWord[0] = '\0';
            do
            {
                char NextLetter[1];
                NextLetter[0] = Letter;

                // int wordLength = strlen(NextWord);
                // std::cout << std::endl << "length: " << wordLength << std::endl;

                strncat(NextWord, NextLetter, 1);
                Resume(theLetterProducer);
            } while (Letter != ' ');
            if (!strcmp(NextWord, "STOP"))
                strcat(Word, ".");
            else
            {
                if (strlen(Word))
                    Resume(thePrinter);
                if (!strcmp(Word, "ZZZZ") && !strcmp(NextWord, "ZZZZ"))
                    Detach();
                strcpy(Word, NextWord);
            }
        }
    }
};

class Printer : public Coroutine
{
    void Routine()
    {
        int LineLength = 0;
        for (;;)
        {
            while (strcmp(Word, "ZZZZ"))
            {
                if (LineLength + strlen(Word) > 20)
                {
                    std::cout << std::endl;
                    LineLength = 0;
                }
                std::cout << Word << " ";
                LineLength += strlen(Word) + 1;
                Resume(theWordProducer);
            }
            std::cout << std::endl
                      << std::endl;
            LineLength = 0;
            Resume(theWordProducer);
        }
    }
};

void TelegramFilter()
{
    do
    {
        std::cout << "Enter file name: ";
        char FileName[80];
        std::cin >> FileName;
        delete TelegramFile;
        TelegramFile = new std::ifstream(FileName, std::ios::in);
    } while (!TelegramFile->is_open());

    theLetterProducer = new LetterProducer;
    theWordProducer = new WordProducer;
    thePrinter = new Printer;
    Resume(theLetterProducer);
}

int main() Sequencing(TelegramFilter())