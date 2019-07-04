#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

#include "scanner.h"

using namespace std;

int translator(list <Token> & tokenlist, char * s);

//void transl_const(list<Token>::iterator it, char * s);
void printios(char * s, vector<int> & io);
list<Token>::iterator transl_mnemonic(list<Token>::iterator it, char * s);
list<Token>::iterator transl_label(list<Token>::iterator it, char * s);
list<Token>::iterator transl_directive(list<Token>::iterator it, char * s, int & linhaif);

extern int pre_error;

#endif /* translator_H_ */
