#include <omp.h>
#include <stdio.h>

int lock();

void main()
{
    int count = 0;
    #pragma omp parallel
    #pragma omp critical
        count++;
    printf("%d\n", count);
    printf("%d\n", lock());
}

int lock() {
    int count = 0;
    omp_lock_t lock;
    omp_init_lock(&lock);
    #pragma omp parallel
    {
        omp_set_lock(&lock);
        count++;
        omp_unset_lock(&lock);
    }
    omp_destroy_lock(&lock);
    return count;

}