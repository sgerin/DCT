#ifndef BASES_H
#define BASES_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <assert.h>

/*
 * Affiche le nom du fichier en train d'être compilé et la ligne
 */
#define ICI fprintf(stderr,"%s:%d\n",__FILE__,__LINE__)
/*
 * Sortie du programme
 */
#define EXIT do { ICI ; abort() ; } while(0)
/*
 * Allocation mémoire. L'exemple alloue un tableau de 10 éléments :
 *
 * struct toto *a ;
 * ALLOUER(a, 10) ;
 */
#define ALLOUER(X,NB) do if ( (X = malloc(sizeof(*(X)) * (NB))) == 0 )\
                             { fprintf(stderr, "Plus de memoire\n") ; \
                                EXIT ; } \
                      while(0)
/*
 * Donne le nombre d'éléments d'un tableau
 */
#define TAILLE(T) ( sizeof(T) / sizeof(T[0]) )
/*
 * Concaténation symbolique (inutile pour le TP)
 */
#define CONCATENE(A,B) A ## B


#ifndef ABS
#define ABS(A) ( (A)>=0 ? (A) : -(A) )
#endif

#ifndef MAX
#define MAX(A,B) ( (A)>=(B) ? (A) : (B) )
#endif

/*
 * "printf" sur "stderr" au lieu de "stdout"
 * NE L'UTILISEZ PAS pour debugger cela perturberait les tests
 */
int eprintf(const char *, ...) ;
int eprintf_utilisee() ;


#endif
