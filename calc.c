//calc.c
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*
 * -----binaryNum(bNum)-----
 * - number: an ASCII string of the binary representation of a number
 * - sign: either '-' or '+' representing the sign of the number
 * - numDigits: the number of digits used to represent the number plus one for the null byte
 */
typedef struct binaryNum{
	int numDigits;
	char sign;
	char * number;
} bNum;

/*****binaryNum Methods*****/

/*
 * _____freebNum_____
 * - Frees all dynamically allocated space for the given bNum
 * - Returns nothing
 */
void freebNum(bNum * num){
	free((*num).number);
	free(num);
}


/*****Number Validity Checkers*****/

/*
 * _____checkNum_____
 * - Checks the base of the number (makes sure base is valid)
 * - Makes sure the number is in compliance with it's base
 * - Returns 0 for success, 1 for Failure
 */
int checkNum(char * str){
	if(*str == '-') //Numbers can be negative
		str++;

	if(*str == 'd'){ //If number is decimal
		str++;
		do { //Make sure all digits are valid decimal digits
			if(isdigit(*str) == 0){
				fprintf(stderr, "ERROR: Decimal number is invalid.\n");
				return 1;
			}
			str++;
		} while (*str != '\0');

	} else if (*str == 'b'){ //If number is binary
		str++;
		do { //Make sure all digits are valid binary digits
			if(*str != '0' && *str != '1'){
				fprintf(stderr, "ERROR: Binary number is invalid.\n");
				return 1;
			}
			str++;
		} while(*str != '\0');

	} else if(*str == 'o'){ //If number is octal
		str++;
		do { //Make sure all digits are valid octal digits
			if( isdigit(*str) == 0 || *str == '9' || *str == '8'){
				fprintf(stderr, "ERROR: Octal number is invalid.\n");
				return 1;
			}
			str++;
		} while(*str != '\0');

	} else if(*str == 'x'){ //If number is hexadecimal
		str++;
		do { //Make sure all digits are valid hexadecimal digits
			if(isxdigit(*str) == 0){
				fprintf(stderr, "ERROR: Hexadecimal number is invalid.\n");
				return 1;
			}
			str++;
		} while(*str != '\0');

	} else { //Invalid String
		fprintf(stderr, "ERROR: Base signifier is improper or missing.\n");
		return 1;
	}

	return 0;
}


/*****Int <-> Char Digit Translators*****/

/*
 * _____getIntVal_____
 * - Takes a char, returns integer value associated with it
 */
int getIntVal(char c){
	if(c <= '9') //If char is 0-9
		return (c - '0');
	else if(c <= 'F') //If char is A-F
		return (c - 'A' + 10);
	else //If char is a-f
		return (c - 'a' + 10);

}

/*
 * _____getCharVal_____
 * - Takes an int, returns the associated char
 */
char getCharVal(int x){
	if(x <= 9) //If int is 0-9
		return x + '0';
	else //If int is 10-15
		return x - 10 + 'A';
}


/*****ASCII <-> bNum Translators*****/

/*
 * _____asciiToBinaryASCII_____
 * - Takes a valid ASCII string (octal, hex, binary or decimal)
 * - Creates & dynamically allocates a bNum based on the string
 * - Returns the bNum
 * - Binary strings are in LSD -> MSD order to allow for easier arithmetic
 */
