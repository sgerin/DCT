#include "exception.h"
#include "bases.h"
#include "ondelette.h"

#define NBM 10

  static struct
  {
    int nb ;
    float in[NBM] ;
    float out[NBM] ;
  } t1[] =
    {
      {2, { 1, 1      	  }, {1  , 0           }},
      {2, { 1, 0      	  }, {0.5, 0.5         }},
      {2, { 0, 1      	  }, {0.5,-0.5         }},
      {3, { 1, 2, 4   	  }, {1.5, 4  ,-0.5    }},
      {4, { 1, 2, 4, 8	  }, {1.5, 6  ,-0.5, -2}},
      {5, { 1, 2, 4, 8, 16}, {1.5, 6  , 16,-0.5, -2}},
    } ;

void ondelette_1d_tst()
{
  int i, j ;
  float res[NBM] ;



  for(i=0; i<TAILLE(t1); i++)
    {
      ondelette_1d(t1[i].in, res, t1[i].nb) ;
      if ( memcmp(res, t1[i].out, t1[i].nb * sizeof(t1[i].out[0])) != 0 )
	{
	  eprintf("ondelette de :") ;
	  for(j=0; j<t1[i].nb; j++)
	    eprintf(" %g", t1[i].in[j]) ;
	  eprintf("\ndonne :") ;
	  for(j=0; j<t1[i].nb; j++)
	    eprintf(" %g", res[j]) ;
	  eprintf("\nau lieu de :") ;
	  for(j=0; j<t1[i].nb; j++)
	    eprintf(" %g", t1[i].out[j]) ;
	  eprintf("\n") ;
	  return ;
	}
    }
}

void ondelette_1d_inverse_tst()
{
  int i, j ;
  float res[NBM] ;

  for(i=0; i<TAILLE(t1); i++)
    {
      ondelette_1d_inverse(t1[i].out, res, t1[i].nb) ;
      if ( memcmp(res, t1[i].in, t1[i].nb * sizeof(t1[i].out[0])) != 0 )
	{
	  eprintf("ondelette inverse de :") ;
	  for(j=0; j<t1[i].nb; j++)
	    eprintf(" %g", t1[i].out[j]) ;
	  eprintf("\ndonne :") ;
	  for(j=0; j<t1[i].nb; j++)
	    eprintf(" %g", res[j]) ;
	  eprintf("\nau lieu de :") ;
	  for(j=0; j<t1[i].nb; j++)
	    eprintf(" %g", t1[i].in[j]) ;
	  eprintf("\n") ;
	  return ;
	}
    }
}

  static struct
  {
    int hau, lar ;
    float in[NBM][NBM] ;
    float out[NBM][NBM] ;
  } t2[] =
    {
      {2, 2
       , {{ 1, 2},
	  { 4, 8}
       }
       , {{ 3.75, -1.25},
	  { -2.25, 0.75}
       } 
      },
      {3, 2
       , {{ 1, 2},
	  { 4, 8},
	  { 16, 32},
       }
       , {{ 13.875, -1.25},
	  { -10.125, -8},
	  { -2.25, 0.75}
       } 
      },
      {3, 3
       , {{ 1, 2, 4},
	  { 8,16,32},
	  {64,128,256},
       }
       , {{ 94.1875, -42.8125,   -2.25},
	  {-81.8125, 37.1875,     -32},
	  {-5.25   ,     -14,    1.75}
       }
      },
      {4, 2
       , {{ 1, 2},
	  { 4, 8},
	  { 16, 32},
	  { 64,128}
       }
       , {{31.875,   -1.25},
	  {-28.125,     -20},
	  {-2.25,    0.75},
	  {-36,      12}
       } 
      },
      {3, 4
       , {{ 1, 2, 4,512},
	  { 8,16,32,1024},
	  {64,128,256,2048},
	  
       }
       , {{411.937500, -360.562500,  -2.250000, -375.000000},
	  {-212.062500, 167.437500, -32.000000, -896.000000},
	  {-5.250000, -135.000000,   1.750000, 121.000000}
       }
      },
      {4, 4
       , {{ 1, 2, 4,512},
	  { 8,16,32,1024},
	  {64,128,256,2048},
	  {4096, 8192, 16384, 32768}
       }
       , {{4095.937500, -2532.562500,  -2.250000, -375.000000},
	  {-3896.062500, 2339.437500, -1040.000000, -4544.000000},
	  {-5.250000, -135.000000,   1.750000, 121.000000},
	  {-3024.000000, -11712.000000, 1008.000000, 3648.000000}
       }
      },

    } ;


