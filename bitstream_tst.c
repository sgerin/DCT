#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "bitstream.h"
#include "exception.h"
#include "bases.h"

void flush_bitstream(struct bitstream *b) ;

void open_bitstream_tst()
{
  struct bitstream *r ;
  struct bitstream *rr ;
  char tmp[99] ;

  if ( bitstream_get_file(open_bitstream("-","r")) != stdin )
    {
      eprintf("open_bitstream(\"-\",\"r\")->fichier != stdin\n") ;
      return ;
    }
  strcpy(tmp, "r") ;
  if ( bitstream_get_file(open_bitstream("-",tmp)) != stdin )
    {
      eprintf("open_bitstream(\"-\",\"?r\"+1)->fichier != stdin\n") ;
      return ;
    }
  strcpy(tmp, "-") ;
  if ( bitstream_get_file(open_bitstream(tmp, "r")) != stdin )
    {
      eprintf("open_bitstream(\"?-\"+1,\"r\")->fichier != stdin\n") ;
      return ;
    }
  if ( bitstream_en_ecriture(open_bitstream("-","r")) )
    {
      eprintf("open_bitstream(\"-\",\"r\")->ecriture : pas bon\n") ;
      return ;
    }
  if ( bitstream_en_ecriture(open_bitstream("-","r+")) )
    {
      eprintf("open_bitstream(\"-\",\"r+\")->ecriture : pas bon\n") ;
      return ;
    }
  if ( bitstream_get_file(open_bitstream("-","w")) != stdout )
    {
      eprintf("open_bitstream(\"-\",\"w\")->fichier != stdout\n") ;
      return ;
    }
  if ( bitstream_get_file(open_bitstream("-a","w")) == stdout )
    {
      eprintf("open_bitstream(\"-a\",\"w\")->fichier == stdout\n") ;
      return ;
    }
  if ( !bitstream_en_ecriture(open_bitstream("-","w")) )
    {
      eprintf("open_bitstream(\"-\",\"w\")->ecriture : pas bon\n") ;
      return ;
    }
  if ( bitstream_nb_bits_dans_buffer(open_bitstream("-","w")) )
    {
      eprintf("open_bitstream(\"-\",\"w\")->nb_bits_dans_buffer != 0\n") ;
      return ;
    }
  if ( bitstream_nb_bits_dans_buffer(open_bitstream("-","r")) )
    {
      eprintf("open_bitstream(\"-\",\"r\")->nb_bits_dans_buffer != 0\n") ;
      return ;
    }

  EXCEPTION(
     if ( bitstream_get_file(open_bitstream("--","r")) != NULL )
        {
	  eprintf("L'ouverture du fichier '--' ne lance pas d'exception !\n") ;
	  return ;
	}
     ,
     ,
     case Exception_fichier_ouverture:
     break ;
     ) ;
  /*
   * Teste ecriture simple
   */
  rr = open_bitstream("xxx", "w") ;
  if ( bitstream_get_file(rr) == NULL )
    {
      eprintf("Ne peut pas ouvrir un simple fichier en ecriture\n") ;
      return ;
    }
  if ( !bitstream_en_ecriture(rr) )
    {
      eprintf("Ouverture avec le mode 'w' n'ouvre pas en écriture !\n") ;
      return ;
    }
  fprintf(bitstream_get_file(rr), "%s", "excof") ;
  fclose(bitstream_get_file(rr)) ;
  /*
   * Teste lecture simple
   */
  rr = open_bitstream("xxx", "r") ;
  if ( bitstream_get_file(rr) == NULL )
    {
      eprintf("Ne peut pas ouvrir un simple fichier en lecture\n") ;
      return ;
    }
  if ( bitstream_en_ecriture(rr) )
    {
      eprintf("Ouverture avec le mode 'r' n'ouvre pas en lecture !\n") ;
      return ;
    }
  assert(fscanf(bitstream_get_file(rr), "%s", tmp) == 1) ;
  EXCEPTION(
	    flush_bitstream(rr)
	    ,
	    eprintf("flush_bitstream ne doit rien faire sur un fichier ouvert enlecture\n") ; 
	    ,
	    ) ;
  if ( strcmp(tmp, "excof") )
    {
      eprintf("Vous n'avez pas ouvert le bon fichier en lecture ou ecriture\n") ;
      return ;
    }
  fclose(bitstream_get_file(rr)) ;

  /*
   * Test exception ouverture fichier
   */
  r = NULL ;
  EXCEPTION(
	    r = open_bitstream("/dev/faewfewrew", "w") ;
	    ,
	    ,
	    case Exception_fichier_ouverture:
	    r = (struct bitstream*)16 ;
	    break ;
	    ) ;
  if ( r != (struct bitstream*)16 )
    {
      eprintf("open_bitstream n'a pas lancé l'exception\n") ;
      eprintf("d'ouverture de fichier impossible !\n");
      return ;
    }
}

int premier_caractere()
{
  FILE *f ;
  int c ;

  f = fopen("xxx","r") ;
  c = fgetc(f) ;
  fclose(f) ;
  return(c) ;
}

int deuxieme_caractere()
{
  FILE *f ;
  int c ;

  f = fopen("xxx","r") ;
  fgetc(f) ;
  c = fgetc(f) ;
  fclose(f) ;
  return(c) ;
}