bNum * asciiToBinaryASCII(char * num){
	//Allocate memory for a bNum struct
	bNum * finNum = malloc(sizeof(bNum));
	if(finNum == NULL){
		fprintf(stderr, "ERROR: asciiToBinaryASCII function failed to allocate memory for bNum...\n");
		exit(1);
	}

	//Determine and associate the correct sign to the bNum
	if(*num == '-'){
		(*finNum).sign = '-';
		num++;
	} else {
		(*finNum).sign = '+';
	}

	//Determine the number of chars needed to represent num in binary
	int i = 0;
	if(*num == 'd'){ //Ints can only be as large as 32
		i = 32;
	} else if(*num == 'o') { //Each octal digit is represented by 3 binary chars
		i = (strlen(num + 1) * 3);
	} else if(*num == 'x') { //Each hexadecimal digit is represented by 4 binary chars
		i = (strlen(num + 1) * 4);
	} else{ //Must I really explain this one?
		i = strlen(num + 1) + 1;
	}

	//Round the size to the next highest number divisible by 32
	while(i % 32 != 0){
		i++;
	}

	//Allocate the proper amount of space for the binary representation
	(*finNum).number = malloc(i*sizeof(char) + sizeof(char));
	(*finNum).numDigits = i + 1; //Include the null byte character
	if((*finNum).number == NULL){
		fprintf(stderr, "ERROR: asciiToBinaryASCII function failed to allocate memory for bNum's number...\n");
		exit(1);
	}

	//Set up Variables for the majority of scenarios- others can change them to fit their needs
	char * binNum = (*finNum).number;
	char * stop = num;
	binNum += i;
	*binNum = '\0';
	binNum = (*finNum).number;

	if(*num == 'd'){ //Decimal
		num++;
		int result = 0;

		//Place ASCII decimal into an integer
		do{
			result *= 10;
			result = result + *num - '0';
			num++;
		} while(*num != '\0');

		//Bitshift binary info out of result integer
		int x = 0;
		int bits = 0;
		for(;x < 31; x++){
			bits = result;
			bits = bits << (31 - x);
			bits = bits >> 31;

			bits *= -1; //Right bitshift will make it -1 if it is 1
			*binNum = '0' + bits;
			binNum++;
		}
		*binNum = '0'; //Last pos will be 0 since sign is tracked separately

		return finNum;

	} if(*num == 'o'){ //Octal
		num += strlen(num) - 1; //Puts num at the end of the string

		//Reads the string one char at a time
		int bits;
		int n;
		int x = 0;
		int j = 0;
		for(; x < i;){
			if(num != stop){
				bits = getIntVal(*num); //Puts value of char into an int
				j = 0;
				for(;j < 3; j++){ //Reads out binary representation using bitshift
					n = bits << (31 - j);
					n = n >> 31;
					n *= -1;
					*binNum = '0' + n;
					binNum++;
					x++;
				}
				num--;
			} else { //Fills in the rest with zeros
				*binNum = '0';
				binNum++;
				x++;
			}
		}
		return finNum;

	} if(*num == 'x'){ //Hexadecimal
		num += strlen(num) - 1; //Puts num at the end of the string

		//Reads the string one char at a time
		int bits;
		int n;
		int x = 0;
		int j = 0;
		for(; x < i;){
			if(num != stop){
				bits = getIntVal(*num); //Puts value of char into an int
				j = 0;
				for(;j < 4; j++){ //Reads out binary representation using bitshift
					n = bits << (31 - j);
					n = n >> 31;
					n *= -1;
					*binNum = '0' + n;
					binNum++;
					x++;
				}
				num--;
			} else { //Fills in the rest with zeros
				*binNum = '0';
				binNum++;
				x++;
			}
		}
		return finNum;

	} else { //Binary
		num += strlen(num) - 1; //Puts num at the end of the string

		//Reads the string one char at a time & places it into the bNum
		int x = 0;
		for(; x < i; x++){
			if(num != stop){
				*binNum = *num;
				num--;
			} else {
				*binNum = '0';
			}
			binNum++;
		}

		return finNum;
	}
}

/*
 * _____bNumToAscii_____
 * - Takes an bNum and a char representing the base of the output char *
 * - Returns a char * representation of the bNum in the given base
 * - Caller is responsible for freeing this char *
 */
