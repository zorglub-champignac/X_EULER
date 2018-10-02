//
//  PB151_200.c
//  X_euler
//
//  Created by Jeannot on 01/10/2018.
//  Copyright © 2018 Jeannot. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>


#include "PB151_200.h"

#define PB198_MAXQ  100000000
#define PB198_N     1
#define PB198_D     9

#define PB198_Nend     1
#define PB198_Dend     100

typedef struct FRACTRED {
    int n ;
    int d ;
} FRACTRED ;
int PB198(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-100)/2 ; // ajout de 1/2k pour k=51,52,...,99,100
    // il faut rajouter 2 fois toutes les fractions p/q < 1/100 , irreductibles et q <= 10**8
    
    
    int32_t N = PB198_MAXQ / 200 ;
    FRACTRED * fr =malloc((N*(int64_t)(N+1))/200*sizeof(fr[0])) ;
    int64_t nb = 0 ;
    int d ;
    int n ;
    int d_end=PB198_Dend ;
    int n_end=PB198_Nend ;
    // on va chercher d0 et n0 tel que
    // on ait besout n x d0 - d * n0 = 1
    int d0, n0 ;
    d=N ;
    n=1 ;
    d0=N+1 ;
    n0=1 ;
//    fr[0].n=0 ;
//    fr[0].d=1 ;
//    nb=1 ;
//    printf("%d/%d ",n,d);
//    nbA += 2 ;
    //    int d0=4 , n0 = 1 ; // satisfait besout n x d0 - d * n0 = 1
    do {
        int a = (N+d0)/d ; // on cherche d = a * d - d0 le plus grand possible
        int tmp = d ;
        d = a * d - d0 ;
        d0 = tmp ;
        tmp = n ;
        n = a * n - n0 ; // n = a * n - n0 ;
        n0 = tmp ;
        fr[nb].n= n;
        fr[nb++].d=d ;
//        printf("%d/%d ",n,d);
        // on garde le couple (n,d) comme (n0,d0) car
        // besout  n x d0 - d * n0 = 1 est toujours satisfait
        // (a *n - n0) *d - (a*d - d0) * n = n x d0 - d * n0 = 1;
    } while(d != d_end || n != n_end ) ;
    int i ;
    FRACTRED maxFr = {0,1} ;
    for(i=0;i<nb-1;i++) {
        d = fr[i].d ;
        int j ;
        if(fr[i].n > 1)
        {
            for(j=i-1;j>=0 &&  fr[j].d>d;j--) ;
            if(j>=0 && 2*(int64_t)fr[j].d*d <= PB198_MAXQ) {
//                if(d>maxFr.d)   { maxFr = fr[i] ; printf("%d/%d<-%d/%d ",fr[j].n,fr[j].d,fr[i].n,fr[i].d) ; }
                nbA++ ;
            }
        }

        for(j=i+1;fr[j].d>d;j++) ;
        if(2*(int64_t)fr[j].d*d <= PB198_MAXQ) {
    // if(d>maxFr.d) */  { maxFr = fr[i] ; printf("%d/%d->%d/%d ",fr[i].n,fr[i].d,fr[j].n,fr[j].d) ; }
            nbA++ ;
        }
    }
    

    
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",nbA) ;
    return 1 ;
}


static int32_t Farey(int d0, int d)
{
    // (p2*q1+p1*q2)/(2*q1*q2)
 //   printf("%d,%d ",d0,d) ;
    int sum=0;
    if (d0*(int64_t)d <= PB198_MAXQ/2)
    {
        sum+=1+Farey(d0, d0+d);
        sum+=Farey(d0+d, d);
    }
    return sum ;
}

int PB198a(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-100)/2 ; //
    int i ;
    int iMax= Sqrt32(PB198_MAXQ) ;
    for(i=100;i<iMax;i++) {
        nbA += Farey(i,i+1) ;
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",nbA) ;
    return 1 ;
}

/*
 N = 10 ** 11
 n = 10 ** 3
 
 su = N / 2 - n / 2
 
 a, x = 1, 2
 stack = [(1, 1)]
 while stack:
 b, y = stack[-1]
 f1 = 2 * a * b
 ad = int(sqrt(f1 * N + 1) - a * y - b * x) / f1 - n + 2
 if ad > 0:
 su += ad
 stack.append((a + b, x + y))
 else:
 a, x = stack.pop()
 
 print su

*/
// Impementation of Stern-Brocot Tree


typedef struct SBTree {
    FRACTRED * stack ;
    int sizeStack ;
    int indS ;
    FRACTRED fr0 ;
    FRACTRED fr1 ;
} SBTree ;



