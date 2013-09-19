#include "bases.h"
#include "tests.h"
#include "exception.h"
#include <sys/wait.h>
#include <string.h>

EXCEPTION_DECLARATION ;

#define F(E) case  E : eprintf("\nLe programme principal a reçu l'exception :\n%s\n", #E) ; if ( E<=Exception_fichier_ecriture ) perror("Message system") ; return(1) ;

int main(int argc, char **argv)
{
  volatile int i, r ;
  int status ;
  extern void filtres(int argc, char **argv) ;

  while ( strchr(argv[0], '/' ) )
    argv[0] = strchr(argv[0], '/') + 1 ;

  if ( strcmp(argv[0], "tests") != 0 )
    filtres(argc, argv) ;

  if ( argc >= 3 )
    {
      fprintf(stderr, "Pas d'argument : tous les tests\n") ;
      fprintf(stderr, "Un argument : la première fonction à tester\n") ;
      exit(1) ;
    }

  fprintf(stderr,
	  "***** Les tests ne garantissent pas à 100%% *****\n"
	  "*****      que la fonction est correcte.   *****\n"
	  );
  for(i=0; i<TAILLE(global); i++)
    {
      if ( argc == 1 || strcmp(global[i].nom, argv[1]) == 0 )
	{
	  argc = 1 ;
	  fprintf(stderr, "%-40s : ", global[i].nom) ;

	  if ( global[i].test == NULL )
	    {
	      fprintf(stderr, "Test non implémenté\n") ;
	      continue ;
	    }

	  fflush(stderr) ;
	  if ( fork() == 0 )
	    {
	      r = 0 ; // Pour éviter un warning
	      EXCEPTION
		(
		 (*global[i].test)() ;
		 r = eprintf_utilisee() ;
		 ,
		 ,
		 F(Exception_fichier_ouverture) ;
		 F(Exception_fichier_fermeture) ;
		 F(Exception_fichier_lecture) ;
		 F(Exception_fichier_ecriture) ;
		 F(Exception_fichier_ecriture_dans_fichier_ouvert_en_lecture) ;
		 F(Exception_fichier_lecture_dans_fichier_ouvert_en_ecriture) ;
		 F(Exception_arbre_shannon_fano_invalide) ;
		 ) ;
	      exit(r) ;
	    }
	  wait(&status) ;
	  if ( WIFSIGNALED(status) )
	    {
	      fprintf(stderr, "Signal %d (coredump ?)\n", WTERMSIG(status)) ;
	    }
	  else
	    {
	      if ( WEXITSTATUS(status) == 0 )
		{
		  fprintf(stderr, "OK\n") ;
		}
	      else
		{
		  fprintf(stderr, "\n\nLa fonction est mauvaise\n") ;
		  return(1) ;
		}
	    }
	      
	}
    }
  unlink("xxx") ;
 return(0) ;
}

