#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int PC =0;
/*
 * Converts a given value into a binary string.
 * @param value : value to be converted
 * @param max_len : number of output bits to be produced
 * @param conv_binary: string in which result will be stored
 * @return : 0 on failure and 1 on success
 */


int convToBinary(int value, int max_len, char * conv_binary, int use_sign) {
	int i = max_len - 1;
	//if the value to be converted is greater than the range
	//return error
	if (use_sign){
		if (value > pow(2 * 1.0,max_len - 1) -1 || value < -pow(2 * 1.0,max_len - 1))
			return 0;
	}
	else
	{
		if (value > pow(2 * 1.0,max_len ) - 1){
			return 0;
		}
	}

	if (value < 0){
		value +=  pow(2 * 1.0,max_len);
	}

	while (value != 0 && i >= 0) {
		conv_binary[i] = '0' + value % 2;
		value = value/2;
		i--;
	}
	return 1;
}

//Converts the hexadecimal value to integer
int convHexToInt(char * string, int *p_value){
	char newString[7]="0";
	int value;
	if (strlen(string) < 2) return 0;
	if (string[0] != 'x') {
		return 0;
	}
	if (string[1] == '-'){
		//this is a negative number
		strcat(newString,"x");
		//copy everything after the -ve sign
		strcat(newString,(string + 2));
	}
	else
	{
		strcat(newString,string);
	}
	sscanf(newString,"%x",&value);

	if (string[1] == '-'){

		value = -value;

	}
	*p_value = value;

	return 1;

}

/*
 * Encode the register to correct binary code
 * @reg register representation to be encoded
 * p_enc_reg encoded representation
 */
int encode_register(char*reg, char *p_enc_reg) {
	if (strcmp(reg, "R0") == 0)
		strcpy(p_enc_reg, "000");

	else if (strcmp(reg, "R1") == 0)
		strcpy(p_enc_reg, "001");

	else if (strcmp(reg, "R2") == 0)
		strcpy(p_enc_reg, "010");

	else if (strcmp(reg, "R3") == 0)
		strcpy(p_enc_reg, "011");

	else if (strcmp(reg, "R4") == 0)
		strcpy(p_enc_reg, "100");

	else if (strcmp(reg, "R5") == 0)
		strcpy(p_enc_reg, "101");

	else if (strcmp(reg, "R6") == 0)
		strcpy(p_enc_reg, "110");

	else if (strcmp(reg, "R7") == 0)
		strcpy(p_enc_reg, "111");
	else
		return 0;
	return 1;
}




/*
 * utility method to create an encoded instruction for a set of instructions which take
 * 3 operands such that the dest and first source is a register while the second
 * source can be a register or a value
 */
int encode_instruction_with_reg_or_immidiate(char *dest, char *src1, char *src2, char * opcode ){
	int value;
	char encoded_instr[17]="";
	char rMode[4] = "000"; //register mode
	char imMode[2] = "1"; //immediate mode
	char dest_reg[4], src1_reg[4], src2_reg[4];
	strcpy(encoded_instr,opcode);

	int result = encode_register(dest, dest_reg);

	if (result == 0)
		return 0;

	strcat(encoded_instr, dest_reg);

	result = encode_register(src1, src1_reg);
	if (result == 0)
		return 0;

	strcat(encoded_instr, src1_reg);

	result = encode_register(src2, src2_reg);

	if (result == 0) {
		//result is 0 indicates the src2 is not a valid
		//register, hence trying the immediate mode
		//for the the current instruction
		strcat(encoded_instr, imMode);

		char binary[6] = "00000";
		if (src2[0] == '#'){
			sscanf(src2 + 1,"%d",&value);
		}
		else{
			result = convHexToInt(src2, &value);
			if (result == 0){
				return 0;
			}
		}
		result = convToBinary(value,5, binary, 1);
		if (result == 0)
			return 0;
		strcat(encoded_instr, binary);
		printf("%s\n", encoded_instr);

		return 1;

	}
	strcat(encoded_instr, rMode);
	strcat(encoded_instr, src2_reg);
	printf("%s\n", encoded_instr);

	return 1;
}

