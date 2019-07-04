#include "translator.h"

//#define __DEBUG2__

int translator (list <Token> & tokenlist, char * s){
	list<Token>::iterator it, aux;
	vector<int> io;
	int linhaif = -1;
	ofstream nasmfile( s, ios_base::out);  //opens NASM file in output mode - always writes at end (append)
	if (nasmfile.is_open())
		nasmfile << "global      _start" << endl;
	else{
		cout << "Falha na criação ou abertura do arquivo." << endl;
		exit(EXIT_FAILURE);
	}
	nasmfile.close();

	for (it = tokenlist.begin();it != tokenlist.end(); it++){	//scans whole file
		if(linhaif == it->line_number){
			ofstream nasmfile( s, ios_base::out | ios::app);
			nasmfile << "_skip: \n";
			nasmfile.close();
			linhaif = -1;
		}
		switch (it->type){
			case TT_MNEMONIC:		//check OPCODE table
				#ifdef __DEBUG2__
				cout << "foi-mnemonic - " << it->str << endl;
				#endif
				if(it->addit_info >= OP_INPUT && it->addit_info <= OP_H_OUTPUT){
					io.push_back(it->addit_info);
				}
				transl_mnemonic(it, s);
			break;
			case TT_LABEL:
				#ifdef __DEBUG2__
				cout << "foi-label - " << it->str << endl;
				#endif
				transl_label(it, s);
			break;
			case TT_DIRECTIVE:
				if(it->addit_info == DIR_SECTION){
					aux = it;
					aux++;
					if(aux->type == TT_DIRECTIVE && aux->addit_info == DIR_DATA){
						printios(s, io);
					}
				}
				#ifdef __DEBUG2__
				cout << "foi-directive - " << it->str << endl;
				#endif
				transl_directive(it, s, linhaif);
			break;
			case TT_OPERAND          	:
				transl_operand(it, s);
			break;
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
			break;
		}		
	}
	return 0;
}