char * bNumToAscii(bNum * num, char form){
	char * stop = (*num).number; //Will be used by each scenario
	char * result;

	if(form == 'd'){ //Decimal
		char * ptr = stop + 31; //Places ptr at the end of the string
		int integer = 0;

		//Bitshifts information in bNum's number into an integer
		while(ptr != stop){
			integer = integer << 1;
			integer += *ptr - '0';
			ptr--;
		}
		integer = integer << 1;
		integer += *ptr - '0';

		//The most digits possible for a decimal number of 32-bits
		char numArray[11] = {'0','0','0','0','0','0','0','0','0','0','\0'};

		//Places LSDs from right to left
		int x = 9;
		for(; ; x--){
			numArray[x] = (integer % 10) + '0';
			integer /= 10;
			if(integer == 0) //Stops when digits run out
				break;
		}

		//Decides the size needed for malloc and mallocs for the output string
		result = malloc((11 - x)*sizeof(char));
		if(result == NULL){
				fprintf(stderr, "ERROR: bNumToAscii failed to allocate space for resulting string...\n");
				exit(1);
		}
		ptr = result;

		//Copies result string into allocated memory
		for(; x < 11; x++){
			*ptr = numArray[x];
			ptr++;
		}

	} else if(form == 'o'){ //Octal
		int maxSize = ((*num).numDigits - 1) / 3; //3 Binary digits represents one octal char
		char numArray[maxSize + 1]; //Includes null byte
		int bits;

		//Bitshifts binary string into an integer to determine the necessary character
		int x = 0;
		int j = 0;
		for(; x < maxSize; x++){
			bits = 0;
			j = 0;
			stop += 3;
			for(; j < 3; j++){
				stop--;
				bits = bits << 1;
				bits += *stop - '0';
			}
			stop += 3;
			numArray[x] = getCharVal(bits);
		}
		numArray[x] = '\0';
		x--;

		//Decide how many digits are actually used
		while(numArray[x] == '0'){
			x--;
		}
		maxSize = x + 2; //Account for null byte and overlapped char

		//Malloc a char * of the determined size
		result = malloc(maxSize*sizeof(char));
		if(result == NULL){
			fprintf(stderr, "ERROR: bNumToAscii failed to allocate space for resulting string...\n");
			exit(1);
		}
		stop = result;

		//Copy the relevant digits into the char *
		for(; maxSize >= 1; maxSize--){
			*stop = numArray[maxSize - 2];
			stop++;
		}
		stop = '\0';

	} else if(form == 'x'){ //Hexadecimal
		int maxSize = ((*num).numDigits - 1) / 4; //4 Binary digits represents one hex char
		char numArray[maxSize + 1]; //Includes null byte
		int bits;

		//Bitshifts binary string into an integer to determine the necessary character
		int x = 0;
		int j = 0;
		for(; x < maxSize; x++){
			bits = 0;
			j = 0;
			stop += 4;
			for(; j < 4; j++){
				stop--;
				bits = bits << 1;
				bits += *stop - '0';
			}
			stop += 4;
			numArray[x] = getCharVal(bits);
		}
		numArray[x] = '\0';
		x--;

		//Decide how many digits are actually used
		while(numArray[x] == '0'){
			x--;
		}
		maxSize = x + 2; //Account for null byte and overlapped char

		//Malloc a char * of the determined size
		result = malloc(maxSize*sizeof(char));
		if(result == NULL){
			fprintf(stderr, "ERROR: bNumToAscii failed to allocate space for resulting string...\n");
			exit(1);
		}
		stop = result;

		//Copy the relevant digits into the char *
		for(; maxSize >= 1; maxSize--){
			*stop = numArray[maxSize - 2];
			stop++;
		}
		stop = '\0';

	} else { //Binary
		char * ptr = stop + 31; //Bring ptr to the end of the string

		//Discredit trailing zeros
		while(ptr != stop && *ptr == '0'){
			ptr--;
		}

		//Determine the size of the resulting char *
		int size = ptr - stop + 2;

		//Malloc for the resulting char *
		result = malloc(size*sizeof(char));
		if(result == NULL){
			fprintf(stderr, "ERROR: bNumToAscii failed to allocate space for resulting string...\n");
			exit(1);
		}
		char * rptr = result;

		//Copy relevant digits into the result char *
		while(ptr != stop){
			*rptr = *ptr;
			rptr++;
			ptr--;
		}
		*rptr = *ptr;
		rptr++;
		*rptr = '\0';
	}

	if(*result == '\0'){
		result = realloc(result, 2*sizeof(char));
		if(result == NULL){
			fprintf(stderr, "ERROR: bNumToAscii failed to allocate space for resulting string...\n");
			exit(1);
		}

		*result = '0';
		*(result+1) = '\0';
	}

	return result;
}


