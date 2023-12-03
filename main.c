#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void shell_sort(int threads, int random_seed, double *time) {
    const int count = 100000;     ///< Number of array elements

    int *array;                 ///< The array we need to find the max in

    /* Initialize the RNG */
    srand(random_seed);

    /* Generate the random array */
    array = (int *) malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) { array[i] = rand(); }

    double start_time = omp_get_wtime();

#pragma omp parallel num_threads(threads) shared(count, array, threads) default(none)

    for (int s = count / 2; s > 0; s /= 2) {
#pragma omp for
        for (int i = s; i < count; ++i) {

            for (int j = i - s; j >= 0 && array[j] > array[j + s]; j -= s) {
                int temp = array[j];
                array[j] = array[j + s];
                array[j + s] = temp;
            }
        }
    }

    double end_time = omp_get_wtime();
    *time = end_time - start_time;
}

int main() {
    double time[16][10];
    for (int i = 1; i <= 16; i++) {
        int random_seed = 920214;
        for (int j = 0; j < 10; j++) {
            shell_sort(i, random_seed + j, &time[i - 1][j]);
        }
    }
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%.8f\n", time[i][j]);
        }
    }
    return 0;
}
