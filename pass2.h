//Armando Alvear
//0254934
//Phase 2 || Pass.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*Decimal to Hex*/
char* GetSubString(char* str, int index, int count) {
	int strLen = strlen(str);
	int lastIndex = index + count;
	int i = 0;//int i
	if (index >= 0 && lastIndex > strLen) return "";

	char* subStr = malloc(count + 1);

	for (i = 0; i < count; i++) {//int i = 0
		subStr[i] = str[index + i];
	}

	subStr[count] = '\0';

	return subStr;
}

char* AppendString(const char* str1, const char* str2) {
	int str1Len = strlen(str1);
	int str2Len = strlen(str2);
	int strLen = str1Len + str2Len + 1;
	char* str = malloc(strLen);
	
	int i = 0;//int i
	for (i = 0; i < str1Len; i++)//int i - 0
		str[i] = str1[i];

	for (i = 0; i < str2Len; i++)//int i = 0
		str[(str1Len + i)] = str2[i];

	str[strLen - 1] = '\0';

	return str;
}

char* InsertString(char* str, int index, char* subStr) {
	char* s = GetSubString(str, 0, index);
	s = AppendString(s, subStr);
	s = AppendString(s, GetSubString(str, index, strlen(str) - index));

	return s;
}

char* CharToString(char c) {
	char* str = malloc(2);
	str[0] = c;
	str[1] = '\0';

	return str;
}

char* DecimalToHexadecimal(int dec) {
	if (dec < 1) return "0";

	int hex = dec;
	char* hexStr = "";

	while (dec > 0)
	{
		hex = dec % 16;

		if (hex < 10)
			hexStr = InsertString(hexStr, 0, CharToString(hex + 48));
		else
			hexStr = InsertString(hexStr, 0, CharToString(hex + 55));

		dec /= 16;
	}

	return hexStr;
}