/*****Arithmetic Operations*****/

/*
 * _____twosComp_____
 * - Takes a bNum and alters it directly
 * - Changes its binary string to it's twos compliment
 */
void twosComp(bNum * num){
	char * front = (*num).number;

	//Flip bits
	while(*front != '\0'){
		if(*front == '1')
			*front = '0';
		else
			*front = '1';
		front++;
	}

	//Add one
	front = (*num).number;
	while(*front != '\0'){
		if(*front == '0'){
			*front = '1';
			break;
		} else{
			*front = '0';
		}
		front++;
	}
}

/*
 * _____posAdd_____
 * - Adds two binary strings
 * - Does not account for size, sign or anything else
 * - Just does basic addition of two strings
 * - Alters value of n1 while n2 remains the same
 */
void posAdd(bNum * n1, bNum * n2){
	//Pointer to traverse each number
	char * ptr1 = (*n1).number;
	char * ptr2 = (*n2).number;
	int carry = 0; //Determines sums of individual columns & carry values

	//Runs for each column - matchSize ensures column numbers are equal
	while(*ptr1 != '\0' && *ptr2 != '\0'){
		carry += (*ptr1 - '0') + (*ptr2 - '0'); //Determines sum of the column

		//Each scenario of column and carry sum
		if(carry == 3){
			*ptr1 = '1';
			carry = 1;

		} else if(carry == 2){
			*ptr1 = '0';
			carry = 1;

		} else if(carry == 1){
			*ptr1 = '1';
			carry = 0;

		} else {
			*ptr1 = '0';
			carry = 0;
		}
		ptr1++;
		ptr2++;
	}
}

/*
 * _____add_____
 * - Determines the sign of n1 and n2 & chooses how to alter them so posAdd works
 * - Determines the sign of the final value of n1 and twosComps it in needed
 * - Adds the value of n2 to n1
 */
void add(bNum * n1, bNum * n2){
	if((*n1).sign == '+'){ //n1 is pos
		if((*n2).sign == '+'){ //n2 is pos
			posAdd(n1, n2);
		} else { //n2 is neg
			twosComp(n2);
			posAdd(n1, n2);
			twosComp(n2);
		}

	} else { //n1 is neg
		twosComp(n1);

		if((*n2).sign == '+'){ //n2 is pos
			posAdd(n1, n2);
		} else { //n2 is neg
			twosComp(n2);
			posAdd(n1, n2);
			twosComp(n2);
		}
	}

	//If the last bit is 1, the number is negative
	//so twosComp it and make its sign negative
	char * check = (*n1).number;
	check += (*n1).numDigits - 2;
	if(*check == '1'){
		twosComp(n1);
		(*n1).sign = '-';
	} else { //Otherwise its positive
		(*n1).sign = '+';
	}

}

/*
 * _____subtract_____
 * - Alters values and redirects to add
 * - Effectively subtracts the value of n2 from n1
 */
void subtract(bNum * n1, bNum * n2){
	if((*n2).sign == '-'){ // +/-n1 - (-n2)
		(*n2).sign = '+';
		add(n1, n2);
		(*n2).sign = '-';

	} else { // +/-n1 - n2
		twosComp(n2);
		add(n1, n2);
		twosComp(n2);
	}
}

