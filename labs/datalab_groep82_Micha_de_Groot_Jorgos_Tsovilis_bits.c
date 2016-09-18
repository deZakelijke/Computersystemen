// Made by Micha de Groot,10434410 and Jorgos Tsovilis,10246878
/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif

/* 
 * CS:APP Data Lab @ Universiteit van Amsterdam
 *
 * Modification to original Data Lab:
 *
 * the collection of puzzles is automatically generated for each 
 * programming pair.
 * 
 * Augustus 2016: A.Visser@uva.nl
 *
 */



/* Global ID to identify the combination of puzzles */
int lab_id = 82;
/* 
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
// We use the logic formula of the disjunction of two negations. That is only false when both arguments are true.
// Negate the result and it becomes a conjunction.
int bitOr(int x, int y) {
  return ~(~x & ~y);
}
/* 
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
// First we create an integer with a 1 on the most signifaicant bit by shifting 31 times to the left.
// Then we rightshift 31 times arithmetically to make an integer with all 1's: -1
int minusOne(void) {
  int integer = 1 << 31;
  integer = integer >> 31;
  return integer;
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
// A mask is made by shifting 0xAA and OR'ing it together with the other pieces of the mask.
// This results in a mask of 0xAAAAAAAA
// By conjunctiong the input with the mask and then XOR'ing it we get 0x00000000 if the input has all 1's on the even bits
// Othewise there wil be at least one 1. Finally we use logic negation to convert the 0 into 1 and vice versa
int allOddBits(int x) {
    int mask = 0xAA;
    int temp = 0xAA;
    int number;
    temp = temp << 8;
    mask = temp | mask;
    temp = temp << 8;
    mask = temp | mask;
    temp = temp << 8;
    mask = temp | mask;
    number = x & mask;
    number = number ^ mask;
    return ! number;
}
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
    int sign = 1 << 31;
    int upperbound = ~(sign | 0x39);
    int lowerbound = ~0x30;

    upperbound = sign & (upperbound+x) >> 31;
    lowerbound = sign & (lowerbound+1+x) >> 31;
  return !(upperbound | lowerbound);
}
/*
 * leftBitCount - returns count of number of consective 1's in
 *     left-hand (most significant) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 4
 */
int leftBitCount(int x) {
	int res;
	int tmp = x;
   	int shft;
    // First shift by 16 bits to check if we are left with one or zero.
    res = !(~(tmp>>16)) << 4; // If the first 16 bits are ones we will be left wih a one. If not then we're left with a zero. 
    tmp = tmp << res;
    // Shift by 24 bits to check the remaining 8 bits.
    shft = !(~(tmp >> 24)) << 3;
    tmp = tmp << shft;
    res = res | shft;
    // Shift by 28 bits to check the remaining 4.
    shft = !(~(tmp>>28)) << 2;
    tmp = tmp << shft;
    res = res | shft;
    // check last two bits.
    shft = !(~(tmp >> 30)) << 1;
    tmp = tmp << shft;
    res = res | shft;
    // check last bit.
    res= res ^ (1&((tmp>>31)));
    // Add one to the result to have 32 bits.
    return res + !(~x);
}
/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
// split the sign, exponent and frac from the input
    int mask = 0x80000000;
    int sign = uf & mask;
    int bias = 127;
    int exp, e, frac;
    mask = 0x7F800000;
    exp = (mask & uf) >> 23;
    e = exp - bias;
    mask = 0x007FFFFF;
    frac = uf & mask;

// Several edge cases
    if(exp == 0x7F800000){
        return 0x80000000u;
    }
    if(!exp){
        return 0;
    }
    if(e<0){
        return 0;
    }
    if(e >30){
        return 0x80000000u;
    }

// Compensate for the fact that the frac is already shifted 23 times
    frac = frac | 0x800000;
    if(e >= 23){
        frac = frac << (e-23);
    }else{
        frac = frac >> (23-e);
    }
// Add sign 
    if(sign){
        frac = ~frac+1;
    }

    return frac;
}
