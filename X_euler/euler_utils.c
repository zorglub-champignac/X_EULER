//
//  euler_utils.c
//  X_euler
//
//  Created by Jeannot on 13/03/2018.
//  Copyright © 2018 Jeannot. All rights reserved.
//
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "euler_utils.h"

u_int32_t PGCD(u_int32_t n1,u_int32_t n2 ) {
    u_int32_t r ;
    if (n1 > n2) {
        r = n2 ;
        n2 = n1 ;
    } else {
        r = n1 ;
    }
    while ( r > 0) {
        n1 = n2 ;
        n2 = r ;
        r = n1 % n2 ;
    }
    return n2 ;
}

u_int64_t PGCD64(u_int64_t n1,u_int64_t n2 ) {
    u_int64_t r ;
    if (n1 > n2) {
        r = n2 ;
        n2 = n1 ;
    } else {
        r = n1 ;
    }
    while ( r > 0) {
        n1 = n2 ;
        n2 = r ;
        r = n1 % n2 ;
    }
    return n2 ;
}

void HeapSortUint8(u_int8_t *H,int n) {
    int i;
    for(i=n-1;i>=0;i--) {
        int i1 ;
        for(i1=(i-1)/2;i1>=0;i1--) {
            int k = i1 ;
            u_int8_t heap = 0 , v=H[k] ;
            while(heap==0 && (2*k)< i) {
                int j=2*k+1;
                if( (j<i) && (H[j]<H[j+1]) ) { j++;  }
                if(v>=H[j]) {
                    heap=1;
                } else {
                    H[k]=H[j];
                    k=j;
                }
            }
            H[k]=v;
        }
        { u_int8_t  t=H[0];  H[0]=H[i];   H[i]=t; }
    }
}


void HeapSortUint8Rev(u_int8_t *H,int n) {
    int i;
    for(i=n-1;i>=0;i--) {
        int i1 ;
        for(i1=(i-1)/2;i1>=0;i1--) {
            int k = i1 ;
            u_int8_t heap = 0 , v=H[k] ;
            while(heap==0 && (2*k)< i) {
                int j=2*k+1;
                if( (j<i) && (H[j]>H[j+1]) ) { j++;  }
                if(v<=H[j]) {
                    heap=1;
                } else {
                    H[k]=H[j];
                    k=j;
                }
            }
            H[k]=v;
        }
        { u_int8_t  t=H[0];  H[0]=H[i];   H[i]=t; }
    }
}



struct  CTX_PRIMETABLE {
    u_int32_t   nbPrime ;
    T_prime     maxValue ;
    u_int32_t   maxNbPrime ;
    T_prime     *tbPrime ;
}  ;

int CPL_tablePrime(void *ptCtx,T_prime nxtPrime) {
    CTX_PRIMETABLE * ctx = (CTX_PRIMETABLE *) ptCtx ;
    if(nxtPrime > ctx->maxValue) return 0 ;
    ctx->tbPrime[ctx->nbPrime] = nxtPrime ;
    ctx->nbPrime++ ;
    if(ctx->nbPrime >= ctx->maxNbPrime) {
        ctx->maxValue = nxtPrime * nxtPrime ;
        return 0 ;
    }
    return 1;
}

const T_prime * GetTbPrime(CTX_PRIMETABLE * ctx) {
    return ctx->tbPrime ;
}

u_int32_t GetNbPrime(CTX_PRIMETABLE * ctx) {
    return ctx->nbPrime ;
}

CTX_PRIMETABLE * Free_tablePrime(CTX_PRIMETABLE * ctx) {
    if(ctx != NULL) free(ctx->tbPrime) ;
    free(ctx);
    return NULL ;
}
#define USE_Primeb  1



#define IS_Composed(p)  (isComposed[(p)/8] & (1 << ((p) & 0x7)) )
#define SET_Composed(p)  (isComposed[(p)/8] |=  (1 << ((p)  & 0x7)) )


