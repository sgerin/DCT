/*
 * Créé le Fri Jan 11 10:16:57 2002 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_RLE_H
#define _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_RLE_H

struct intstream ;

void compresse(struct intstream *entier, struct intstream *entier_signe, int nbe, const float *dct) ;
void decompresse(struct intstream *entier, struct intstream *entier_signe, int nbe, float *dct) ;


#endif
