//Armando Alvear
//0254934
//Phase 1 || Pass1.h

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void process1(char *filename){
	int opMnemonicNotFound = 1;
	int mulLabel = 0;
	FILE *f1,*f2,*f3,*f4;//f1 = sic code, f2 = optab, f3 = symtable, f4 = intermediate
   	int loc,sa,sicopimm,optabopc,len, symtabloc;								//t1.2
   	char sicmn[20],siclab[20],sicop[20],optabmn[20],symtablab[20]; 
	
	
  	f1=fopen(filename, "r");	
   	f3=fopen("symtab.txt","w+");								
	f4=fopen("intermediate.txt", "w");							

														
    fscanf(f1,"%s %s %s",siclab,sicmn,sicop);			//working code change: 	%d",...&sicopimm -> %s"...sicop 					
	if(strcmp(sicmn,"START")==0){		
		if(atoi(sicop)){
			sicopimm = atoi(sicop);
			sa=sicopimm;										
     			loc=sa;											
     			fprintf(f4,"-\t%s\t%s\t%d\n",siclab,sicmn,sicopimm);	//change: added '-' at beginning of string for pass2 int reader
		}
		else{
			fprintf(f4,"-\t%s\t%s\t%s\n",siclab,sicmn,sicop);
			fprintf(f4, "Error: Missing or illegal op to START directive");	
		}									
     	}												
     	else											
     		loc=0;											
														
														
		/*once the observation of starting location is done, now read lines*/			
    														
	fscanf(f1,"%s %s",siclab,sicmn);								
	while(!feof(f1)){										
		fscanf(f1,"%s",sicop);									
		fprintf(f4, "%d\t%s\t%s\t%s\n",loc,siclab,sicmn,sicop);					
														
		/*if label actually has something, then SYMTABLE has to note it*/			
		if(strcmp(siclab,"-")!=0){								
			rewind(f3);									
			if(f3 == NULL)									
				fprintf(f3,"%d\t%s\n",loc,siclab);					
			else{										
				fscanf(f3, "%d", &symtabloc);						
				while(!feof(f3)){							
					fscanf(f3, "%s", symtablab);					
					if(strcmp(symtablab, siclab)==0){				
						mulLabel = 1;						
						break;							
					}								
					else								
						mulLabel == 0;						
				}									
				fseek(f3, 0, SEEK_END);							
				if(mulLabel == 0)							
					fprintf(f3,"%d\t%s\n",loc,siclab);				
				else if(mulLabel == 1)							
					fprintf(f4, "%s",".\tERROR: Duplicate Label!\n");		
			}										
		}											
														
		/*Begin to search for matching mnemonics*/						
		f2=fopen("optab.txt","r");								
		fscanf(f2,"%s %d",optabmn,&optabopc);							
		while(!feof(f2)){									
														
			/*when there is matching menemonics*/						
			if(strcmp(sicmn,optabmn)==0){							
														
				if(strcmp(sicmn,"WORD")==0){						
					opMnemonicNotFound = 0;						
					loc=loc+3;							
					break;								
				}									
				else if(strcmp(sicmn,"RESW")==0){					
					opMnemonicNotFound = 0;						
					sicopimm=atoi(sicop);						
        				loc=loc+(3*sicopimm);						
					break;								
				}									
       				else if(strcmp(sicmn,"BYTE")==0){					
       					if(sicop[0]=='X'){						
						opMnemonicNotFound = 0;					
	  					loc=loc+1;						
						break;							
					}								
	  				else{								
						opMnemonicNotFound = 0;					
	  					len=strlen(sicop)-2;					
	  					loc=loc+len;						
						break;							
					}								
				}									
				else if(strcmp(sicmn,"RESB")==0){					
					opMnemonicNotFound = 0;						
	 				sicopimm=atoi(sicop);						
	 				loc=loc+sicopimm;						
					break;								
       	 			}									
				else if(strcmp(sicmn,"END")==0){					
					opMnemonicNotFound = 0;						
			        	printf("Program Assembled...\nPass One Complete...\nPass Two Complete...\n\nOutput files:\nintermediate.txt\nsymtab.txt\nlisting.txt\nobjectCode.txt\n");				
					break;								
				}//end specific psuedo mnemonic search					
														
				opMnemonicNotFound = 0;							
        			loc=loc+3;								
       				break;									
      			}										
			else										
				opMnemonicNotFound = 1;							
      			
			fscanf(f2,"%s %d",optabmn,&optabopc);//progess next line reading		
      		}//end the mnemonic search while loop							
		if(opMnemonicNotFound == 1)								
			fprintf(f4,"%s", ".\tERROR: Mnemonic not in optab!\n");				
      		
		fclose(f2);															
	      	fscanf(f1,"%s %s",siclab,sicmn);							
														
        }//end sic source reading while loop								
														
														
 	fclose(f1);											
 	fclose(f3);											
	fclose(f4);																						
}														
