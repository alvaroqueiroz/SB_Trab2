#include "translator.h"

//#define __DEBUG2__

int translator (list <Token> & tokenlist, char * s){
	list<Token>::iterator it;
	ofstream nasmfile( s, ios_base::app);  //opens NASM file in output mode - always writes at end (append)
	if (nasmfile.is_open())
		nasmfile << "global      _start" << endl;
	else{
		cout << "Falha na criação ou abertura do arquivo." << endl;
		exit(EXIT_FAILURE);
	}
	nasmfile.close();

	for (it = tokenlist.begin();it != tokenlist.end(); it++){	//scans whole file
		switch (it->type){
			case TT_MNEMONIC:		//check OPCODE table
				#ifdef __DEBUG2__
				cout << "foi-mnemonic - " << it->str << endl;
				#endif
				transl_mnemonic(it, s);
			break;

			case TT_LABEL:
				#ifdef __DEBUG2__
				cout << "foi-label - " << it->str << endl;
				#endif
				transl_label(it, s);
			break;

			case TT_DIRECTIVE:
				#ifdef __DEBUG2__
				cout << "foi-directive - " << it->str << endl;
				#endif
				transl_directive(it, s);
			break;
			case TT_OPERAND          	:
			case TT_CONST        		:
			case TT_COMMA_OPERATOR   	:
			case TT_PLUS_OPERATOR		:
			case TT_AMPERSAND_OPERATOR 	:
				#ifdef __DEBUG2__
				cout << "foi-outro - " << it->str << endl;
				#endif
			break;
			default:
				cerr << "Parser: unknowm token type (" << it->str << ")." << endl;
				it++;
			break;
		}
	}
	return 0;
}

