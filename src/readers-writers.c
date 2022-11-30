#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>

// Initilization
pthread_mutex_t m_rcount;
pthread_mutex_t m_wcount;

sem_t db;  // accès exclusif à la db
sem_t rsem; // bloquer les readers

int writecount = 0;
int readcount = 0; // nombre de readers

sem_init(&db, 0, 1);    // MOVE IT INSIDE THE MAIN FUNCTION
sem_init(&rsem, 0, 1);

// Writer function
void writer(void) {
    while(true) {
        prepare_data();
        
        pthread_mutex_lock(&m_wcount);
        writecount++;
        if (writecount == 1) {
            // Si on est le premier écrivain, on bloque les lecteurs
            sem_wait(&rsem);
        }
        pthread_mutex_unlock(&m_wcount);
        
        sem_wait(&db); // -1 => oj si sem = 1
        // section critique, un seul writer à la fois
        write_database();
        sem_post(&db); // +1

        pthread_mutex_lock(&m_wcount);
        writecount--;
        if (writecount == 0) {
            sem_post(&rsem);
        }
        pthread_mutex_unlock(&m_wcount);
    }
}

// Reader function
void reader(void) {
    while(true) {
        
        sem_wait(&rsem);

        pthread_mutex_lock(&m_rcount);
        // section critique
        readcount++;
        if (readcount==1) { // arrivée du premier reader
            sem_wait(&db);
        }
        pthread_mutex_unlock(&m_rcount);
        sem_post(&rsem);

        read_database();
        
        pthread_mutex_lock(&m_rcount);
        // section critique
        readcount--;
        if(readcount==0) { // départ du dernier reader
            sem_post(&db);
        }
        pthread_mutex_unlock(&m_rcount);
        
        process_data();
    }
}