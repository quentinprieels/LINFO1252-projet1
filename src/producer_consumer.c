#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include "buffer.h"

// Threads
int nbr_conso;
int nbr_prod;
pthread_t *consomateurs;
pthread_t *producteurs;

// Counter
int max_count = 8192;

// Buffer
buffer_t *buffer;
pthread_mutex_t buffer_mutex;
sem_t empty_buffer;
sem_t full_buffer;


// Fonction des consomateurs
void *consomme(void *data) {
    int count = *((int *) data);

    while(count != 0) {
        // Take an item into the buffer
        // TODO: Add the error mechanism
        sem_wait(&full_buffer);
        pthread_mutex_lock(&buffer_mutex);
        
        int *removed = get(buffer);
        
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&empty_buffer);

        // Simulation d'un traitement
        for (int i = 0; i < 10000; i++);
        free(removed);

        count--;
    } 
    
    return 0;
}

// Fonction des producteurs
void *produit(void *data) {
    int count = *((int *) data);

    while (count != 0) {

        // Simulation d'un traitement
        for (int i = 0; i < 10000; i++);
        
        // Création de l'entier à placer dans le buffer
        int *to_put = (int *) malloc(sizeof(int));
        *to_put = 1;

        // Placement dans le buffer 
        // TODO: Add the error mechanism
        sem_wait(&empty_buffer);
        pthread_mutex_lock(&buffer_mutex);

        put(buffer, to_put);

        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&full_buffer);

        count--;
    }

    return 0;
}

// Main program
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Vous n'avez pas entré le bon nombre (2) d'arguments (le nombre de threads consomateur et le nombre de threads producteurs).");
        exit(EXIT_FAILURE);
    }
    nbr_conso = atoi(argv[1]);
    nbr_prod = atoi(argv[2]);

    // Initialisation du buffer et de son mutex
    buffer = malloc_buffer(8);

    if (buffer->buffer == NULL) {
        printf("Erreur lors de la creation (malloc) du buffer.");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&buffer_mutex, NULL) != 0) {
        printf("Erreur lors de l'initalisation du mutex buffer.");
        exit(EXIT_FAILURE);
    }

    // Initialiser les sémaphores
    if (sem_init(&empty_buffer, 0, 8) == -1) {
        printf("Erreur lors de l'initialisation du sémaphore put");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&full_buffer, 0, 0) == -1) {
        printf("Erreur lors de l'initialisation du sémaphore take");
        exit(EXIT_FAILURE);
    }

    // Calcule du nombre d'éléments que chaque thread doit traiter
    int to_compute_conso = max_count / nbr_conso;
    int rest_compute_conso = max_count % nbr_conso + to_compute_conso;
    int to_compute_prod = max_count / nbr_prod;
    int rest_compute_prod = max_count % nbr_prod + to_compute_prod;

    // Creation des threads produteurs
    pthread_t producteurs[nbr_prod];
    for (int i = 0; i < nbr_prod; i++) {  // donne le reste des threads non distribués au dernier initialisé.
        if (i == nbr_prod - 1){to_compute_prod += rest_compute_prod;}
        if (pthread_create(&producteurs[i], NULL, produit, &to_compute_prod) != 0) {
            fprintf(stderr, "Erreur lors de la creation d'un tread producteurs.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Création des threads consomateurs
    pthread_t consomateurs[nbr_conso];
    for (int i = 0; i < nbr_conso; i++) {
        if (i == nbr_conso - 1){to_compute_conso += rest_compute_conso;}  // donne le reste des threads non distribués au dernier initialisé.
        if (pthread_create(&consomateurs[i], NULL, consomme, &to_compute_conso) != 0) {
            fprintf(stderr, "Erreur lors de la creation d'un tread consomateurs.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Join sur tout les threads
    for (int i = 0; i < nbr_prod; i++) {
        if (pthread_join(producteurs[i], NULL) != 0) {
			fprintf(stderr, "Erreur lors de la terminaison d'un producteurs.\n");
			exit(EXIT_FAILURE);
		}
    }
    for (int i = 0; i < nbr_prod; i++) {
        if (pthread_join(consomateurs[i], NULL) != 0) {
			fprintf(stderr, "Erreur lors de la terminaison d'un consomateurs.\n");
			exit(EXIT_FAILURE);
		}
    }
    
    // Free
    free_buffer(buffer);

    // Destroy mutex and semaphore
    // TODO: Implement the error mechanism
    pthread_mutex_destroy(&buffer_mutex);
    sem_destroy(&empty_buffer);
    sem_destroy(&full_buffer);

    exit(EXIT_SUCCESS);
}