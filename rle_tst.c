#include "bases.h"
#include "rle.h"
#include "bitstream.h"
#include "intstream.h"

void compresse_test(int nb_t, float *t, int nb_ok, int *ok)
{
  struct intstream *entier ;
  struct intstream *entier_signe ;
  struct bitstream *bs ;
  int i ;

  bs = open_bitstream("xxx", "w") ;
  entier = open_intstream(bs, Entier, NULL) ;
  entier_signe = open_intstream(bs, Entier_Signe, NULL) ;
  compresse(entier, entier_signe, nb_t, t) ;
  put_entier_intstream(entier_signe, -123) ;
  close_bitstream(bs) ;
  close_intstream(entier) ;
  close_intstream(entier_signe) ;

  bs = open_bitstream("xxx", "r") ;
  entier = open_intstream(bs, Entier, NULL) ;
  entier_signe = open_intstream(bs, Entier_Signe, NULL) ;
  for(i=0; i<nb_ok; i++)
    {
      if ( (i % 2) == 0 )
	{
	  if ( get_entier_intstream(entier) != ok[i] )
	    {
	      eprintf("Pour l'entier %d, j'attendais %d\n", i, ok[i]) ;
	      return ;
	    }
	}
      else
	{
	  if ( get_entier_intstream(entier_signe) != ok[i] )
	    {
	      eprintf("Pour l'entier %d, j'attendais %d\n", i, ok[i]) ;
	      return ;
	    }
	}
    }
  if ( get_entier_intstream(entier_signe) != -123 )
    {
      eprintf("Un entier de trop a été stocké\n") ;
      return ;
    }
}

void compresse_tst()
{
  static float t1[] = { 0, 0, 5 } ;
  static int  ok1[] = { 2, 5 } ;

  static float t2[] = { -0.4, -0.9, 0, 0.4, 0.9, 2    , 0,0,0 } ;
  static int  ok2[] = {      1,-1,          2,1, 0,2,        3 } ;

  compresse_test(TAILLE(t1), t1, TAILLE(ok1), ok1) ;
  compresse_test(TAILLE(t2), t2, TAILLE(ok2), ok2) ;

  return ;
}

void decompresse_tst()
{
  static float ok[] = { 0, -1, 0, 0, 1, 2, 0,0,0 } ;

  struct intstream *entier ;
  struct intstream *entier_signe ;
  struct bitstream *bs ;

  float t[TAILLE(ok)+1] ;
  int i ;

  compresse_tst() ;		/* Pour créer "xxx" */

  bs = open_bitstream("xxx", "r") ;
  entier = open_intstream(bs, Entier, NULL) ;
  entier_signe = open_intstream(bs, Entier_Signe, NULL) ;

  for(i=0; i<TAILLE(t); i++)
    t[i] = 1234 ;

  decompresse(entier, entier_signe, TAILLE(ok), t) ;

  for(i=0; i<TAILLE(ok); i++)
    if ( rint(ok[i]) != rint(t[i]) )
      {
	eprintf("Mauvais décodage RLE pour l'entier %d\n", i) ;
	return ;
      }
  if ( t[i] != 1234 )
      {
	eprintf("Vous avez débordé du tableau\n", i) ;
	return ;
      }
}