list<Token>::iterator transl_mnemonic(list<Token>::iterator it, char * s){
	ofstream nasmfile( s, ios::out | ios::app);  //opens NASM file in output mode - always writes at end (append)
	switch (it->addit_info){
		 case OP_ADD      :
			if (nasmfile.is_open()){
			 	nasmfile << "add eax, ";
                it++;
                if (it->type == TT_CONST) {
                    nasmfile << it->str << endl;
                }else{
                    nasmfile << "[" << it->str << "]" << endl;
                }
            //    it++;

			}else{
		 		cout << "Falha na criação ou abertura do arquivo." << endl;
		 		exit(EXIT_FAILURE);
			}

		 break;
		 case OP_SUB      :
		 if (nasmfile.is_open()){
			nasmfile << "sub eax, ";
             it++;
             if (it->type == TT_CONST) {
                 nasmfile << it->str << endl;
             }else{
                 nasmfile << "[" << it->str << "]" << endl;
             }
             it++;

         }else{
			cout << "Falha na criação ou abertura do arquivo." << endl;
			exit(EXIT_FAILURE);
		}

		 break;
		 case OP_MULT     :
		 if (nasmfile.is_open()){
			 nasmfile << "mov edx, 0" << endl;
			 nasmfile << "imul ";
			 it++;
			 if (it->type == TT_CONST) {
				 nasmfile << it->str << endl;
			 }else{
				 nasmfile << "DWORD[" << it->str << "]" << endl;
			 }
			 nasmfile << "cmp edx, 0xffffffff" << endl;
			 nasmfile << "je Overflow" << endl;
			 nasmfile << "jmp NotOverflow" << endl;
			 nasmfile << "Overflow:" << endl;
			 nasmfile << "push eax"<< endl;
			 nasmfile << "push ebx"<< endl;
			 nasmfile << "push ecx"<< endl;
			 nasmfile << "push edx"<< endl;

			 nasmfile << "mov eax, 4"<< endl;
			 nasmfile << "mov ebx, 1"<< endl;
			 nasmfile << "mov ecx, 'OVFL' " << endl;

			 nasmfile << "mov edx, 1" << endl;
			 nasmfile << "int 0x80"<< endl;

			 nasmfile << "pop edx"<< endl;
			 nasmfile << "pop ecx"<< endl;
			 nasmfile << "pop ebx"<< endl;
			 nasmfile << "pop eax"<< endl;
			 nasmfile << "NotOverflow:" << endl;
		 }else{
			 cout << "Falha na criação ou abertura do arquivo." << endl;
			 exit(EXIT_FAILURE);
		 }
		 break;

		 case OP_DIV      :
			 if (nasmfile.is_open()){
				 nasmfile << "push edx" << endl << "idiv ";
                 it++;
                 if (it->type == TT_CONST) {
                     nasmfile << it->str << endl;
                 }else{
                     nasmfile << "DWORD[" << it->str << "]" << endl;
                 }
				 nasmfile << "pop edx" << endl;
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }
		 break;

		 case OP_JMP      :
			 if (nasmfile.is_open()){
				 nasmfile << "jmp ";
				 it++;
				 nasmfile << it->str << endl;
				 it++;
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }

		 break;
		 case OP_JMPN     :
			 if (nasmfile.is_open()){
				 nasmfile << "cmp eax, 0" << endl;
				 nasmfile << "jb ";
				 it++;
				 nasmfile << it->str << endl;
	//			 it++;
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }


		 break;
		 case OP_JMPP     :
			 if (nasmfile.is_open()){
				 nasmfile << "cmp eax, 0" << endl;
				 nasmfile << "ja ";
				 it++;
				 nasmfile << it->str << endl;
		//		 it++;
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }

		 break;
		 case OP_JMPZ     :
			 if (nasmfile.is_open()){
				 nasmfile << "cmp eax, 0" << endl;
				 nasmfile << "je ";
				 it++;
				 nasmfile << it->str << endl;
	//			 it++;
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }

		 break;
		 case OP_COPY     :
			 if (nasmfile.is_open()){
				 nasmfile << "mov ";
                 it++;
                 if (it->type == TT_CONST || it->str == "eax" || it->str == "ebx" || it->str == "ecx" || it->str == "edx" ) {
                     nasmfile << it->str;
                 }else{
                     nasmfile << "[" << it->str << "]";
                 }

                 nasmfile << " , ";
				 it++;
				 it++;
                 if (it->type == TT_CONST || it->str == "eax" || it->str == "ebx" || it->str == "ecx" || it->str == "edx" ) {
                     nasmfile << it->str << endl;
                 }else{
                     nasmfile << "[" << it->str << "]" << endl;
                 }
	//			 it++;
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }

			break;
		 case OP_LOAD     :
			 if (nasmfile.is_open()){
				 nasmfile << "mov eax, ";
				 it++;
                 if (it->type == TT_CONST || it->str == "eax" || it->str == "ebx" || it->str == "ecx" || it->str == "edx" ) {
                     nasmfile << it->str << endl;
                 }else{
                     nasmfile << "[" << it->str << "]" << endl;
                 }
	//			 it++;
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }

		 break;
		 case OP_STORE    :
			 if (nasmfile.is_open()){
				 nasmfile << "mov ";
				 it++;
				 if (it->type == TT_CONST || it->str == "eax" || it->str == "ebx" || it->str == "ecx" || it->str == "edx" ) {
					 nasmfile << it->str;
				 }else{
					 nasmfile << "[" << it->str << "]";
				 }

				 nasmfile << " , eax " << endl;
				 //			 it++;
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }


		 break;
		 case OP_INPUT    :
			 if (nasmfile.is_open()){
				 nasmfile << "push eax"<< endl;
				 nasmfile << "push ebx"<< endl;
				 nasmfile << "push ecx"<< endl;
				 nasmfile << "push edx"<< endl;

				 nasmfile << "mov eax, 3"<< endl;
				 nasmfile << "mov ebx, 1"<< endl;
				 nasmfile << "mov ecx, ";
				 it++;
				 nasmfile << it->str << endl;

				 nasmfile << "mov edx, 1" << endl;
				 nasmfile << "int 0x80"<< endl;

				 nasmfile << "pop edx"<< endl;
				 nasmfile << "pop ecx"<< endl;
				 nasmfile << "pop ebx"<< endl;
				 nasmfile << "pop eax"<< endl;
//				 it++;
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }


		 break;
		 case OP_OUTPUT   :
			 if (nasmfile.is_open()){
				 nasmfile << "push eax"<< endl;
				 nasmfile << "push ebx"<< endl;
				 nasmfile << "push ecx"<< endl;
				 nasmfile << "push edx"<< endl;

				 nasmfile << "mov eax, 4"<< endl;
				 nasmfile << "mov ebx, 1"<< endl;
				 nasmfile << "mov ecx, ";
				 it++;
				 nasmfile << it->str << endl;

				 nasmfile << "mov edx, 1" << endl;
				 nasmfile << "int 0x80"<< endl;

				 nasmfile << "pop edx"<< endl;
				 nasmfile << "pop ecx"<< endl;
				 nasmfile << "pop ebx"<< endl;
				 nasmfile << "pop eax"<< endl;
	//			 it++;

			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }



			break;
		 case OP_C_INPUT  :
			 if (nasmfile.is_open()){
				 nasmfile << "push eax"<< endl;
				 nasmfile << "push ebx"<< endl;
				 nasmfile << "push ecx"<< endl;
				 nasmfile << "push edx"<< endl;

				 nasmfile << "mov eax, 3"<< endl;
				 nasmfile << "mov ebx, 1"<< endl;
				 nasmfile << "mov ecx, ";
				 it++;
				 nasmfile << it->str << endl;

				 nasmfile << "mov edx, 1" << endl;
				 nasmfile << "int 0x80"<< endl;

				 nasmfile << "pop edx"<< endl;
				 nasmfile << "pop ecx"<< endl;
				 nasmfile << "pop ebx"<< endl;
				 nasmfile << "pop eax"<< endl;
	//			 it++;
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }


			break;
		 case OP_C_OUTPUT :
			 if (nasmfile.is_open()){
				 nasmfile << "push eax"<< endl;
				 nasmfile << "push ebx"<< endl;
				 nasmfile << "push ecx"<< endl;
				 nasmfile << "push edx"<< endl;

				 nasmfile << "mov eax, 4"<< endl;
				 nasmfile << "mov ebx, 1"<< endl;
				 nasmfile << "mov ecx, ";
				 it++;
				 nasmfile << it->str << endl;

				 nasmfile << "mov edx, 1" << endl;
				 nasmfile << "int 0x80"<< endl;

				 nasmfile << "pop edx"<< endl;
				 nasmfile << "pop ecx"<< endl;
				 nasmfile << "pop ebx"<< endl;
				 nasmfile << "pop eax"<< endl;
	//			 it++;

			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }


			break;
		 case OP_S_INPUT  :
			 if (nasmfile.is_open()){
				 nasmfile << "push eax"<< endl;
				 nasmfile << "push ebx"<< endl;
				 nasmfile << "push ecx"<< endl;
				 nasmfile << "push edx"<< endl;

				 nasmfile << "mov eax, 3"<< endl;
				 nasmfile << "mov ebx, 1"<< endl;
				 nasmfile << "mov ecx, ";
				 it++;
				 nasmfile << it->str << endl;

				 nasmfile << "mov edx, ";
				 it++;
				 nasmfile << it->str << endl;
				 it++;
				 nasmfile << "int 0x80"<< endl;

				 nasmfile << "pop edx"<< endl;
				 nasmfile << "pop ecx"<< endl;
				 nasmfile << "pop ebx"<< endl;
				 nasmfile << "pop eax"<< endl;

			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }


		 break;
		 case OP_S_OUTPUT :
			 if (nasmfile.is_open()){
				 nasmfile << "push eax"<< endl;
				 nasmfile << "push ebx"<< endl;
				 nasmfile << "push ecx"<< endl;
				 nasmfile << "push edx"<< endl;

				 nasmfile << "mov eax, 4"<< endl;
				 nasmfile << "mov ebx, 1"<< endl;
				 nasmfile << "mov ecx, ";
				 it++;
				 nasmfile << it->str << endl;

				 nasmfile << "mov edx, ";
				 it++;
				 nasmfile << it->str << endl;
				 it++;
				 nasmfile << "int 0x80"<< endl;

				 nasmfile << "pop edx"<< endl;
				 nasmfile << "pop ecx"<< endl;
				 nasmfile << "pop ebx"<< endl;
				 nasmfile << "pop eax"<< endl;

			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }


			break;
		 case OP_STOP     :
			 if (nasmfile.is_open()){
				 nasmfile << "mov eax, 1"<< endl;
				 nasmfile << "mov ebx, 0"<< endl;
				 nasmfile << "int 0x80"<< endl;

			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }

		 break;
		 case OP_BASIC_OP :      //"+, -, /, *, %"

		 break;

		 default:
			 cerr << "Parser: unknowm token type (" << it->str << ")." << endl;
//			 it++;
		 break;
	}
	nasmfile.close();
	return it;
}

