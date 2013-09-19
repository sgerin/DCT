#include "bases.h"
#include "matrice.h"

void allocation_matrice_carree_float_tst()
{
  float **m ;
  int n, i , j ;

  n = 10 ;

  m = allocation_matrice_carree_float(n) ;
  for(j=0; j<n; j++)
    for(i=0; i<n; i++)
      m[j][i] = 1000*i + j ;

  for(j=0; j<n; j++)
    for(i=0; i<n; i++)
      if ( m[j][i] != 1000*i + j )
	{
	  eprintf("Le contenu de la matrice s'auto écrase\n") ;
	  return ;
	}
}

void liberation_matrice_carree_float_tst()
{
  float **m ;
  int n ;

  n = 10 ;
  m = allocation_matrice_carree_float(n) ;
  liberation_matrice_carree_float(m, n) ;

  if ( allocation_matrice_carree_float(n) != m )
    {
      eprintf("Vous êtes sur de tout libérer ?\n") ;
      return ;
    }
}
