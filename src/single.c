#include<omp.h>
#include <stdio.h>

void main()
{
    int input;
    #pragma omp parallel num_threads(4) shared(input)
    {
        #pragma omp single
        {
            scanf("%d", &input);
        }
    printf("input is %d\n", input);
    }
}