list<Token>::iterator transl_label(list<Token>::iterator it, char * s){
	ofstream nasmfile( s, ios::out | ios::app);  //opens NASM file in output mode - always writes at end (append)
	if (nasmfile.is_open())
		nasmfile << it->str << " ";
	else{
		cout << "Falha na criação ou abertura do arquivo." << endl;
		exit(EXIT_FAILURE);
	}
//	it ++;
	nasmfile.close();
	return it;
}

list<Token>::iterator transl_directive(list<Token>::iterator it, char * s){
	int i =0;
	ofstream nasmfile( s, ios::out | ios::app);  //opens NASM file in output mode - always writes at end (append)
	switch (it->addit_info){
		case DIR_SECTION :
			if (nasmfile.is_open())
				nasmfile << "section ";
			else{
				cout << "Falha na criação ou abertura do arquivo." << endl;
				exit(EXIT_FAILURE);
			}
//			it ++;
		break;
		case DIR_SPACE :
            nasmfile << "resw 1" << endl;
//            it++;

		break;
		case DIR_CONST :
            nasmfile << "dw ";
            it++;
            nasmfile << it->str << endl;
//            it++;

		break;
		case DIR_EQU :
		if (nasmfile.is_open()){
			nasmfile << "%define ";
			it++;
			nasmfile << it->str << " ";
			it++;
			nasmfile << it->str << endl;
		}else{
			cout << "Falha na criação ou abertura do arquivo." << endl;
			exit(EXIT_FAILURE);
		}
		break;
		case DIR_IF :
		i = it->line_number;
		if (nasmfile.is_open()){
			nasmfile << "%if<";
			it++;
			while (it->line_number == i){
				nasmfile << it->str << " ";
				it++;
			}
			nasmfile << ">" << endl;
			i++;
			while (it->line_number == i){
				nasmfile << it->str << " ";
				it++;
			}
			nasmfile << endl;
		}else{
			cout << "Falha na criação ou abertura do arquivo." << endl;
			exit(EXIT_FAILURE);
		}
		break;
		case DIR_MACRO :

		break;
		case DIR_ENDMACRO :

		break;
		case DIR_TEXT :
			if (nasmfile.is_open())
				nasmfile << ".text" << endl << "_start: " << endl;
			else{
				cout << "Falha na criação ou abertura do arquivo." << endl;
				exit(EXIT_FAILURE);
			}
//			it ++;
		break;
		case DIR_DATA :
			if (nasmfile.is_open())
				nasmfile << ".data" << endl;
			else{
				cout << "Falha na criação ou abertura do arquivo." << endl;
				exit(EXIT_FAILURE);
			}
//			it ++;
		break;
		case DIR_BEGIN :

		break;
		case DIR_END :

		break;
		case DIR_EXTERN :

		break;
		case DIR_PUBLIC :

		break;
		case DIR_BSS :
			if (nasmfile.is_open())
				nasmfile << ".bss" << endl;
			else{
				cout << "Falha na criação ou abertura do arquivo." << endl;
				exit(EXIT_FAILURE);
			}
//			it ++;
		break;

		default:
			cerr << "Parser: unknowm token type (" << it->str << ")." << endl;
//			it++;
		break;
		}
		nasmfile.close();
	return it;
}
