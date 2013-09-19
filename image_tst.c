#include "image.h"

void lire_ligne_tst()
{
  char ligne[MAXLIGNE] ;
  FILE *f ;

  f = fopen("xxx", "w") ;
  fprintf(f,
	  "a b c\n"
	  "# x\n"
	  "d e f\n"
	  "# y\n"
	  "# z\n"
	  "g h i\n"
	  ) ;
  fclose(f) ;


  f = fopen("xxx", "r") ;
  lire_ligne(f, ligne) ;
  if ( strcmp(ligne, "a b c\n") )
    {
      eprintf("Pas bon\n") ;
      return ;
    }
  lire_ligne(f, ligne) ;
  if ( strcmp(ligne, "d e f\n") )
    {
      eprintf("Pas bon\n") ;
      return ;
    }
  lire_ligne(f, ligne) ;
  if ( strcmp(ligne, "g h i\n") )
    {
      eprintf("Pas bon\n") ;
      return ;
    }
  lire_ligne(f, ligne) ;
}


void allocation_image_tst()
{
  struct image *image ;
  int i ;

  image = allocation_image(1, 1000) ;
  for(i=0; i<1000; i++)
    image->pixels[0][i] = 99 ;

  image = allocation_image(1000, 1) ;
  for(i=0; i<1000; i++)
    image->pixels[i][0] = 99 ;
}


void liberation_image_tst()
{
  struct image *image, *image2 ;

  image = allocation_image(100, 100) ;
  image2 = image ;
  liberation_image(image) ;
  image = allocation_image(100, 100) ;
  if ( image != image2 )
    {
      eprintf("Vous oubliez de libérer quelque chose\n") ;
      return ;
    }
}

void lecture_image_tst()
{
  struct image *image ;
  int j, i ;
  int s ;

  image = lecture_image(fopen("DONNEES/bat710.pgm","r")) ;
  if ( image->hauteur != 135 )
    {
      eprintf("Mauvaise hauteur : %d au lieu de 135\n", image->hauteur) ;
      return ;
    }
  if ( image->largeur != 254 )
    {
      eprintf("Mauvaise largeur : %d au lieu de 254\n", image->largeur) ;
      return ;
    }
  s = 0 ;
  for(j=0; j<image->hauteur; j++)
    for(i=0; i<image->largeur; i++)
      s += (j*image->pixels[j][i])/(i+1) ;

  if ( s != 5709193 )
    eprintf("Mauvais pixels : Checksum = %d\n", s) ;
}


void ecriture_image_tst()
{
  struct image *image ;
  FILE *f, *g ;
  int i ;
  int ff, gg ;
  char tmpf[100], tmpg[100] ;

  image = lecture_image(fopen("DONNEES/bat710.pgm","r")) ;
  image->pixels[50][50] = '\0' ;
  image->pixels[51][51] = '\n' ;
  f = fopen("xxx","w") ;
  ecriture_image(f, image) ;
  fclose(f) ;

  f = fopen("DONNEES/bat710.pgm", "r") ;
  g = fopen("xxx", "r") ;

  do
	{
	  assert( fgets(tmpf, sizeof(tmpf), f) == tmpf) ;
	  assert( fgets(tmpg, sizeof(tmpg), g) == tmpg) ;
	  if ( strcmp(tmpf, tmpg) != 0 )
	    {
	      eprintf("entete différent dans le fichier ecrit\n") ;
	      return ;
	    }
	}
 while( strcmp(tmpg, "255\n") != 0 ) ;

  for(i=0;;i++)
    {
      ff = fgetc(f) ;
      gg = fgetc(g) ;
      if ( feof(f) && feof(g) )
	return ;
      if ( feof(f) || feof(g) )
	{
        eprintf("L'écriture de l'image : probleme de longueur de fichier\n") ;
	return ;
	}
      if ( i == 50*image->largeur + 50 )
	ff = '\0' ;
      if ( i == 51*image->largeur + 51 )
	ff = '\n' ;
      if ( ff != gg )
	{
        eprintf("L'écriture de l'image ne redonne pas le fichier de départ\n") ;
        eprintf("i=%d ff=%d gg=%d\n", i, ff, gg) ;
	return ;
	}
    }
}
