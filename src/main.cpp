/*
Universidade de Brasilia - 02/2018
CIC 116432 - Software Basico - Turma B
Professor Bruno Macchiavello
Trabalho Pratico 2 - Tradutor

Alunos: 
        Alvaro Queiroz dos Reis Silva       15/0155280
        Ian Moura Alexandre                 15/0129661
*/


#include "scanner.h"
#include "translator.h"
using namespace std;

//#define __DEBUG__

int error_found;


int main (int argc, char* argv[]){
    list<Token> tokenlist, labellist;
    list<Token>::iterator it, aux;
   
    char* inp_filename = argv[1];       //defines input file name
                                          
    char out_filename[] = "arquivo.s";       //defines input file name

    scanner(inp_filename , tokenlist, labellist);     //creates tokenlist with educational assembly code

    #ifdef __DEBUG__
        print_list (tokenlist);
    #endif

    translator(tokenlist, out_filename);                     //uses parser structure to translate code


    return 0;
}
