#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <omp.h>

//Ускорить выполнение цикла for в программе, вычисляющей
//покоординатную функцию от элементов массива a: a[i]=F(a[i]);

int main()
{
    const long int N = 1000*1000*1000;
    double time;
    int NumThreads = omp_get_num_procs();
    
    double *a;
    a = (double*) malloc(N*sizeof(double));
    
    long int i;
    for(i=0; i<N; i++)
    {
		a[i] = (double) i;
    }
    printf("a[10000]=%f\n",a[10000]);

    printf("Threads count is: %d\n", NumThreads);
    
    time = omp_get_wtime();
    for(i=0; i<N; i++)
    {
		a[i] = sqrt(a[i]);
    }
    time = omp_get_wtime() - time;
    printf("Serial time: %f\n", time);
    printf("a[10000]=%f\n",a[10000]);


    for(i=0; i<N; i++)
    {
        a[i] = (double) i;
    }
    printf("a[10000]=%f\n",a[10000]);

    //TODO: try to accelerate
    time = omp_get_wtime();

    #pragma omp parallel default(none) shared(a)
    {
        #pragma omp for
        for (i = 0; i < N; i++) {
            a[i] = sqrt(a[i]);
        }
    }

    time = omp_get_wtime() - time;
    printf("Parallel time: %f\n", time);
    printf("a[10000]=%f\n",a[10000]);

    free(a);
    return 0;
}