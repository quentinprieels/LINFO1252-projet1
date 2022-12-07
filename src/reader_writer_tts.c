#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "headers/new_semaphore.h"
#include "headers/locker.h"

// Initialisation

#define towrite_tot 2560
#define toread_tot 640
int nbr_writers;
int nbr_readers;

// les int suivants remplacent les mutex, mais notre lock n'a besoin que de int*
int lock_wcount;
int lock_rcount;
int lock_wsecurity;
// security est là pour empêcher des readers de s'accumuler sur rsem.
// De cette manière, quand un writer arrive il n'y ait qu'un lecteur d'occupé.
// Autrement dit, donne + de poids au writer

new_semaphore_t wsem;  // bloque les writers
new_semaphore_t rsem;  // bloque les readers

int writercount = 0; // nombre de writers actifs
int readercount = 0; // nombre de readers actifs

void *writes(void *data){
    int count = *(int *)data;
    while(count > 0){

        // simule la préparation des données à écrire
        for (int i=0; i<10000; i++);
        
        // modification du nombre de writers actifs
        // section critique donc on lock
        tts_lock(&lock_wcount);
        writercount++;
        if (writercount == 1){new_wait(&rsem);} // premier writer --> on bloque les readers
        tts_unlock(&lock_wcount);
        // fin de la section critique

        // Un seul writer peut écrire à la fois donc on met un sémaphore
        new_wait(&wsem);
        // écriture dans la base de donnée (instantané)
        new_post(&wsem);

        // modification du nombre de writer actifs
        // section critique donc on lock
        tts_lock(&lock_wcount);
        writercount--;
        if (writercount == 0){new_post(&rsem);} // dernier writer --> on libère les readers
        tts_unlock(&lock_wcount);
        // fin de la section critique

        count--;
    }
    return NULL;
}

void *reads(void *data){
    int count = *(int *) data;

    while(count > 0){

        // protège rsem
        tts_lock(&lock_wsecurity);

        // on vérifie qu'aucun reader n'est en train d'être créé 
        // ni qu'aucun writer n'est occupé d'écrire
        new_wait(&rsem);

        // modification du nombre de readers actifs
        // section critique donc on lock
        tts_lock(&lock_rcount);
        readercount++;
        if (readercount == 1){new_wait(&wsem);} // premier reader --> on bloque les writers
        tts_unlock(&lock_rcount);
        // fin de la section critique

        // libère le prochain reader ou writer
        new_post(&rsem);

        // plus d'interraction avec rsem, on peut le unlock
        tts_unlock(&lock_wsecurity);

        // lecture de la base de donnée (instantané)

        // modification du nombre de readers actifs
        // section critique donc on lock
        tts_lock(&lock_rcount);
        readercount--;
        if (readercount == 0){new_post(&wsem);} // dernier reader --> on libère les writers
        tts_unlock(&lock_rcount);
        // fin de la section critique

        count--;
    }
    return NULL;
}

int main(int argc, char const *argv[]){
    // Récupérer le nombre de writers et de readers (dans cet ordre)
    if (argc != 3){
        printf("Vous n'avez pas donné correctement le nombre de writer suivi du nombre de readers.\n");
        exit(EXIT_FAILURE);
    }
    nbr_writers = atoi(argv[1]);
    nbr_readers = atoi(argv[2]);

    //initialisation des mutex
    lock_wcount = 0;
    lock_rcount = 0;
    lock_wsecurity = 0;
    
    //initialisation des sémaphores
    new_semaphore_init(&wsem, 1);
    new_semaphore_init(&rsem, 1);

    //initialiser les writers
    int towrite = towrite_tot / nbr_writers;
    int rest_towrite = toread_tot % nbr_writers;

    pthread_t writers[nbr_writers];
    for (int i = 0; i < nbr_writers; i++) {
        if (i == nbr_writers - 1){towrite += rest_towrite;} // donne le reste des threads non distribués au dernier initialisé
        if (pthread_create(&writers[i], NULL, writes, &towrite) != 0){
            fprintf(stderr, "Erreur lors de la creation d'un tread writer.\n");
            exit(EXIT_FAILURE);
        }
    }

    //initialiser les readers
    int toread = toread_tot / nbr_readers;
    int rest_toread = toread_tot % nbr_readers;
    
    pthread_t readers[nbr_readers];
    for (int i = 0; i < nbr_readers; i++) {
        if (i == nbr_readers - 1){toread += rest_toread;} // donne le reste des threads non distribués au dernier initialisé
        if (pthread_create(&readers[i], NULL, reads, &toread) != 0){
            fprintf(stderr, "Erreur lors de la creation d'un tread reader.\n");
            exit(EXIT_FAILURE);
        }
    }

    //join sur les threads writers
    for (int i = 0; i < nbr_writers; i++){
        if (pthread_join(writers[i], NULL) != 0){
			fprintf(stderr, "Erreur lors de la terminaison d'un writer.\n");
			exit(EXIT_FAILURE);
		}
    }

    //join sur les threads readers
    for (int i = 0; i < nbr_readers; i++){
        if (pthread_join(readers[i], NULL) != 0){
			fprintf(stderr, "Erreur lors de la terminaison d'un writer.\n");
			exit(EXIT_FAILURE);
		}
    }

    //destruction des mutex n'est plus nécessaire
    
    //destruction des sémaphores
    new_semaphore_destroy(&wsem);
    new_semaphore_destroy(&rsem);

    exit(EXIT_SUCCESS);
}
