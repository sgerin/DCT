/*
 * Créé le Wed Dec 19 15:52:03 2001 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_ENTIER_H
#define _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_ENTIER_H

#include "bitstream.h"

void put_entier(struct bitstream*, unsigned int) ;
unsigned int get_entier(struct bitstream*) ;

void put_entier_signe(struct bitstream*, int) ;
int get_entier_signe(struct bitstream*) ;

#endif
