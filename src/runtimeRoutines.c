#include <omp.h>
#include <stdio.h>

void main()
{
    int n = 10;
    omp_set_num_threads(n);
    #pragma omp parallel for
    for(int i = 0; i<n; i++)
    {
        printf("Thread num: %d\n", omp_get_thread_num());
        printf("In parallel: %d\n" , omp_in_parallel());
    }
    printf("In parallel: %d\n" , omp_in_parallel());
    printf("%d\n", omp_get_num_threads());
    printf("%d\n", omp_get_thread_limit());
    printf("%d\n", omp_get_num_procs());
}

