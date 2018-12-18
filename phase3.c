//Armando Alvear
//0254934
//Phase 3 || main.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pass1.h"
#include "pass2.h"

void fexit(char *a){
	*a = '1';
}

void fffread(char *comm, char *cFirst, char *argues,int *argNum){
	char commandFirst[256];
	char arguments[256];
	int argumentNumber = 0;
	
	if(comm[0] == ' ' || comm[0] == '\n' || comm[0] == '\0')
		printf("Error in spaced syntax; Type 'help' for assistance.");	
	else{
		int c = 0;
		int d = 0;
		
		
		while(comm[c] != '\0'){
			if(argumentNumber==0 && (comm[c] != ' ' && comm[c] != '\n') //read the NONEMPTY input
				&& comm[c+1] != ' ' && comm[c+1] != '\0' && comm[c+1] != '\n'){
					//printf("0");																														//dlt
					commandFirst[c] = comm[c];
					c++;//commandFirst[0] -> commandFirst[final]
			}
			else if(argumentNumber==0 && (comm[c] != ' ' && comm[c] != '\n')
				&& (comm[c+1] == ' ' || (comm[c+1] == '\0' || comm[c+1] == '\n'))){
					//printf("9");																														//dlt
					if(comm[c+1] == '\0')
						strcpy(arguments, "nil3");
					if(comm[c+1] == '\n')
						strcpy(arguments, "nil4");
					commandFirst[c] = comm[c];
					commandFirst[c+1] = '\0';
					c++;//commandFirst[final] -> space-before-args-command1st  || commamdFirst[final] -> 1stendgame
			}
			else if(argumentNumber==0 && comm[c] == ' ' //proper transition
				&& comm[c+1] != ' ' && comm[c+1] != '\0' && comm[c+1] != '\n'){
					//printf("8");																														//dlt
					argumentNumber++;										
					c++;// space-b4-args-command1st -> arguments[block]
			}
			else if(argumentNumber==0 && comm[c] == '\n'){//argNum needed?
				//printf("n");
				c++;//endgame -> endWhile
			}
			else if(argumentNumber==0 && comm[c] == ' ' && comm[c+1] == ' '){
				printf("s");																															//dlt
				c++;// space-b4-args-command1st -> space-b4-args-command1st || arguments[space] -> arguments[space]/endgame
			}
			else if(argumentNumber == 0 && comm[c] == ' ' && (comm[c+1] == '\0' || comm[c+1] == '\n')){
				//printf("6");																															//dlt
				strcpy(arguments, "nil2");	
				c++;//after command -> endgame	
				}															//necessary?
			else if(argumentNumber > 0 && (comm[c] != ' ' && comm[c] != '\n') //typical argument not in space
				&& (comm[c+1] != '\0' && comm[c+1] != '\n')){
					//printf("5");																														//dlt
					arguments[d] = comm[c];
					c++;
					d++;//arguments[block] -> arguments[space-between-NXTblock]||arguments[space-that-leads-nowhere]
			}
			else if(argumentNumber > 0 && (comm[c] != ' ' && comm[c] != '\n') //argument is end times read from final block
				 && (comm[c+1] == '\0' || comm[c+1] == '\n')){
				 	//printf("4");																														//dlt
					arguments[d] = comm[c];
					d++;//arguments[end-of-block] -> arguments[FINAL]
					arguments[d] = '\0';
					c++;//should be end of input
			}
			else if(argumentNumber > 0 && comm[c] == ' ' //proper link; makes ' '
				&& comm[c+1] != ' ' && (comm[c+1] != '\0' && comm[c+1] != '\n')){
					//printf("3");																														//dlt
					argumentNumber++;												//*argumentNumber = *argumentNumber + 1 if not working
					arguments[d] = comm[c];//arguments[space-between-block] -> arguments[NXTblock]
					c++;
					d++;
			}
			else if(argumentNumber > 0 && comm[c] == ' ' //if extra space after the final block
				&& (comm[c+1] == '\0' || comm[c+1] == '\n')){
					//printf("2");																														//dlt
					arguments[d] = '\0';
					c++;
			}
			else if(argumentNumber > 0 && comm[c] == ' '
				&& comm[c+1] == ' '){//necessary?
				//printf("S");																															//dlt
				c++;//necessary?
			}
			else if(argumentNumber>0 && comm[c] == '\n'){//argNum needed?
				//printf("N");
				c++;//endgame -> endWhile
			}
		}//end while
		
		strcpy(cFirst, commandFirst);
		strcpy(argues, arguments);
		*argNum = argumentNumber;
			
	}//end total else
}//void read


void help(){
	system("@cls||clear");
	printf("List of available Commands:\n"
		   "----------------------------------\n"
			"exit\t\t\t-\tExits the Loop.\n"
			"assembly\t\t-\tDoes pass1 assembly\n"
			"assemble <filename>\t-\tWill assemble an SIC assembly language program into a load tmodule and store it in a file.\n"
			"load <filename>\t\t-\tWill call the load function to load specified file.\n"
			"dir\t\t\t-\tWill list files in current directory.\n"
			"dump start end\t\t-\tWill call the dump function, passing the values of start and end."
			"debug\t\t\t-\tWill allow you to execute in debug mode.\n"
			"execute\t\t\t-\tWill call the computer simulation program to execute the program that was previously loaded in memory.\n"
			"help\t\t\t-\tGives you these options again.\n");
}


void assemble(char *argue){
	process1(argue);
	process2();
}

//Interpreter
int main(){
	FILE *fileCheck;
	char a = '0';
	char command[256];
	char commandFirst[256];
	char arguments[256];
	int argumentNumber = 0;
	system("@cls||clear");
	printf("Welcome to the command line on Linux!\nType 'help' for assistance.\n");

	do{
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		printf("\ncommand::");
	
		do{
			fgets(command, 256, stdin);
		}while(command[0] == '\0' || command[0] == ' ');//or strcmp(command, "")==0 if desparate

		fffread(command, commandFirst, arguments, &argumentNumber);
		
		if (strcmp(commandFirst, "exit")==0){
			printf("\nGoodbye!\n\n");
			fexit(&a); 
		}
		else if(strcmp(commandFirst, "assemble")==0){
			
			if(argumentNumber != 1)
				printf("\nError: Not one argument.\nType 'help'.\n");
			else{
				fileCheck = fopen(arguments, "r");
				if(fileCheck == NULL){
					printf("\nError: Not a real filename\nHint, it's one of these:\n"
						   "testyeserrors.txt\n"
						   "testnoerrors.txt\n");
					fclose(fileCheck);
				}
				else if(strcmp(arguments, "testyeserrors.txt")==0 || strcmp(arguments, "testnoerrors.txt")==0){
						fclose(fileCheck);	
						assemble(arguments);	
						}
				else{
					fclose(fileCheck);
					printf("\nError: May be a file, but not designated source files.\n");
				}
			}
		}
		
		else if(strcmp(commandFirst, "dir")==0)
			system("ls");
		else if(strcmp(commandFirst, "help")==0)
			help();
		else
			printf("\nType 'help' for assistance.");												
	
	strcpy(commandFirst, "");
	strcpy(arguments, "");
	argumentNumber = 0;
	}while(a == '0');
	
	return 0;
}



