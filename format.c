//format.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * _____Number_____
 * - Used to create a float and an int
 * - Bitshifts data into the int
 * - It occupies the same space as the float
 * - Use whichever one the number should be interpreted as
 */
typedef union Number{
	int i;
	float f;
} Num;

/*****Number -> String Conversions*****/

/*
 * _____intToDecASCII_____
 * - Converts 32-bit int to an ASCII string
 * - Puts chars in given array so no memory allocation is needed
 */
void intToDecASCII(int i, char * integ){
	char * ptr;
	char sign = '+';

	//32-Bits for a int
	char numArray[12] = {'0', '0','0','0','0','0','0','0','0','0','0','\0'};

	//If sign is negative make i positive and the sign
	if(i < 0){
		sign = '-';
		i *= -1;
	}

	//Places LSDs from right to left
	int x = 10;
	for(; ; x--){
		numArray[x] = (i % 10) + '0';
		i /= 10;
		if(i == 0) //Stops when digits run out
			break;
	}

	//Adds a dash if number is negative
	if(sign == '-'){
		x--;
		numArray[x] = '-';
	}

	//Copies resulting string into allocated memory
	ptr = integ;
	for(; x < 12; x++){
		*ptr = numArray[x];
		ptr++;
	}
}

/*
 * _____get_float_digit_____
 * - Converts IEEE 32-bit floating point to printable ASCII string
 * - input: x is the 32-bit input.
 * - output:  digit is the output ASCII character
 * - output:  pow10 is the original pre-normalization power of 10.
 * - output: remainder is x after subtracting the high-digit value
*/
static void get_float_digit( float x, char * digit, int * pow10, float * remainder ) {
	int	p10;

	if ( x == 0.0 ) {
		*digit = '0';			// digit is always '0'
		*pow10 = 0;
		*remainder = 0.0;
	} else {
		*pow10 = 0;			// start with 10^0
		while ( x >= 10.0 ) { // reduce
			x /= 10.0;		// large values get smaller
			*pow10 += 1;
		}
		while ( x < 1.0 ) { // increase
			x *= 10.0;		// small values get larger
			*pow10 -= 1;
		}
		*digit = '0';
		do {				// 10.0 > x >= 1.0
			x -= 1.0;
			*digit += 1;		// digit is always non-'0'zero
		} while ( x >= 1.0 );
		p10 = 0;
		while ( p10 > *pow10 ) { // leading digit is now removed from x
			x /= 10;
			p10 -= 1;
		}
		while ( p10 < *pow10 ) {
			x *= 10;
			p10 += 1;
		}
		*remainder = x;
	}
}

/*
 * _____append_____
 * - Appends char c to string s
 */
static void append( char * s, char c ) {
	char buf[2];

	buf[0] = c;
	buf[1] = '\0';
	strcat( s, buf );
}

/*
 * _____floatToASCII_____
 * - Converts IEEE 32-bit floating point to printable ASCII string
 * - Puts chars in given array so no memory allocation is needed
 */
void floatToASCII( float x, char * output ) {
	char		c;
	int		pow10, p10, plast;
	int		i;
	float		remainder;
	char		exponent[10];
	union Number	a;
	unsigned int	biasedExp;
	unsigned int	mantissa;
	int		sign;

	a.f = x;
	biasedExp = a.i >> 23 & 0x000000ff;
	mantissa = a.i & 0x007fffff;
	sign = a.i >> 31;

	if ( biasedExp == 0xff ) {
		if ( mantissa == 0 ) {
			if ( sign != 0 ) {
				strcpy( output, "ninf" );
				return;
			} else {
				strcpy( output, "pinf" );
				return;
			}

		} else {
			if ( sign != 0 ) {
				strcpy( output, "NaN" ); // +NaN
				return;
			} else {
				strcpy( output, "NaN" ); //-NaN
				return;
			}
		}
	}
	output[0] ='\0';

	if ( x < 0.0 ) {
		append( output, '-' );
		x = -x;					// make x positive
	}

	get_float_digit( x, &c, &pow10, &remainder );
	append( output, c );
	append( output, '.' );
	x = remainder;
	plast = p10 = pow10;			// pow10 set by get_float_digit()
	for ( i = 1 ; i < 7 ; i++ ) {	// 7 significant digits in 32-bit float
		get_float_digit( x, &c, &p10, &remainder );
		if ( (plast - p10) > 1 ) {
			append( output, '0' );	// fill in zero to next nonzero digit
			plast -= 1;
		} else {
			append( output, c );
			x = remainder;
			plast = p10;
		}
	}
	if ( pow10 < 0 ) { // negative exponent
		exponent[0] = 'e';
		intToDecASCII( pow10, exponent+1 );

	} else if ( pow10 < 10 ) { // positive single-digit exponent
		exponent[0] = 'e';

		if(pow10 != 0)
			intToDecASCII( pow10, exponent+1 );
		else {
			exponent[1] = '0';
			exponent[2] = '\0';
		}

	} else {			// positive multi-digit exponent
		exponent[0] = 'e';
		intToDecASCII( pow10, exponent+1 );
	}
	strcat( output, exponent );
}


/*****Input Checkers*****/

/*
 * _____checkInput_____
 * - Makes sure input is valid
 * - Returns 1 if float and 0 if int
 */
int checkInput(char * bits, char * type){
	//Checks if the bit string has any chars other than 1 or 0
	int x = 0;
	for(; x < 32; x++){
		if(*bits != '1' && *bits != '0'){
			fprintf(stderr, "ERROR: 32-Bit Binary String contains a digit that is not a 1 or a 0...\n");
			exit(1);
		}
		bits++;
	}

	//Checks to see if the bit string is longer than 32 bits
	if(*bits != '\0'){
		fprintf(stderr, "ERROR: Binary String is longer than 32-Bits...\n");
		exit(1);
	}

	//Checks if type is valid and returns proper type
	if(strcmp(type,"float") == 0)
		return 1;
	else if(strcmp(type,"int") == 0)
		return 0;

	//If type is invalid
	fprintf(stderr, "ERROR: Input type is invalid. Please use \"float\" or \"int\"...");
	exit(1);
}


/*****Main Method*****/
 int main(int argc, char ** argv){
	 //Check to see if the proper number of arguments were sent in
	 if(argc != 3){
		 fprintf(stderr, "ERROR: Improper number of arguments.\n");
		 exit(1);
	 }

	 //Checks if input is valid and returns 0 for int 1 for float
	 int data = checkInput(argv[1], argv[2]);

	 //Creates number for bitshifting - then bitshifts
	 Num number;
	 number.i = 0;

	 char * ptr = argv[1];
	 while(*ptr != '\0'){
		 number.i = number.i << 1;
		 number.i += *ptr - '0';
		 ptr++;
	 }

	 //Creates an array for output with 20 positions for chars
	 char result[20];

	 //Decides type and places correct string into result
	 if(data == 0) //int
		intToDecASCII(number.i, result);
	 else //float
		floatToASCII(number.f, result);

	 //Prints out result
	 if(data == 1 && number.f == -0.0 && *(argv[1]) == '1') //If its negative zero add a minus sign
		printf("-%s\n", result);
 	 else
		printf("%s\n", result);

	 return 0;
 }
