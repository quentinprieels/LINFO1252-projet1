#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "locker.c"

// Initialisation du verrou
int verrou;
#define JOB 6400

// Fonction d'un tread. Il execute arg fois la section critique
void* thread_function(void *arg) {
    int *work = (int *) arg;
    for (size_t i = 0; i < *work; i++) {
        ts_lock(&verrou);
        // Section critique
        for (int i=0; i<10000; i++);
        ts_unlock(&verrou);
    }
    return (NULL);
}

int main(int argc, char *argv[]) {
    // Vérifier le nombre d'arguments
    if (argc != 2 && argc != 3) {
        printf("Vous n'avez pas donné un seul argument correspondant au nombre de thread.\n");
        exit(EXIT_FAILURE);
    }
    int n_threads = atoi(argv[1]);

    // Initialisation du verrou
    verrou = 0;

    // Calculer le nombre de travail à faire par thread
    int work = JOB / n_threads;
    int rest = JOB % n_threads;

    // Initialisation des threads
    pthread_t threads[n_threads];
    for (int i = 0; i < n_threads; i++){
        if (i == n_threads - 1){work += rest;} // donne le reste des threads non distribués au dernier initialisé.
        if (pthread_create(&threads[i], NULL, thread_function, &work) != 0) {
            printf("Erreur lors de la création du thread %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }
    
    // Attendre la fin des threads
    for (int i = 0; i < n_threads; i++){
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Erreur lors de l'attente du thread %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}