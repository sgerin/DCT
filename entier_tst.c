#include "entier.h"
#include "bases.h"

static struct
{
    int entier ;
  char *chaine ;
} t[] =
{
  {0    , "00"                },
  {1    , "010"               },
  {2    , "0110"              },
  {3    , "0111"              },
  {4    , "100000"            },
  {5    , "100001"            },
  {6    , "100010"            },
  {7    , "100011"            },
  {8    , "1001000"           },
  {9    , "1001001"           },
  {15   , "1001111"           },
  {16   , "10100000"          },
  {17   , "10100001"          },
  {31   , "10101111"          },
  {32   , "101100000"         },
  {33   , "101100001"         },
  {63   , "101111111"         },
  {64   , "11000000000"       },
  {65   , "11000000001"       },
  {8191 , "11110111111111111" },
  {8192 , "1111100000000000000"},
  {8193 , "1111100000000000001"},
  {16383, "1111101111111111111"},
  {16384, "11111100000000000000"},
  {16385, "11111100000000000001"},
  {32767, "11111111111111111111"},
} ;


void put_entier_tst()
{
  int i, j ;
  struct bitstream *bs ;

  bs = open_bitstream("xxx", "w") ;
  for(i=0; i<TAILLE(t); i++)
    put_entier(bs, t[i].entier) ;
  close_bitstream(bs) ;

  bs = open_bitstream("xxx", "r") ;
  for(i=0; i<TAILLE(t); i++)
    {
      for(j=0; t[i].chaine[j]; j++)
	if ( get_bit(bs) != t[i].chaine[j] - '0' )
	  {
	    eprintf("Ecriture de l'entier %d (%s en binaire)\n",
			t[i].entier, t[i].chaine) ;
	    eprintf("Mauvaise écriture du bit numero %d (a partir de 0)\n",
			j) ;
	    return ;
	  }
    }
  close_bitstream(bs) ;
}

void get_entier_tst()
{
  int i, j ;
  struct bitstream *bs ;

  put_entier_tst() ;

  bs = open_bitstream("xxx", "r") ;
  for(i=0; i<TAILLE(t); i++)
    {
    j = get_entier(bs) ;
    if ( j != t[i].entier )
	  {
	    eprintf("Lecture de l'entier %d (%s en binaire)\n",
			t[i].entier, t[i].chaine) ;
	    eprintf("Je recois %d\n", j) ;
	    return ;
	  }
    }
  close_bitstream(bs) ;
}

void put_entier_signe_tst()
{
  int i ;
  struct bitstream *bs ;

  bs = open_bitstream("xxx", "w") ;
  for(i=0; i<TAILLE(t); i++)
    {
      put_entier_signe(bs, t[i].entier) ;
      put_entier_signe(bs, -t[i].entier) ;
    }
  close_bitstream(bs) ;

  bs = open_bitstream("xxx", "r") ;
  for(i=0; i<TAILLE(t); i++)
    {
      if ( get_bit(bs) )
	{
	  eprintf("Mauvais bit de signe en écriture (positif) pour %d\n", i) ;
	  return ;
	}
      if ( get_entier(bs) != t[i].entier)
	{
	  eprintf("Mauvaise écriture de l'entier %d\n", i) ;
	  return ;
	}
      if ( i != 0 )
	{
	  if ( !get_bit(bs) )
	    {
	      eprintf("Mauvais bit de signe en écriture (negatif) %d\n", i) ;
	      return ;
	    }
	  if ( get_entier(bs) != t[i].entier-1 )
	    {
	      eprintf("Mauvaise écriture de l'entier %d\n", -i) ;
	      return ;
	    }
	}
      else
	{
	  if ( get_bit(bs) )
	    {
	      eprintf("Mauvais bit de signe pour 0\n") ;
	      return ;
	    }
	  if ( get_entier(bs) != 0 )
	    {
	      eprintf("Mauvaise valeur pour 0\n") ;
	      return ;
	    }
	}
    }
  close_bitstream(bs) ;
}

void get_entier_signe_tst()
{
  int i ;
  struct bitstream *bs ;

  put_entier_signe_tst() ;

  bs = open_bitstream("xxx", "r") ;
  for(i=0; i<TAILLE(t); i++)
    {
      if ( get_entier_signe(bs) != t[i].entier )
	{
	  eprintf("Mauvaise lecture de l'entier signe %d\n", i) ;
	  return ;
	}
      if ( get_entier_signe(bs) != -t[i].entier )
	{
	  eprintf("Mauvaise lecture de l'entier signe %d\n", -i) ;
	  return ;
	}
    }
  close_bitstream(bs) ;
}