/*
 * Creates the encoding for the ADD instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int add(char *dest, char *src1, char *src2) {
	return encode_instruction_with_reg_or_immidiate(dest, src1, src2, (char*)"0001");
}

/*
 * Creates the encoding for the AND instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int and_fn(char *dest, char *src1, char *src2) {
	return encode_instruction_with_reg_or_immidiate(dest, src1, src2, (char*)"0101");
}

/*
 * Creates the encoding for the BR instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int br(char *conditions, char *label) {
	char encoded_instr[17]="0000";
	char BR[5]="000";
	char BRn[5]="100";
	char BRz[5]="010";
	char BRp[5]="001";
	char BRzp[5]="011";
	char BRnp[5]="101";
	char BRnz[5]="110";
	char BRnzp[5]="111";

	if(strcmp(conditions,"BR")==0)
		strcat(encoded_instr,BR);
	else if(strcmp(conditions,"BRn")==0)
		strcat(encoded_instr,BRn);
	else if(strcmp(conditions,"BRz")==0)
		strcat(encoded_instr,BRz);
	else if(strcmp(conditions,"BRp")==0)
		strcat(encoded_instr,BRp);
	else if(strcmp(conditions,"BRzp")==0)
		strcat(encoded_instr,BRzp);
	else if(strcmp(conditions,"BRnp")==0)
		strcat(encoded_instr,BRnp);
	else if(strcmp(conditions,"BRnz")==0)
		strcat(encoded_instr,BRnz);
	else if(strcmp(conditions,"BRnzp")==0)
		strcat(encoded_instr,BRnzp);

	int value;
	char binary[10]="000000000";
	int result = getValueByLabel(label, &value);
	if (result == 0) return 0;
	int offset = value - PC;

	result =convToBinary(offset,9,binary,1);
	if(result==0)
		return 0;
	strcat(encoded_instr,binary);
	printf("%s\n",encoded_instr);
	return 1;
}

/*
 * Creates the encoding for the JMP instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int jmp(char *reg) {
	char encoded_instr[17] = "1100000"; //oppcode for jmp
	char enc_reg[4];
	char remaining[7] = "000000";
	int result = encode_register(reg, enc_reg);
	if (result == 0)
		return 0;
	strcat(encoded_instr, enc_reg);
	strcat(encoded_instr, remaining);
	printf("%s\n", encoded_instr);
	return 1;

}

/*
 * Creates the encoding for the JSR instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int jsr(char *label) {
	char encoded_instr[17] = "01001";
	char binary[12] = "00000000000";
	int value = 0;
	int result = getValueByLabel(label, &value);
	if (result == 0) return 0;
	int offset = value - PC;
	convToBinary(offset, 11,binary,1);
	strcat(encoded_instr, binary);
	printf("%s\n", encoded_instr);
	return 1;
}

/*
 * Creates the encoding for the JSRR instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int jsrr(char *reg) {
	char encoded_instr[17] = "0100000"; //oppcode for jssr
	char enc_reg[4];
	char remaining[7] = "000000";
	int result = encode_register(reg, enc_reg);
	if (result == 0)
		return 0;
	strcat(encoded_instr, enc_reg);
	strcat(encoded_instr, remaining);
	printf("%s\n", encoded_instr);
	return 1;
}

/*
 * Creates the encoding for the LD instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int ld(char *dest, char *label) {
	int value =0;
	char encoded_instr[17] ="0010";
	char DR[5];
	char binary[10] = "000000000";
	int result = encode_register(dest,DR);
	if(result==0)
		return 0;
	strcat(encoded_instr,DR);
	result = getValueByLabel(label, &value);
	if (result == 0) return 0;
	int offset = value - PC;

	convToBinary(offset, 9,binary,1);
	strcat(encoded_instr, binary);
	printf("%s\n", encoded_instr);
	return 1;
}

/*
 * Creates the encoding for the LDI instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int ldi(char *dest, char *label) {
	int value =0;
	char binary[10] = "000000000";
	char encoded_instr[17] ="1010";
	char DR[5];
	int result = encode_register(dest,DR);
	if(result==0)
		return 0;
	strcat(encoded_instr,DR);
	result = getValueByLabel(label, &value);
	if (result == 0) return 0;
	int offset = value - PC;
	convToBinary(offset, 9,binary,1);
	strcat(encoded_instr, binary);
	printf("%s\n", encoded_instr);
	return 1;
}



/*
 * Creates the encoding for the LDR instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int ldr(char *dest, char *base, char *offset) {
	int value;
	char binary[7]="000000";
	char encoded_instr[17]="0110";//oppcode for ldr
	char DR[4],SR[4];
	int result = encode_register(dest, DR);
	if (result ==0)
		return 0;
	strcat(encoded_instr,DR);
	result = encode_register(base,SR);
	if(result==0)
		return 0;
	strcat(encoded_instr,SR);
	result = convHexToInt(offset, &value);
	if (result==0){
		return 0;
	}
	result = convToBinary(value,6, binary,1);
	if(result==0)
		return 0;
	strcat(encoded_instr,binary);
	printf("%s\n",encoded_instr);
	return 1;

}

/*
 * Creates the encoding for the LEA instru1110010000001000ction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int lea(char *dest, char *label) {
	int value =0;
	char encoded_instr[17] ="1110";
	char binary[10] = "000000000";
	char DR[5];
	int result = encode_register(dest,DR);
	if(result==0)
		return 0;
	strcat(encoded_instr,DR);
	result = getValueByLabel(label, &value);
	if (result == 0) return 0;
	int offset = value - PC;
	convToBinary(offset, 9,binary,1);
	strcat(encoded_instr, binary);
	printf("%s\n", encoded_instr);
	return 1;
}

/*
 * Creates the encoding for the NOT instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int not_fn(char *dest, char *src) {

	char encoded_instr[17]="1001";//oppcode for not

	char SR[4],DR[4];
	char remaining[7]="111111";
	int result = encode_register(dest,DR);
	if(result==0)
		return 0;
	strcat(encoded_instr,DR);
	result = encode_register(src,SR);
	if(result==0)
		return 0;
	strcat(encoded_instr,SR);

	strcat(encoded_instr,remaining);
	printf("%s\n",encoded_instr);
	return 1;
}

/*
 * Creates the encoding for the RET instruction and print it to the screen.
 */
