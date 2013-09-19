#include <stdarg.h>
#include <stdio.h>

static int  premiere = 1 ;

int eprintf(const char *format, ...)
{
    va_list ap;
    int i ;

    if ( premiere )
      {
	premiere = 0 ;
	fprintf(stderr, "\n\n************** ERREUR ! ***************\n\n") ;
      }

    va_start(ap, format);
    i = vfprintf(stderr, format, ap );
    va_end(ap);
    return(i) ;
}

int eprintf_utilisee()
{
  return(premiere==0) ;
}
