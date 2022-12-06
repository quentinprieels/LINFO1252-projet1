#include<stdlib.h>

#include "headers/new_semaphore.h"
#include "headers/locker.h"

void new_semaphore_init(new_semaphore_t *sem, int value) {
    sem->value = value;

    // Initialisation du verrou à 0 (passant au permier appel de lock)
    sem->verrou = (int *) calloc(1, sizeof(int));
}

void new_wait(new_semaphore_t *sem) {
    tts_lock(sem->verrou);

    // Decrementation la valeur du semaphore
    sem->value--;

    // Si la valeur est negative, on attend que la valeur soit positive
    while(sem->value < 0) {
        // On libere le verrou pour que le post puisse le prendre
        tts_unlock(sem->verrou);
        tts_lock(sem->verrou);
    }
    
    tts_unlock(sem->verrou);
}

void new_post(new_semaphore_t *sem) {
    // Incrementation de la valeur du semaphore
    tts_lock(sem->verrou);
    sem->value++;
    tts_unlock(sem->verrou);
}

void new_semaphore_destroy(new_semaphore_t *sem) {
    free(sem->verrou);
}
