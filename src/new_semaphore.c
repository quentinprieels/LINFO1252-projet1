#include<stdlib.h>

#include "headers/new_semaphore.h"
#include "headers/locker.h"

void new_semaphore_init(new_semaphore_t *sem, int value) {
    sem->value = value;
    sem->verrou = malloc(sizeof(int));
    sem->protect = malloc(sizeof(int));
    *sem->verrou = 1;
    *sem->protect = 0;
}

void new_wait(new_semaphore_t *sem) {
    tts_lock(sem->protect);
    sem->value--;
    tts_unlock(sem->protect);

    // Si la valeur est negative, on bloque le thread et on attend un signal
    if (sem->value < 0) {
        tts_lock(sem->verrou);
    }
}

void new_post(new_semaphore_t *sem) {
    tts_lock(sem->protect);
    sem->value++;
    tts_unlock(sem->protect);

    if (sem <= 0) {
        // Signal to the process that the lock is released
        tts_unlock(sem->verrou);
    }
}

void new_semaphore_destroy(new_semaphore_t *sem) {
    free(sem->verrou);
    free(sem->protect);
}