/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/*
 * Please fill in the following student struct
 */
team_t team = {
    "2237147",              /* Student ID */

    "Seda Civelek",     /* full name */
    "seda.civelek@metu.edu.tr",  /* email address */

    "",                   /* leave blank */
    ""                    /* leave blank */
};


/***************
 * Sobel KERNEL
 ***************/

/******************************************************
 * Your different versions of the sobel functions  go here
 ******************************************************/

/*
 * naive_sobel - The naive baseline version of Sobel
 */
char naive_sobel_descr[] = "sobel: Naive baseline implementation";
void naive_sobel(int dim,int *src, int *dst) {
    int i,j,k,l;
    int ker[3][3] = {{-1, 0, 1},
                     {-2, 0, 2},
                     {-1, 0, 1}};

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++) {
	   dst[j*dim+i]=0;
            if(!((i == 0) || (i == dim-1) || (j == 0) || (j == dim-1))){
            for(k = -1; k <= 1; k++)
                for(l = -1; l <= 1; l++) {
                 dst[j*dim+i]=dst[j*dim+i]+src[(j + l)*dim+(i + k)] * ker[(l+1)][(k+1)];
                }


      }

}
}
void sobel2(int dim, int *src, int *dst){
  int i,j;
  //int ker[3][3] = {{-1, 0, 1},
                //   {-2, 0, 2},
                  // {-1, 0, 1}};

  int ker0=-1;
  int ker1=0;
  int ker2=1;
  int ker3=-2;
  int ker4=0;
  int ker5=2;
  int ker6=-1;
  int ker7=0;
  int ker8=1;
  int rdim = dim-2;
  int add0, add1;
  int register *srci,  *dstj,*srcij, *src1;
  i=0;
  j=0;

  for(; i < dim; j+=dim)
  {
    dst[i] = 0;
    dst[j] = 0;
    ++i;

  }

  i = j-1;
  j -=(dim+1);

  for(; j > 0; j-=dim)
  {
    dst[j] = 0;
    dst[i] = 0;
    --i;
  }

  src1=src;

  for(i=rdim;i>2;i-=3){

    srci = src1; dstj =(dst+dim)+1;

    for(j=rdim;j>14;j-=15){

      srcij = srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);
    }
    for(;j--;){
      srcij=srci;
      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);
    }

    dst += dim;
    src1 += dim;

    srci = src1; dstj =(dst+dim)+1;
    for(j=rdim;j>14;j-=15){

      srcij = srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);
    }

    for(;j--;){
      srcij=srci;
      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);
    }


    dst += dim;
    src1 += dim;

    srci = src1; dstj =(dst+dim)+1;
    for(j=rdim;j>14;j-=15){

      srcij = srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);
    }

    for(;j--;){
      srcij=srci;
      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);
    }

    dst += dim;
    src1 += dim;


  }

  for(;i--;){
    srci = src1; dstj =(dst+dim)+1;

    for(j=rdim;j>14;j-=15){

      srcij = srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);

      srcij=srci;

      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);
    }
    for(;j--;){
      srcij=srci;
      add0=srcij[0]*ker0;
      add1=srcij[1]*ker1;
      add0 += srcij[2]*ker2;
      srcij +=dim;
      add1 += srcij[0]*ker3;
      add0 += srcij[1]*ker4;
      add1 += srcij[2]*ker5;
      srcij +=dim;
      add0 += srcij[0]*ker6;
      add1 += srcij[1]*ker7;
      add0 += srcij[2]*ker8;
      srci++;
      *dstj++ = (add0 + add1);
    }
    dst+=dim;
    src1+=dim;
  }



}


/*
 * sobel - Your current working version of sobel
 * IMPORTANT: This is the version you will be graded on
 */

char sobel_descr[] = "Dot product: Current working version";
void sobel(int dim,int *src,int *dst)
{
        sobel2(dim,src,dst);
        //naive_sobel(dim,src,dst);
}

/*********************************************************************
 * register_sobel_functions - Register all of your different versions
 *     of the sobel functions  with the driver by calling the
 *     add_sobel_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_sobel_functions() {
    add_sobel_function(&naive_sobel, naive_sobel_descr);
    add_sobel_function(&sobel, sobel_descr);
    /* ... Register additional test functions here */
}




/***************
 * MIRROR KERNEL
 ***************/

/******************************************************
 * Your different versions of the mirror func  go here
 ******************************************************/

/*
 * naive_mirror - The naive baseline version of mirror
 */
char naive_mirror_descr[] = "Naive_mirror: Naive baseline implementation";
void naive_mirror(int dim,int *src,int *dst) {

 	 int i,j;

  for(j = 0; j < dim; j++)
        for(i = 0; i < dim; i++) {
            dst[RIDX(j,i,dim)]=src[RIDX(j,dim-1-i,dim)];

        }

}



void mirror2(int dim,int *src, int *dst){

  int i,j;
  int *srci, *dstj,*src1;
  src1=src;
  for(i=dim;i--;){
    srci=src1;
    dstj=dst+dim-1;
    for(j=dim;j>7;j-=8){

      *dstj--=*srci;
      srci++;

      *dstj--=*srci;
      srci++;

      *dstj--=*srci;
      srci++;

      *dstj--=*srci;
      srci++;

      *dstj--=*srci;
      srci++;

      *dstj--=*srci;
      srci++;

      *dstj--=*srci;
      srci++;

      *dstj--=*srci;
      srci++;

    }
    for(;j--;){
      *dstj--=*srci;
      srci++;
    }
    src1+=dim;
    dst+=dim;
  }

}



/*
 * mirror - Your current working version of mirror
 * IMPORTANT: This is the version you will be graded on
 */
char mirror_descr[] = "Mirror: Current working version";
void mirror(int dim,int *src,int *dst)
{


 //naive_mirror(dim,src,dst);
  mirror2(dim,src,dst);
}

/*********************************************************************
 * register_mirror_functions - Register all of your different versions
 *     of the mirror functions  with the driver by calling the
 *     add_mirror_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_mirror_functions() {
    add_mirror_function(&naive_mirror, naive_mirror_descr);
    add_mirror_function(&mirror, mirror_descr);
    /* ... Register additional test functions here */
}
