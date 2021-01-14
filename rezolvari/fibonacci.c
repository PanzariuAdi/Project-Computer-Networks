#include <stdio.h>

int n, a, b, c, i;
int main() {

    FILE *pread = fopen("fibonacci.in", "r");
    FILE *pwrite = fopen("fibonacci.out", "w");

    if(pread == NULL) return 0;
    if(pwrite == NULL) return 0;

    fscanf(pread, "%d", &n);

    a = b = 1;
    if(n == 1) fprintf(pwrite, "1");
    else if(n == 2) fprintf(pwrite, "1 1");
    else {
        fprintf(pwrite, "1 1 ");
        i = 2;
        while(i < n) {
            c = a + b;
            fprintf(pwrite, "%d ", c);
            a = b;
            b = c;
            i++;
        }
    }

    fclose(pwrite);
    fclose(pread);
    return 0;
}