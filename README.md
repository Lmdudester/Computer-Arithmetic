# Computer-Arithmetic

#### Liam Davies - Rutgers University
#### Computer Architecture - Fall 2016  
#### Professor Brian Russell

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
    
### THINGS TO NOTE ABOUT bNum:
 - The binary string (number) is ordered with the least significant digit at the beginning  to make arithmetic easier
 - The binary string (number) is kept in a string with size divisible by 32, meaning that if a signed input number needs 33 bits to be represented, its number variable would have 64 non-null byte characters. This allows for quick size matching so twos compliment can be used in arithmetic.
 - The null byte is included in the number of chars used as tracked by numDigits
 - The struct itself is dynamically allocated along with the char * inside
 
                Next it matches the string sizes of both numbers. This makes it so if a number is negative or being 
        subtracted, we can twos compliment it and just add it to the other number, then twos compliment the result 
        and change the sign if needed. In matching the size it makes both numbers have a string length equal to the
        higher of the two, adding extra zeros onto the end of the smaller. Then it adds 4 zeros to both numbers to 
        account for overflow. (Only one zero is needed for potential overflow but 4 are needed to ensure the 
        conversion methods back to ASCII collect the extra overflow data) 
