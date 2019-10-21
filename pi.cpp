#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N_THREAD 1
int main(int argc, char **argv) {
    double pi_estimate, distance_squared, x, y;
    unsigned long long number_of_cpu, number_of_tosses, number_in_circle, toss;
    if (argc < 2) {
        exit(-1);
    }
    number_of_cpu = atoi(argv[1]);
    number_of_tosses = atoi(argv[2]);
    if ((number_of_cpu < 1) || (number_of_tosses < 0)) {
        exit(-1);
    }

    // start here
    pthread_t threads[N_THREAD];
    /*
    for (int i = 0; i < N_THREAD; i++) {
        pthread_create(&threads[i], NULL, )
    }
    */
    number_in_circle = 0;
    for (toss = 0; toss < number_of_tosses; toss++) {
        // x = random double between -1 and 1;
        // y = random double between -1 and 1;
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;

        distance_squared = x * x + y * y;
        if (distance_squared <= 1)
            number_in_circle++;
    }
    pi_estimate = 4 * number_in_circle / ((double)number_of_tosses);

    printf("%f\n", pi_estimate);

    return 0;
}