void SBT_init(SBTree * sbt,FRACTRED fr0, FRACTRED fr1) {
    sbt->fr0 = fr0 ;
    sbt->indS = 0 ;
    sbt->stack[sbt->indS++] = sbt->fr1 = fr1 ;
}
#define SBT_SIZE    1024
SBTree * SBT_alloc() {
    SBTree *sbt = calloc(1,sizeof(sbt[0]));
    sbt->sizeStack = SBT_SIZE ;
    sbt->stack = malloc(sbt->sizeStack*sizeof(sbt->stack[0]));
//    SBT_init(sbt,fr0,fr1) ;
    return sbt ;
}


int SBT_ValidNxt(SBTree * sbt, int isOK) {
    if(isOK ) {
        if(sbt->indS == sbt->sizeStack) {
            FRACTRED * nstack = realloc(sbt->stack,sbt->sizeStack*2*sizeof(nstack[0])) ;
            if(nstack) {
                sbt->sizeStack *= 2 ;
                sbt->stack = nstack ;
            } else {
                return 0 ;
            }
        }
        sbt->fr1.d += sbt->fr0.d ;
        sbt->fr1.n += sbt->fr0.n ;
        sbt->stack[sbt->indS++] = sbt->fr1 ;
        return sbt->indS ;
    } else {
        sbt->fr0 = sbt->fr1 ;
        sbt->indS-- ;
        if(sbt->indS>0) sbt->fr1 = sbt->stack[sbt->indS-1] ;
        return(sbt->indS);
    }
}
SBTree * SBT_free(SBTree * sbt) {
    if(sbt) {
        free(sbt->stack);
        free(sbt);
    }
    return NULL ;
}


typedef struct SBdTree {
    int32_t * stack ;
    int sizeStack ;
    int indS ;
    int32_t d0 ;
    int32_t d1 ;
} SBdTree ;



void SBdT_init(SBdTree * sbdt,int32_t d0, int32_t d1) {
    sbdt->d0 = d0 ;
    sbdt->indS = 0 ;
    sbdt->stack[sbdt->indS++] = sbdt->d1 = d1 ;
}
#define SBT_SIZE    1024
SBdTree * SBdT_alloc() {
    SBdTree *sbdt = calloc(1,sizeof(sbdt[0]));
    sbdt->sizeStack = SBT_SIZE ;
    sbdt->stack = malloc(sbdt->sizeStack*sizeof(sbdt->stack[0]));
    //    SBT_init(sbt,fr0,fr1) ;
    return sbdt ;
}


int SBdT_ValidNxt(SBdTree * sbdt, int isOK) {
    if(isOK ) {
        if(sbdt->indS == sbdt->sizeStack) {
            int32_t * nstack = realloc(sbdt->stack,sbdt->sizeStack*2*sizeof(nstack[0])) ;
            if(nstack) {
                sbdt->sizeStack *= 2 ;
                sbdt->stack = nstack ;
            } else {
                return 0 ;
            }
        }
        sbdt->d1 += sbdt->d0 ;
        sbdt->stack[sbdt->indS++] = sbdt->d1 ;
        return sbdt->indS ;
    } else {
        sbdt->d0 = sbdt->d1 ;
        sbdt->indS-- ;
        if(sbdt->indS>0) sbdt->d1 = sbdt->stack[sbdt->indS-1] ;
        return(sbdt->indS);
    }
}
SBdTree * SBdT_free(SBdTree * sbdt) {
    if(sbdt) {
        free(sbdt->stack);
        free(sbdt);
    }
    return NULL ;
}


typedef int (* STBrcvCB)(FRACTRED fr0,FRACTRED fr1) ;


int STBrcv(FRACTRED fr0, FRACTRED fr1,STBrcvCB stbRcvCB )
{
    int sum=0;
    int k ;
    if ((k=stbRcvCB(fr0,fr1)) != 0)
    {
        FRACTRED frs ;
        frs.d = fr1.d + fr0.d ; frs.n = fr0.n + fr1.n ;
        sum+=k+STBrcv(fr0, frs,stbRcvCB);
        sum+=STBrcv(frs, fr1,stbRcvCB);
    }
    return sum ;
}

typedef int (* STBrcvDenCB)(int d0,int d1) ;



int STBrcvDen(int d0, int d1,STBrcvDenCB stbRcvDenCB )
{
    int sum=0;
    int k ;
    if ((k=stbRcvDenCB(d0,d1)) != 0)
    {
        sum+=k+STBrcvDen(d0, d0+d1,stbRcvDenCB);
        sum+=STBrcvDen(d0+d1, d1,stbRcvDenCB);
    }
    return sum ;
}