int ret() {
	printf("1100000111000000");
	return 1;
}

int rti(){
	printf("1000000000000000");
	return 1;
}
/*
 * Creates the encoding for the ST instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int st(char *src, char *label) {
	int value =0;
	char encoded_instr[17] ="0011";
	char SR[5];
	char binary[10] = "000000000";
	int result = encode_register(src,SR);
	if(result==0)
		return 0;
	strcat(encoded_instr,SR);
	result = getValueByLabel(label, &value);
	if (result == 0) return 0;
	int offset = value - PC;
	convToBinary(offset, 9,binary,1);
	strcat(encoded_instr, binary);
	printf("%s\n", encoded_instr);
	return 1;
}

/*
 * Creates the encoding for the STI instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int sti(char *src, char *label) {
	char encoded_instr[17]= "1011";
	int value =0;
	char binary[10] = "000000000";
	char SR[5];
	int result = encode_register(src,SR);
	if(result==0)
		return 0;
	strcat(encoded_instr,SR);
	result = getValueByLabel(label, &value);
	if (result == 0) return 0;
	int offset = value - PC;
	convToBinary(offset, 9,binary,1);
	strcat(encoded_instr, binary);
	printf("%s\n", encoded_instr);
	return 1;
}

/*
 * Creates the encoding for the STR instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int str(char *src, char *base, char *offset) {
	int value;
	char newString[6]="0";
	char binary[7]="000000";

	char encoded_instr[17]="0111";//oppcode for str
	char DR[4],SR[4];

	int result = encode_register(src, SR);
	if (result ==0)
		return 0;

	strcat(encoded_instr,SR);
	result = encode_register(base,DR);
	if(result==0)
		return 0;

	strcat(encoded_instr,DR);


	result = convHexToInt(offset, &value);
	if (result == 0) return 0;
	result = convToBinary(value,6, binary,1);

	if(result==0)
		return 0;
	strcat(encoded_instr,binary);
	printf("%s\n",encoded_instr);
	return 1;

}

/*
 * Creates the encoding for the TRAP instruction and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int trap(char *vector) {
	char encoded_instr[17]="11110000";// opcode for trap
	char binary[9]="00000000";
	int value;

	int result = convHexToInt(vector, &value);
	if(result == 0) return 0;
	result = convToBinary(value,8,binary,0);//converting to binary
	if(result == 0)
		return 0;

	strcat(encoded_instr,binary);
	printf("%s\n",encoded_instr);
	return 1;
}

/*
 * Creates the encoding for the .FILL pseudo-op and print it to the screen.
 * Return 1 if successful and 0 if an error occurs.
 */
