#include <omp.h>
#include <stdio.h>

void main()
{
    int i, n, chunk, a[100], b[100], result;
    n = 100; chunk = 2; result=0;
    for(i=0;i<n;i++) a[i] = b[i] = i;

    #pragma omp parallel for default(shared) private(i) schedule(static, chunk) reduction(+:result)
    for(i=0;i<n;i++) result = result+(a[i]*b[i]);

    printf("Final result=%d\n", result);
}