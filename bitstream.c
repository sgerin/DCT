#include "bitstream.h"
#include "exception.h"

/*
 * Le but de ce fichier est de fournir des fonctions permettant
 * d'écrire/lire un flot de bit, bit par bit dans ou à partir d'un fichier.
 * On ne mélange pas écriture et lecture, on ouvre pour lire OU
 * pour écrire, pas les 2.
 *
 * Le principe est simple on utilise un buffer d'entrée/sortie.
 * On ne fait réellement la sortie que lorsque le buffer est plein
 * ou l'entrée quand il est vide.
 */


/*
 * Cette structure contient toutes les informations
 * permettant d'ecrire (ou de lire) les bits un par un dans un fichier.
 * Evidemment aucune fonction de gestion de fichier ne permet de faire cela.
 * On va donc stocker les bits un par un dans un entier (buffer)
 * et quand celui-ci sera plein, on le stockera dans le fichier.
 *
 * Pour la lecture, le procédé est inverse, on lit le buffer.
 * Puis on en extrait les bits un par un
 * jusqu'à ce qu'il soit vide.
 */
struct bitstream
 {
  FILE          *fichier ;		     /* En lecture ou Ecriture */
  Buffer_Bit     buffer ;		     /* Tampon intermediaire */
  Position_Bit   nb_bits_dans_buffer ;	     /* Nb bits dans le tampon */
  Booleen        ecriture ;		     /* Faux, si ouvert avec "r" */
 } ;

/*
 * Cette fonction alloue la structure, l'initialise et ouvre le fichier.
 * Evidemment elle vide le buffer.
 * Le "mode" est passe tel quel à la fonction "fopen".
 *
 * On considère que le fichier est ouvert en lecture si
 * le mode est "r"
 *
 * Il faut que vous testiez un nom de fichier particulier : "-"
 *     - Ce fichier est l'entrée standard s'il est ouvert en lecture
 *     - Ce fichier est la sortie standard s'il est ouvert en écriture.
 *
 * Un fichier est ouvert en lecture ou ecriture.
 * Pas les deux en même en temps.
 *     - En lecture on ne fera que des "getbit" (définie plus loin)
 *     - En écriture on ne fera que des "putbit" (définie plus loin)
 *
 * Si le fichier ne peut être ouvert, on lance l'exception :
 *         "Exception_fichier_ouverture"
 * Pour plus d'explications sur les exceptions, regardez "exception.h"
 */

struct bitstream *open_bitstream(const char *fichier, const char* mode)
{
	//struct bitstream *bs = malloc(sizeof *bs); 
	struct bitstream *bs;
	ALLOUER(bs,1);
	if(strcmp(fichier, "-") == 0)
	{
		if(strcmp(mode, "r") == 0 || strcmp(mode, "r+") == 0)
		{
			bs->fichier = stdin;
			bs->ecriture = Faux;
		}
		else
		{
			bs->fichier = stdout;
			bs->ecriture = Vrai;
		}
	}
	else
	{
			bs->fichier = fopen(fichier, mode);
			if(bs->fichier == NULL)
				EXCEPTION_LANCE(1);
			if(strcmp(mode, "r") == 0 || strcmp(mode, "r+") == 0)
				bs->ecriture = Faux;
			else
				bs->ecriture = Vrai;
	}
	
	bs->buffer = 0;
	bs->nb_bits_dans_buffer = 0;

	return bs;
	

//return 0 ; /* pour enlever un warning du compilateur */
}

/*
 * Cette fonction ne fait rien si le fichier est ouvert en lecture.
 * 
 * Si le buffer n'est pas vide :
 *    - Cette fonction stocke le buffer dans le fichier
 *      que le buffer soit "complet" ou non.
 *      Elle ne stocke rien si le buffer est vide
 *    - Elle vide ensuite le buffer.
 * Cette fonction n'est appelée que lorsque le fichier est ouvert
 * en écriture.
 *
 * Si il y a une erreur d'écriture, elle lance l'exception :
 *         "Exception_fichier_ecriture"  
 */

void flush_bitstream(struct bitstream *b)
{
	if(b != NULL)
	{
		if(b->ecriture == Vrai)
		{
			if(b->nb_bits_dans_buffer != 0)
			{
				//b->buffer = 0;
				if(fprintf(b->fichier, "%hhu", b->buffer) < 0)
					EXCEPTION_LANCE(2);
				b->nb_bits_dans_buffer = 0;
				b->buffer = 0;
			}
		}
	}
}

