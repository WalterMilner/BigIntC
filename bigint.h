// bigint.h

#ifndef _BIGINT
#define _BIGINT

#include <stdbool.h>

/****************************************************

A BigInt is held in a form of binary-coded decimal, in an array of
unsigned long long elements
ANSI requires an unsigned long long to be at least 18 X 10^18
We hold 0 to 10^9-1 ( 9 decimal digits) in each element
Then an intermediate product of the two = 10^18 will also fit into an unsigned long long
 
This format is designed to be able to use 64bit processor instructions to do arithmetic in 10^9 size chunks.

 * The sign is held in the 'sign' field as a char, + or -

The array length is the constant ARRAY_LENGTH.
In this version this is 10000, so we can handle up to 9 X 10000 = 90 000 decimal digits.
 * A number will use around 10 000 X 8 = 80k bytes

 * In future versions we may use a linked list of these, to have number size limited only by available memory




*****************************************************/

#define ARRAY_LENGTH 10000
#define ONE_BILLION 1000000000 

struct BIStruct
{
char sign;                          // + or -
unsigned long long digits[ARRAY_LENGTH];    // The digits, in 10^9 chunks
int elementCount;                   // The array has elementCount elements in use - that is, 0 to elementCount-1
// any others are garbage
};


/* a BigIntPtr is a pointer to a BIStruct
 * */   
typedef struct BIStruct *  BigIntPtr;

typedef unsigned long long bits64;
// bits64 is in fact a long long. This is at least 64 bits long, maybe more. We
// treat it as 64 bits long.

/*  Function prototypes */

BigIntPtr newBigInt(long long);
BigIntPtr newBigIntFromString(char * value);
BigIntPtr  newBigIntRand();
BigIntPtr add(BigIntPtr this, int x);
bool equals(BigIntPtr this, BigIntPtr other);
void show(BigIntPtr ptr);

#endif
