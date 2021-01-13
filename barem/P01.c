#include <stdio.h>

int n, a, b, c, i;
int main() {

    scanf("%d", &n);
    a = b = 1;
    if(n == 1) printf("1");
    else if(n == 2) printf("1 1");
    else {
        printf("1 1 ");
        i = 2;
        while(i < n) {
            c = a + b;
            printf("%d ", c);
            a = b;
            b = c;
            i++;
        }
    }
    return 0;
}