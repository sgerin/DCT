/*
 * Créé le Wed Jan  9 16:23:10 2002 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_MATRICE_H
#define _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_MATRICE_H

#include "bases.h"

float** allocation_matrice_carree_float(int nbe) ;
void liberation_matrice_carree_float(float **table, int nbe) ;

/*
 * Fonctions gracieusement fournies
 */

void produit_matrices_carrees_float(int nbe, float **a, float **b, float **resultat) ; /**/
void transposition_matrice_carree(int nbe, float **a, float **a_t) ; /**/
void produit_matrice_carree_vecteur(int nbe, float **m, const float *v, float *resultat) ; /**/
void affiche_matrice_carree(int nbe, float **a, FILE *f) ; /**/


#endif
