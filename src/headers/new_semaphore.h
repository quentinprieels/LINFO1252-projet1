#ifndef NEW_SEMAPHORE_H
#define NEW_SEMAPHORE_H

/** Structure utilisee pour les semaphores.
 *  - La valeur correspond à l'initialisation du sémpaphore (nbr d'exections simultanees allouees)
 *  - Le verrou permet l'attente active
*/
typedef struct {
    int value;
    int *protect;
    int *verrou;
} new_semaphore_t;

/** 
 * Initialise la strcuture du semaphore (force le verrou a 1 (bloque)) 
 * 
 * @param sem : pointeur vers la structure semaphore
 * @param value : valeur d'initialisation du semaphore
*/
void new_semaphore_init(new_semaphore_t *sem, int value);

/** 
 * Decremente le semaphore et attend si necessaire
 * 
 * @param sem : pointeur vers la structure semaphore
*/
void new_wait(new_semaphore_t *sem);

/** 
 * Incremente le semaphore et attend si necessaire
 * 
 * @param sem : pointeur vers la structure semaphore
*/
void new_post(new_semaphore_t *sem);

/** 
 * Free les variables allouees sur le tas dans le strucutre semaphore
 * 
 * @param sem : pointeur vers la structure semaphore
*/
void new_semaphore_destroy(new_semaphore_t *sem);

#endif /* NEW_SEMAPHORE_H */