#include <stdio.h>

int prim(int m);
int n, a, c;
int main(int argc, char* argv[]) {

    scanf("%d", &n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &a);
        if(prim(a)) c++;
    }
    printf("%d", c);
    return 0;
}

int prim(int m) {
    if(m == 0 || m == 1) return 0;
    for(int i = 2; i * i <= m; i++) {
        if(m % i == 0) return 0;
    }
    return 1;
}