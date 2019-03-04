#include <omp.h>
#include <stdio.h>

void main(){
    int var1 = 10;
    #pragma omp parallel firstprivate(var1)
    {
        printf("var1:%d\n", var1);
    }
}