u_int32_t FindPrime_a(T_prime nbMax,void *ctx,TY_CPL_nxtPrime nxtPrime) {
    int32_t nSqrt = 1+ (int32_t)sqrt( (double) nbMax ) ;
    T_prime sizeTable = nbMax ;
    T_prime sizeTable2 = nbMax >> 1 ;
    u_int8_t *isComposed = calloc( (sizeTable+15) /16,  sizeof(isComposed[0])) ;
    u_int32_t nbPrime = 0 ;
    T_prime curPrime = 0 ;
    T_prime lastPrime = 0 ;
    
    // on traite le cas 2 a part, car apres on ne traite que les nombres impairs
    nbPrime ++ ;
    lastPrime = 2 ;
    if(nxtPrime(ctx,2) == 0) {
        return nbPrime ;
    }
    while(++curPrime < sizeTable2) {
        if( ! IS_Composed(curPrime)) {
            lastPrime = 2*curPrime+1 ;
            nbPrime++ ;
            if(nxtPrime(ctx,lastPrime) == 0) {
                break ; // demande d'arret
            }
            if(lastPrime < nSqrt ) {
                T_prime icp2 ;
                for(icp2 = curPrime+lastPrime ; icp2 < sizeTable2; icp2 += lastPrime ) {
                    SET_Composed(icp2) ;
                }
            }
        }
    }
    free(isComposed);
    return nbPrime ;
}


//
// On replie la table.
// la taille de la table peut être quelconque
// Plus rapide pour taille nSqrt ou 32368 si trop grande valeurs
//
u_int32_t FindPrime_b(T_prime nbMax,void *ctx,TY_CPL_nxtPrime nxtPrime) {
    int32_t nSqrt = 1+ (int32_t)sqrt( (double) nbMax ) ;
    int isEnd = 0;
    T_prime *tbPrime = malloc(nSqrt * sizeof(tbPrime[0])) ;
    int32_t *offSet = malloc(nSqrt * sizeof(offSet[0])) ;
    int32_t sizeTable =  (nSqrt < 32768) ? nSqrt : 32768 ;
    int8_t *isComposed = calloc( sizeTable , sizeof(isComposed[0])) ;
    T_prime nbPrime = 0 ;
    T_prime lastPrime = 0 ;
    T_prime offSetTable = 0 ;
    T_prime nbPrimeSqrt = 0 ;
    
    nbPrime ++ ;
    lastPrime = 2 ;
    if(nxtPrime(ctx,2) == 0) {
        return nbPrime ;
    }
    // remarque le nb premier 2 n'est pas stocke dans tbPrime car pas utilise dans le crible erasto.
    // pour commencer a 3 donc indPrime = (3>>1)
    T_prime indPrime = 1 ;
    
    while ( 1) {
        T_prime icp ;
        for(icp=indPrime - offSetTable ;icp < sizeTable; icp++ ) {
            if(!isComposed[icp] ) {
                lastPrime = 2*(icp+offSetTable)+1 ;
                
                if(lastPrime < nSqrt) {
                    T_prime icp2 ;
                    tbPrime[nbPrimeSqrt] = lastPrime ;
                    for(icp2 = icp + lastPrime; icp2 < sizeTable ; icp2 += lastPrime ) {
                        isComposed[icp2] = 1 ;
                    }
                    offSet[nbPrimeSqrt++] = icp2 - sizeTable ;
                }
                nbPrime++ ;
                if(nxtPrime(ctx,lastPrime) == 0) {
                    isEnd = 1;
                    break ; // demande d'arret
                }
            }
        }
        offSetTable += sizeTable ;
        if(isEnd || offSetTable >= nbMax) break ;
        indPrime = offSetTable ;
        if ( offSetTable + sizeTable > nbMax) { sizeTable = nbMax - offSetTable ; }
        memset(isComposed,0,sizeTable) ;
        {
            int np ;
            for(np=0;np<nbPrimeSqrt;np++) {
                int32_t p = tbPrime[np] ;
                int32_t indPrime = offSet[np] ;
                while ( indPrime < sizeTable) {
                    isComposed[indPrime] = 1 ;
                    indPrime += p ;
                }
                offSet[np] = indPrime - sizeTable ;
            }
        }
    }
    free(tbPrime);
    free(offSet) ;
    free(isComposed);
    return nbPrime ;
}

u_int32_t FindPrime(T_prime maxValue,void *ctx,TY_CPL_nxtPrime nxtPrime) {
#if USE_PRIMEb
    return FindPrime_b(maxValue,ctx,nxtPrime) ;
#else
    return FindPrime_a(maxValue,ctx,nxtPrime) ;
#endif
}

