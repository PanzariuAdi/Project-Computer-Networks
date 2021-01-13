#include <stdio.h>
#include <limits.h>

int n, a, minim;

int main(int argc, char * argv[]) {
    minim = INT_MAX;
    scanf("%d", &n);
    
    for(int i = 0; i < n; i++) {
        scanf("%d", &a);
        if(a < minim)
            minim = a;
    }

    printf("%d", minim);
    return 0;
}