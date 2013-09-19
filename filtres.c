#include <string.h>
#include "bases.h"
#include "dct.h"
#include "psycho.h"
#include "rle.h"
#include "sf.h"
#include "jpg.h"
#include "image.h"
#include "matrice.h"
#include "intstream.h"
#include "bitstream.h"
#include "exception.h"
#include "ondelette.h"

#define LARG 8 /* 8 blocs à afficher */

struct parametres
{
  char *nom ;
  void (*fct)(struct parametres *p) ;

  int lit_flottant ;
  int nbe ;
  int position ;
  float qualite ;
  int shannon ;
  int saute_entete ;
} ;

void fread_safe(void *ptr, size_t size, size_t nr, FILE *f)
{
  assert(fread(ptr, size, nr, f) == nr) ;
}

#define fwrite(A,B,C,D) assert(fwrite(A,B,C,D) == (C))

void affiche_son(struct parametres *p)
{
  unsigned char *buf ;
  float *pf ;
  unsigned int h ;
  int i ;
  int taille ;
  int y ;

  taille       = p->lit_flottant ? sizeof(float) : 1 ;
  y            = p->lit_flottant ? 800 : 256 ;

  ALLOUER(buf, taille*LARG*p->nbe) ;
  pf           = (float*)buf ;

  if ( 0 )
    fprintf(stderr,"%d\n", p->nbe*taille*LARG*p->position) ;
  for(i=p->position; i>0; i--)
    if ( (h=fread((char*)buf,1,p->nbe*taille*LARG,stdin)) != p->nbe*taille*LARG)
      {
	fprintf(stderr, "Bug affiche_son : %d\n", h) ;
	abort() ;
      }

  fread_safe((char*)buf,1,p->nbe*taille*LARG, stdin) ;

  fprintf(stdout, "P5\n%d %d\n255\n", p->nbe*LARG, y ) ;

  for(h=0;h<y/2;h++)
    for(i=0;i<p->nbe*LARG;i++)
      if ( p->lit_flottant )
	  putc(h>pf[i]+y/2 ? 255 : 0, stdout) ;
	else
	  putc(h>buf[i] ? 255 : 0, stdout) ;

  for(;h<y;h++)
    for(i=0;i<p->nbe*LARG;i++)
      if ( p->lit_flottant )
	  putc(h<pf[i]+y/2 ? 255 : 0, stdout  ) ;
	else
	  putc(h<buf[i] ? 255 : 0, stdout  ) ;

  free(buf) ;
}

void filtre_prediction(struct parametres *p)
{
  int c, c1, c2, c3 ;

  c1 = 0 ;
  c2 = 0 ;
  c3 = 0 ;
  for(;;)
    {
      c = getchar() ;
      if ( c == -1 )
	break ;
      switch(p->shannon)
	{
	case 0:
	  /* c' = c - c1 */
	  putchar(128 + c-c1) ;
	  break ;
	case 1:
	  /* c'' = (c - c1) - (c1 - c2) = c - 2*c1 + c2 */
	  putchar(128 + c - 2*c1 + c2) ;
	  break ;
	case 2:
	  /* c''' = ((c - c1) - (c1 - c2)) - ((c1 - c2) - (c2 - c3))
	     c''' = c - 3*c1 + 3*c2 - c3
	  */
	  putchar(128 + c - 3*c1 + 3*c2 - c3) ;
	  break ;

	}
      c3 = c2 ;
      c2 = c1 ;
      c1 = c ;
    }
}


void filtre_dct(struct parametres *p)
{
  unsigned char *buf ;
  float *entree, *sortie ;
  int i ;

  ALLOUER(buf, p->nbe) ;
  ALLOUER(entree, p->nbe) ;
  ALLOUER(sortie, p->nbe) ;
  while( fread((char*)buf,1,p->nbe,stdin) == p->nbe )
    {
      for(i=0;i<p->nbe;i++)
	entree[i] = buf[i] - 128. ;
      dct(0, p->nbe, entree, sortie) ;
      assert(write(1, (char*)sortie, p->nbe*sizeof(*sortie))
	     == p->nbe*sizeof(*sortie)) ;
    } 
  free(buf) ;
  free(entree) ;
  free(sortie) ;
}

