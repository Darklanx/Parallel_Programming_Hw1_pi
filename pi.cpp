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

pthread_mutex_t lock;
unsigned long long number_of_cpu, number_of_tosses, number_in_circle = 0;
clock_t begin_time;

void *toss(void *params) {

    Params *_params = (Params *)params;
    //printf("thread %d start at: %f\n", _params->id, float(clock() - begin_time) / CLOCKS_PER_SEC);
    unsigned long long _number_in_circle = 0;
    double x, y, distance_squared;
    unsigned int seed = time(NULL);
    for (ull toss = 0; toss < _params->toss; toss++) {
        // x = random double between -1 and 1;
        // y = random double between -1 and 1;

        //printf("seed %u\n", seed);
        x = (double)rand_r(&seed) / RAND_MAX;
        y = (double)rand_r(&seed) / RAND_MAX;

        /*
        if (toss % 100 == 0) {
            printf("thread %d access toss: %lld at: %f\n", _params->id, toss, float(clock() - begin_time) / CLOCKS_PER_SEC);
        }
        */
        distance_squared = x * x + y * y;
        if (distance_squared <= 1)
            _number_in_circle++;
    }
    //printf("total toss: %lld\n", _params->toss);
    /*
    pthread_mutex_lock(&lock);
    number_in_circle += _number_in_circle;
    pthread_mutex_unlock(&lock);
    */
    //printf("thread %d done at: %f\n", _params->id, float(clock() - begin_time) / CLOCKS_PER_SEC);
    delete _params;

    pthread_exit((void *)new unsigned long long(_number_in_circle));
}

int32_t main(int32_t argc, char **argv) {
    begin_time = clock();
    double pi_estimate, distance_squared, x, y;
    pthread_mutex_init(&lock, NULL);
    if (argc < 2) {
        exit(-1);
    }
    N_THREAD = number_of_cpu = atoi(argv[1]);
    number_of_tosses = atoi(argv[2]);
    if ((number_of_cpu < 1) || (number_of_tosses < 0)) {
        exit(-1);
    }

    // start here
    pthread_t threads[20];
    //printf("Before create: %f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);
    for (int i = 0; i < N_THREAD; i++) {
        Params *params = new Params();
        params->toss = number_of_tosses / N_THREAD + (i < (number_of_tosses % N_THREAD));

        //printf("%d\n", params->toss);
        params->id = i;
        pthread_create(&threads[i], NULL, toss, (void *)params);
    }

    //printf("After create: %f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);
    //printf("Before Join: %f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);

    void *rv;
    for (int i = 0; i < N_THREAD; i++) {

        //pthread_join(threads[i], NULL);
        pthread_join(threads[i], &rv);
        number_in_circle += *(unsigned long long *)rv;
        delete (unsigned long long *)rv;
    }
    //printf("After Join: %f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);

    /*
    for (toss = 0; toss < number_of_tosses; toss++) {
        // x = random double between -1 and 1;
        // y = random double between -1 and 1;
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        distance_squared = x * x + y * y;
        if (distance_squared <= 1)
            number_in_circle++;
    }
    */

    pi_estimate = 4 * number_in_circle / ((double)number_of_tosses);

    printf("%f\n", pi_estimate);
    pthread_mutex_destroy(&lock);
    printf("Total Time: %f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);
    return 0;
}
