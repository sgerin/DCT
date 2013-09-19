/*
 * Créé le Wed Dec 12 16:44:32 2001 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_BITS_H
#define _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_BITS_H

struct bitstream ;

void         put_bits(struct bitstream *b, unsigned int nb, unsigned long v) ;
unsigned int get_bits(struct bitstream *b, unsigned int nb) ;
void   put_bit_string(struct bitstream *b, const char *bits) ;

#endif
