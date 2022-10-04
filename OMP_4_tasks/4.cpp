#include <cstdio>
#include <omp.h>

//Написать программу, вычисляющую количество простых чисел в
//диапазоне от 2 до N. Ускорить ее с помощью OpenMP.

int main() {
    const int N = 100000;
    int i, j;
    int div, num_prime;
    double time;
    int NumThreads;

    #pragma omp parallel default(none) shared(NumThreads)
    {
        #pragma omp single
        {
            NumThreads = omp_get_num_threads();
            printf("Threads count is: %d\n", NumThreads);
        }
    }

    num_prime = 0;
    time = omp_get_wtime();
    // TODO: try to accelerate the loop below
    for (i = 2; i <= N; i++) {
        div = 0;
        for (j = 2; j <= i; j++) {
            if (i % j == 0) {
                div++;
            }
        }
        if (div == 1) {
            num_prime++;
        }
    }
    time = omp_get_wtime() - time;

    printf("Prime numbers serial: %d\n", num_prime);
    printf("Time serial: %f\n", time);

    num_prime = 0;
    time = omp_get_wtime();

    #pragma omp parallel default(none) shared(num_prime) private(i, j, div)
    {
        #pragma omp for reduction(+:num_prime)
        for (i = 2; i <= N; i++)
        {
            div = 0;

            for (j = 2; j <= i; j++)
            {
                if (i % j == 0)
                {
                    div++;
                }
            }

            if (div == 1) {
                num_prime++;
            }
        }
    }

    time = omp_get_wtime() - time;

    printf("Prime numbers parallel: %d\n", num_prime);
    printf("Time parallel: %f\n", time);

    return 0;
}