int fill(char *value) {

	int val;

	char encoded_instr[17]="0000000000000000";

	char newString[17]="0";
	strcat(newString,value);//concatinating 0 in the beginning

	sscanf(newString,"%x",&val);
	int result= convToBinary(val,16,encoded_instr,0);//converting in to binary
	if(result==0)
		return 0;
	printf("%s\n",encoded_instr);
	return 1;

}

/*
 * Creates the encoding for the .BLKW pseudo-op and print it to the screen.
 * Note that all values in the block of words should be initialized to zero.
 * Return 1 if successful and 0 if an error occurs.
 */
int blkw(char *size) {
	char binary[17] = "0000000000000000";
	char newString[17]="0";
	int val = 0;
	int i=0;
	strcat(newString,size);//concatinating 0 in the beginning

	sscanf(newString,"%x",&val);


	for(i=0;i<val;i++){
		printf("%s\n",binary);
	}
	return 1;
}


/*
 * Creates the encoding for the characters in a .STRINGZ pseudo-op and print them to the screen.
 * The string may contain up to three words separated by whitespace.
 * Return 1 if successful and 0 if an error occurs.
 */
int stringz(char *s0, char *s1, char *s2) {

	int i;
	int len1,len2,len3;

	len1 = strlen(s0);
	len2 = strlen(s1);
	if (len2) len2++;
	len3 = strlen(s2);
	if (len3) len3++;

	int len = len1+len2+len3 - 2;
	if (s0[0] != '"') return 0;
	char * str = (char*)malloc(len + 1);
	strcpy(str, s0 + 1);
	if (len2 > 0) strcat(str," ");
	strcat(str, s1);
	if (len3 > 0) strcat(str," ");
	strcat(str, s2);
	len= strlen(str);
	if (str[len - 1] != '"') {
		free(str);
		return 0;
	}
	str[len - 1] = '\0';
	for ( i = 0; i < len; i++){
		char convString[9]="00000000";
		convToBinary(str[i],8,convString,0);
		printf("00000000%s\n",convString);

	}

	return 1;


}

