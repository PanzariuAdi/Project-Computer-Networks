#include <stdio.h>

int main(int argc, char * argv[]) {
    long long n,i,s,r,c;
    scanf("%lld", &n);
    s = 0;
    r = n % 10;
    c = n / 10;
    s += 33 * c;
    if(r == 1) s += 1;
    if(r == 2) s += 7;
    if(r == 3) s += 8;
    if(r == 4) s += 14;
    if(r == 5) s += 19;
    if(r == 6) s += 25;
    if(r == 7) s += 26;
    if(r == 8) s += 32;
    if(r == 9) s += 33;
    printf("%lld", s % 10);
    return 0;
}