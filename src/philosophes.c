#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

// 10.000.000 if tested onto own computer - 100.000 on Inginious
#define CYCLES 100000

// Variables gloables - permet de partage des données entre les treads 
pthread_mutex_t *baguette;
int nbr_philosophes;

int *ids; // Numéros des philosphes;

/* Fonction philosophe
   Chaque philosophe prend les baguettes qui se trouve à sa gauche et à sa droite. Il peut ensuite
   manger, relacher les baguettes et penser apres. Il execute cette suite d'actions un nombre
   CYCLES de fois. 
*/
void* philosophe (void *arg) {
    int *id = (int *) arg;
    int left = *id;
    int right = (left + 1) % nbr_philosophes;
    
    for (size_t i = 0; i < CYCLES; i++) {     

        if(left < right) {
            pthread_mutex_lock(&baguette[left]);
            pthread_mutex_lock(&baguette[right]);
        }
        else {
            pthread_mutex_lock(&baguette[right]);
            pthread_mutex_lock(&baguette[left]);
        }
        
        // Philosophe mange
        // printf("philosophe %d mange\n", *id);

        pthread_mutex_unlock(&baguette[left]);
        pthread_mutex_unlock(&baguette[right]);

        // Philosophe pense
    }

  return (NULL);
}


int main(int argc, char *argv[]) {
    // Récupérer le nombre de philosophes
    if (argc != 2 && argc != 3) { // On ajoute la possibilité de donner un 2e argument qui ne sera pas pris en compte (simplifier les scripts de mesure du temps)
        printf("Vous n'avez pas donné un seul argument correspondant au nombre de philosophe.\n");
        exit(EXIT_FAILURE);
    }
    nbr_philosophes = atoi(argv[1]);

    // Initialisation des baguettes
    baguette = (pthread_mutex_t *) malloc(nbr_philosophes * sizeof(pthread_mutex_t));
    if (baguette == NULL) {
        fprintf(stderr, "Erreurs de malloc en initialisant les baguettes.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nbr_philosophes; i++) {
        if (pthread_mutex_init(&baguette[i], NULL) != 0) {
    	    fprintf(stderr,"Erreur lors de l'initalisation des mutex baguettes.\n");
    	    exit(EXIT_FAILURE);
        }
    }

    // Initialisation des ids des philosophes 
    /* (la structure ids permet d'éviter que deux threads prennent le même id dans le cas où l'incrémentation
        de i se ferait avant l'assignantion de la valeur de i dans le corps de la fonction philosophe) */
    ids = (int *) malloc(sizeof(int) * nbr_philosophes);
    for (int j = 0; j < nbr_philosophes; j++) {
        ids[j] = j;
    }
    
    // Initialisation des philosophes
    pthread_t phil[nbr_philosophes];
    for (int i = 0; i < nbr_philosophes; i++) {
        if (pthread_create(&phil[i], NULL, philosophe, (void *) &ids[i]) != 0) {
			fprintf(stderr, "Erreur lors de la creation d'un tread philophe.\n");
			exit(EXIT_FAILURE);
		}
    }

    // Join sur tout les treads
    for (int k = 0; k < nbr_philosophes; k++) {
        if (pthread_join(phil[k], NULL) != 0) {
			fprintf(stderr, "Erreur lors de la terminaison d'un philosophe.\n");
			exit(EXIT_FAILURE);
		}
    }

    // Destruction des des mutex et free
    for (int i = 0; i < nbr_philosophes; i++) {
        if (pthread_mutex_destroy(&baguette[i]) != 0) {
    	    fprintf(stderr,"Erreur lors de la destruction des mutex baguettes.\n");
    	    exit(EXIT_FAILURE);
        }
    }
    free(baguette);
    free(ids);

    // Exit
    exit(EXIT_SUCCESS);
}