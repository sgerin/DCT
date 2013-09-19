/*
 * Créé le Wed Jan 30 10:35:05 2002 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_IMAGE_H
#define _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_IMAGE_H

#include "bases.h"

struct image
{
  int largeur ;
  int hauteur ;
  unsigned char **pixels ;
} ;

#define MAXLIGNE 9999 /* Longueur maximale d'une ligne de commentaire */

void lire_ligne(FILE *f, char *ligne) ;
struct image* allocation_image(int hauteur, int largeur) ;
void liberation_image(struct image*) ;
struct image* lecture_image(FILE *f) ;
void ecriture_image(FILE *f, const struct image *image) ;

#endif
