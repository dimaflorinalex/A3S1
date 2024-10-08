/*
6. Scrieti un program care creaza 9 thread-uri, fiecare dintre acestea
 executand o bucla infinita in care asteapta un timp aleator intre 1 si
 5 secunde, apoi isi afisaza pe ecran numarul (de la 1 la 9). Pe thread-ul
 principal se citesc cifre de la tastatura. Atunci cand se introduce cifra
 corespunzatoare unui thread, acesta va fi anulat. După ce s-au inchis
 toate cele 9 thread-uri (trebuie sa se si astepte terminarea lor), se va
 inchide si thread-ul principal.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 9

// Structura pentru a pasa argumente către funcția thread-ului
typedef struct {
    int thread_num;
    int *should_exit;
    pthread_mutex_t *mutex;
} thread_arg_t;

// Funcția executată de fiecare thread
void *thread_function(void *arg) {
    thread_arg_t *targ = (thread_arg_t *)arg;
    srand(time(NULL) + targ->thread_num);  // Inițializarea generatorului de numere aleatorii

    while (1) {
        // Așteaptă între 1 și 5 secunde
        int sleep_time = rand() % 5 + 1;
        sleep(sleep_time);

        // Verifică dacă thread-ul ar trebui să se oprească
        pthread_mutex_lock(targ->mutex);
        if (*(targ->should_exit)) {
            pthread_mutex_unlock(targ->mutex);
            printf("Thread %d se oprește.\n", targ->thread_num);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(targ->mutex);

        // Afișează numărul thread-ului
        printf("Thread %d așteaptă %d secunde.\n", targ->thread_num, sleep_time);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_arg_t thread_args[NUM_THREADS];
    int should_exit[NUM_THREADS] = {0};  // Folosim un vector pentru a semnala când trebuie să iasă un thread
    pthread_mutex_t mutexes[NUM_THREADS];  // Câte un mutex pentru fiecare thread

    // Inițializarea mutex-urilor
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
    }

    // Crearea thread-urilor
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].thread_num = i + 1;
        thread_args[i].should_exit = &should_exit[i];
        thread_args[i].mutex = &mutexes[i];

        if (pthread_create(&threads[i], NULL, thread_function, &thread_args[i]) != 0) {
            perror("Eroare la crearea thread-ului");
            exit(1);
        }
    }

    // Thread-ul principal citește cifre de la tastatură
    while (1) {
        int input;
        printf("Introdu un număr între 1 și 9 pentru a opri thread-ul corespunzător: ");
        scanf("%d", &input);

        if (input < 1 || input > 9) {
            printf("Număr invalid. Introdu un număr între 1 și 9.\n");
            continue;
        }

        // Semnalează thread-ul să se oprească
        int idx = input - 1;
        pthread_mutex_lock(&mutexes[idx]);
        should_exit[idx] = 1;
        pthread_mutex_unlock(&mutexes[idx]);

        // Așteaptă terminarea thread-ului respectiv
        if (pthread_join(threads[idx], NULL) != 0) {
            perror("Eroare la așteptarea thread-ului");
            exit(1);
        }
        printf("Thread %d a fost oprit.\n", input);

        // Verifică dacă toate thread-urile au fost oprite
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

    // Curățarea mutex-urilor
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }

    printf("Toate thread-urile au fost oprite. Thread-ul principal se închide.\n");
    return 0;
}