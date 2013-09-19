#include "bases.h"
#include "matrice.h"

/*
 * Allocation d'une matrice carrée de float.
 * (tableau de pointeur sur tableau de flottants)
 */

float** allocation_matrice_carree_float(int nbe)
{







return 0 ; /* pour enlever un warning du compilateur */
}

/*
 * Libération
 */

void liberation_matrice_carree_float(float **table, int nbe)
{






}


/*
 * Produit matriciel de matrices carrées (le résultat est déjà alloué).
 *             resultat = a * b 
 */

void produit_matrices_carrees_float(int nbe, float **a, float **b, float **resultat)
 {
  int j, i, k ;
  float s ;

  for(j=0; j<nbe; j++)
    for(i=0; i<nbe; i++)
      {
	s = 0 ;
	for(k=0;k<nbe;k++)
	  s += a[j][k]*b[k][i] ;
	resultat[j][i] = s ;
      }
 }

/*
 * Produit matrices carrée vecteur
 *             resultat = m * v
 * Le résultat est supposé annulé
 */

void produit_matrice_carree_vecteur(int nbe, float **m, const float *v, float *resultat)
 {
  int j, i ;
  float s ;

  for(j=0; j<nbe; j++)
    {
      s = 0 ;
      for(i=0;i<nbe;i++)
	s += m[j][i] * v[i] ;
      resultat[j] = s ;
    }
 }

/*
 * Transposition d'une matrice carrée (le résultat est déjà alloué).
 *        a_t est la transposée de a
 */

void transposition_matrice_carree(int nbe, float **a, float **a_t)
 {
  int i, j ;

  for(j=0;j<nbe;j++)
    for(i=0;i<nbe;i++)
      a_t[j][i] = a[i][j] ;
 }

/*
 * Affiche
 */

void affiche_matrice_carree(int nbe, float **a, FILE *f)
 {
   int i, j ;
   
   for(j=0;j<nbe;j++)
     {
       for(i=0;i<nbe;i++)
	 fprintf(f, " %8.4g", a[j][i]) ;
       fprintf(f, "\n") ;
     }
 }

