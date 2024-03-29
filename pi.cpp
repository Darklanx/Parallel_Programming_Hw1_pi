#include <algorithm>
#include <ctime>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
//#define N_THREAD 1
typedef unsigned long long ull;
//#define int ull
int N_THREAD;
struct Params {
    int id;
    ull toss;
};

pthread_mutex_t lock1;
unsigned long long number_of_cpu, number_of_tosses, number_in_circle = 0;

void *toss(void *params) {

    Params *_params = (Params *)params;

    unsigned long long _number_in_circle = 0;
    double x, y, distance_squared;
    unsigned int seed = time(NULL);
    for (ull toss = 0; toss < _params->toss; toss++) {
        // x = random double between -1 and 1;
        // y = random double between -1 and 1;

        x = (double)rand_r(&seed) / RAND_MAX;
        y = (double)rand_r(&seed) / RAND_MAX;

        distance_squared = x * x + y * y;
        if (distance_squared <= 1)
            _number_in_circle++;
    }

    pthread_mutex_lock(&lock1);
    number_in_circle += _number_in_circle;
    pthread_mutex_unlock(&lock1);

    delete _params;

    //pthread_exit((void *)new unsigned long long(_number_in_circle));
    pthread_exit(0);
}

int32_t main(int32_t argc, char **argv) {
    double pi_estimate, distance_squared, x, y;
    pthread_mutex_init(&lock1, NULL);
    if (argc < 2) {
        exit(-1);
    }
    N_THREAD = number_of_cpu = std::min(atoi(argv[1]), 20);
    number_of_tosses = strtoull(argv[2], NULL, 10);

    if ((number_of_cpu < 1) || (number_of_tosses < 0)) {
        exit(-1);
    }

    // start here
    pthread_t threads[20];
    for (int i = 0; i < N_THREAD; i++) {
        Params *params = new Params();
        params->toss = number_of_tosses / N_THREAD + (i < (number_of_tosses % N_THREAD));
        params->id = i;
        pthread_create(&threads[i], NULL, toss, (void *)params);
    }

    for (int i = 0; i < N_THREAD; i++) {
        pthread_join(threads[i], NULL);
    }

    pi_estimate = 4 * number_in_circle / ((double)number_of_tosses);

    printf("%f\n", pi_estimate);

    pthread_mutex_destroy(&lock1);
    return 0;
}
