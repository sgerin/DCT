#include "bases.h"
#include "bits.h"
#include "bitstream.h"
#include "exception.h"

#define N 1000

void put_bits_tst()
{
  int i, j ;
  struct bitstream *s ;
  FILE *f ;

  s = open_bitstream("xxx", "w") ;  
  for(i=0; i<N; i++)
    {
      put_bits(s, 8, i) ;
      put_bits(s, 16, i) ;
    }
  close_bitstream(s) ;


  f = fopen("xxx", "r") ;  
  for(i=0; i<N; i++)
    {
      j = fgetc(f) ;
      if ( j != (i%256) )
	{
	  eprintf("put_bits ne fonctionne pas, attendu : %d, lu : %d\n"
		  , i%256, j) ;
	  return ;
	}
      j = fgetc(f) ;
      j = 256*j + fgetc(f) ;
      if ( j != i )
	{
	  eprintf("put_bits ne fonctionne pas, attendu : %d, lu : %d\n"
		  , i%256, j) ;
	  return ;
	}
    }
  fclose(f) ;
}


void get_bits_tst()
{
  int i, j ;
  struct bitstream *s ;
  FILE *f ;

  f = fopen("xxx", "r") ;  
  for(i=0; i<N; i++)
    {
      fputc(i%256, f) ;
      fputc(i/256, f) ;
      fputc(i%256, f) ;
    }
  fclose(f) ;

  s = open_bitstream("xxx", "r") ;
  if ( get_bits(s, 0) != 0 )
    	{
	  eprintf("get_bits(stream, 0) ne fonctionne pas\n") ;
	  return ;
	}
  for(i=0; i<N; i++)
    {
      j = get_bits(s, 8) ;
      if ( j != (i%256) )
	{
	  eprintf("get_bits ne fonctionne pas, attendu : %d, lu : %d\n"
		  , i%256, j) ;
	  return ;
	}
      j = get_bits(s, 16) ;
      if ( j != i )
	{
	  eprintf("get_bits ne fonctionne pas, attendu : %d, lu : %d\n"
		  , i, j) ;
	  return ;
	}
    }
  close_bitstream(s) ;
}


void put_bit_string_tst()
{
  struct bitstream *s ;
  char buf[8*256+1] ;
  int i ;

  for(i=0; i<TAILLE(buf)-1; i++)
    if ( prend_bit(i/8, i%8) )
	{
          buf[i] = i%255 + 1 ;
	  if ( buf[i] == '0' )
		buf[i] = '1' ;
	}
    else
          buf[i] = '0' ;
  buf[i] = '\0' ;

  s = open_bitstream("xxx", "w") ;  
  put_bit_string(s, buf) ;
  close_bitstream(s) ;

  s = open_bitstream("xxx", "r") ;
  for(i=0; i<TAILLE(buf)-1; i++)
    if ( get_bit(s) != prend_bit(i/8, i%8) )
      {
	eprintf("Tous caractère != '0' doit être considéré comme Vrai.\n") ;
	eprintf("Es-ce le cas pour vous ?") ;
	return ;
      }
 EXCEPTION(
     get_bit(s) ;
	eprintf("Fichier généré trop long\n") ;
     ,
     ,
     case Exception_fichier_lecture:
		break;
	) ;
      
}

