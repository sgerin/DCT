#include "image.h"


/*
 * Lecture d'une ligne du fichier.
 * On saute les lignes commençant par un "#" (commentaire)
 * On simplifie en considérant que les lignes ne dépassent pas MAXLIGNE
 */

void lire_ligne(FILE *f, char *ligne)
{






}

/*
 * Allocation d'une image
 */

struct image* allocation_image(int hauteur, int largeur)
{










return 0 ; /* pour enlever un warning du compilateur */
}

/*
 * Libération image
 */

void liberation_image(struct image* image)
{






}

/*
 * Allocation et lecture d'un image au format PGM.
 * (L'entête commence par "P5\nLargeur Hauteur\n255\n"
 * Avec des lignes de commentaire possibles avant la dernière.
 */

struct image* lecture_image(FILE *f)
{






































return 0 ; /* pour enlever un warning du compilateur */
}

/*
 * Écriture de l'image (toujours au format PGM)
 */

void ecriture_image(FILE *f, const struct image *image)
{










}