void saute_entete(struct parametres *p)
{
  int buf[2] ;

  if ( p->saute_entete )
    {
       fread_safe((char*)buf, 1, sizeof(buf), stdin ) ;
      fwrite((char*)buf, 1, sizeof(buf), stdout) ;
    }
}

void filtre_rle(struct parametres *p)
{
  float *entree ;
  struct intstream *entier, *entier_signe ;
  struct bitstream *bs ;
  struct shannon_fano *sf ;

  if ( p->saute_entete )
    p->nbe *= p->nbe ;

  saute_entete(p) ;
  bs = open_bitstream("-", "w") ;
  if ( p->shannon )
    {
      sf = open_shannon_fano() ;
      entier = open_intstream(bs, Shannon_fano, sf) ;
      entier_signe = open_intstream(bs, Shannon_fano, sf) ;
    }
  else
    {
      entier = open_intstream(bs, Entier, NULL) ;
      entier_signe = open_intstream(bs, Entier_Signe, NULL) ;
    }

  ALLOUER(entree, p->nbe) ;

  while( fread((char*)entree,1,p->nbe*sizeof(*entree),stdin) == p->nbe*sizeof(*entree) )
    {
      compresse(entier, entier_signe, p->nbe, entree) ;
    } 
  free(entree) ;
  close_intstream(entier) ;
  close_intstream(entier_signe) ;
  close_bitstream(bs) ;
}

void filtre_rleinv(struct parametres *p)
{
  float *entree ;
  struct intstream *entier, *entier_signe ;
  struct bitstream *bs ;
  struct shannon_fano *sf ;

  if ( p->saute_entete )
    p->nbe *= p->nbe ;

  saute_entete(p) ;
  bs = open_bitstream("-", "r") ;
  if ( p->shannon )
    {
      sf = open_shannon_fano() ;
      entier = open_intstream(bs, Shannon_fano, sf) ;
      entier_signe = open_intstream(bs, Shannon_fano, sf) ;
    }
  else
    {
      entier = open_intstream(bs, Entier, NULL) ;
      entier_signe = open_intstream(bs, Entier_Signe, NULL) ;
    }
 
  ALLOUER(entree, p->nbe) ;
  EXCEPTION(
  {
    for(;;)
      {
	decompresse(entier, entier_signe, p->nbe, entree) ;
	fwrite(entree, p->nbe, sizeof(*entree), stdout) ;
      }
  }
    ,
	,
	case Exception_fichier_lecture:
	  break ;

  ) ;

  free(entree) ;
  close_intstream(entier) ;
  close_intstream(entier_signe) ;
  close_bitstream(bs) ;
}

void filtre_psycho(struct parametres *p)
{
  float *buf ;

  ALLOUER(buf, p->nbe) ;
  while( fread((char*)buf,1,p->nbe*sizeof(*buf),stdin) == p->nbe*sizeof(*buf) )
    {
      psycho(p->nbe, buf, p->qualite) ;
      assert(write(1, (char*)buf, p->nbe*sizeof(*buf))
	     == p->nbe*sizeof(*buf));
    } 
  free(buf) ;
}

void filtre_imagedct(struct parametres *p)
{
  struct image *image ;

  image = lecture_image(stdin) ;
  fwrite(&image->hauteur, 1, sizeof(image->hauteur), stdout) ;
  fwrite(&image->largeur, 1, sizeof(image->largeur), stdout) ;
  compresse_image(p->nbe, image, stdout) ;
}

void filtre_shannon_fano_8(struct parametres *p)
{
  struct shannon_fano *sf ;
  struct bitstream *bs ;
  int c ;

  sf = open_shannon_fano() ;
  bs = open_bitstream("-", "w") ;

  for(;;)
    {
      c = getchar() ;
      if ( c == -1 )
	break ;
      put_entier_shannon_fano(bs, sf, c) ;
    }
  close_bitstream(bs) ;
  close_shannon_fano(sf) ;  
}

