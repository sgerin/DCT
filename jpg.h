/*
 * Créé le Wed Jan 30 11:41:22 2002 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_JPG_H
#define _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_JPG_H

#include "bases.h"

struct image ;

void dct_image(int inverse, int nbe, float **image) ;
void quantification(int nbe, int qualite, float **extrait, int inverse) ;
void zigzag(int nbe, int *y, int *x) ;

void compresse_image(int nbe, const struct image *entree, FILE *f) ; /**/
void decompresse_image(int nbe, struct image *entree, FILE *f) ; /**/

#endif
