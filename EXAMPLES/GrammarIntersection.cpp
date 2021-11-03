#include "CBack.h"
#include <iostream.h>

const int strmax = 25;

enum type {alt, conc, term};    // rule forms
type ruletype[CHAR_MAX+1];      // rule types
char rule[CHAR_MAX+1][2];       // rules of grammars
char root1, root2;              // grammar root symbols
char str[strmax];               // string buffer
int lim;                        // length limit  
int ok, strfound;               // success signals

class parseboss : public Coroutine {
friend class genboss;
public:
    parseboss(genboss *genref) : genref(genref) {}
    void Routine();
    void parse(char goal);
    int parseptr;               // parser's string pointer
    genboss *genref;            // reference to generator
};

class genboss : public Coroutine {
friend class parseboss;
public:
    genboss(int k) : k(k) { parseref = new parseboss(this); }
    ~genboss() { delete parseref; }
private:
    void Routine();
    void generate(char goal, int substrlength);
    int k;                      // string length
    int genptr;                 // generator's string pointer 
	parseboss *parseref;          // reference to parser
};

void parseboss::Routine() {
    Notify(parseptr);           // backtrace parseptr
    parseptr = 0;               // initialize string porinter
    parse(root2);               // start up parser
    if (parseptr < genref->genptr + 1)   // parsed only prefix of string
    Backtrack();                             // so backup
    ok = 1;                     // signal success on last character
    strfound = 1;               // and overall success
    Detach();                   // await generator's pleasure
    strfound = 0;               // another character has arrived
    Backtrack();                    // so backup
}

 void parseboss::parse(char goal) {
    // find string spanned by goal, starting at parseptr
    switch(ruletype[goal]) {
    case alt:
        parse(rule[goal][Choice(2)-1]);
        break;
    case conc: 
        parse(rule[goal][0]);
        parse(rule[goal][1]);
        break;
    case term:
        if (parseptr > genref->genptr) {    // need another character
            ok = 1;             // so far, so good
            Detach();	          // if controls returns, we have it
        }
        if (str[parseptr] != rule[goal][0]) 
            Backtrack();
        parseptr++;
    }
}

void genboss::Routine() {
    Notify(genptr);            // backtrace genptr	
    genptr = 0;                // initialize string pointer
    generate(root1,k);         // start up generation
    if (!strfound)             // generator finshed, but not parser
        Backtrack();               // so back up
}

void genboss::generate(char goal, int k) {
	switch(ruletype[goal]) {
	case alt: 
        generate(rule[goal][Choice(2)-1],k);
        break;
	case conc:
        int j = Choice(k-1);   // form all 2-partions of k
        generate(rule[goal][0],j);
        generate(rule[goal][1],k-j);
        break;
	case term: 
        if (k > 1) 
            Backtrack();
        str[genptr] = rule[goal][0];
        ok = 0;                // clear signal
        Call(parseref);        // give char. to parser
        if (!ok)               // parser could not oblige
            Backtrack();           // so back up
        genptr++;              // otherwise, onward
	}
}

void GrammarIntersection() {
    ruletype['S'] = alt;  rule['S'][0] = 'T'; rule['S'][1] = 'U';
    ruletype['T'] = conc; rule['T'][0] = 'W'; rule['T'][1] = 'S';
    ruletype['W'] = term; rule['W'][0] = 'b';
    ruletype['U'] = conc; rule['U'][0] = 'A'; rule['U'][1] = 'X';
    ruletype['X'] = term; rule['X'][0] = 'c';

    ruletype['A'] = alt;  rule['A'][0] = 'B'; rule['A'][1] = 'C';
    ruletype['B'] = term; rule['B'][0] = 'a';
    ruletype['C'] = alt;  rule['C'][0] = 'A'; rule['C'][1] = 'D';
    ruletype['D'] = alt;  rule['D'][0] = 'X'; rule['D'][1] = 'E';
    ruletype['E'] = alt;  rule['E'][0] = 'W'; rule['E'][1] = 'B';

    root1 = 'S'; root2 = 'A'; lim = 2;

    BreadthFirst = 1;	// use breadth-first search
    strfound = 0;
    int k = 0; 
    while (k < lim && !strfound) {
        // see if a common string of length k+1 exists
        k++;
        genboss *g = new genboss(k);
        Call(g);
        delete g;
        ClearAll();
    }
    if (strfound) {
        cout << "Success, string = ";
        for (int i = 0; i < k; i++) 
            cout << str[i];
        cout << endl;
    }
    else
        cout << "No common string of length less than or equal to "
             << lim << endl;
}

int main() Backtracking(GrammarIntersection())