#ifndef SCANNER_H_
#define SCANNER_H_

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <list>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <unordered_set>
#include <algorithm>
//#include "pre_processor.h"
//#include "ifequ.h"



/*----------TOKEN-TYPE-TABLE----------*/
#define TT_MNEMONIC         	1
#define TT_LABEL            	2
#define TT_DIRECTIVE        	3
#define TT_OPERAND          	4
#define TT_CONST        		5
#define TT_COMMA_OPERATOR   	6
#define TT_PLUS_OPERATOR		7
#define TT_AMPERSAND_OPERATOR 	8
/*------------------------------------*/

/*------------OPCODE-TABLE------------*/
#define OP_ADD              1
#define OP_SUB              2
#define OP_MULT             3
#define OP_DIV              4
#define OP_JMP              5
#define OP_JMPN             6
#define OP_JMPP             7
#define OP_JMPZ             8
#define OP_COPY             9
#define OP_LOAD             10
#define OP_STORE            11
#define OP_STOP             12
#define OP_INPUT            13
#define OP_OUTPUT           14
#define OP_C_INPUT          15
#define OP_C_OUTPUT         16
#define OP_S_INPUT          19
#define OP_S_OUTPUT         20
#define OP_H_INPUT          21
#define OP_H_OUTPUT         22
#define OP_BASIC_OP			215
/*------------------------------------*/

/*----------DIRECTIVE-TABLE-----------*/
#define DIR_SECTION         1
#define DIR_SPACE           2
#define DIR_CONST           3
#define DIR_EQU             4
#define DIR_IF              5
#define DIR_MACRO           6
#define DIR_ENDMACRO        7
#define DIR_TEXT            8
#define DIR_DATA            9
#define DIR_BEGIN			10
#define DIR_END 			11
#define DIR_EXTERN			12
#define DIR_PUBLIC 			13
#define DIR_BSS             14
/*------------------------------------*/

#define INVALID_TOKEN       -1




/*--------------STRUCTS---------------*/
struct Token {
    std::string str;
    int line_number;
    int token_pos_il;
    int type;
    int addit_info;
    int flag;
    std::string info_str;
};
typedef struct Token Token;
/*------------------------------------*/



int scanner(char * file_name, std::list<Token> & tokenlist, std::list<Token> & labellist);
void print_list (std::list<Token> & tokenlist);
int identify_tokens (char * s, std::list<Token> & tokenlist);

void verify_tokens (std::list<Token> & tokenlist, std::list<Token> & labellist);
int categorize_token(Token & token, std::list<Token> & labellist, std::list<Token> & tokenlist);

int is_mnemonic(Token & token);
int is_label(Token & token, std::list<Token> & labellist);
int is_directive(Token & token);
int is_operand(Token & token, std::list<Token> & tokenlist);
int is_decimal(Token & token);
int is_hexadecimal(Token & token);

extern int error_found;

#endif /* SCANNER_H_ */
