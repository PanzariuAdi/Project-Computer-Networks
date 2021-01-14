#include <stdio.h>
#include <limits.h>

int n, a, maxim;

int main(int argc, char * argv[]) {
    FILE *pread = fopen("sumdiv.in", "r");
    FILE *pwrite = fopen("sumdiv.out", "w");
    
    if(pread == NULL) return 0;
    if(pwrite == NULL) return 0;

    fscanf(pread, "%d", &n);
    
    for(int i = 1; i <= n; i++) {
        if(n % i == 0)
            maxim += i;
    }
    fprintf(pwrite, "%d", maxim);
    fclose(pwrite);
    fclose(pread);
    return 0;
}
