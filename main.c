#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

void test1() {
    // Constructors and show
    long long n = 1234LL;
    BigIntPtr p = newBigInt(n);
    show(p);
    n = 789456123LL;
    p = newBigInt(n);
    show(p);
    n = 123789456123LL;
    p = newBigInt(n);
    show(p);

    n = -123123LL;
    p = newBigInt(n);
    show(p);

    char * string = "423323456789223456789123456789";
    p = newBigIntFromString(string);
    show(p);
    
    p=newBigIntRand();
    show(p);
}

void test2()
{// test add int
    long long n;
    BigIntPtr p, p2;
    
    printf("-------------\nbillion add 1\n");
    n = 1000000000LL;
    p = newBigInt(n);
    p2=add(p,1);
    show(p2);
    printf("-------------\n1234 add 1234\n");
    n = 1234LL;
    p = newBigInt(n);
    p2=add(p,1234);
    show(p2);
    printf("-------------\n1 add 999 999 999\n");
    n = 1LL;
    p = newBigInt(n);
    p2 = add(p, 999999999);
    show(p2);
    printf("-------------\n-1234 add 1234\n");
    n = -1234LL;
    p = newBigInt(n);
    p2=add(p,1234);
    show(p2);
    printf("-------------\n-1233 add 1234\n");
    n = -1233LL;
    p = newBigInt(n);
    p2=add(p,1234);
    show(p2);
    printf("-------------\n1233 add -1233\n");
    n = 1233LL;
    p = newBigInt(n);
    p2=add(p,-1232);
    show(p2);
    printf("-------------\n1234 add billion and 1\n");
    n = 1234LL;
    p = newBigInt(n);
    p2=add(p,ONE_BILLION+1);
    show(p2);
    
    
    // cannot handle negative results eg -1235 + 1234
    
}

void test3()
{
    BigIntPtr p1, p2;
    p1=newBigInt(2);
    p2=newBigInt(3);
    printf("%d\n", equals(p1,p2));
    p1=newBigInt(1234);
    p2=newBigInt(1234);
    printf("%d\n", equals(p1,p2));
}

int main() {
    // Testing BigInt
    test1(); // ctrs and show
    //test2(); // add int
    //test3(); // equals

    return 0;
}
