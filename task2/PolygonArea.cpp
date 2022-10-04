#include <iostream>
#include <cmath>
#include <clocale>
#include <omp.h>
#include <cfloat>
#include <windows.h>

using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    printf("Введите количество вершин");
    int n;
    cin >> n;
    auto* x = (double*)malloc(n * sizeof(int));
    auto* y = (double*)malloc(n * sizeof(int));

    printf("Введите координаты в виде пар X_i Y_i по часовой стрелке");
    for (int i = 0; i < n; i++)
    {
        cin >> x[i] >> y[i];
    }

    int max_threads_count = omp_get_num_procs();
    int best_thread_count = 1;
    auto best_time = DBL_MAX;
    for (int current_threads_count = 1; current_threads_count <= max_threads_count; current_threads_count++)
    {
        printf("\n\nКоличество потоков %d\n", current_threads_count);
        omp_set_num_threads(current_threads_count);

        double start = omp_get_wtime();

        double s = 0;
        int i = 0;
        #pragma omp parallel default(none) shared(s, n, x, y) private(i)
        {
            #pragma omp for reduction(+:s)
            for (i = 0; i < n - 1; i++) {
                s += x[i] * y[i + 1] - x[i + 1] * y[i];
            }
        }

        s += x[n - 1] * y[0] - x[0] * y[n - 1];

        s = fabs(s) / 2;

        double time = omp_get_wtime() - start;

        printf("Площадь прямоугольника %f\n", s);
        printf("Время выполнения %f секунд", time);

        if (time < best_time)
        {
            best_thread_count = current_threads_count;
            best_time = time;
        }
    }

    printf("\nЛучшее количество потоков: %d, время: %f\n", best_thread_count, best_time);

    getchar();

    return 0;
}