int PB198bCB(FRACTRED fr0, FRACTRED fr1) {
    int64_t dd = 2 * (int64_t) fr0.n * fr1.n ;
    int k = (int)((int64_t)(Sqrt64(dd*PB198_MAXQ+1)-fr0.n * (int64_t)fr1.d - fr1.n * (int64_t)fr0.d)/dd) - 100 +2 ;
    if(k>0) return k ;
    else return 0 ;
}

int PB198b(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-100)/2 ; //
    FRACTRED fr0 = {1,2} , fr1 = {1,1} ; ;
    nbA += STBrcv(fr0,fr1,PB198bCB) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",nbA) ;
    return 1 ;
}


static inline int PB198cCB(int d0,int d) {
    if (d0*(int64_t)d <= PB198_MAXQ/2) return 1 ;
    else return 0 ;
}

int PB198c(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-100)/2 ; //
    int i ;
    int iMax= Sqrt32(PB198_MAXQ) ;
    for(i=100;i<iMax;i++) {
        nbA += STBrcvDen(i,i+1,PB198cCB) ;
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",nbA) ;
    return 1 ;
}

/*
int PB198b(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-100)/2 ; //
    FRACTRED fr0 = {1,2} , fr1 = {1,1} ; ;
    int nv = 0 ;
    SBTree *sbt = SBT_alloc() ;
    SBT_init(sbt,fr0, fr1) ;
    while(sbt->indS > 0 ) {
        int64_t dd = 2 * (int64_t) sbt->fr0.n * sbt->fr1.n ;
        int k = (int)((int64_t)(Sqrt64(dd*PB198_MAXQ+1)-sbt->fr0.n * (int64_t)sbt->fr1.d - sbt->fr1.n * (int64_t)sbt->fr0.d)/dd) - 100 +2 ;
        if(k > 0) {
             nbA += k ;
            if(SBT_ValidNxt(sbt,1)==0) {
                if(pbR->isVerbose) fprintf(stdout,"\tPB%s ERROR REALLOC SBT(%d)\n",pbR->ident,sbt->sizeStack) ;
                SBT_free(sbt);
                return 0 ;
            }
        } else {
            SBT_ValidNxt(sbt,0) ;
        }
        nv++ ;
    }
    printf("nv=%d\n",nv) ;
    SBT_free(sbt);
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",nbA) ;
    return 1 ;
}
*/

/*

int PB198b(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-100)/2 ; //
    FRACTRED fr[5000] ,fr0, fr1 ;
    int is = 0 ;
    fr[is].n = fr[is].d = 1 ;
    is++ ;
    fr0.d = 2 ; fr0.n = 1 ;
    int isMax = 0 ;
    int nv= 0;
    while(is>0) {
         fr1 = fr[is-1] ;
//        printf("%d/%d,%d/%d",fr0.n,fr0.d,fr1.n,fr1.d) ;
       int64_t dd = 2 * (int64_t) fr0.n * fr1.n ;
        nv++ ;
        int k = (int)((int64_t)(Sqrt64(dd*PB198_MAXQ+1)-fr0.n * (int64_t)fr1.d - fr1.n * (int64_t)fr0.d)/dd) - 100 +2 ;
        if(k > 0) {
            nbA += k ;
            fr[is].n = fr0.n + fr1.n ; fr[is].d = fr0.d + fr1.d ;
            is++ ;
            if(is > isMax) isMax= is ;
        } else {
            fr0 = fr1 ;
            is-- ;
        }
    }
    printf("Ismax=%d nv=%d\n",isMax,nv) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",nbA) ;
    return 1 ;
}

*/


/*
int PB198c(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-100)/2 ; //
    int i ;
    int iMax= Sqrt32(PB198_MAXQ) ;
     SBdTree *sbdt = SBdT_alloc() ;
    for(i=100;i<iMax;i++) {
        SBdT_init(sbdt,i,i+1) ;
        while(sbdt->indS > 0) {
            if (sbdt->d0*(int64_t)sbdt->d1 <= PB198_MAXQ/2) {
                nbA++ ;
                SBdT_ValidNxt(sbdt,1) ;
            } else {
                SBdT_ValidNxt(sbdt,0) ;
            }
        }
    }
    printf("Size=%d\n",sbdt->sizeStack) ;
    SBdT_free(sbdt);
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",nbA) ;
    return 1 ;
}
*/