void ondelette_2d_tst()
{
  int i, y, x ;
  float **tf, tmp ;
  int transposee ;

  transposee = 0 ;
  for(i=0; i<TAILLE(t2); i++)
    {
      tf = allocation_matrice_float(t2[i].hau, t2[i].lar) ;
      for(y=0; y<t2[i].hau; y++)
	for(x=0; x<t2[i].lar; x++)
	  tf[y][x] = t2[i].in[y][x] ;

      ondelette_2d(tf, t2[i].hau, t2[i].lar) ;
      for(y=0; y<t2[i].hau; y++)
	for(x=0; x<t2[i].lar; x++)
	  if ( tf[y][x] != t2[i].out[y][x] )
	    {
	      eprintf("Difference (%d,%d) = %g :\n",y,x,tf[y][x] - t2[i].out[y][x] ) ;
	      eprintf("ondelette de :\n") ;
	      for(y=0; y<t2[i].hau; y++)
		{
		  for(x=0; x<t2[i].lar; x++)
		    eprintf(" %10f", t2[i].in[y][x]) ;
		  eprintf("\n") ;
		}
	      
	      eprintf("\ndonne :\n") ;
	      for(y=0; y<t2[i].hau; y++)
		{
		  for(x=0; x<t2[i].lar; x++)
		    eprintf(" %10f", tf[y][x]) ;
		  eprintf("\n") ;
		}
	      
	      eprintf("\nau lieu de :\n") ;
	      for(y=0; y<t2[i].hau; y++)
		{
		  for(x=0; x<t2[i].lar; x++)
		    eprintf(" %10f", t2[i].out[y][x]) ;
		  eprintf("\n") ;
		}
	      return ;
	    }
      liberation_matrice_float(tf, t2[i].hau) ;

      /* Transpose */
	  
      for(y=0; y<NBM; y++)
	for(x=0; x<y; x++)
	  {
	    tmp = t2[i].in[x][y] ;
	    t2[i].in[x][y] = t2[i].in[y][x] ;
	    t2[i].in[y][x] = tmp ;
	    
	    tmp = t2[i].out[x][y] ;
	    t2[i].out[x][y] = t2[i].out[y][x] ;
	    t2[i].out[y][x] = tmp ;
	  }
      tmp = t2[i].lar ;
      t2[i].lar = t2[i].hau ;
      t2[i].hau = tmp ;
      
      transposee ^= 1 ;

      if ( transposee == 1 )
	{
	  i-- ; /* reste sur le meme */
	}


    }

}


void ondelette_2d_inverse_tst()
{
  int i, y, x ;
  float **tf, tmp ;
  int transposee ;

  transposee = 0 ;
  for(i=0; i<TAILLE(t2); i++)
    {
      tf = allocation_matrice_float(t2[i].hau, t2[i].lar) ;
      for(y=0; y<t2[i].hau; y++)
	for(x=0; x<t2[i].lar; x++)
	  tf[y][x] = t2[i].out[y][x] ;

      ondelette_2d_inverse(tf, t2[i].hau, t2[i].lar) ;
      for(y=0; y<t2[i].hau; y++)
	for(x=0; x<t2[i].lar; x++)
	  if ( tf[y][x] != t2[i].in[y][x] )
	    {
	      eprintf("Difference (%d,%d) = %g :\n",y,x,tf[y][x] - t2[i].in[y][x] ) ;
	      eprintf("ondelette de :\n") ;
	      for(y=0; y<t2[i].hau; y++)
		{
		  for(x=0; x<t2[i].lar; x++)
		    eprintf(" %10f", t2[i].out[y][x]) ;
		  eprintf("\n") ;
		}
	      
	      eprintf("\ndonne :\n") ;
	      for(y=0; y<t2[i].hau; y++)
		{
		  for(x=0; x<t2[i].lar; x++)
		    eprintf(" %10f", tf[y][x]) ;
		  eprintf("\n") ;
		}
	      
	      eprintf("\nau lieu de :\n") ;
	      for(y=0; y<t2[i].hau; y++)
		{
		  for(x=0; x<t2[i].lar; x++)
		    eprintf(" %10f", t2[i].in[y][x]) ;
		  eprintf("\n") ;
		}
	      return ;
	    }
      liberation_matrice_float(tf, t2[i].hau) ;

      /* Transpose */
	  
      for(y=0; y<NBM; y++)
	for(x=0; x<y; x++)
	  {
	    tmp = t2[i].in[x][y] ;
	    t2[i].in[x][y] = t2[i].in[y][x] ;
	    t2[i].in[y][x] = tmp ;
	    
	    tmp = t2[i].out[x][y] ;
	    t2[i].out[x][y] = t2[i].out[y][x] ;
	    t2[i].out[y][x] = tmp ;
	  }
      tmp = t2[i].lar ;
      t2[i].lar = t2[i].hau ;
      t2[i].hau = tmp ;
      
      transposee ^= 1 ;

      if ( transposee == 1 )
	{
	  i-- ; /* reste sur le meme */
	}


    }

}


