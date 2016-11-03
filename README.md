# Computer-Arithmetic

#### Liam Davies - Rutgers University
#### Computer Architecture - Fall 2016  
#### Professor Brian Russell

# Synopsis:

Computer Arithmetic is a C Library with two libraries. 

**Calc** takes in 4 arguments, the first being the operation (+ or -), the next two being numbers (of bases hexadecimal, decimal, octal or binary) in the format for a string and the last being a single character string (representing the base of the output). It adds the two given numbers together and returns the resulting number in the given output base.

**Format** takes in a bit sequence of 32 bits representative of a float or integer and a string that gives the type the first string should be interpreted as. It interprets the number as such and prints out the finalized number in its repective format.

# Calc.c ReadMe

## Description: 
        The program is designed to take in 4 arguments, the first being the operation (+ or -), the next two being 
    numbers (of bases hexadecimal, decimal octal or binary) in the format for a string and the last being a single 
    character string (representing the base of the output). (The program does not account for decimal inputs or 
    outputs over what would fit into a 32 bit signed int, however it does arbitrary size for all other bases)  The 
    first thing calc.c does is it checks the arguments to make sure it can run the program using these arguments. 
    In this process, it ensures the number strings do not contain characters that their base cannot have so the rest 
    of the program can run under the assumption that each character is valid. Next it creates a bNum for both the 
    given numbers. This data structure is a collection of an int numDigits (representing the number of chars used in 
    the char *), a char sign (holding ‘+’ or ‘-‘, to represent the sign) and a char * number (holding the number’s 
    representation in binary as an ASCII string).
    
        Next it matches the string sizes of both numbers. This makes it so if a number is negative or being 
    subtracted, we can twos compliment it and just add it to the other number, then twos compliment the result 
    and change the sign if needed. In matching the size it makes both numbers have a string length equal to the
    higher of the two, adding extra zeros onto the end of the smaller. Then it adds 4 zeros to both numbers to 
    account for overflow. (Only one zero is needed for potential overflow but 4 are needed to ensure the 
    conversion methods back to ASCII collect the extra overflow data)
    
        Then the program decides what operation to do based on the arguments given and passes the two bNums to 
    either add or subtract.  These functions do not actually do any real binary addition, they just prep the bNums 
    and send them to another function to handle them. Subtract just changes the signs and/or two compliments the 
    second bNum depending on the situation and sends it to add. Add changes none, one or both before sending them 
    to posAdd which actually calculates adding the second bNum to the first. From there add checks to see if it’s 
    negative (the highest digit is one) and twos compliments it and changes its sign if necessary.
    
        Now the first bNum contains the sum of the two in binary. So the program passes it to bNumToASCII which 
    returns a dynamically allocated string representation of the bNum in the output base given by the last 
    argument. From there it is determined if the number is negative and it is printed out with a ‘-‘ before the 
    number if it is or nothing if it is positive. (0 will always be positive)

    
## THINGS TO NOTE ABOUT bNum:
 - The binary string (number) is ordered with the least significant digit at the beginning  to make arithmetic easier
 - The binary string (number) is kept in a string with size divisible by 32, meaning that if a signed input number needs 33 bits to be represented, its number variable would have 64 non-null byte characters. This allows for quick size matching so twos compliment can be used in arithmetic.
 - The null byte is included in the number of chars used as tracked by numDigits
 - The struct itself is dynamically allocated along with the char * inside
 
## IMPORTANT:
### Following the spec, this is how this program handles negative input:
 - It is assumed that the number after the base signifier is positive
 - If a number is negative it is denoted by a ‘–‘ before the base signifier

## Challenges:

        The only real issue I faced while writing this code was deciding how I would handle each function. I 
    decided very early on what methods I would need and what they would need to do, but sitting down and deciding 
    exactly how they would do it took more time than actually coding them.
    
## Analysis:

        The program must run at some linear time since the program doesn’t use any algorithms that do anything 
    more than read though the number character by character or digit by digit. Since the program stores data in 
    multiples of 32 contiguous chars and 36 with 4 digits for overflow it is easier and faster to convert data 
    back to ASCII for octal and hex without losing data, however it takes up more space than will be needed in 
    most cases.
    
# Format.c ReadMe

## Description: 

        This program takes in a bit sequence of 32 bits representative of a float or integer and a string that 
    gives the type the first string should be interpreted as. First it takes the string and bitshifts the data 
    into a number union. The union consists of an integer and a float of equal size, namely 32 bits. Once the 
    program has bitshifted the bit pattern into the integer part of the number union it decides if it is an 
    integer or a float bit pattern and passes the proper section of it to its respective function.
    
        If it is an integer, num.i is passed to intToDecASCII which converts the number into a backwards string
    by modding it and diving by 10, and then it reverses it and places it into the buffer array passed to the 
    function.
    
        If it is a float it is handled by floatToASCII which is a function written by Professor Russell which 
    turns a floating point number into a string in the format of (-)x.xxxxxxe(-)x.

## Important Details & Edits to *floatToASCII*:
 - The original function return values of +inf, -inf, +NaN and –NaN have been altered to pinf, ninf, NaN and NaN respectively
 - When a single digit exponent was found the old function padded it on the left with one zero, I removed this padded zero.
