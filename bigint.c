// bigint.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <time.h>


#include "bigint.h"

// prototypes..
void checkBuffer(char * ptr);

// make a pseudo-random BigInt     

BigIntPtr newBigIntRand() {
    time_t t;
    srand((unsigned) time(&t));
    BigIntPtr ptr = (BigIntPtr) malloc(sizeof (struct BIStruct)); //make space
    //ptr->digits = (long long *) malloc(ARRAY_LENGTH*sizeof(long long));
    for (int index = 0; index < ARRAY_LENGTH; index++) {
        ptr->digits[index] = rand() % ONE_BILLION;
    }
    ptr->elementCount = ARRAY_LENGTH; 
    return ptr;

}

// make a BigInt from a signed long long

BigIntPtr newBigInt(long long value) {
    BigIntPtr ptr = (BigIntPtr) malloc(sizeof (struct BIStruct)); //make space
    // handle sign
    if (value > 0)
        ptr->sign = '+';
    else {
        ptr->sign = '-';
        value = -value;
    }
    // split value into 10^9 sized chunks
    bits64 n = value;
    int index = 0;
    while (n > 0LL) {
        bits64 r = n % 1000000000;
        ptr->digits[index] = r;
        index++;
        n = n / 1000000000;
    }
    ptr->elementCount = index;
    return ptr;
}

bool equals(BigIntPtr this, BigIntPtr other) {
    if (this->sign != other->sign) return false;
    if (this->elementCount != other->elementCount) return false;
    for (int index = 0; index < this->elementCount; index++) {
        if (this->digits[index] != other->digits[index]) return false;
    }
    return true;
}

BigIntPtr add(BigIntPtr this, int x) {
    // This will be the result
    struct BIStruct * temp = (struct BIStruct *) malloc(sizeof (struct BIStruct));
    BigIntPtr newPtr = temp;
    // an int might be over 999 999 999, so may need to add into 
    // first two elements
    int lo = x % ONE_BILLION;
    int hi = x / ONE_BILLION;
    // printf("hi = %d lo = %d \n",hi,lo);
    bits64 carry = (bits64) lo;
    bits64 sum;
    for (int index = 0; index < this->elementCount; index++) {
        sum = this->digits[index] + carry;
        if (index == 1)
            sum += hi;
        carry = sum / 1000000000LL;
        newPtr->digits[index] = sum % 1000000000LL;
        // printf("index=%d sum = %llu thisDigit = %llu carry =.. %llu\n", index, sum, newPtr->digits[index], carry);
    }
    if (carry == 0LL)
        newPtr->elementCount = this->elementCount;
    else {
        newPtr->elementCount = this->elementCount + 1;
        newPtr->digits[newPtr->elementCount - 1] = carry;

    }
    if (this->elementCount == 1 && hi != 0) {
        newPtr->digits[1] += hi;
        newPtr->elementCount = 2;
        // printf("\nhi = %d\n", hi);
    }
    return newPtr;
}
// make a BigInt from a string ----------------------------------------------
// string should be like 12323 or +273737 or -27273737

BigIntPtr newBigIntFromString(char * value) {
    // make a struct
    struct BIStruct * temp = (struct BIStruct *) malloc(sizeof (struct BIStruct));
    BigIntPtr ptr = temp;
    // do we start with a sign?
    char c = *value;
    if (c == '+') {
        ptr->sign = '+';
        value++; // move past sign
    } else if (c == '-') {
        ptr->sign = '-';
        value++;
    } else { // no sign in string
        ptr->sign = '+';
    }
    // need to process string in blocks of 9 digits, plus those at the front
    int stringLength = strlen(value);
    int wholeBlockCount = stringLength / 9;
    int restCount = stringLength % 9;
    // value consists of wholeBlockCounts blocks of 9 digits
    // and restCount of digits beyond that, at most significant end

    char * buffer = (char *) malloc(10); // place to put 9 digit chars
    *(buffer + 10) = 0; // with terminating zero

    // split value into 10^9 sized chunks
    int index = 0;
    char * pointer = value + stringLength - 9;
    // start pointer at 'far end' least sig digit of value, 9 digits in 
    for (int index = 0; index < wholeBlockCount; index++) {
        strncpy(buffer, pointer, 9); // copy 9 digits
        char * endPtr = buffer + 9;
        char * * what = &endPtr; // this is weird..
        unsigned long long n = strtoumax(buffer, what, 10); // convert to number
        ptr->digits[index] = n; // put in array
        pointer -= 9; // move to next block of 9
    }
    ptr->elementCount = wholeBlockCount;
    if (restCount != 0) { // are some left over?
        pointer += 9; // at start of remainers
        strncpy(buffer, pointer - restCount, restCount); // copy them
        *(buffer + restCount) = 0; // terminate
        //checkBuffer(buffer);
        char * endPtr = buffer + restCount;
        char * * what = &endPtr; // and this..
        unsigned long long n = strtoumax(buffer, what, 10); // to number
        ptr->digits[wholeBlockCount] = n; // in array
        ptr->elementCount++; // one extra incomplete block
    }
    // done
    return ptr;
}

// end newBigIntFromString -------------------------------------------------

void checkBuffer(char * ptr) {
    char c;
    printf("Check buffer\n");
    do {
        c = *ptr;
        printf("%p %c %d\n", ptr, c, (int) c);
        if (c == 0) break;
        ptr++;
    } while (1);
    printf("\n");
}


// console display a BigInt
// formatting in blocks of 9 decimal digits

void show(BigIntPtr ptr) {

    printf("BigInt: elementCount = %d\n", ptr->elementCount);
    if (ptr->sign == '-')
        printf("-");
    // got just 1 block of 9 digits?
    if (ptr->elementCount == 1) {
        int dig3 = (ptr->digits[0] % 1000000000) / 1000000;
        printf(" %03d", dig3);
        dig3 = (ptr->digits[0] % 1000000) / 1000;
        printf(" %03d", dig3);
        dig3 = ptr->digits[0] % 1000;
        printf(" %03d", dig3);
    } else // print all, most sig block first
    {
        for (int index = ptr->elementCount - 1; index>-1; index--) {
            int dig3 = (ptr->digits[index] % 1000000000) / 1000000;
            printf("x%03d", dig3);
            dig3 = (ptr->digits[index] % 1000000) / 1000;
            printf(" %03d", dig3);
            dig3 = ptr->digits[index] % 1000;
            printf(" %03d", dig3);
            if (index % 12 == 0) printf("\n");
        }
    }
    printf("\n");
}