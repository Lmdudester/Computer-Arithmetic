# Computer-Arithmetic

##### Liam Davies - Rutgers University
##### Computer Architecture - Fall 2016  
##### Professor Brian Russell

# Synopsis:

Computer Arithmetic is a C Library with two libraries. 

Calc takes in 4 arguments, the first being the operation (+ or -), the next two being numbers (of bases hexadecimal, decimal octal or binary) in the format for a string and the last being a single character string (representing the base of the output). It adds the two given numbers together and returns the resulting number in the given output base.

Format takes in a bit sequence of 32 bits representative of a float or integer and a string that gives the type the first string should be interpreted as. It interprets the number as such and prints out the finalized number in its repective format.

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
