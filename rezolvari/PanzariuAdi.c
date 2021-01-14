#include <stdio.h>
#include <limits.h>

int n, a, maxim;

int main(int argc, char * argv[]) {
    FILE *pread = fopen("PanzariuAdi.in", "r");
    FILE *pwrite = fopen("PanzariuAdi.out", "w");
    
    if(pread == NULL) return 0;
    if(pwrite == NULL) return 0;

    maxim = INT_MIN;
    fscanf(pread, "%d", &n);
    
    for(int i = 0; i < n; i++) {
        fscanf(pread, "%d", &a);
        if(a > maxim)
            maxim = a;
    }
    fprintf(pwrite, "%d", maxim);
    fclose(pwrite);
    fclose(pread);
    return 0;
}