void filtre_shannon_fano_16(struct parametres *p)
{
  struct shannon_fano *sf ;
  struct bitstream *bs ;
  int c, d ;

  sf = open_shannon_fano() ;
  bs = open_bitstream("-", "w") ;

  for(;;)
    {
      c = getchar() ;
      if ( c == -1 )
	break ;
      d = getchar() ;
      if ( d == -1 )
	break ;
      put_entier_shannon_fano(bs, sf, c*256+d) ;
    }
  close_bitstream(bs) ;
  close_shannon_fano(sf) ;  
}

void filtre_imagedctinv(struct parametres *p)
{
  struct image *image ;
  int hauteur, largeur ;

  fread_safe(&hauteur, 1, sizeof(hauteur), stdin) ;
  fread_safe(&largeur, 1, sizeof(largeur), stdin) ;
  image = allocation_image(hauteur, largeur) ;
  decompresse_image(p->nbe, image, stdin) ;
  ecriture_image(stdout, image) ;
}

void filtre_dctinv(struct parametres *p)
{
  unsigned char *buf ;
  float *entree, *sortie ;
  int i ;

  ALLOUER(buf, p->nbe) ;
  ALLOUER(entree, p->nbe) ;
  ALLOUER(sortie, p->nbe) ;
  while( fread((char*)entree,1,p->nbe*sizeof(*entree),stdin) == p->nbe*sizeof(*entree) )
    {
      dct(1, p->nbe, entree, sortie) ;
      for(i=0;i<p->nbe;i++)
	buf[i] = sortie[i] + 128. ;
      assert(write(1, (char*)buf, p->nbe) == p->nbe) ;
    } 
  free(buf) ;
  free(entree) ;
  free(sortie) ;
}

void filtre_quantif(struct parametres *p)
{
  float **bloc ;
  int largeur, hauteur, nb_blocs ;
  int i ;

  fread_safe(&hauteur, 1, sizeof(hauteur), stdin) ;
  fread_safe(&largeur, 1, sizeof(largeur), stdin) ;
  fwrite(&hauteur, 1, sizeof(hauteur), stdout) ;
  fwrite(&largeur, 1, sizeof(largeur), stdout) ;
  bloc = allocation_matrice_carree_float(p->nbe) ;

  nb_blocs = ((hauteur+p->nbe-1)/p->nbe) * ((largeur+p->nbe-1)/p->nbe) ;

  while( nb_blocs-- )
    {
      for(i=0; i<p->nbe; i++)
	fread_safe((char*)bloc[i],p->nbe,sizeof(**bloc),stdin) ;

      quantification(p->nbe, p->qualite, bloc, p->lit_flottant) ;

      for(i=0; i<p->nbe; i++)
	fwrite((char*)bloc[i],p->nbe,sizeof(**bloc),stdout) ;
    }
}

void filtre_zigzag(struct parametres *p)
{
  float **bloc ;
  int largeur, hauteur, nb_blocs ;
  int i, x, y ;

  fread_safe(&hauteur, 1, sizeof(hauteur), stdin) ;
  fread_safe(&largeur, 1, sizeof(largeur), stdin) ;
  fwrite(&hauteur, 1, sizeof(hauteur), stdout) ;
  fwrite(&largeur, 1, sizeof(largeur), stdout) ;
  bloc = allocation_matrice_carree_float(p->nbe) ;

  nb_blocs = ((hauteur+p->nbe-1)/p->nbe) * ((largeur+p->nbe-1)/p->nbe) ;

  while( nb_blocs-- )
    {
      for(i=0; i<p->nbe; i++)
	fread_safe((char*)bloc[i],p->nbe,sizeof(**bloc),stdin) ;

      x = 0 ;
      y = 0 ;
      for(;;)
	{
	  fwrite((char*)&bloc[y][x], 1, sizeof(**bloc),stdout) ;
	  if ( x==p->nbe-1 && y==p->nbe-1 )
	    break ;
	  zigzag(p->nbe, &y, &x) ;
	}
    }
}