/*
 * _____matchSize_____
 * - Makes the size of n1 and n2 equal
 * - Adds four 0's to the end of both numbers once sizes are matched
 * 	 	^This allows for overflow to not be lost
 * - Arithmetic algorithms will not work if this is not called first
 */
void matchSize(bNum * n1, bNum * n2){
	char * zeroBits = "00000000000000000000000000000000"; //32-Bits of 0s
	char * overflowBits = "0000"; //The last 4 will always be added to both once they match

	//If n1 is larger - make them equal
	while((*n1).numDigits > (*n2).numDigits){
		//Realloc with 32 more chars
		(*n2).number = realloc((*n2).number, ((*n2).numDigits + 32)*sizeof(char));
		if((*n2).number == NULL){
			fprintf(stderr, "ERROR: Failed to realloc for resizing of n2...\n");
			exit(1);
		}
		strcat((*n2).number, zeroBits); //Add 32 zeros on to the end
		(*n2).numDigits += 32; //Adjust size variable
	}

	//If n2 is larger - make them equal
	while((*n1).numDigits < (*n2).numDigits){
		//Realloc with 32 more chars
		(*n1).number = realloc((*n1).number, ((*n1).numDigits + 32)*sizeof(char));
		if((*n1).number == NULL){
			fprintf(stderr, "ERROR: Failed to realloc for resizing of n1...\n");
			exit(1);
		}
		strcat((*n1).number, zeroBits); //Add 32 zeros on to the end
		(*n1).numDigits += 32; //Adjust size variable
	}

	//Realloc for the extra 4 digits for overflow
	(*n1).number = realloc((*n1).number, ((*n1).numDigits + 4)*sizeof(char));
	(*n2).number = realloc((*n2).number, ((*n2).numDigits + 4)*sizeof(char));
	if((*n1).number == NULL || (*n2).number == NULL){
		fprintf(stderr, "ERROR: Failed to realloc for resizing of n1 or n2...\n");
		exit(1);
	}

	//Add those digits
	strcat((*n1).number, overflowBits);
	strcat((*n2).number, overflowBits);

	//Increase the recorded size
	(*n1).numDigits += 4;
	(*n2).numDigits += 4;
}


/*****Main Method*****/

int main(int argc, char ** argv){
	//Check Arguments - Terminate if any are improper
	if(argc != 5){
		fprintf(stderr, "ERROR: Improper Number Of Arguments...\n");
		exit(1);
	} if(*argv[1] != '+' && *argv[1] != '-'){
		fprintf(stderr, "ERROR: Improper Operation Argument...\n");
		exit(1);
	} if(*argv[4] != 'd' && *argv[4] != 'o' &&
			  *argv[4] != 'x' && *argv[4] != 'b'){
		fprintf(stderr, "ERROR: Improper Output Base Argument...\n");
		exit(1);
	} if(checkNum(argv[2]) == 1 || checkNum(argv[3]) == 1){
			fprintf(stderr, "ERROR: One of the number arguments in invalid...\n");
			exit(1);
	}

	//Create bNums for maths
	bNum * n1 = asciiToBinaryASCII(argv[2]);
	bNum * n2 = asciiToBinaryASCII(argv[3]);

	//Make sure they are the same size before maths
	matchSize(n1, n2);

	//Perform arithmetic
	if(*argv[1] == '+'){
		add(n1, n2);
	} else {
		subtract(n1, n2);
	}

	//Get result string in the required base
	char * result = bNumToAscii(n1, *argv[4]);

	//Print out the string
	if((*n1).sign == '-')
		printf("-%c%s\n", *argv[4], result);
	else
		printf("%c%s\n", *argv[4], result);

	//Free allocated space
	free(result);
	freebNum(n1);
	freebNum(n2);

	return 0;
}