int main(int argc, char *argv[]) {

	// the first argument is the name of the file to read
	char *filename = argv[1];
	printf("Filename is %s\n", filename);

	// first pass: create the symbol table
	create_symbol_table(filename);

	/*
  Now add the following functionality:
	 * read each line of the text file and use the "parse" function to break it up into its substrings.
	 * Then, determine what the operation is (AND, LD, BR, etc.) and then call the appropriate function, passing the corresponding arguments.
	 * If the operation is not valid, report an error and end the program.
	 * Keep looping until you've reached the end of the program (including .FILL, .STRINGZ, and .BLKW!)
	 */
	FILE *infile = fopen(filename, "r");
	if (infile == NULL) {
		printf("Error! Could not open file \"%s\" for reading\n", filename);
		return 0;
	}
	int i, j = 0;

	int numWords;
	char *s[10];
	int error = 1;
	for (i = 0; i < 10; i++)
		s[i] = (char *) malloc(sizeof(char) * 100);

	// open and read one line (assume it's no longer than 100 chars)
	// note that the newline *is* included!
	char line[100];
	while ((error == 1) && fgets(line, 100, infile)) {
		PC =PC+1;
		for (i = 0; i < 10; i++){
			strcpy(s[i],"");
		}
		numWords = parse(line, s);
		j=0;
		if (numWords == 0) // if empty line then goes back to read new line.
			continue;
		label:
			//Checking the operation

			if (strcmp(s[j], "NOT") == 0) {
				if ((numWords - j) != 3) {
					error = 0;
					break;
				}
				error = not_fn(s[j + 1], s[j + 2]);
			} else if (strcmp(s[j], "AND") == 0) {
				if ((numWords - j) != 4) {
					error = 0;
					break;
				}
				error = and_fn(s[j + 1], s[j + 2], s[j + 3]);
			} else if (strcmp(s[j], "ADD") == 0) {
				if ((numWords - j) != 4) {
					error = 0;
					break;
				}
				error = add(s[j + 1], s[j + 2], s[j + 3]);
			} else if (strcmp(s[j], "STR") == 0) {
				if ((numWords - j) != 4) {
					error = 0;
					break;
				}
				error = str(s[j + 1], s[j + 2], s[j + 3]);
			} else if (strcmp(s[j], "ST") == 0) {
				if ((numWords - j) != 3) {
					error = 0;
					break;
				}
				error = st(s[j + 1], s[j + 2]);
			} else if (strcmp(s[j], "STI") == 0) {
				if ((numWords - j) != 3) {
					error = 0;
					break;
				}
				error = sti(s[j + 1], s[j + 2]);
			} else if (strcmp(s[j], "RET") == 0) {
				if ((numWords - j) != 1) {
					error = 0;
					break;
				}
				error = ret();
			} else if (strcmp(s[j], "TRAP") == 0) {
				if ((numWords - j) != 2) {
					error = 0;
					break;
				}
				error = trap(s[j + 1]);
			}

			else if (strcmp(s[j], "JMP") == 0) {
				if ((numWords - j) != 2) {
					error = 0;
					break;
				}

				error = jmp(s[j + 1]);
			}

			else if (strcmp(s[j], "JSRR") == 0) {
				if ((numWords - j) != 2) {
					error = 0;
					break;
				}
				error = jsrr(s[j + 1]);
			}

			else if (strcmp(s[j], "JSR") == 0) {
				if ((numWords - j) != 2) {
					error = 0;
					break;
				}
				error = jsr(s[j + 1]);
			} else if (strcmp(s[j], "LDR") == 0) {
				if ((numWords - j) != 4) {
					error = 0;
					break;
				}
				error = ldr(s[j + 1], s[j + 2], s[j + 3]);
			} else if (strcmp(s[j], "LD") == 0) {
				if ((numWords - j) != 3) {
					error = 0;
					break;
				}
				error = ld(s[j + 1], s[j + 2]);
			} else if (strcmp(s[j], "LDI") == 0) {
				if ((numWords - j) != 3) {
					error = 0;
					break;
				}

				error = ldi(s[j + 1], s[j + 2]);
			}

			else if (strcmp(s[j], "LEA") == 0) {
				if ((numWords - j) != 3) {
					error = 0;
					break;
				}
				error = lea(s[j + 1], s[j + 2]);
			}

			else if(strcmp(s[j],"BR")==0){
				if((numWords -j)!=2){
					error =0;
					break;
				}
				error = br(s[j],s[j+1]);
			}

			else if(strcmp(s[j],"BRn")==0){
				if((numWords -j)!=2){
					error =0;
					break;
				}
				error = br(s[j],s[j+1]);
			}

			else if(strcmp(s[j],"BRz")==0){
				if((numWords -j)!=2){
					error =1;
					break;
				}
				error = br(s[j],s[j+1]);
			}

			else if(strcmp(s[j],"BRp")==0){
				if((numWords -j)!=2){
					error =0;
					break;
				}
				error = br(s[j],s[j+1]);
			}

			else if(strcmp(s[j],"BRnz")==0){
				if((numWords -j)!=2){
					error =0;
					break;
				}
				error = br(s[j],s[j+1]);
			}

			else if(strcmp(s[j],"BRnp")==0){
				if((numWords -j)!=2){
					error =0;
					break;
				}
				error = br(s[j],s[j+1]);
			}

			else if(strcmp(s[j],"BRzp")==0){
				if((numWords -j)!=2){
					error =0;
					break;
				}
				error = br(s[j],s[j+1]);
			}

			else if(strcmp(s[j],"BRnzp")==0){
				if((numWords -j)!=2){
					error =0;
					break;for (i = 0; i < 10; i++){

						free(s[i]);

					}
				}
				error = br(s[j],s[j+1]);
			}
			else if(strcmp(s[j],".BLKW")==0){
				if((numWords -j)!=2){
					error=0;
					break;
				}
				error = blkw(s[j+1]);
			}
			else if(strcmp(s[j],".FILL")==0){
				if((numWords -j)!=2){
					error =0;
					break;
				}
				error = fill(s[j+1]);
			}
			else if(strcmp(s[j],".STRINGZ")==0){
				if(((numWords -j)<2)&& ((numWords -j)>4)){
					error=0;
					break;
				}
				error = stringz(s[j+1],s[j+2],s[j+3]);
			}
			else if (j == 0){
				j = 1;
				goto label;
			}
			else{
				error ==0;
			}
	}
	if (error == 0) {
		printf("Error in file!");
		return 1; // tell the os that there is an error.

	}
	for (i = 0; i < 10; i++){
		free(s[i]);
	}
	return 0;
}
