#include "intstream.h"
#include "sf.h"
#include "entier.h"

struct intstream
{
  enum intstream_type type ;
  struct bitstream *bitstream ;           /* Dans tous les cas, le bitstream */
  struct shannon_fano *shannon_fano ;     /* Si type==Shanno_fano */
} ;


struct intstream* open_intstream(struct bitstream *bitstream
				 , enum intstream_type type
				 , struct shannon_fano *shannon_fano)
{
  struct intstream *is ;

  ALLOUER(is, 1) ;
  is->bitstream = bitstream ;
  is->type = type ;

  if ( type == Shannon_fano )
    {
      if ( shannon_fano == NULL )
	EXIT ;
      is->shannon_fano = shannon_fano ;
    }

  return(is) ;
}

void close_intstream(struct intstream *is)
{
  free(is) ;
}

void put_entier_intstream(struct intstream *is, int evenement)
{
  switch(is->type)
    {
    case Shannon_fano:
      put_entier_shannon_fano(is->bitstream, is->shannon_fano, evenement) ;
      break ;
    case Entier:
      put_entier(is->bitstream, evenement) ;
      break ;
    case Entier_Signe:
      put_entier_signe(is->bitstream, evenement) ;
      break ;
    default:
      EXIT ;
    }
}

int get_entier_intstream(struct intstream *is)
{
  switch(is->type)
    {
    case Shannon_fano:
      return(get_entier_shannon_fano(is->bitstream, is->shannon_fano)) ;
    case Entier:
      return( get_entier(is->bitstream) ) ;
    case Entier_Signe:
      return( get_entier_signe(is->bitstream) ) ;
    default:
      EXIT ;
    }
}
