#include <stdio.h>
int n, suma;
int main() {

    FILE *pread = fopen("iulian.in", "r");
    FILE *pwrite = fopen("iulian.out", "w");
    
    if(pread == NULL) return 0;
    if(pwrite == NULL) return 0;


    fscanf(pread, "%d", &n);
    suma = 1 + n;
    for(int i = 2; i <= n / 2; i++) {
        if(n % i == 0) {
            suma += i;
        }
    }
    fprintf(pwrite, "%d", suma);

    fclose(pwrite);
    fclose(pread);
    return 0;
}