/*Pass2 code*/
void process2(){

		
	char intaddr[10], intlab[20], intmn[10], intop[10];				//Intermediate scan: address, label, mnemonic, operand
	char opmne[10], opc[5];								//Optab scan: mnemonic, opcode
	char symadd[10], symlab[10];							//Symtab scan: address location, label
	int textRecB4End, finaladdr, startaddr, textRecLen, headerlength;				//correlated to fint's address value
	int len, realLen, byteIndex;
	int errorUSymbol = 0;								//undefined symbol error, not found is 0
	
	FILE *fint, *fobj, *flist, *fsymtab, *foptab; 					//optab may not be needed?
	
	
	
	fint = fopen("intermediate.txt", "r");
	fsymtab = fopen("symtab.txt", "r");	
	fobj = fopen("objectCode.txt", "w");
	flist = fopen("listing.txt", "w");
	foptab = fopen("optab.txt", "r");
	int flawedFile = 0;								//searches initial intermediate, 0 good 1 bad
	
	char buffy[256];
	/*the search for the final address for objcode purposes*/
	

	fscanf(fint,"%s%s%s%s",intaddr,intlab,intmn,intop);

	while(strcmp(intmn, "END") != 0){
		if(strcmp(intaddr, ".")==0){				//n10
			if(strcmp(intlab, "Error:")==0){		//n10
				flawedFile = 1;				//n10
				fprintf(fobj, "Flawed :^)");		//n11
			}						//n10
			fgets(buffy, 256, fint);			//n10
		}							//n10
		else							//n10
			textRecB4End = atoi(intaddr);
		
		fscanf(fint, "%s %s %s %s", intaddr, intlab, intmn, intop);
	}
	finaladdr = atoi(intaddr);
	//fclose(fint);									//t1.1
	//fint = fopen(textFileIntermediate, "r");						//t1.1	was "intermediate.txt"	CRUCIAL MISTAKE FOR DO LOOP
	fseek(fint, 0, SEEK_SET);							//t1.1
	
	
	fscanf(fint, "%s %s %s %s", intaddr, intlab, intmn, intop);
	if(strcmp(intmn, "START")==0){
		if(flawedFile == 0){//if okay									//n10
			if(strlen(DecimalToHexadecimal(atoi(intop)))==6)
				fprintf(fobj, "H^%s^%s", intlab, DecimalToHexadecimal(atoi(intop)));		//pass2fg has H^%s^%s^%d for header
			else if(strlen(DecimalToHexadecimal(atoi(intop)))==5)
				fprintf(fobj, "H^%s^0%s", intlab, DecimalToHexadecimal(atoi(intop)));
			else if(strlen(DecimalToHexadecimal(atoi(intop)))==4)
				fprintf(fobj, "H^%s^00%s", intlab, DecimalToHexadecimal(atoi(intop)));
			else if(strlen(DecimalToHexadecimal(atoi(intop)))==3)
				fprintf(fobj, "H^%s^000%s", intlab, DecimalToHexadecimal(atoi(intop)));
			else if(strlen(DecimalToHexadecimal(atoi(intop)))==2)
				fprintf(fobj, "H^%s^0000%s", intlab, DecimalToHexadecimal(atoi(intop)));
			else if(strlen(DecimalToHexadecimal(atoi(intop)))==1)
				fprintf(fobj, "H^%s^00000%s", intlab, DecimalToHexadecimal(atoi(intop)));
			else
				fprintf(fobj, "H^%s^eitherHexTooBigOrNonExistent", intlab);
		}												//n10
		fprintf(flist, "-\t%s\t%s\t%s\n", intlab, intmn, DecimalToHexadecimal(atoi(intop)));
		
		fscanf(fint, "%s %s %s %s", intaddr, intlab, intmn, intop);//not header at this point
		if(strcmp(intaddr, ".")==0){				//n10
			fgets(buffy, 256, fint);			//n10
			fscanf(fint, "%s %s %s %s", intaddr, intlab, intmn, intop);//not foretold line, logically next is right one	n10
		}							//n10

		startaddr = atoi(intaddr);
		headerlength = finaladdr - startaddr;
		if(flawedFile == 0){													//n10
			if(strlen(DecimalToHexadecimal(headerlength))==6)
				fprintf(fobj, "^%s\n", DecimalToHexadecimal(headerlength));//convert headerlength to Hex String
			else if(strlen(DecimalToHexadecimal(headerlength))==5)
				fprintf(fobj, "^0%s\n", DecimalToHexadecimal(headerlength));
			else if(strlen(DecimalToHexadecimal(headerlength))==4)
				fprintf(fobj, "^00%s\n", DecimalToHexadecimal(headerlength));
			else if(strlen(DecimalToHexadecimal(headerlength))==3)
				fprintf(fobj, "^000%s\n", DecimalToHexadecimal(headerlength));
			else if(strlen(DecimalToHexadecimal(headerlength))==2)
				fprintf(fobj, "^0000%s\n", DecimalToHexadecimal(headerlength));
			else if(strlen(DecimalToHexadecimal(headerlength))==1)
				fprintf(fobj, "^00000%s\n", DecimalToHexadecimal(headerlength));
			else
				fprintf(fobj, "^eitherHexTooBigOrNonExistant\n");
		}



		textRecLen = textRecB4End - startaddr;
		if(flawedFile == 0){													//n10
			if(strlen(DecimalToHexadecimal(atoi(intaddr)))==6)
				fprintf(fobj, "T^%s^%s", DecimalToHexadecimal(atoi(intaddr)), DecimalToHexadecimal(textRecLen));//startaddr here?
			else if(strlen(DecimalToHexadecimal(atoi(intaddr)))==5)
				fprintf(fobj, "T^0%s^%s", DecimalToHexadecimal(atoi(intaddr)), DecimalToHexadecimal(textRecLen));
			else if(strlen(DecimalToHexadecimal(atoi(intaddr)))==4)
				fprintf(fobj, "T^00%s^%s", DecimalToHexadecimal(atoi(intaddr)), DecimalToHexadecimal(textRecLen));
			else if(strlen(DecimalToHexadecimal(atoi(intaddr)))==3)
				fprintf(fobj, "T^000%s^%s", DecimalToHexadecimal(atoi(intaddr)), DecimalToHexadecimal(textRecLen));
			else if(strlen(DecimalToHexadecimal(atoi(intaddr)))==2)
				fprintf(fobj, "T^0000%s^%s", DecimalToHexadecimal(atoi(intaddr)), DecimalToHexadecimal(textRecLen));
			else if(strlen(DecimalToHexadecimal(atoi(intaddr)))==1)
				fprintf(fobj, "T^00000%s^%s", DecimalToHexadecimal(atoi(intaddr)), DecimalToHexadecimal(textRecLen));
			else
				fprintf(fobj, "T^eitherHexTooBigOrNonExistant^%s", DecimalToHexadecimal(textRecLen));
		}


		
	}
	
	
	while(strcmp(intmn, "END")!=0){
		if(strcmp(intaddr, ".")==0){				//n10
			fgets(buffy, 256, fint);			//n10
			fprintf(flist, "%s\t%s %s %s", intaddr, intlab, intmn, intop);
			fputs(buffy, flist);						//might need \n if doesnt separate
		}							//n10
		else{
			//rewind(foptab);
			fseek(foptab, 0, SEEK_SET);
			fscanf(foptab, "%s %s", opmne, opc);
			while(!feof(foptab)){// add breaks?
				if(strcmp(intmn, opmne)==0){
					if(strcmp(intmn, "BYTE")==0){
						fprintf(flist,"%s\t%s\t%s\t%s\t",DecimalToHexadecimal(atoi(intaddr)),intlab,intmn,intop);
						len = strlen(intop);
						realLen = len - 3; // minus <X> <'> <'> || <C> <'> <'>
						if(flawedFile == 0)										//n10
							fprintf(fobj, "^");
			
						if(intop[0] == 'C'){
							for(byteIndex = 2; byteIndex < (realLen + 2); byteIndex++){
								fprintf(flist, "%s", DecimalToHexadecimal((int)intop[byteIndex]));
								if(flawedFile == 0)								//n10
									fprintf(fobj, "%s", DecimalToHexadecimal((int)intop[byteIndex]));//t2.1 
							}
						}
						else if(intop[0] == 'X'){
							for(byteIndex = 2; byteIndex < (realLen + 2); byteIndex++){
								fprintf(flist, "%c", intop[byteIndex]);
								if(flawedFile == 0)								//n10
									fprintf(fobj, "%c", intop[byteIndex]);//t2.1 ifs for ^ overflow
							}
						} 
						fprintf(flist, "\n");
						break;//#####################################################################################
					}//byte	
					else if(strcmp(intmn, "WORD")==0){
						len=strlen(intop);//useful at all?
    						fprintf(flist,"%s\t%s\t%s\t%s\t",DecimalToHexadecimal(atoi(intaddr)),intlab,intmn,intop);
						
						if(strlen(DecimalToHexadecimal(atoi(intop)))==6)//should I've not converted dec to hex at all?
							fprintf(flist, "%s\n", DecimalToHexadecimal(atoi(intop)));
						else if(strlen(DecimalToHexadecimal(atoi(intop)))==5)
							fprintf(flist, "0%s\n", DecimalToHexadecimal(atoi(intop)));//h
						else if(strlen(DecimalToHexadecimal(atoi(intop)))==4)
							fprintf(flist, "00%s\n", DecimalToHexadecimal(atoi(intop)));
						else if(strlen(DecimalToHexadecimal(atoi(intop)))==3)
							fprintf(flist, "000%s\n", DecimalToHexadecimal(atoi(intop)));
						else if(strlen(DecimalToHexadecimal(atoi(intop)))==2)
							fprintf(flist, "0000%s\n", DecimalToHexadecimal(atoi(intop)));
						else if(strlen(DecimalToHexadecimal(atoi(intop)))==1)
							fprintf(flist, "00000%s\n", DecimalToHexadecimal(atoi(intop)));
						else
							fprintf(flist, "eitherHexDecTooBigOrNonExistant\n");
						
			
			
			
						if(flawedFile == 0){										//n10
							if(strlen(DecimalToHexadecimal(atoi(intop)))==6)//should I've not converted dec to hex at all?
								fprintf(fobj, "^%s", DecimalToHexadecimal(atoi(intop)));//combine with above?
							else if(strlen(DecimalToHexadecimal(atoi(intop)))==5)
								fprintf(fobj, "^0%s", DecimalToHexadecimal(atoi(intop)));
							else if(strlen(DecimalToHexadecimal(atoi(intop)))==4)
								fprintf(fobj, "^00%s", DecimalToHexadecimal(atoi(intop)));
							else if(strlen(DecimalToHexadecimal(atoi(intop)))==3)
								fprintf(fobj, "^000%s", DecimalToHexadecimal(atoi(intop)));
							else if(strlen(DecimalToHexadecimal(atoi(intop)))==2)
								fprintf(fobj, "^0000%s", DecimalToHexadecimal(atoi(intop)));
							else if(strlen(DecimalToHexadecimal(atoi(intop)))==1)
								fprintf(fobj, "^00000%s", DecimalToHexadecimal(atoi(intop)));
							else
								fprintf(fobj, "^eitherHexDecTooBigOrNonExistant");
						}
						break;//###########################################################################
				
					}//word				
					else if(strcmp(intmn, "RESW")==0 || strcmp(intmn, "RESB")==0){
						fprintf(flist, "%s\t%s\t%s\t%s\n", DecimalToHexadecimal(atoi(intaddr)), intlab, intmn, intop);
						break;//############################################################################
					}//resw or resb
					else{
						//fclose(foptab);						//t1.1
						rewind(fsymtab);
						fscanf(fsymtab, "%s %s", symadd, symlab);
						while(strcmp(intop, symlab)!=0 && !feof(fsymtab))
							fscanf(fsymtab, "%s %s", symadd, symlab);
						if(strcmp(intop, symlab)==0){
							fprintf(flist, "%s\t%s\t%s\t", DecimalToHexadecimal(atoi(intaddr)),intlab,intmn);
							fprintf(flist, "%s\t%s%s\n", intop, opc, DecimalToHexadecimal(atoi(symadd)));
							if(flawedFile == 0)								//n10
 	    							fprintf(fobj,"^%s%s",opc, DecimalToHexadecimal(atoi(symadd)));
						}
						else{
							fprintf(flist, "%s\t%s\t%s\t", DecimalToHexadecimal(atoi(intaddr)),intlab,intmn);
							fprintf(flist, "%s\t%s0000\n", intop, opc);
							if(flawedFile == 0)								//n10
    	 							fprintf(fobj,"^%s0000",opc);

							errorUSymbol = 1;
						}
						break;//##################################################					

					}//final else
				}//optab something found
			
				fscanf(foptab, "%s %s", opmne, opc);
			}//while loop optab finish
		
		
		}//SHOULD BE NORMAL ELSE IF NOT COMMENT LINE										//n10


		fscanf(fint, "%s %s %s %s", intaddr, intlab, intmn, intop);
	}//while loop intermediate looked through besides END line




	/*Final End reading*/
	
	fprintf(flist,"%s\t%s\t%s\t%s\n",DecimalToHexadecimal(atoi(intaddr)), intlab, intmn, intop);

	if(flawedFile == 0){													//n10
		if(strlen(DecimalToHexadecimal(startaddr))==6)
 		 	fprintf(fobj,"\nE^%s", DecimalToHexadecimal(startaddr));
		else if(strlen(DecimalToHexadecimal(startaddr))==5)
 		 	fprintf(fobj,"\nE^0%s", DecimalToHexadecimal(startaddr));
		else if(strlen(DecimalToHexadecimal(startaddr))==4)
 		 	fprintf(fobj,"\nE^00%s", DecimalToHexadecimal(startaddr));
		else if(strlen(DecimalToHexadecimal(startaddr))==3)
 		 	fprintf(fobj,"\nE^000%s", DecimalToHexadecimal(startaddr));
		else if(strlen(DecimalToHexadecimal(startaddr))==2)
 		 	fprintf(fobj,"\nE^0000%s", DecimalToHexadecimal(startaddr));
		else if(strlen(DecimalToHexadecimal(startaddr))==1)
 		 	fprintf(fobj,"\nE^00000%s", DecimalToHexadecimal(startaddr));
		else
			fprintf(fobj, "\nE^eitherHexTooBigOrNonExistant");
	}
	fscanf(fint, "%s %s", intaddr, intlab);//sole purpose after END is to check comment or error
	if(strcmp(intaddr, ".")==0){
		fgets(buffy, 256, fint);
		fprintf(flist, "%s\t%s", intaddr, intlab);
		fputs(buffy, flist); 
	}
		

  	printf("\nFiles have been converted\n");
	fclose(fint);
	fclose(flist);
	fclose(fobj);
	fclose(foptab);
	fclose(fsymtab);


}