void filtre_zigzaginv(struct parametres *p)
{
  float **bloc ;
  int largeur, hauteur, nb_blocs ;
  int i, x, y ;

  fread_safe(&hauteur, 1, sizeof(hauteur), stdin) ;
  fread_safe(&largeur, 1, sizeof(largeur), stdin) ;
  fwrite(&hauteur, 1, sizeof(hauteur), stdout) ;
  fwrite(&largeur, 1, sizeof(largeur), stdout) ;
  bloc = allocation_matrice_carree_float(p->nbe) ;

  nb_blocs = ((hauteur+p->nbe-1)/p->nbe) * ((largeur+p->nbe-1)/p->nbe) ;

  while( nb_blocs-- )
    {
      x = 0 ;
      y = 0 ;
      for(;;)
	{
	  fread_safe((char*)&bloc[y][x], 1, sizeof(**bloc),stdin) ;
	  if ( x==p->nbe-1 && y==p->nbe-1 )
	    break ;
	  zigzag(p->nbe, &y, &x) ;
	}

      for(i=0; i<p->nbe; i++)
	fwrite((char*)bloc[i],p->nbe,sizeof(**bloc),stdout) ;
    }
}

void filtre_ondelette(struct parametres *p)
{
   ondelette_encode_image(p->qualite) ;
}

void filtre_ondeletteinv(struct parametres *p)
{
   ondelette_decode_image() ;
}

#define ARG(X) { #X, (char*)&pp.X - (char*)&pp }

void filtres(int argc, char **argv)
{
  const static struct parametres p[] =
  {
    { "affiche_son" , affiche_son            , 0, 128, 33, 10 , 0},
    { "affiche_dct" , affiche_son            , 1, 128, 33, 10 , 0},
    { "dct"         ,  filtre_dct            , 0, 128, 33, 10 , 0},
    { "dctinv"      ,  filtre_dctinv         , 0, 128, 33, 10 , 0},
    { "psycho"      ,  filtre_psycho         , 0, 128, 33, 0.5, 0},
    { "rle"         ,  filtre_rle            , 0, 128, 33, 10 , 0},
    { "rleinv"      ,  filtre_rleinv         , 0, 128, 33, 10 , 0},
    { "imagedct"    ,  filtre_imagedct       , 0,   8, 33, 10 , 0},
    { "imagedctinv" ,  filtre_imagedctinv    , 0,   8, 33, 10 , 0},
    { "quantif"     ,  filtre_quantif        , 0,   8, 33, 10 , 0},
    { "quantifinv"  ,  filtre_quantif        , 1,   8, 33, 10 , 0},
    { "zigzag"      ,  filtre_zigzag         , 0,   8, 33, 10 , 0},
    { "zigzaginv"   ,  filtre_zigzaginv      , 0,   8, 33, 10 , 0},
    { "sf8"         ,  filtre_shannon_fano_8 , 0,   8, 33, 10 , 0},
    { "sf16"        ,  filtre_shannon_fano_16, 0,   8, 33, 10 , 0},
    { "ondelette"   ,  filtre_ondelette      , 0,   8, 33, 10 , 0},
    { "ondeletteinv",  filtre_ondeletteinv   , 0,   8, 33, 10 , 0},
    { "prediction"  ,  filtre_prediction     , 0, 128, 33, 10 , 0},
    { "prediction2" ,  filtre_prediction     , 0, 128, 33, 10 , 1},
    { "prediction3" ,  filtre_prediction     , 0, 128, 33, 10 , 2},
  } ;

  struct parametres pp ;
  int i ;
  char *pc ;

  pc = argv[0] ;

  for(i=0 ; i<TAILLE(p); i++)
    if ( strcmp( pc, p[i].nom) == 0 )
      {
	pp = p[i] ;

	if ( 0 )
	  fprintf(stderr, "%s\n", argv[0]) ;

	if ( getenv("NBE") )
	  pp.nbe = atoi(getenv("NBE")) ;

	if ( getenv("POSITION") )
	  pp.position = atoi(getenv("POSITION")) ;

	if ( getenv("QUALITE") )
	  pp.qualite = atof(getenv("QUALITE")) ;

	if ( getenv("SHANNON") )
	  pp.shannon = atof(getenv("SHANNON")) ;

	if ( getenv("SAUTE_ENTETE") )
	  pp.saute_entete = atof(getenv("SAUTE_ENTETE")) ;

	(*p[i].fct)(&pp) ;
	exit(0) ;
      }
  fprintf(stderr, "Nom d'exécutable inconnu, prévenir le prof....\n") ;
  exit(1) ;
}
