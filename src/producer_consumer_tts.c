#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "headers/buffer.h"
#include "headers/new_semaphore.h"
#include "headers/locker.h"

// Threads
int nbr_conso;
int nbr_prod;
pthread_t *consomateurs;
pthread_t *producteurs;

// Counter
int max_count = 8192;

// Buffer
buffer_t *buffer;
int buffer_mutex;
new_semaphore_t empty_buffer;
new_semaphore_t full_buffer;


// Fonction des consomateurs
void *consomme(void *data) {
    int count = *((int *) data);

    while(count != 0) {
        // Take an item into the buffer
        // TODO: Add the error mechanism
        new_wait(&full_buffer);
        tts_lock(&buffer_mutex);
        
        int *removed = get(buffer);
        
        tts_unlock(&buffer_mutex);
        new_post(&empty_buffer);

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
        new_wait(&empty_buffer);
        tts_lock(&buffer_mutex);

        put(buffer, to_put);

        tts_unlock(&buffer_mutex);
        new_post(&full_buffer);

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
    buffer_mutex = 0;

    // Initialiser les sémaphores
    new_semaphore_init(&empty_buffer, 8);
    new_semaphore_init(&full_buffer, 0);

    // Calcule du nombre d'éléments que chaque thread doit traiter
    int to_compute_conso = max_count / nbr_conso;
    int rest_compute_conso = max_count % nbr_conso;
    int to_compute_prod = max_count / nbr_prod;
    int rest_compute_prod = max_count % nbr_prod;

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
        // Important pour eviter un deadlock (notifie tout les consomateurs que les producteurs sont terminés)
        for (int i = 0; i < nbr_prod; i++) {new_post(&full_buffer);}
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
    new_semaphore_destroy(&empty_buffer);
    new_semaphore_destroy(&full_buffer);

    exit(EXIT_SUCCESS);
}