void printios(char * s, vector<int> & io){
	ofstream nasmfile( s, ios::out | ios::app);
	unordered_set<int> aux;
	for (int i : io)
	    aux.insert(i);
	io.assign( aux.begin(), aux.end() );
	sort( io.begin(), io.end() );

	for (auto it = io.begin(); it != io.end(); it++){
		ifstream infile(to_string(*it)+".txt");
		nasmfile << infile.rdbuf();
		infile.close();
	}
	nasmfile << "\n";
	nasmfile.close();


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
			 nasmfile << "imul ";
			 it++;
			 if (it->type == TT_CONST) {
				 nasmfile << it->str << endl;
			 }else{
				 nasmfile << "DWORD [" << it->str << "]" << endl;
			 }
			 nasmfile << "sub eax, 0x30" << endl;

		 }else{
			 cout << "Falha na criação ou abertura do arquivo." << endl;
			 exit(EXIT_FAILURE);
		 }
		 break;

		 case OP_DIV      :
			 if (nasmfile.is_open()){
				 nasmfile << "cdq\nidiv ";
                 it++;
                 if (it->type == TT_CONST) {
                     nasmfile << it->str << endl;
                 }else{
                     nasmfile << "DWORD[" << it->str << "]" << endl;
                 }
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
				 nasmfile << "mov eax, ";
                 it++; 
                 nasmfile << "[" << it->str << "]\nmov ";
				 it++;
				 it++;
                 nasmfile << "[" << it->str << "], eax" << endl;
                 
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
				 nasmfile << "pusha\npush dword [";
				 it++;
				 nasmfile << it->str << "]\n call leInteiro\npopa\n";
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }

		 break;
		 case OP_OUTPUT   :
			 if (nasmfile.is_open()){
				 nasmfile << "pusha\npush dword [";
				 it++;
				 nasmfile << it->str << "]\n call escreveInteiro\npopa\n";
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }
			break;
		 case OP_C_INPUT  :
			 if (nasmfile.is_open()){
				 nasmfile << "pusha\npush dword [";
				 it++;
				 nasmfile << it->str << "]\n call leCaractere\npopa\n";
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }

			break;
		 case OP_C_OUTPUT :
			 if (nasmfile.is_open()){
				 nasmfile << "pusha\npush dword [";
				 it++;
				 nasmfile << it->str << "]\n call escreveCaractere\npopa\n";
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }

			break;
		 case OP_S_INPUT  :
			 if (nasmfile.is_open()){
				 nasmfile << "pusha\npush dword [";
				 it++;
				 nasmfile << it->str << "]\npush dword [";
				 it++;
				 nasmfile << it->str << "]\n";
				nasmfile<<"call leString\npopa\n";
			 }else{
				 cout << "Falha na criação ou abertura do arquivo." << endl;
				 exit(EXIT_FAILURE);
			 }



		 break;
		 case OP_S_OUTPUT :
			 if (nasmfile.is_open()){
				 nasmfile << "pusha\npush dword [";
				 it++;
				 nasmfile << it->str << "]\npush dword [";
				 it++;
				 nasmfile << it->str << "]\n";
				nasmfile<<"call escreveString\npopa\n";
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
		 case OP_H_INPUT:
		 	if (nasmfile.is_open()){
				nasmfile << "pusha\ncall _inputHEX\npopa\n";
			}	 
		 case OP_H_OUTPUT:
			if (nasmfile.is_open()){
				nasmfile << "pusha\npush dword [hexnum]\ncall _printHEX\npopa\n";
			}
		 break;
		 case OP_BASIC_OP :      //"+, -, /, *, %"

		 break;

		 default:
			 //cerr << "Parser: unknowm token type (" << it->str << ")." << endl;
//			 it++;
		 break;
	}
	nasmfile.close();
	return it;
}
void transl_label(list<Token>::iterator it, char * s){
	ofstream nasmfile( s, ios::out | ios::app);
	if (nasmfile.is_open()){
		nasmfile << it->str << " ";
	}else{
		cout << "Falha na criação ou abertura do arquivo." << endl;
		exit(EXIT_FAILURE);
	}
	nasmfile.close();
}
list<Token>::iterator transl_operand(list<Token>::iterator it, char * s){
	ofstream nasmfile( s, ios::out | ios::app);  //opens NASM file in output mode - always writes at end (append)
	list<Token>::iterator a;
	a = it;
	a++;
	if (nasmfile.is_open()){
		if(a->type == TT_DIRECTIVE){
			if(a->addit_info >= DIR_SPACE && a->addit_info <= DIR_EQU){
				nasmfile << it->str << " ";
			}
		}
	}else{
		cout << "Falha na criação ou abertura do arquivo." << endl;
		exit(EXIT_FAILURE);
	}
//	it ++;
	nasmfile.close();
	return it;
}
bool data_bss_flag = false;
list<Token>::iterator transl_directive(list<Token>::iterator it, char * s, int & linhaif){

	ofstream nasmfile( s, ios::out | ios::app);  //opens NASM file in output mode - always writes at end (append)
	switch (it->addit_info){
		break;
		case DIR_SPACE :
			it--;
			nasmfile << it->str;
            nasmfile << " times ";
			it++;
			it++;

			if (it->type == TT_CONST){
				nasmfile << it->str << " ";
			}else{
				nasmfile << "1 ";
			}
			nasmfile << " dd 0\n";
		break;
		case DIR_CONST :
			it--;
			nasmfile << it->str;
            nasmfile << " dd ";
            it++;
			it++;
			nasmfile << it->str << "\n";

		break;
		case DIR_EQU :
		if (nasmfile.is_open()){
			it--;
			nasmfile << it->str;
			nasmfile << " EQU ";
			it++;
			it++;
			nasmfile << it->str << "\n";
		}else{
			cout << "Falha na criação ou abertura do arquivo." << endl;
			exit(EXIT_FAILURE);
		}
		break;
		case DIR_IF :
		if (nasmfile.is_open()){
			nasmfile << "mov ebx, ";
			it++;
			nasmfile << it->str << " \n";
			nasmfile << "cmp ebx, 1\npop ebx\njne _skip\n";
			
			linhaif = it->line_number + 2;

		}else{
			cout << "Falha na criação ou abertura do arquivo." << endl;
			exit(EXIT_FAILURE);
		}
		break;

		case DIR_TEXT :
			if (nasmfile.is_open())
				nasmfile << "section .text" << endl << "_start: " << endl;
			else{
				cout << "Falha na criação ou abertura do arquivo." << endl;
				exit(EXIT_FAILURE);
			}
//			it ++;
		break;
		case DIR_DATA :
			if (!data_bss_flag) {
				data_bss_flag = !data_bss_flag;
				if (nasmfile.is_open())
					nasmfile << "section .data" << endl;
				else{
					cout << "Falha na criação ou abertura do arquivo." << endl;
					exit(EXIT_FAILURE);
				}
			}
		break;

		case DIR_BSS :
			if (!data_bss_flag) {
				data_bss_flag = !data_bss_flag;
				if (nasmfile.is_open())
					nasmfile << "section .data" << endl;
				else{
					cout << "Falha na criação ou abertura do arquivo." << endl;
					exit(EXIT_FAILURE);
				}
			}
		break;

		default:
			if (it->addit_info != DIR_SECTION)
				cerr << "Parser: unknowm token type (" << it->str << ")." << endl;
//			it++;
		break;
		}
		nasmfile.close();
	return it;
}