/*
 * Avant de fermer le fichier ouvert en écriture on copie le buffer
 * dans le fichier.
 * On ferme MEME si le fichier est l'entrée ou la sorte standard.
 *
 * Si jamais, il y a une erreur de fermeture, on lance l'exception
 *         Exception_fichier_fermeture
 */

void close_bitstream(struct bitstream *b)
{
	//printf("passage close\n");
	
	if(b != NULL)
	{
		//if(b->nb_bits_dans_buffer != 0)
		//{
		flush_bitstream(b);
			//if(fprintf(b->fichier, "%hhx", b->buffer) < 0)
			//	EXCEPTION_LANCE(2);
			//b->nb_bits_dans_buffer = 0;
		//}
		if(fclose(b->fichier) != 0)
			EXCEPTION_LANCE(4);
		free(b);
		b = NULL;
	}
}

/*
 * Cette fonction ajoute le "bit" dans le buffer.
 *    - Si celui-ci est plein, alors on sauvegarde le buffer
 *      dans le fichier avec "flush_bitstream".
 *
 *    - On pose le bit dans le buffer.
 *
 * Cette fonction n'est appelée que lorsque
 * le fichier est ouvert en écriture.
 *
 * On stocke les bits du numéro le plus grand possible jusqu'à 0.
 * On les stocke donc du poids fort au poid faible, c.a.d. de gauche à droite
 *
 * Si le fichier est ouvert en lecture, on lance l'exception
 *         Exception_fichier_ecriture_dans_fichier_ouvert_en_lecture
 */

void put_bit(struct bitstream *b, Booleen bit)
{
	if(b != NULL)
	{
		if(b->ecriture == Vrai)
		{
			if(b->nb_bits_dans_buffer == NB_BITS)
			{
				flush_bitstream(b);
				b->nb_bits_dans_buffer++;
				pose_bit(b->buffer, NB_BITS-1, bit);
			}
			else
			{
				//printf("before %hhu %i %hhu\n", b->buffer, bit,  NB_BITS-1-b->nb_bits_dans_buffer);
				//printf("result %hhu\n", pose_bit(b->buffer, NB_BITS-1-b->nb_bits_dans_buffer, bit));
				b->buffer = (unsigned char)pose_bit(b->buffer, NB_BITS-1-b->nb_bits_dans_buffer, bit);
				b->nb_bits_dans_buffer++;
				//printf("after %hhu %i %hhu\n", b->buffer, bit,  NB_BITS-1-b->nb_bits_dans_buffer);
			}
		}
		else
			EXCEPTION_LANCE(5);
	}
}


/*
 * Cette fonction lit un bit du buffer (du poid fort au poid faible)
 * Si le buffer est vide, elle va le lire dans le fichier.
 * Les valeurs retournées possibles sont :
 *    - (Faux)
 *    - (Vrai)
 *
 * Cette fonction n'est appelée que lorsque
 * le fichier est ouvert en lecture.
 *
 * ATTENTION ATTENTION : fgetc() RETOURNE UN int, PAS UN char.
 * faites donc attention aux affectations et aux tests par rapport à EOF.
 * Posez-vous les questions :
 *    - Pourquoi la valeur de retour est "int" et non "char"
 *    - Quel est le type de "b->buffer"
 *
 * En cas d'erreur de lecture (fin de fichier) on lance l'exception
 *         Exception_fichier_lecture
 *
 * Si le fichier est ouvert en écriture, on lance l'exception
 *         Exception_fichier_lecture_dans_fichier_ouvert_en_ecriture
 */

Booleen get_bit(struct bitstream *b)
{













return 0 ; /* pour enlever un warning du compilateur */
}

/*
 * Ne modifiez pas la fonctions suivantes
 *
 * VOUS NE DEVEZ PAS LES UTILISER, ELLE SONT SEULEMENT LA
 * POUR QU'IL SOIT POSSIBLE D'ECRIRE DES TESTS.
 */
FILE *bitstream_get_file(const struct bitstream *b)
 {
  return( b->fichier ) ;
 }
Booleen bitstream_en_ecriture(const struct bitstream *b)
 {
  return( b->ecriture ) ;
 }
int bitstream_nb_bits_dans_buffer(const struct bitstream *b)
 {
  return( b->nb_bits_dans_buffer ) ;
 }
