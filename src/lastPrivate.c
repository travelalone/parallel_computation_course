#include <omp.h>
#include <stdio.h>
#include <unistd.h>

void main(){
    int i ;
    int x = 44;
    #pragma omp parallel for lastprivate(x) num_threads(4)
        for(i = 0; i<=10; i++)
        {
            x= i;
            printf("%d: %d\n", omp_get_thread_num(),x);
        }
    printf("x is %d\n", x);
}