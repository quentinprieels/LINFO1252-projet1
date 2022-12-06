#ifndef LOCKER_H
#define LOCKER_H

/**
 * Verrou par attente active test-and-set.
 * 
 * @param verrou : pointeur vers le verrou
 * Le verrou peut prendre 2 valeurs : 0 ou 1.
 * 0 signifie que le verrou est libre, 1 signifie que le verrou est pris.
 * La fonction attend que le verrou soit libre, le prends et remet sa valeur à 1.
 */
void ts_lock(int *verrou);

/**
 * Déverrouille le verrou (remet sa valeur à 0)
 * @param verrou : pointeur vers le verrou
 */
void ts_unlock(int *verrou);

/** 
 * Verrou par attente active test-and-test-and-set.
 * 
 * @param verrou : pointeur vers le verrou
 * L'algorithme est sensiblement le même que pour le test-and-set, sauf que
 * la fonction attend que le verrou soit libre avant de tenter l'instruction atomique.
*/
void tts_lock(int *verrou);

/**
 * Déverrouille le verrou (remet sa valeur à 0)
 * 
 * @param verrou : pointeur vers le verrou
 * Cette fonction est identique à la fonction ts_unlock (et sont implémentation fait par
 * consequent appel à la fonction ts_unlock)
 */
void tts_unlock(int *verrou);

#endif /* LOCKER_H */