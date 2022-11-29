#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>

// Threads
int nbr_consos;
int nbr_prods;
pthread_t *conomateurs;
pthread_t *producteurs;

// Counter
int max_count = 8192;
volatile int count = 0; // Variable volatile pour éviter la mise en cache (non visible par les autres producteurs)
pthread_mutex_t count_mutex;

// Buffer
int buffer_size = 8;
int *buf;
pthread_mutex_t buffer_mutex;
sem_t put_buffer;
sem_t take_buffer;

// Fonction des consomateur
void *consomme(void *data) {
    while (true) { // TODO: Change the while condition to avoid the infinite loop
        
        // Création de l'entier à placer dans le buffer
        int to_put = 1;

        // Placement dans le buffer 
        // TODO: Add the error mechanism
        sem_wait(&put_buffer);
        pthread_mutex_lock(&buffer_mutex);

        // TODO: Put the integer value into the buffer

        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&take_buffer);
    }
}

// Fonction des producteurs
void *produit(void *data) {
    // TODO: Implement the producer function

    // Simulation d'un traitement
    for (int i = 0; i < 10000; i++);
    
}

// Main program
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Vous n'avez pas entré le bon nombre (2) d'arguments (le nombre de threads consomateur et le nombre de threads producteurs).");
        exit(EXIT_FAILURE);
    }
    nbr_consos = atoi(argv[1]);
    nbr_prods = atoi(argv[2]);

    // Initilisation du compteur
    if (pthread_mutex_init(&count_mutex, NULL) != 0) {
        printf("Erreur lors de l'initilisation du mutex compteur.");
        exit(EXIT_FAILURE);
    }

    // Initialisation du buffer et de son mutex
    buf = (int *) malloc(buffer_size * sizeof(int));
    if (buf == NULL) {
        printf("Erreur lors de la creation (malloc) du buffer.");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&buffer_mutex, NULL) != 0) {
        printf("Erreur lors de l'initalisation du mutex buffer.");
        exit(EXIT_FAILURE);
    }

    // Initialiser les sémaphores
    if (sem_init(&put_buffer, NULL, buffer_size) == NULL) {
        printf("Erreur lors de l'initialisation du sémaphore put");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&take_buffer, NULL, 0) == NULL) {
        printf("Erreur lors de l'initialisation du sémaphore take");
        exit(EXIT_FAILURE);
    }


    // TODO: Créer les threads, join et puis réaliser tout les destroys et free nécessaires

    exit(EXIT_SUCCESS);
}