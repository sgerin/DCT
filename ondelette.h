/*
 * Créé le Mon Nov 25 10:56:49 2002 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_ONDELETTE_H
#define _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_ONDELETTE_H

void ondelette_1d(const float *entree, float *sortie, int nbe) ;
void ondelette_2d(float **image, int hauteur, int largeur) ;

void ondelette_1d_inverse(const float *entree, float *sortie, int nbe) ;
void ondelette_2d_inverse(float **image, int hauteur, int largeur) ;

float** allocation_matrice_float(int hauteur, int largeur) ; /**/
void liberation_matrice_float(float **table, int hauteur) ; /**/

void ondelette_encode_image(float qualite) ; /**/
void ondelette_decode_image() ; /**/


#endif
