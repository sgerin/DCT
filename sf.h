/*
 * Créé le Wed Jan  9 13:28:54 2002 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_SF_H
#define _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_SF_H

#include "bitstream.h"

struct shannon_fano ;

struct shannon_fano* open_shannon_fano() ;

void close_shannon_fano(struct shannon_fano *sf) ;
void put_entier_shannon_fano(struct bitstream *bs, struct shannon_fano *sf, int evenement) ;
int get_entier_shannon_fano(struct bitstream *bs, struct shannon_fano *sf) ;

/* Pour les tests */

int sf_get_nb_evenements(struct shannon_fano *sf) ; /**/
void sf_get_evenement(struct shannon_fano *sf, int i, int *valeur, int *nb_occ) ; /**/
int sf_table_ok(const struct shannon_fano *sf) ; /**/

#endif
