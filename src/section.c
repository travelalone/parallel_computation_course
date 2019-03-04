#include <omp.h>
#include <stdio.h> 

void main() {
    int N = 1000;
    int a[N], b[N], c[N], d[N];

    #pragma omp parallel num_threads(2) shared(a,b,c,d)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                for(int i = 0; i<N; i++) c[i] = a[i] + b[i];
            }
            #pragma omp section
            {
                for (int i=0; i<N; i++) d[i] = a[i] + b[i];
            }

        }
    }
    printf("%d\n", c[0]);
    printf("%d\n", d[0]);

}
