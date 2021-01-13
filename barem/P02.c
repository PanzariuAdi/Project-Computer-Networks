#include <stdio.h>
#include <limits.h>

int n, a, maxim;

int main(int argc, char * argv[]) {
    maxim = INT_MIN;
    scanf("%d", &n);
    
    for(int i = 0; i < n; i++) {
        scanf("%d", &a);
        if(a > maxim)
            maxim = a;
    }

    printf("%d", maxim);
    return 0;
}