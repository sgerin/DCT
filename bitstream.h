/*
 * Créé le Wed Dec 12 11:07:27 2001 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_BITSTREAM_H
#define _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_BITSTREAM_H

#include "bases.h"
#include "bit.h"

/*
 * Le buffer dans lequel on stocke les bits.
 * On pourrai prendre "long" mais il faudrait alors
 * déterminer si la machine est Little or Big Endian
 */
typedef unsigned char Buffer_Bit ;
/*
 * Nombre de bit dans le buffer
 */
#define NB_BITS (8*sizeof(Buffer_Bit))

struct bitstream ;

struct bitstream  *open_bitstream(const char *fichier, const char* mode) ;
void              close_bitstream(struct bitstream *b) ;
void                      put_bit(struct bitstream *b, Booleen bit) ;
Booleen 	          get_bit(struct bitstream *b) ;

FILE          *bitstream_get_file(const struct bitstream *b) ; /**/
Booleen     bitstream_en_ecriture(const struct bitstream *b) ; /**/
int bitstream_nb_bits_dans_buffer(const struct bitstream *b) ; /**/


#endif
