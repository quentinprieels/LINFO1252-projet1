#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>

// Initilisation

#define towrite_max 2560
#define toread_max 860

int nbr_writers;
int nbr_readers;

pthread_mutex_t m_wcount;
pthread_mutex_t m_rcount;
pthread_mutex_t m_rsecurity;

sem_t wsem;  // bloque les writers
sem_t rsem;  // bloque les readers

int writercount = 0; // nombre de writers actifs
int readercount = 0; // nombre de readers actifs

// Writer function
void *writes(void *data) {
    int count = *(int *) data;

    while(count != 0) {

        // simule preparing the data
        for (int i=0; i<10000; i++);

        // section critique donc on lock
        pthread_mutex_lock(&m_wcount);
        writercount++;
        if (writercount == 1) {sem_wait(&rsem);} // arrivée du premier writer
        pthread_mutex_unlock(&m_wcount);
        
        sem_wait(&wsem);
        // écrire dans la database --> instantané
        sem_post(&wsem);

        // section critique donc on lock
        pthread_mutex_lock(&m_wcount);
        writercount--;
        if (writercount == 0) {sem_post(&rsem);} // départ du dernier writer
        pthread_mutex_unlock(&m_wcount);
    }
}

// Reader function
void *reads(void *data) {
    int count = *(int *) data;

    while(count != 0) {    

        // lock pour empêcher des readers de s'accumuler sur rsem
        // pour que : quand un writer arrive il n'y ait qu'un lecteur d'occupé.
        // autrement dit, donne + de poids au writer
        pthread_mutex_lock(&m_rsecurity);
        sem_wait(&rsem);

        // section critique donc on lock
        pthread_mutex_lock(&m_rcount);
        readercount++;
        if (readercount==1) {sem_wait(&wsem);} // arrivée du premier reader
        pthread_mutex_unlock(&m_rcount);

        sem_post(&rsem);
        pthread_mutex_unlock(&m_rsecurity);

        // lire la database --> instantané
        
        // section critique donc on lock
        pthread_mutex_lock(&m_rcount);
        readercount--;
        if(readercount==0) {sem_post(&wsem);} // départ du dernier reader
        pthread_mutex_unlock(&m_rcount);
        
        // simule processing the data
        for (int i=0; i<10000; i++);
    }
}

int main(int argc, char const *argv[]){
    // Récupérer le nombre de writers et de readers (dans cet ordre)
    if (argc != 3) {
        printf("Vous n'avez pas donné correctement le nombre de writer suivi du nombre de readers.\n");
        exit(EXIT_FAILURE);
    }
    nbr_writers = atoi(argv[1]);
    nbr_readers = atoi(argv[2]);

    //initialisation des mutex
    if (pthread_mutex_init(&m_wcount, NULL) != 0) {
        printf("Erreur lors de l'initalisation du mutex wcount.");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&m_rcount, NULL) != 0) {
        printf("Erreur lors de l'initalisation du mutex rcount.");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&m_rsecurity, NULL) != 0) {
        printf("Erreur lors de l'initalisation du mutex rsecurity.");
        exit(EXIT_FAILURE);
    }
    
    //initialisation des sémaphores
    if (sem_init(&wsem, 0, 1) == -1) {
        printf("Erreur lors de l'initialisation du sémaphore des writers");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&rsem, 0, 1) == -1) {
        printf("Erreur lors de l'initialisation du sémaphore des readers");
        exit(EXIT_FAILURE);
    }

    //initialiser les writers
    int towrite = towrite_max / nbr_writers;
    int first_towrite = toread_max - (towrite * nbr_writers ) + nbr_writers;

    pthread_t writers[nbr_writers];
    if (pthread_create(&writers[0], NULL, writes, &first_towrite) != 0) {
        fprintf(stderr, "Erreur lors de la creation d'un tread writer.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i < nbr_writers; i++) {
        if (pthread_create(&writers[i], NULL, writes, &towrite) != 0) {
            fprintf(stderr, "Erreur lors de la creation d'un tread writer.\n");
            exit(EXIT_FAILURE);
        }
    }

    //initialiser les readers
    int toread = toread_max / nbr_readers;
    int first_toread = toread_max - toread * (nbr_readers) + nbr_readers;
    
    pthread_t readers[nbr_readers];
    if (pthread_create(&readers[0], NULL, reads, &first_toread) != 0) {
        fprintf(stderr, "Erreur lors de la creation d'un tread reader.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i < nbr_readers; i++) {
        if (pthread_create(&readers[i], NULL, reads, &toread) != 0) {
            fprintf(stderr, "Erreur lors de la creation d'un tread reader.\n");
            exit(EXIT_FAILURE);
        }
    }

    //join sur les threads writers
    for (int i = 0; i < nbr_writers; i++) {
        if (pthread_join(writers[i], NULL) != 0) {
			fprintf(stderr, "Erreur lors de la terminaison d'un writer.\n");
			exit(EXIT_FAILURE);
		}
    }

    //join sur les threads readers
    for (int i = 0; i < nbr_readers; i++) {
        if (pthread_join(readers[i], NULL) != 0) {
			fprintf(stderr, "Erreur lors de la terminaison d'un writer.\n");
			exit(EXIT_FAILURE);
		}
    }

    //destruction des mutex
    if (pthread_mutex_destroy(&m_wcount) != 0) {
        printf("Erreur lors de la suppression du mutex wcount.");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_destroy(&m_rcount) != 0) {
        printf("Erreur lors de la suppression du mutex rcount.");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_destroy(&m_rsecurity) != 0) {
        printf("Erreur lors de la suppression du mutex rsecurity.");
        exit(EXIT_FAILURE);
    }
    
    //destruction des sémaphores
    if (sem_destroy(&wsem) == -1) {
        printf("Erreur lors de la suppression du sémaphore des writers");
        exit(EXIT_FAILURE);
    }
    if (sem_destroy(&rsem) == -1) {
        printf("Erreur lors de la suppression du sémaphore des readers");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
