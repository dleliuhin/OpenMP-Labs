#include <omp.h>
#include <stdio.h>

int main(int argc, char** argv)
{
#pragma omp parallel
    {
        auto count = omp_get_thread_num();
        auto ItsMe = omp_get_num_threads();
        printf("Hello, OpenMP! I am %d of %d\n", count, ItsMe );
    }
    return 0;
}
