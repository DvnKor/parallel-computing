#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include <ctime>


//Элементы массива a инициализируются 0, массива b[0]…b[n-1] –
//случайными числами от 0 до n-1. Распараллелить цикл
//for (i=0; i<n; i++) a[b[i]]++;
//(то есть, построить гистограмму по массиву)


void fill_zero(long int *arr, long int count)
{
    int i;
    for(i=0; i<count; i++)
    {
		arr[i] = 0;
    }
}

void fill_rand(long int *arr, long int count, long int range)
{
    srand(time(nullptr));
    int i;
    for(i=0; i<count; i++)
    {
		arr[i] = rand() % (range);
    }
}

int main()
{
    const long int Nhist = 500;
    const long int Nupd = 10*1000;

    long int *hist1, *hist2, *upd;
    hist1 = (long int*) malloc(Nhist*sizeof(long int));
    hist2 = (long int*) malloc(Nhist*sizeof(long int));

    upd = (long int*) malloc(Nupd*sizeof(long int));

    int NumThreads = omp_get_num_procs();
    printf("Thread count is: %d\n", NumThreads);
    //double time;
    
    fill_zero(hist1, Nhist);
    fill_zero(hist2, Nhist);
    fill_rand(upd, Nupd, Nhist);

    long int i;
    for(i=0; i<Nupd; i++)
    {
		hist1[upd[i]]++;
    }

    //TODO: why does it work incorrectly?
    //совместное использование hist2 в разных потоках, возможны состояния гонки
    //#pragma omp parallel for
    //for (i = 0; i<Nupd; i++)
    //{
    //	hist2[upd[i]]++;
    //}


    #pragma omp parallel default(none) private(i) shared(Nhist, upd, hist2)
    {
        #pragma omp for reduction(+:hist2[:Nhist])
        for (i = 0; i < Nupd; i++) {
            hist2[upd[i]]++;
        }
    }
    for(i=0; i<Nhist; i++)
    {
		if(hist1[i] != hist2[i])
		{
			printf("Error!\n");
			return 0;
		}
    }
	printf("OK!\n");
    
    free(hist1);
    free(hist2);
    free(upd);
    return 0;
}
