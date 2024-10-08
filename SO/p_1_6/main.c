/*
6. Scrieti un program care creaza 9 thread-uri, fiecare dintre acestea
 executand o bucla infinita in care asteapta un timp aleator intre 1 si
 5 secunde, apoi isi afisaza pe ecran numarul (de la 1 la 9). Pe thread-ul
 principal se citesc cifre de la tastatura. Atunci cand se introduce cifra
 corespunzatoare unui thread, acesta va fi anulat. După ce s-au inchis
 toate cele 9 thread-uri (trebuie sa se si astepte terminarea lor), se va
 inchide si thread-ul principal.
*/

/*
gcc -o main.o main.c
./main.o
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 9

typedef struct {
    int thread_num;
    int *should_exit;
    pthread_mutex_t *mutex;
} thread_arg_t;

void *thread_function(void *arg) {
    thread_arg_t *targ = (thread_arg_t *)arg;
    
    // printf("Thread %d s-a deschis.\n", targ->thread_num);

    srand(time(NULL) + targ->thread_num);

    while (1) {
        int sleep_time = rand() % 5 + 1;
        
        // printf("Thread %d asteapta %d secunde.\n", targ->thread_num, sleep_time);

        sleep(sleep_time);

        pthread_mutex_lock(targ->mutex);

        if (*(targ->should_exit)) {
            pthread_mutex_unlock(targ->mutex);
            // printf("Thread %d se inchide...\n", targ->thread_num);
            pthread_exit(NULL);
        }

        pthread_mutex_unlock(targ->mutex);

        printf("Thread %d a asteptat %d secunde.\n", targ->thread_num, sleep_time);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_arg_t thread_args[NUM_THREADS];
    int should_exit[NUM_THREADS] = {0};
    pthread_mutex_t mutexes[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].thread_num = i + 1;
        thread_args[i].should_exit = &should_exit[i];
        thread_args[i].mutex = &mutexes[i];

        if (pthread_create(&threads[i], NULL, thread_function, &thread_args[i]) != 0) {
            perror("Eroare la deschiderea threadului!\n");
            exit(EXIT_FAILURE);
        }
    }

    while (1) {
        int input = 0;

        printf("Introdu un numar intre 1 si %d pentru a opri threadul corespunzator.\n", NUM_THREADS);
        scanf("%d", &input);

        if (input < 1 || input > NUM_THREADS) {
            printf("Numar invalid! Introdu un numar intre 1 si %d.\n", NUM_THREADS);
            continue;
        }

        int idx = input - 1;

        pthread_mutex_lock(&mutexes[idx]);

        should_exit[idx] = 1;

        pthread_mutex_unlock(&mutexes[idx]);

        if (pthread_join(threads[idx], NULL) != 0) {
            perror("Eroare la inchiderea threadului!\n");
            exit(EXIT_FAILURE);
        }

        printf("Thread %d a fost inchis.\n", input);

        int all_threads_terminated = 1;

        for (int i = 0; i < NUM_THREADS; i++) {
            if (should_exit[i] == 0) {
                all_threads_terminated = 0;
                break;
            }
        }

        if (all_threads_terminated) {
            break;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }

    printf("Threadul principal se inchide, deoarece toate threadurile secundare au fost inchise.\n");

    return 0;
}