void close_bitstream_tst()
{
  struct bitstream *bs, *bs2 ;
  int i ;

  bs = open_bitstream("xxx", "w") ;
  close_bitstream(bs) ;

  if ( premier_caractere() != EOF )
    {
      eprintf("Vous générez un fichier non vide\n");
      return ;
    }

  i = open("xxx", O_WRONLY) ;
  if ( i != 3 )
    {
      eprintf("Vous oubliez de fermer le fichier, fildes=%d\n", i);
      return ;
    }
  bs2 = open_bitstream("xxx", "w") ;
  if ( bs != bs2 )
    {
      eprintf("Vous oubliez de libérer de la mémoire\n");
      return ;
    }
  close_bitstream(bs2) ;
}

void put_bit_tst()
{
  struct bitstream *s ;
  int i, c ;
  FILE *f ;
  volatile int t ;

  s = open_bitstream("xxx", "w") ;
  put_bit(s, 0) ;
  close_bitstream(s) ;
  if ( premier_caractere() >= 128 || deuxieme_caractere() != EOF )
    {
      eprintf("Un seul appel a put_bit(b, 0) ne génère pas le bon fichier\n");
      eprintf("Pensez à vérifier : close_bitstream et flush_bitstream\n");
      return ;
    }

  s = open_bitstream("xxx", "w") ;
  put_bit(s, 1) ;
  close_bitstream(s) ;
  if ( premier_caractere() < 128 || deuxieme_caractere() != EOF )
    {
      eprintf("Un seul appel a put_bit(b, 1) ne génère pas le bon fichier\n");
      eprintf("Pensez à vérifier : close_bitstream et flush_bitstream\n");
      return ;
    }
 
  s = open_bitstream("xxx", "w") ;
  put_bit(s, 2) ;
  close_bitstream(s) ;
  if ( premier_caractere() < 128 || deuxieme_caractere() != EOF )
    {
      eprintf("Un seul appel a put_bit(b, 2) ne génère pas le bon fichier\n");
      return ;
    }

  for(c=0; c<256; c++)
    {
      s = open_bitstream("xxx", "w") ;
      for(i=7; i>=0; i--)
	put_bit(s, prend_bit(c,i)) ;
      close_bitstream(s) ;
      if ( premier_caractere() != c )
	{
	  eprintf("Ecrit %d et récupère %d\n", c, premier_caractere());
	  return ;
	}
      if ( deuxieme_caractere() != EOF )
	{
	  eprintf("8 pubits donnent un fichier de 2 octets\n") ;
	  return ;
	}
    }

  s = open_bitstream("xxx", "w") ;
  for(c=0; c<256; c++)
    {
      for(i=7; i>=0; i--)
	put_bit(s, prend_bit(c,i)) ;
    }
  close_bitstream(s) ;
  f = fopen("xxx", "r") ;
  for(c=0; c<256; c++)
    if ( fgetc(f) != c )
	{
	  eprintf("Ecriture des octets 0 à 255\n") ;
	  eprintf("écrit mal le code %d dans le fichier\n", c) ;
	  return ;
	}
  if ( fgetc(f) != EOF )
    {
      eprintf("Ecriture des octets 0 à 255\n") ;
      eprintf("écrit un octet de trop dans le fichier\n") ;
      return ;
    }


  s = open_bitstream("xxx", "r") ;
  t = 0 ;
  EXCEPTION(put_bit(s,1) ;
	    ,
	    ,
	    case Exception_fichier_ecriture_dans_fichier_ouvert_en_lecture:
	    t = 1 ;
	    break ;
	    ) ;
  if ( t == 0 )
    {
      eprintf("put_bit n'a pas lancé l'exception Exception_fichier_ecriture_dans_fichier_ouvert_en_lecture\n");
      return ;
    }
  close_bitstream(s) ;
}

void get_bit_tst()
{
  struct bitstream *s ;
  int i, c ;
  FILE *f ;
  volatile int t ;

  f = fopen("xxx", "w") ;
  for(c=0; c<256; c++)
    fputc(c, f) ;
  fclose(f) ;

  s = open_bitstream("xxx", "r") ;
  for(c=0; c<256; c++)
    {
      for(i=7; i>=0; i--)
	if ( get_bit(s) != prend_bit(c,i))
	  {
	    eprintf("getbit retourne un mauvais bit\n") ;
	    eprintf("Pour l'octet %d du fichier\n", c) ;
	    eprintf("Le bit %d de cet octet\n", i) ;
	    return ;
	  }
    }

  t = 0 ;
  EXCEPTION(get_bit(s) ;
	    ,
	    ,
	    case Exception_fichier_lecture:
	    t = 1 ;
	    break ;
	    ) ;
  if ( t == 0 )
    {
      eprintf("get_bit n'a pas lancé l'exception fin de fichier\n");
      return ;
    }
  close_bitstream(s) ;


  s = open_bitstream("xxx", "w") ;
  t = 0 ;
  EXCEPTION(get_bit(s) ;
	    ,
	    ,
	    case Exception_fichier_lecture_dans_fichier_ouvert_en_ecriture:
	    t = 1 ;
	    break ;
	    ) ;
  if ( t == 0 )
    {
      eprintf("put_bit n'a pas lancé l'exception Exception_fichier_lecture_dans_fichier_ouvert_en_ecriture\n");
      return ;
    }
  close_bitstream(s) ;

}

