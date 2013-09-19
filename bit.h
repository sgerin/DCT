/*
 * Créé le Tue Dec 11 09:59:35 2001 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_BIT_H
#define _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_BIT_H

/*
 * La valeur d'un bit.
 */
typedef enum { Faux=0, Vrai=1 } Booleen ;
/*
 * Un position de bit, indique la position d'un bit dans un entier.
 * Le bit numero 0 est celui de droite (poid faible).
 */
typedef unsigned char Position_Bit ;

unsigned int nb_bits_utile(unsigned long) ;
unsigned long         pow2(Position_Bit) ;
Booleen          prend_bit(unsigned long, Position_Bit) ;
unsigned long     pose_bit(unsigned long, Position_Bit, Booleen) ;

#endif
