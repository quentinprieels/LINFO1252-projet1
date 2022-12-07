#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>

// Initialisation

#define towrite_tot 2560
#define toread_tot 640

int nbr_writers;
int nbr_readers;

pthread_mutex_t m_wcount;
pthread_mutex_t m_rcount;
pthread_mutex_t m_wsecurity;
// security est là pour empêcher des readers de s'accumuler sur rsem.
// De cette manière, quand un writer arrive il n'y ait qu'un lecteur d'occupé.
// Autrement dit, donne + de poids au writer

sem_t wsem;  // bloque les writers
sem_t rsem;  // bloque les readers

int writercount = 0; // nombre de writers actifs
int readercount = 0; // nombre de readers actifs

void *writes(void *data){
    int count = *(int *)data;
    while(count > 0){

        // simule la préparation des données à écrire
        for (int i=0; i<10000; i++);
        
        // modification du nombre de writers actifs
        // section critique donc on lock
        if(pthread_mutex_lock(&m_wcount)){
            printf("Error : failed to lock wcount");
            exit(EXIT_FAILURE);
        }
        writercount++;
        if (writercount == 1){ // premier writer --> on bloque les readers
            if (sem_wait(&rsem)){
                printf("Error : failed to sem_wait rsem");
                exit(EXIT_FAILURE);
            }
        }
        if(pthread_mutex_unlock(&m_wcount)){
            printf("Error : failed to unlock wcount");
            exit(EXIT_FAILURE);
        }
        // fin de la section critique

        // Un seul writer peut écrire à la fois donc on met un sémaphore
        if (sem_wait(&wsem)){
            printf("Error : failed to sem_wait wsem");
            exit(EXIT_FAILURE);
        }
        // écriture dans la base de donnée (instantané)
        if (sem_post(&wsem)){
            printf("Error : failed to sem_post wsem");
            exit(EXIT_FAILURE);
        }

        // modification du nombre de writer actifs
        // section critique donc on lock
        if(pthread_mutex_lock(&m_wcount)){
            printf("Error : failed to lock wcount");
            exit(EXIT_FAILURE);
        }
        writercount--;
        if (writercount == 0){ // dernier writer --> on libère les readers
            if (sem_post(&rsem)){
                printf("Error : failed to sem_post rsem");
                exit(EXIT_FAILURE);
            }
        }
        if(pthread_mutex_unlock(&m_wcount)){
            printf("Error : failed to unlock wcount");
            exit(EXIT_FAILURE);
        }
        // fin de la section critique

        count--;
    }
    return NULL;
}

void *reads(void *data){
    int count = *(int *) data;

    while(count > 0){

        // protège rsem
        if (pthread_mutex_lock(&m_wsecurity)){
            printf("Error : failed to lock wsecurity");
            exit(EXIT_FAILURE);      
        }

        // on vérifie qu'aucun reader n'est en train d'être créé 
        // ni qu'aucun writer n'est occupé d'écrire
        if (sem_wait(&rsem)){
            printf("Error : failed to sem_wait rsem");
            exit(EXIT_FAILURE);
        }

        // modification du nombre de readers actifs
        // section critique donc on lock
        if(pthread_mutex_lock(&m_rcount)){
            printf("Error : failed to lock rcount");
            exit(EXIT_FAILURE);
        }
        readercount++;
        if (readercount == 1){ // premier reader --> on bloque les writers
            if (sem_wait(&wsem)){
                printf("Error : failed to sem_wait wsem");
                exit(EXIT_FAILURE);
            }
        }
        if(pthread_mutex_unlock(&m_rcount)){
            printf("Error : failed to unlock rcount");
            exit(EXIT_FAILURE);
        }
        // fin de la section critique

        // libère le prochain reader ou writer
        if (sem_post(&rsem)){
            printf("Error : failed to sem_post rsem");
            exit(EXIT_FAILURE);
        }

        // plus d'interraction avec rsem, on peut le unlock
        if (pthread_mutex_unlock(&m_wsecurity)){
            printf("Error : failed to unlock wsecurity");
            exit(EXIT_FAILURE);      
        }

        // lecture de la base de donnée (instantané)

        // modification du nombre de readers actifs
        // section critique donc on lock
        if(pthread_mutex_lock(&m_rcount)){
            printf("Error : failed to lock rcount");
            exit(EXIT_FAILURE);
        }
        readercount--;
        if (readercount == 0){ // dernier reader --> on libère les writers
            if (sem_post(&wsem)){
                printf("Error : failed to sem_post wsem");
                exit(EXIT_FAILURE);
            }
        }
        if(pthread_mutex_unlock(&m_rcount)){
            printf("Error : failed to unlock rcount");
            exit(EXIT_FAILURE);
        }
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
    if (pthread_mutex_init(&m_wcount, NULL) != 0){
        printf("Erreur lors de l'initalisation du mutex wcount.");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&m_rcount, NULL) != 0){
        printf("Erreur lors de l'initalisation du mutex rcount.");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&m_wsecurity, NULL) != 0){
        printf("Erreur lors de l'initalisation du mutex rsecurity.");
        exit(EXIT_FAILURE);
    }
    
    //initialisation des sémaphores
    if (sem_init(&wsem, 0, 1) == -1){
        printf("Erreur lors de l'initialisation du sémaphore des writers");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&rsem, 0, 1) == -1){
        printf("Erreur lors de l'initialisation du sémaphore des readers");
        exit(EXIT_FAILURE);
    }

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

    //destruction des mutex
    if (pthread_mutex_destroy(&m_wcount) != 0){
        printf("Erreur lors de la suppression du mutex wcount.");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_destroy(&m_rcount) != 0){
        printf("Erreur lors de la suppression du mutex rcount.");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_destroy(&m_wsecurity) != 0){
        printf("Erreur lors de la suppression du mutex rsecurity.");
        exit(EXIT_FAILURE);
    }
    
    //destruction des sémaphores
    if (sem_destroy(&wsem) == -1){
        printf("Erreur lors de la suppression du sémaphore des writers");
        exit(EXIT_FAILURE);
    }
    if (sem_destroy(&rsem) == -1){
        printf("Erreur lors de la suppression du sémaphore des readers");
        exit(EXIT_FAILURE);
    }   

    exit(EXIT_SUCCESS);
}
