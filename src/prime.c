#include <stdio.h>
#include <math.h>
#include "prime.h"

int is_prime(const int x){
    if(x<2)
        return -1; //undefined
    if(x==2)
        return 1;
    if(x%2 ==0)
        return 0;

    int rt= ceil(sqrt(x));
    for(int i=3;i<=rt;i+=2){
        if(x%i == 0)
            return 0;//not prime
    }

    return 1; //prime
}
int next_prime(int x){
    if(x<=2)
        return 2;
    if(x%2 == 0)
        x++;//make it odd

    while(is_prime(x) != 1){x+=2;}

    return x;
}
