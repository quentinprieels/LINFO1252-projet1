#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <unistd.h>
#include <semaphore.h>

// Initilization

#define towrite_max 2560
#define toread_max 860

int writers;
int readers;

pthread_mutex_t m_rcount;
pthread_mutex_t m_wcount;
pthread_mutex_t m_rsecurity;

sem_t wsem;  // bloque les writers
sem_t rsem;  // bloque les readers

int writercount = 0; // nombre de writers actifs
int readercount = 0; // nombre de readers actifs

// Writer function
void writer(void) {

    while(true) {

        // simule preparing the data
        for (int i=0; i<10000; i++)

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
void reader(void) {

    while(true) {    

        // lock pour empêcher trop de readers de s'accumuler, pour que quand un writer arrive il n'y ait qu'un lecteur d'occupé.
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
        for (int i=0; i<10000; i++)
    }
}

int main(int argc, char const *argv[]){
    // Récupérer le nombre de writers et de readers (dans cet ordre)
    if (argc != 3) {
        printf("Vous n'avez pas donné correctement le nombre de writer et le nombre de readers.\n");
        exit(EXIT_FAILURE);
    }
    writers = atoi(argv[1]);
    readers = atoi(argv[2]);

    //TODO : initialiser sem et mutex
    sem_init(&wsem, 0, 1);
    sem_init(&rsem, 0, 1);

    //TODO : initialiser les writers
    // chaque writer va devoir faire (# écritures tot / # d'écrivains) écritures
    int * towrite = *(towrite_max / writers);
    int * rest_towrite = *(toread_max - (towrite * writers));

    //TODO : initialiser les readers
    // chaque reader va devoir faire (# lectures tot / # lecteurs) lectures
    int * toread = *(toread_max / readers);
    int * rest_toread = *(toread_max - (toread * readers));

    //TODO : join sur les threads writers

    //TODO : join sur les threads readers

    //TODO : destruction des sémaphores, mutex's et threads




    exit(EXIT_SUCCESS);
}
