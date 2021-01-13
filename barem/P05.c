#include <stdio.h>
int n, suma;
int main() {

    scanf("%d", &n);
    suma = 1 + n;
    for(int i = 2; i <= n / 2; i++) {
        if(n % i == 0) {
            suma += i;
        }
    }
    printf("%d", suma);
    return 0;
}