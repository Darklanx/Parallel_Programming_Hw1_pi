#include <ctime>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
//#define N_THREAD 1
using namespace std;
typedef unsigned long long ull;
//#define int ull
int N_THREAD;
struct Params {
    int id;
    ull toss;
};

pthread_mutex_t lock1;
unsigned long long number_of_cpu, number_of_tosses, number_in_circle = 0;

ull toss(ull n_toss) {

    unsigned long long _number_in_circle = 0;
    double x, y, distance_squared;
    unsigned int seed = time(NULL);
    for (ull toss = 0; toss < n_toss; toss++) {
        // x = random double between -1 and 1;
        // y = random double between -1 and 1;

        x = (double)rand_r(&seed) / RAND_MAX;
        y = (double)rand_r(&seed) / RAND_MAX;

        distance_squared = x * x + y * y;
        if (distance_squared <= 1)
            _number_in_circle++;
    }

    return _number_in_circle;
}
void *sum_toss(void *params) {

    Params *_params = (Params *)params;
    //cout << "id: " << _params->id << endl;
    //cout << "work: " << _params->toss << endl;
    bool wait = 0;
    pthread_t thread;
    if (number_of_cpu - 1 != _params->id) { // still has to create more thread
        wait = 1;
        Params *__params = new Params();
        __params->id = _params->id + 1;
        __params->toss = number_of_tosses / N_THREAD + (__params->id < (number_of_tosses % N_THREAD));
        pthread_create(&thread, NULL, sum_toss, (void *)__params);
    }
    // compute self value
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

    delete _params;
    if (wait) {
        ull *rv;
        pthread_join(thread, (void **)&rv);
        _number_in_circle += *rv;
    }
    pthread_exit((void *)new unsigned long long(_number_in_circle));
}

int32_t main(int32_t argc, char **argv) {
    double pi_estimate, distance_squared, x, y;
    pthread_mutex_init(&lock1, NULL);
    if (argc < 2) {
        exit(-1);
    }
    N_THREAD = number_of_cpu = atoi(argv[1]);
    number_of_tosses = strtoull(argv[2], NULL, 10);

    if ((number_of_cpu < 1) || (number_of_tosses < 0)) {
        exit(-1);
    }

    // start here
    pthread_t thread;
    int id = 0;
    if (number_of_cpu > 1) {

        Params *params = new Params();
        params->id = 1;
        params->toss = number_of_tosses / N_THREAD + (params->id < (number_of_tosses % N_THREAD));

        pthread_create(&thread, NULL, sum_toss, (void *)params);
        ull *rv;
        ull n_toss = number_of_tosses / N_THREAD + (id < (number_of_tosses % N_THREAD));
        number_in_circle = toss(n_toss);
        pthread_join(thread, (void **)&rv);
        //printf("rv: %llu\n", *rv);
        number_in_circle += *rv;
    }

    pi_estimate = 4 * number_in_circle / ((double)number_of_tosses);

    printf("%f\n", pi_estimate);

    pthread_mutex_destroy(&lock1);
    return 0;
}