CTX_PRIMETABLE * Gen_tablePrime(T_prime maxValue) {
    CTX_PRIMETABLE *ctx ;
    ctx = calloc(1,sizeof(ctx[0]));
    if(ctx == NULL) return ctx ;
    ctx->maxValue = maxValue ;
    if(maxValue > 100) {
        ctx->maxNbPrime = (T_prime) (1+ maxValue / (log((double)maxValue) - 4)) ;
    } else {
        ctx->maxNbPrime = 30 ;
    }
    ctx->tbPrime = malloc(ctx->maxNbPrime * sizeof(ctx->tbPrime[0]));
    if(ctx->tbPrime == NULL) { return Free_tablePrime(ctx) ; }
    FindPrime(maxValue,ctx,CPL_tablePrime) ;
    return ctx ;
}

CTX_PRIMETABLE * Gen_tablePrimeNb(T_prime maxNb) {
    CTX_PRIMETABLE *ctx ;
    ctx = calloc(1,sizeof(ctx[0]));
    if(ctx == NULL) return ctx ;
    if(maxNb < 30)  {
        ctx->maxNbPrime = 30 ;
    } else {
        ctx->maxNbPrime = maxNb ;
    }
    ctx->tbPrime = malloc(ctx->maxNbPrime * sizeof(ctx->tbPrime[0]));
    if(ctx->tbPrime == NULL) { return Free_tablePrime(ctx) ; }
    ctx->maxValue = 0x7fffffff ;
    FindPrime(ctx->maxValue,ctx,CPL_tablePrime) ;
    return ctx ;
}

static int32_t CmpPrime(const void *p1,const void *p2) {
    return *((int32_t *)p1) - *((int32_t *)p2) ;
}

int Search_TablePrime(CTX_PRIMETABLE *ctxP, T_prime n) {
    return (bsearch(&n,ctxP->tbPrime,ctxP->nbPrime,sizeof(n),CmpPrime) != NULL) ;
}

int SearchRg_TablePrime(CTX_PRIMETABLE *ctxP, T_prime n) {
    T_prime * pt= bsearch(&n,ctxP->tbPrime,ctxP->nbPrime,sizeof(n),CmpPrime) ;
    if(pt != NULL) return (int)(pt-ctxP->tbPrime) ;
    else return -1 ;
}

u_int64_t Sqrt64(u_int64_t val) {
    // on utilse sqrt beaucoup plus efficace (30 fois)
#if 0
    return (u_int64_t) sqrt((double)val);
#else
    // Sylvain racine carre a la main classique
    uint64_t sq = 0, b = 1LL << (sizeof(val) * 8 - 2);
    while (b > val) {
        b >>= 2;
    }
    while (b) {
        uint64_t c = sq | b;
        sq >>= 1 ;
        if (val >= c) {
            val -= c;
            sq |= b;
        }
        b >>= 2;
    }
    return sq ;
#endif
}

u_int32_t FindNbDiv(u_int64_t N, const T_prime *tbPrime) {
    u_int64_t sqr = Sqrt64(N) ;
    T_prime p ;
    u_int32_t nDiv = 1;
    for(p= *tbPrime++ ; p<= sqr ;p=*tbPrime++) {
        u_int32_t exp = 0 ;
        while((N % p) == 0) {
            N /= p ;
            exp++ ;
        }
        nDiv *= (exp+1) ;
    }
    if(N > 1) nDiv *= 2 ;
    return nDiv ;
}

u_int32_t FindNbDivPrime(u_int64_t N, const T_prime *tbPrime) {
    u_int64_t sqr = Sqrt64(N) ;
    T_prime p ;
    u_int32_t nDivP = 0;
    for(p= *tbPrime++ ; p<= sqr ;p=*tbPrime++) {
        u_int32_t exp = 0 ;
        while((N % p) == 0) {
            N /= p ;
            exp = 1 ;
        }
        nDivP += exp ;
    }
    if(N > 1) nDivP++ ;
    return nDivP ;
}


int Is_Prime(u_int64_t N, const T_prime *tbPrime) {
    u_int64_t sqr = Sqrt64(N) ;
    T_prime p ;
    for(p= *tbPrime++ ; p<= sqr ;p=*tbPrime++) {
        if((N % p) ==0) return 0 ;
    }
    return 1 ;
}

// return true if P1 and P2 are prime
int Is_Prime2(u_int64_t N1,u_int64_t N2,const T_prime *tbPrime) {
    u_int64_t sqr = Sqrt64((N1>N2) ? N1 : N2 ) ;
    
    T_prime p ;
    for(p= *tbPrime++ ; p<= sqr ;p=*tbPrime++) {
        if((N1 % p) ==0 || (N2 % p) ==0) return 0 ;
    }
    return 1 ;
}


