//
//  PB101_150.c
//  X_euler
//
//  Created by Jeannot on 19/03/2018.
//  Copyright © 2018 Jeannot. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>



#include "PB101_150.h"
#include "p105_data.h"

#define PB101_DEG   10
int64_t P(int32_t n) {
    // P(n) = (n**11+1)/(n+1)
    int64_t N = n ;
    int64_t N2 = N*N ;
    int64_t N4=N2*N2 ;
    int64_t N8 = N4*N4 ;
    int64_t Pn = (N8*N2*N+1)/(N+1) ;
    return Pn ;
}
int PB101(PB_RESULT *pbR) {
    int64_t P_[PB101_DEG+3] ;
    //Q[n]=Qn(n+1) = OP(n,n+1) ; D[n] = P[n+1] - OP(n,n+1)
    int64_t Q[PB101_DEG+2] ;
    int64_t D[PB101_DEG+2] ;

    pbR->nbClock = clock() ;
    Q[0] = 0 ;
    P_[1] =P(1) ;
    D[0] = P_[1]-Q[0];
    
    Q[1] = D[0] ;
    P_[2] =P(2) ;
    D[1] = P_[2] - Q[1] ;

    //Q2(2) = P(2)
    // Q2(n) = Q1(n)+ (n-1)/1! D[1] => Q2(3)=Q[2]
    Q[2] = D[0]
    + (3-1) * D[1] / 1 ;
    P_[3] =P(3) ;
    D[2] = P_[3]-Q [2] ;

    // Q3(n) = Q2(n) + (n-1)(n-2))/2! D[2]  ;  Q3(4)=Q[3]
    Q[3] = D[0]
    +  (4-1)*D[1]
    + ((4-1)*(4-2))/2 * D[2] ;
    P_[4] =P(4) ;
    D[3] = P_[4]-Q[3] ;
 
    // Q4(n) = Q3(n) + (n-1)(n-2)(n-3))/3! D[3]  Q4(5) = Q[4]
    Q[4] = D[0]
    +  (5-1)*D[1]
    + ((5-1)*(5-2))/2 * D[2]
    + ((5-1)*(5-2)*(5-3))/(2*3)  *D[3] ;
    P_[5] =P(5) ;
    D[4] = P_[5] - Q[4] ;
   
    // Q5(n) = Q4(n) + (n-1)(n-2)(n-3)(n-4) D[4] / 4! Q5(6) = Q[5]
    Q[5] = D[0]
    +  (6-1)*D[1]
    + ((6-1)*(6-2))/2 * D[2]
    + ((6-1)*(6-2)*(6-3))/(2*3) * D[3]
    + ((6-1)*(6-2)*(6-3)*(6-4))/(2*3*4) *D[4] ;
    P_[6] =P(6) ;
    D[5] = P_[6] - Q[5] ;
   
    // Q6(n) = Q5(n) + ((n-1)(n-2)(n-3)(n-4)(n-5))/5!  D[5]  Q6(7) = Q[6]
    Q[6] = D[0]
    +  (7-1)*D[1]
    + ((7-1)*(7-2))/2 * D[2]
    + ((7-1)*(7-2)*(7-3))/(2*3) * D[3]
    + ((7-1)*(7-2)*(7-3)*(7-4))/(2*3*4) *D[4]
    + ((7-1)*(7-2)*(7-3)*(7-4)*(7-5))/(2*3*4*5) *D[5] ;
    P_[7] =P(7) ;
    D[6] = P_[7] - Q[6] ;
 
    // Q7(n) = Q6(n) + ((n-1)(n-2)(n-3)(n-4)(n-5)(n-6))/6!  D[6]  Q7(8) = Q[7]
    Q[7] = D[0]
    +  (8-1)*D[1]
    + ((8-1)*(8-2))/2 * D[2]
    + ((8-1)*(8-2)*(8-3))/(2*3) * D[3]
    + ((8-1)*(8-2)*(8-3)*(8-4))/(2*3*4) *D[4]
    + ((8-1)*(8-2)*(8-3)*(8-4)*(8-5))/(2*3*4*5) *D[5]
    + ((8-1)*(8-2)*(8-3)*(8-4)*(8-5)*(8-6))/(2*3*4*5*6) *D[6] ;
    P_[8] =P(8) ;
    D[7] = P_[8] - Q[7] ;

    // Q8(n) = Q7(n) + ((n-1)(n-2)(n-3)(n-4)(n-5)(n-6)(n-7))/7!  D[7]  Q8(9) = Q[8]
    Q[8] = D[0]
    +  (9-1)*D[1]
    + ((9-1)*(9-2))/2 * D[2]
    + ((9-1)*(9-2)*(9-3))/(2*3) * D[3]
    + ((9-1)*(9-2)*(9-3)*(9-4))/(2*3*4) *D[4]
    + ((9-1)*(9-2)*(9-3)*(9-4)*(9-5))/(2*3*4*5) *D[5]
    + ((9-1)*(9-2)*(9-3)*(9-4)*(9-5)*(9-6))/(2*3*4*5*6) *D[6]
    + ((9-1)*(9-2)*(9-3)*(9-4)*(9-5)*(9-6)*(9-7))/(2*3*4*5*6*7) * D[7] ;
    P_[9] =P(9) ;
    D[8] = P_[9] - Q[8] ;

    // Q9(n) = Q8(n) + ((n-1)(n-2)(n-3)(n-4)(n-5)(n-6)(n-7)(n-8))/8!  D[8]  Q9(10) = Q[9]
    Q[9] = D[0]
    + (10-1)*D[1]
    + ((10-1)*(10-2))/2 * D[2]
    + ((10-1)*(10-2)*(10-3))/(2*3) * D[3]
    + ((10-1)*(10-2)*(10-3)*(10-4))/(2*3*4) *D[4]
    + ((10-1)*(10-2)*(10-3)*(10-4)*(10-5))/(2*3*4*5) *D[5]
    + ((10-1)*(10-2)*(10-3)*(10-4)*(10-5)*(10-6))/(2*3*4*5*6) *D[6]
    + ((10-1)*(10-2)*(10-3)*(10-4)*(10-5)*(10-6)*(10-7))/(2*3*4*5*6*7) * D[7]
    + ((10-1)*(10-2)*(10-3)*(10-4)*(10-5)*(10-6)*(10-7)*(10-8))/(2*3*4*5*6*7*8) * D[8] ;
    P_[10] =P(10) ;
    D[9] = P_[10] - Q[9] ;
    
    // Q10(n) = Q9(n) + ((n-1)(n-2)(n-3)(n-4)(n-5)(n-6)(n-7)(n-8)(n-9))/9!  D[9]  Q10(11) = Q[10]
    Q[10] = D[0]
    + (11-1)*D[1]
    + ((11-1)*(11-2))/2 * D[2]
    + ((11-1)*(11-2)*(11-3))/(2*3) * D[3]
    + ((11-1)*(11-2)*(11-3)*(11-4))/(2*3*4) *D[4]
    + ((11-1)*(11-2)*(11-3)*(11-4)*(11-5))/(2*3*4*5) *D[5]
    + ((11-1)*(11-2)*(11-3)*(11-4)*(11-5)*(11-6))/(2*3*4*5*6) *D[6]
    + ((11-1)*(11-2)*(11-3)*(11-4)*(11-5)*(11-6)*(11-7))/(2*3*4*5*6*7) * D[7]
    + ((11-1)*(11-2)*(11-3)*(11-4)*(11-5)*(11-6)*(11-7)*(11-8))/(2*3*4*5*6*7*8) * D[8]
    + ((11-1)*(11-2)*(11-3)*(11-4)*(11-5)*(11-6)*(11-7)*(11-8)*(11-9))/(2*3*4*5*6*7*8*9) * D[9];
    P_[11] =P(11) ;
    D[10] = P_[11] - Q[10] ;

    // Q11(n) = Q10(n) + ((n-1)(n-2)(n-3)(n-4)(n-5)(n-6)(n-7)(n-8)(n-9)(n-10))/10!  D[10]  Q11(12) = Q[11]
    Q[11] = D[0]
    + (12-1)*D[1]
    + ((12-1)*(12-2))/2 * D[2]
    + ((12-1)*(12-2)*(12-3))/(2*3) * D[3]
    + ((12-1)*(12-2)*(12-3)*(12-4))/(2*3*4) *D[4]
    + ((12-1)*(12-2)*(12-3)*(12-4)*(12-5))/(2*3*4*5) *D[5]
    + ((12-1)*(12-2)*(12-3)*(12-4)*(12-5)*(12-6))/(2*3*4*5*6) *D[6]
    + ((12-1)*(12-2)*(12-3)*(12-4)*(12-5)*(12-6)*(12-7))/(2*3*4*5*6*7) * D[7]
    + ((12-1)*(12-2)*(12-3)*(12-4)*(12-5)*(12-6)*(12-7)*(12-8))/(2*3*4*5*6*7*8) * D[8]
    + ((12-1)*(12-2)*(12-3)*(12-4)*(12-5)*(12-6)*(12-7)*(12-8)*(12-9))/(2*3*4*5*6*7*8*9) * D[9]
    + ((12-1)*(12-2)*(12-3)*(12-4)*(12-5)*(12-6)*(12-7)*(12-8)*(12-9)*(12-10))/(2*3*4*5*6*7*8*9*10) * D[10];
    P_[12] =P(12) ;
    D[11] = P_[12] - Q[11] ;
   
    int64_t S=0 ;
    int k ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s S",pbR->ident);
    for(k=1;k<PB101_DEG+2;k++) {
        if(D[k]) {
            S += Q[k] ;
            if(pbR->isVerbose) fprintf(stdout,"%c%lld",(k==1) ? '=' : '+',Q[k]);
        }
    }
    if(pbR->isVerbose) fprintf(stdout,"=%lld\n",S);
    sprintf(pbR->strRes,"%lld",S);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB103_MAXNB     16
#define PB103_NB  12
#define PB103_MAX_DELTA   300

typedef int32_t sum103_t ;

int CheckEquality(sum103_t *v,int lg) {
    int k ;
    for(k=2;2*k<=lg;k++) {
        int j ;
        u_int8_t perm2[PB103_MAXNB] ;
        for(j=0;j<2*k;j++)perm2[j] = j ;
        do {
            sum103_t S = 0 ;
            for(j=0;j<2*k;j++){
                S += v[perm2[j]] ;
            }
            if (S & 1) continue ; // pas divisible par 2
            S  /= 2 ;
            u_int8_t perm[PB103_MAXNB] ;
            for(j=0;j<k;j++)perm[j] = j ;
            do {
                int D = S ;
                for(j=0;j<k;j++){
                    D -= v[perm2[perm[j]]] ;
                    if( D == 0) {
                        return 0 ;
                    }
                }
            } while(NextSub(perm,k,2*k) >= 0 ) ;
        } while(NextSub(perm2,2*k,lg) >= 0) ;
    }
    return 1;
}

typedef struct AlterPaths {
    int maxK ;  // k max for the path length (k in 2k)
    int * nbK ; // number of path for length k  (nb=1 por k=2, nk=5, for k= 3
    //   nb(k) = 1/2 k(-1)/(k+1) x C[2k,k]
    int * begK ;// index for the first path of lengh k
    // a path is k indexes  in [0, 2k[
    int16_t *path ;
} AlterPaths ;

typedef struct CheckPaths {
    int maxN ; // N max, = length of ensemble to check
    int * npermK ; // number of permutation og lengh k
    int * nsumK ; // number of sum (or permutations to check) of length
                  // one more than hte number of sum (for global sum)
    int *begK ; // index for the first sum to calculate (of length k)
    int16_t *indSum ; // list on index for sum
    
} CheckPaths ;



AlterPaths * FreeAlterPath(AlterPaths * AltP) {
    if(AltP != NULL) {
        free(AltP->begK) ;
        free(AltP->nbK);
        free(AltP->path);
        free(AltP);
    }
    return NULL ;
}
AlterPaths * GetAlterPath(int maxk) {
    AlterPaths * AltP = calloc(1,sizeof(AltP[0])) ;
    AltP->nbK = malloc(maxk*sizeof(AltP->nbK[0])) ;
    AltP->begK = malloc(maxk*sizeof(AltP->begK[0])) ;
    int k ;
    AltP->nbK[0] = 0 ;
    AltP->begK[0] = 0 ;
    AltP->maxK = maxk ;
    for(k=1;k<maxk;k++) {
        AltP->begK[k] = AltP->begK[k-1]+k*AltP->nbK[k-1] ; //AltP->nbK[k-1] de longueur k
        int j;
        int k1= k+1 ; // pour tenir compte du decalage d'indice
        u_int64_t N = k1-1;
        for(j=k1+2;j<=2*k1;j++) { N *= j ;} ; // on par de k1+2 car 1/(k1+1)
        for(j=2;j<=k1;j++) N /= j ;
        AltP->nbK[k] = (int) N/2 ;
    }
    AltP->path = malloc((AltP->begK[maxk-1]+ maxk * AltP->nbK[maxk-1]) * sizeof(AltP->path[0])) ;
    u_int8_t ind[PB103_MAXNB] ;
    for(k=1;k<maxk;k++) { // on va generer les path
        int k1= k+1 ; // pour tenir compte du decalage d'indice
        int is = AltP->begK[k] ;
        int j ;
        for(j=0;j<k1-1;j++) ind[j]=j ; // choix de k1-1 parmis 2k1
        //       printf("*****Gen %d, beg=%d \n",k1,is) ;
        do {
            // on recopie ind[0..2k1] en appliquant la reverse path(2n,-2) => path(2n,0)
            int ij,  s = 1 ; // car premier element force a 1
            AltP->path[is++] = ind[0];
            for(j=1,ij=1;j<2*k1;j++) {
                if(ij < k1-1 && j==ind[ij]) { // +1
                    s++ ;
                    AltP->path[is++] = j ;
                    ij++ ;
                } else { // -1
                    s-- ;
                    if(s==-1) { j++ ; break ;} // on arrive en mode reverse
                }
            }
            for(;j<2*k1;j++) {
                if(ij < k1-1 && j==ind[ij]) { // +1
                    ij++ ;
                    s-- ;  // normalement inutile de compter
                } else { // -1
                    s++ ;
                    AltP->path[is++] = j ;
                }
            }
            //           for(j=0;j<k1;j++) printf("%d%c",AltP->path[is-k1+j],j==k1-1 ? '\n' : ',') ;
        } while(NextSub(ind+1, k1-2, 2*k1)>=0) ; // on contraint le premier a etre +1 pour gerer la symetrie
        //       printf("*****End %d, beg=%d \n",k1,is) ;
    }
    return AltP ;
}

CheckPaths * FreeCheckPath(CheckPaths * chkP) {
    if(chkP != NULL) {
        free(chkP->begK) ;
        free(chkP->nsumK);
        free(chkP->indSum);
        free(chkP->npermK);
        free(chkP);
    }
    return NULL ;
}

CheckPaths * GetCheckPath(int N, AlterPaths * altP,int isSup) {
    CheckPaths * chkP = calloc(1,sizeof(chkP[0])) ;
    int maxk = N/2 ;
    int dN1 = isSup ? 1 : 0 ;
    chkP->nsumK = malloc(maxk*sizeof(chkP->nsumK[0])) ;
    chkP->npermK = malloc(maxk*sizeof(chkP->npermK[0])) ;
    chkP->begK = malloc(maxk*sizeof(chkP->begK[0])) ;
    int k ;
    chkP->nsumK[0] = 0 ;
    chkP->begK[0] = 0 ;
    chkP->npermK[0] = 0 ;
    chkP->maxN = N ;
    for(k=1;k<maxk;k++) {
        int j ;
        int k1= k+1 ; // pour tenir compte du decalage d'indice
        chkP->begK[k] = chkP->begK[k-1]+k*chkP->nsumK[k-1]*chkP->npermK[k-1] ; //chkP->nbK[k-1] de longueur k
        u_int64_t CN2k = 1;
        for(j=0;j<2*k1-dN1;j++) { CN2k *= N-dN1-j ; }
        for(j=2;j<=2*k1-dN1;j++) { CN2k /= j ; }
        chkP->npermK[k] = (int) CN2k ;
        chkP->nsumK[k] = (altP->nbK[k] + 1)  ;
    }
    chkP->indSum = malloc((chkP->begK[maxk-1]+ maxk * chkP->nsumK[maxk-1])* chkP->npermK[maxk-1] * sizeof(chkP->indSum[0])) ;
    u_int8_t perm2[PB103_MAXNB] ;
    for(k=1;k<maxk;k++) { // on va generer les index
        int k1= k+1 ; // pour tenir compte du decalage d'indice
        int is = chkP->begK[k] ;
        int j ;
        if(dN1) perm2[2*k1-1] = N-1 ;
        for(j=0;j<2*k1-dN1;j++)perm2[j] = j ;
 //       printf("*****Gen %d, beg=%d \n",k1,is) ;
        do {
            int16_t * ind  = altP->path + altP->begK[k] ;
            int nb = altP->nbK[k] ;
            // add of the complement of the first sum
            int ij ;
            for(ij=0 ;ij<2*k1;) {
                while(*ind != ij) {
                    chkP->indSum[is++] = perm2[ij++] ;
                }
                ind++; ij++ ;
            }
 //           for(j=0;j<k1;j++) printf("%d%c",chkP->indSum[is-k1+j],j==k1-1 ? ' ' : '.') ;
            ind -= k1 ;
            while(nb-- > 0) {
                for(j=0;j<k1;j++) {
                    chkP->indSum[is++] = perm2[*ind++] ;
                }
  //              for(j=0;j<k1;j++) printf("%d%c",chkP->indSum[is-k1+j],j==k1-1 ? '\n' : ',') ;
            }
        } while(NextSub(perm2,2*k1-dN1,N-dN1) >= 0) ; //
  //      printf("*****End %d, beg=%d \n",k1,is) ;
    }
    return chkP ;

    
}

typedef struct HalfPaths {
    int maxN ; // N max, = length of ensemble to check
    int npermk ; // number of permutation of lengh k
    int16_t * indSum ; // list on index for sum
    
} HalfPaths ;


HalfPaths * FreeHalfPath(HalfPaths * hlfP) {
    if(hlfP != NULL) {
        free(hlfP->indSum);
        free(hlfP);
    }
    return NULL ;
}

HalfPaths * GetHalfPath(int N) {
    HalfPaths * hlfP = calloc(1,sizeof(hlfP[0])) ;
    int k = N/2 ;
    hlfP->maxN = N ;
    {
        int j ;
        u_int64_t CNk = 1;
        for(j=0;j<k;j++) { CNk *= N-j ; }
        for(j=2;j<=k;j++) { CNk /= j ; }
        hlfP->npermk = (int) CNk ;
    }
    hlfP->indSum = malloc(k * hlfP->npermk * sizeof(hlfP->indSum[0])) ;
    u_int8_t perm2[PB103_MAXNB] ;
    int is = 0 ;
    int j ;
    for(j=0;j<k;j++)perm2[j] = j ;
    do {
        for(j=0;j<k;j++) hlfP->indSum[is++] = perm2[j] ;
    } while(NextSub(perm2,k,N) >= 0) ; //
    return hlfP ;
}


typedef struct GlobalPaths {
    int maxN ; // N max, = length of ensemble to check
    int npermk ; // number of permutation of lengh k
    int npermk1 ; // number of permutation of lengh k-1
    int16_t * indSumk ; // list on index for sum
    int16_t * indSumk1 ; // list on index for sum
    
} GlobalPaths ;


GlobalPaths * FreeGlobalPath(GlobalPaths * glbP) {
    if(glbP != NULL) {
        free(glbP->indSumk);
        free(glbP->indSumk1);
        free(glbP);
    }
    return NULL ;
}

GlobalPaths * GetGlobalPath(int N) {
    GlobalPaths * glbP = calloc(1,sizeof(glbP[0])) ;
    int k = N/2 ;
    int k1 = k-1 ;
    glbP->maxN = N ;
    {
        int j ;
        u_int64_t CNk1 = 1;
        for(j=0;j<k1;j++) { CNk1 *= N-1-j ; }
        for(j=2;j<=k1;j++) { CNk1 /= j ; }
        glbP->npermk1 = (int) CNk1 ;
        u_int64_t CNk = (CNk1 * (N-k)) /k ;
        glbP->npermk = (int) CNk ;
    }
    glbP->indSumk = malloc(k * glbP->npermk * sizeof(glbP->indSumk[0])) ;
    glbP->indSumk1 = malloc((k-1) * glbP->npermk1 * sizeof(glbP->indSumk1[0])) ;
    u_int8_t perm2[PB103_MAXNB] ;
    int is = 0 ;
    int j ;
    for(j=0;j<k;j++)perm2[j] = j ;
    do {
        for(j=0;j<k;j++) glbP->indSumk[is++] = perm2[j] ;
    } while(NextSub(perm2,k,N-1) >= 0) ; //
    is = 0 ;
    for(j=0;j<k1;j++)perm2[j] = j ;
    do {
        for(j=0;j<k1;j++) glbP->indSumk1[is++] = perm2[j] ;
    } while(NextSub(perm2,k1,N-1) >= 0) ; //
    return glbP ;
}



// Comparaison avec parcours des sous-ensembles a 2*k elements du total a N elements
// Les indices des sous-ensembles a k elements sont precalcules dans AltP
// Ce precalcul ne depend pas de N
// si isSup est mis on suppose que le test a N-1 element est OK
// donc on ne prend que les sous-ensemble 2*k contenant N-1
static int CheckEqualityAlt(sum103_t *v,int N,AlterPaths * AltP,int isSup) {
    int k ;
    int dN = (isSup) ? 1 : 0 ;
    for(k=2;2*k<=N;k++) {
        int j ;
        u_int8_t perm2[PB103_MAXNB] ;
        for(j=0;j<2*k-dN;j++)perm2[j] = j ;
        if(dN) perm2[2*k-1] = N-1 ;
        do {
            sum103_t S = 0 ;
            for(j=0;j<2*k;j++){
                S += v[perm2[j]] ;
            }
            if (S & 1) continue ; // pas divisible par 2
            S  /= 2 ;
            int16_t * ind  = AltP->path + AltP->begK[k-1] ;
            int nb = AltP->nbK[k-1] ;
            while(nb-- > 0) {
                int D = S ;
                for(j=0;j<k;j++) {
                    D -=v[perm2[*ind++]] ;
                }
                if( D == 0) {
                    return 0 ;
                }
            }
        } while(NextSub(perm2,2*k-dN,N-dN) >= 0) ;
    }
    return 1;
}

// Comparaison avec precalcul total des sous-ensembles a 2*k elements parmi N
// ainsi que des sous-ensemble a k elements a traite
// Ce precalcul DEPEND de N
// Le precalcul peut integrer le fait que N-1 est verifie
static int CheckEqualityPreN(sum103_t *v,int N,CheckPaths * chkP) {
    int k ;
    for(k=(N>>1);k>1;k--) {
        int j ;
        int nbPerm = chkP->npermK[k-1] ;
        int16_t * ind  = chkP->indSum + chkP->begK[k-1] ;
        int nbSum = chkP->nsumK[k-1] -2 ;
        while (nbPerm-- > 0) {
            int16_t * indk = ind+k ;
            sum103_t S0 = v[*ind++] , S = v[*indk++] ;
            for(j=1;j<k;j++) { S0 += v[*ind++] ; S += v[*indk++] ; }
            ind = indk ;
            if(S==S0) return 0 ;
            S += S0 ; // sum total
            if(S & 1) { ind += k * (nbSum) ; continue ; }
            S >>= 1;
            int nb = nbSum ;
            while(nb-- > 0) {
                int16_t D = v[*ind++] ;
                for(j=1;j<k;j++) {
                    D +=v[*ind++] ;
                }
                if( D == S) {
                    return 0 ;
                }
                
            }
        }
    }
    return 1;
}


static u_int8_t isSum[20000] ;

static int CheckEqualityHalf(sum103_t *v,int N) {
    int k = N/2 ;
    if(k==1) return 1;
    int j ;
    sum103_t ST = v[N-1] ;
    for(j=2;j<=k;j++) ST += v[N-j] ;
    memset(isSum,0,ST+1) ;
    
    u_int8_t perm2[PB103_MAXNB] ;
    for(j=0;j<k;j++)perm2[j] = j ;
        int il=0  ;
        sum103_t S=0 ;
        do {
            if(il < k-1) {
                S=v[perm2[0]] ;
                for(j=1;j<k-1;j++) S += v[perm2[j]] ;
            }
            if(isSum[S+v[perm2[k-1]]]) {
                return 0 ;
            }
            isSum[S+v[perm2[k-1]]] = 1 ;
        } while( (il=NextSub(perm2,k,N)) >= 0) ;
        return 1;
}

static int CheckEqualityPreH(sum103_t *v,HalfPaths * hlfP) {
    int N = hlfP->maxN ;
    int k = hlfP->maxN / 2  ;
    if(k==1) return 1;
    int j ;
    sum103_t ST = v[N-1] ;
    for(j=2;j<=k;j++) ST += v[N-j] ;
    memset(isSum,0,ST+1) ;
    int nbPerm = hlfP->npermk ;
    int16_t *ind = hlfP->indSum ;
    while(nbPerm-- > 0) {
        sum103_t S = v[*ind++] ;
        for(j=1;j<k;j++) S += v[*ind++] ;
        if(isSum[S]) {
            return 0 ;
        }
        isSum[S] = 1 ;
    }
    return 1;
}

static u_int8_t isSumk[20000] ;
static u_int8_t isSumk1[20000] ;

static u_int8_t isDelta[1000] ;

static int CheckEqualityPreG(sum103_t *v,GlobalPaths * glbP,sum103_t *vDelta,int maxDeltaVal, int maxCand) {
    int N = glbP->maxN ;
    int k = glbP->maxN / 2  ;
    int k1 = k - 1 ;
    if(k==1) return 1;
    int j ;
    sum103_t maxSk1 = v[N-2] ;
    sum103_t minSk1 = v[0] ;
    for(j=1;j<k1;j++) { maxSk1 += v[N-j-2] ; minSk1 += v[j] ; }
    memset(isSumk1,0,maxSk1+1) ;
    sum103_t maxSk = maxSk1 + v[N-k-1] ;
    sum103_t minSk = minSk1 + v[k-1] ;
        
    memset(isSumk,0,maxSk+1) ;
    
    int nbPermk = glbP->npermk ;
    int16_t *ind = glbP->indSumk ;
    while(nbPermk-- > 0) {
        sum103_t S = v[*ind++] ;
        for(j=1;j<k;j++) S += v[*ind++] ;
        isSumk[S] = 1 ;
    }

    nbPermk = glbP->npermk1 ;
    ind = glbP->indSumk1 ;
    while(nbPermk-- > 0) {
        sum103_t S = v[*ind++] ;
        for(j=1;j<k1;j++) S += v[*ind++] ;
        isSumk1[S] = 1 ;
    }
    
    sum103_t deltaOK = maxSk - minSk1 - v[N-2] ;
    
    sum103_t maxChk = (deltaOK < maxDeltaVal ) ? deltaOK : maxDeltaVal ;
    
    memset(isDelta,0,maxChk+1) ;
    int ik, ik1 ;
    int lastIk1 = maxSk - v[N-2] -1  ;
    if(lastIk1 > maxSk1 ) lastIk1 = maxSk1 ;
    for(ik1=minSk1;ik1<=lastIk1;ik1++) {
        if(isSumk1[ik1] == 0) continue ;
        ik = v[N-2] + ik1  + 1;
        if(ik < minSk ) ik = minSk ;
        int Dk = ik - ik1 - v[N-2]  - 1  ;
        int lastIk = v[N-2] + maxChk + ik1 +  1  ;
        if(lastIk > maxSk ) lastIk = maxSk ;
        for(;ik <= lastIk ;Dk++,ik++) {
            if(isSumk[ik])
                isDelta[Dk] = 1 ;
        }
    }
    int nbSol = 0 ;
    int d = 0;
    for(d=0;d<maxChk;d++) {
        if(isDelta[d]==0) {
            vDelta[nbSol++] = d ;
            if(nbSol >= maxCand) break ;
        }
    }
    if(d < maxDeltaVal ) {
        if(maxDeltaVal-d < maxCand) maxCand = maxDeltaVal-d ;
        for( ;nbSol < maxCand; nbSol++) vDelta[nbSol] = deltaOK++ ;
    }
 /*
    {
        int j ;
        printf("%d,%d ",N,nbSol) ;
        for(j=0;j<N-1;j++) printf("%d%c",v[j],(j==N-2) ? ' ': ',') ;
        for(j=1;j<N-1;j++) printf("%d%c",v[j]-v[j-1]-1,(j==N-2) ? ' ': '.') ;
        for(j=0;j<nbSol;j++) printf("%d%c",vDelta[j],(j==nbSol-1) ? '\n': ' ') ;
    }
 */
    return nbSol ;
}


// calcule v0 minimum (mandatory) pour que la condition ii soit verifiee
sum103_t MinCheck(sum103_t * v,int lg) {
    sum103_t minV0 = 1 ;
    int j ;
    int lg2 = (lg-1)/2 ;
    for(j=0;j<lg2;j++){
        minV0 += v[lg-1-j] - v[j+1] ;
    }
    return minV0 ;
}

// calcul le v[N-1] a partir duquel plus besoin de faire de test
// en incremental si N-1 a ete teste.
sum103_t MaxCheck(sum103_t * v,int lg) {
    sum103_t maxV0 = 1+ v[lg-2] ;
    int j ;
    int lg2 = lg/2-1 ;
    for(j=0;j<lg2;j++){
        maxV0 += v[lg-3-j] - v[j] ;
    }
    return maxV0 ;
}


int Check(sum103_t *v, int lg) {
    if(v[0] < MinCheck(v,lg)) return 0 ;
    return CheckEquality(v,lg);
}

// version sans ponderation, on parcours par deltaS=SIGMA(delta[is]) croissant
// peu performant car la contribution de v0 est preponderante et
// n'est pris en compte qu'apres coup.
// Critere d'arret avec une pauvre estimation de la valeur min de v0
// Utilise CheckEqualityAlt
int PB103(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    sum103_t values[PB103_NB] ;
    sum103_t minV0 = 32000 ;
    static int MinV0[] ={1,1,1,2,3,6,11,20,39} ;
    int16_t Smin = 32000 ;
    int AntMinV0 ;
    if(PB103_NB*sizeof(MinV0[0]) <= sizeof(MinV0)) {
        AntMinV0 = MinV0[PB103_NB-1] * PB103_NB ;
    } else {
        AntMinV0 = (((PB103_NB-1)/2) * ((PB103_NB-1)/2)) *  PB103_NB ;
    }
    AlterPaths *AltP =GetAlterPath(PB103_NB/2) ;
    CheckPaths *chkP =GetCheckPath(PB103_NB,AltP,0) ;
        values[0] = 0 ;
        int j ;
        int is = 1 ;
        int deltaS = 0 ;
        sum103_t delta = deltaS ;
        sum103_t deltaMax = Smin - AntMinV0 ;
        sum103_t Delta[PB103_NB] ;
        for(j=0;j<PB103_NB-1;j++) Delta[j] = 0;
        Delta[PB103_NB-1] = deltaS ;
        sum103_t offsetDeltaS = (PB103_NB*(PB103_NB-1))/2  ;
        while(deltaS <= deltaMax) {
        for(;is<PB103_NB;is++) {
            values[is] = values[is-1]+Delta[is]+1 ;
        }
        sum103_t v0 = MinCheck(values,PB103_NB) ;
        sum103_t S = deltaS + PB103_NB * v0 ;
        if(S <= Smin && CheckEqualityPreN(values,PB103_NB,chkP))  {
            int j ;
            Smin = S ;
            deltaMax = Smin - AntMinV0 ;
            if(v0 < minV0) minV0= v0 ;
        
            if(pbR->isVerbose) {
                fprintf(stdout,"\t PB%s S=%d Delta=%d,minv0=%d,DeltaMax=%d ",pbR->ident,S+offsetDeltaS,deltaS,minV0,deltaMax ) ;
                for(j=0;j<PB103_NB;j++){
                    fprintf(stdout,"%d%c",values[j]+v0,(j==PB103_NB-1) ? ' ' : ',' ) ;
                }
                for(j=1;j<PB103_NB;j++){
                    fprintf(stdout,"%d%c",values[j]-values[j-1]-1,(j==PB103_NB-1) ? '\n' : '.' ) ;
                }
            }
            int lg = 0 ;
            for(j=0;j<PB103_NB;j++){
                lg+=sprintf(pbR->strRes+lg,"%2.2d",values[j]+v0) ;
            }
        }
        is = PB103_NB-2 ;
        while(PB103_NB-is > delta && is){
            delta += (PB103_NB-is) * Delta[is] ;
            Delta[is-- ] = 0;
        }
        if(is == 0) {
            deltaS++ ;
            delta = deltaS  ;
            is = 1 ;
        } else {
            Delta[is]++ ; delta -= PB103_NB-is ;
        }
        Delta[PB103_NB-1] = delta ;

    }
    FreeCheckPath(chkP) ;
    FreeAlterPath(AltP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

//
// calcule les ponderations des deltas[is] ds la somme
// la relation entre les v[is] et les deltas est la suivante :
// v[is] = v[is-1]+ delta[is]+1
// Les pondérations tiennent compte de 2 facteurs :
// - la recurrence ci-dessus (les delta[is] ont un poids de PB103_NB - i
// - le fait qu'il vaur rajouter a toutes les valeurs v0 = 1+ v[N-1] +v[N-2]+v[N-lg2]+..+ v[(N-1-lg2] -v[lg2]-v[lg2-1]...-v[2]
//  donc un poids x N.
// On stocke dans pond[0] l'offset constant( donc ss influence sur les comparaisons)
// entre la somme des deltas ponderes (SIGMA(pond[is]*delta[is]) et la vrai somme
sum103_t InitPondSum(sum103_t * pondDelta,int lgAsk , AlterPaths *AltP) {
    sum103_t v[PB103_NB] ;
    sum103_t offsetSum = 1 ;
    int i ;
    for(i=1;i<PB103_NB;i++) {
        pondDelta[i] = PB103_NB - i ;
        offsetSum += i+1 ;
    }
    int lg2 = (PB103_NB-1)/2 ;
    for(i=0;i<lg2;i++) {
        int k ;
        offsetSum +=(PB103_NB-1-i)  * PB103_NB ;
        for(k=1;k<=PB103_NB-1-i;k++) {
            pondDelta[k]+= PB103_NB ;
        }
        offsetSum -= (i+1) * PB103_NB ;
        for(k=1;k<=i+1;k++) {
            pondDelta[k]-= PB103_NB ;
        }
    }
    pondDelta[0] = offsetSum ; // inutilise
    //  for(i=0;i<PB103_NB;i++) { printf("%d%c",pondDelta[i],(i==PB103_NB-1) ? '\n' : ' '); }
    
    
    //on va initialiser avec une valeur pas trop loin de l'optimum
    v[0] = 0 ;  v[1] = 2 ; v[2] = 3 ;     v[3] = 4 ;
    switch(lgAsk){
        case 0: return 0;
        case 1: return v[0] ;
        case 2: return v[0] + v[1]  ;
        case 3: return v[0] + v[1] + v[2];
        case 4: return v[0] + v[1] + v[2]+ v[3];
    }
    int j, lg ;
    for(lg=3;;lg++) {
        sum103_t v0 = MinCheck(v,lg+1) ;
        for(j=0;j<=lg;j++) v[j] += v0 ;
        if(!CheckEqualityAlt(v,lg,AltP,0) ) {
            for(j=0;j<=lg;j++){ printf("%d%c",v[j],(j==lg) ? '\n' : ',' ) ; }
            return 0 ;
        }
        if(lg < lgAsk-1) {
            for(j=lg;j>=0;j--) v[j+1] = v[j] ;
            v[0] = 0 ;
        } else {
            break ;
        }
    }
    sum103_t Smin = 0 ;
    for(j=0;j<lgAsk;j++) Smin += v[j] ;
    return Smin ;
    
}


// version avec ponderation, mais le parcours se fait en tenant compte
// pour l'ordre de toutes les ponderations sauf la derniere valeur
// Du coup nombre de cas excede largement Smin et sont rejetes au dernier moment
// Utilise CheckEqualityPreN

int PB103a(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    sum103_t values[PB103_NB] ;
    sum103_t Smin = 32000 ;
    sum103_t deltaMax = 20000 ;
    sum103_t pondDelta[PB103_NB] ;
    AlterPaths *AltP =GetAlterPath(PB103_NB/2) ;
    CheckPaths *chkP =GetCheckPath(PB103_NB,AltP,0) ;
    Smin = InitPondSum(pondDelta,PB103_NB ,AltP) ;
    sum103_t offsetSum = pondDelta[0]  ;
    
    if(pbR->isVerbose) {
        int i ;
        fprintf(stdout,"\t PB%3s Smin=%d,Dmin=%d Pond=",pbR->ident, Smin+offsetSum,Smin) ;
        for(i=0;i<PB103_NB;i++) { fprintf(stdout,"%d%c",pondDelta[i],(i==PB103_NB-1) ? '\n' : ' '); }
    }
    values[0] = 0 ;
    int j ;
    int is = 1 ;
    int deltaS = 0 ;
    sum103_t delta = deltaS ;
    sum103_t Delta[PB103_NB] ;
    for(j=0;j<PB103_NB-1;j++) Delta[j] = 0;
    Delta[PB103_NB-1] = deltaS ;
    while(deltaS<=deltaMax) {
        for(;is<PB103_NB;is++) {
            values[is] = values[is-1]+Delta[is]+1 ; /*S+= values[is] ;*/
        }
        sum103_t S = deltaS + Delta[PB103_NB-1] * ( pondDelta[PB103_NB-1]-1) ;
        if( S <= Smin &&  CheckEqualityPreN(values,PB103_NB,chkP))  {
            sum103_t v0 = MinCheck(values,PB103_NB) ;
            int j ;
            Smin = S ;
            deltaMax = S ;
            if(pbR->isVerbose) {
                fprintf(stdout,"\t PB%s S=%d Delta=%d DeltaMax=%d ",pbR->ident,Smin+offsetSum,deltaS,deltaMax ) ;
                for(j=0;j<PB103_NB;j++){
                    fprintf(stdout,"%d%c",values[j]+v0,(j==PB103_NB-1) ? ' ' : ',' ) ;
                }
                for(j=1;j<PB103_NB;j++){
                    fprintf(stdout,"%d%c",values[j]-values[j-1]-1,(j==PB103_NB-1) ? '\n' : '.' ) ;
                }
            }
            int lg = 0 ;
            for(j=0;j<PB103_NB;j++){
                lg+=sprintf(pbR->strRes+lg,"%2.2d",values[j]+v0) ;
            }
        }
        is = PB103_NB-2 ;
        while(pondDelta[is] > delta && is){
            delta += pondDelta[is] * Delta[is];
            Delta[is-- ] = 0;
        }
        if(is == 0) {
            deltaS++ ;
            delta = deltaS  ;
            is = 1 ;
        } else {
            Delta[is]++ ; delta -= pondDelta[is] ;
        }
        Delta[PB103_NB-1] = delta ;
    }
    FreeCheckPath(chkP) ;
    FreeAlterPath(AltP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}



// version avec ponderation, mais le parcours se fait en tenant compte
// pour l'ordre de toutes les ponderations sauf la derniere valeur
// Mais on ne teste que si la derniere valeur est un multiple de la ponderation
// Donc economie de test par rapport a la version a)
// Ne sort que les solutions correctes (puisque parcourrues ds l'ordre croissant
// Utilise CheckEqualityPreN
int PB103b(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    
    sum103_t values[PB103_NB] ;
    sum103_t Smin = 32000 ;
    sum103_t deltaMax = 20000 ;
    
    sum103_t pondDelta[PB103_NB] ;
    AlterPaths *AltP =GetAlterPath(PB103_NB/2) ;
    CheckPaths *chkP =GetCheckPath(PB103_NB,AltP,0) ;
 
    Smin = InitPondSum(pondDelta,PB103_NB ,AltP) ;
    sum103_t offsetSum = pondDelta[0]  ;
    
    if(pbR->isVerbose) {
        int i ;
        fprintf(stdout,"\t PB%sb Smin=%d,Dmin=%d Pond=",pbR->ident, Smin+offsetSum,Smin) ;
        for(i=0;i<PB103_NB;i++) { fprintf(stdout,"%d%c",pondDelta[i],(i==PB103_NB-1) ? '\n' : ' '); }
    }


    int j ;
    int is = 1 ;
    int deltaS = 0 ;
    values[0] = 0 ;
    sum103_t delta = deltaS ;
    sum103_t Delta[PB103_NB] ;
    for(j=0;j<PB103_NB-1;j++) Delta[j] = 0;
    Delta[PB103_NB-1] = deltaS ;
    while(deltaS<=deltaMax) {
        if(( Delta[PB103_NB-1] * pondDelta[PB103_NB-1])  == delta ){
            for(is=1;is<PB103_NB;is++) {
                values[is] = values[is-1]+Delta[is]+1 ; /*S+= values[is] ;*/
            }
            sum103_t S = deltaS  ;
            if( S <= Smin &&  CheckEqualityPreN(values,PB103_NB,chkP))  {
                sum103_t v0 = MinCheck(values,PB103_NB) ;
                int j ;
                Smin = S ;
                deltaMax = S ;
                if(pbR->isVerbose) {
                    fprintf(stdout,"\t PB%s S=%d Delta=%d DeltaMax=%d ",pbR->ident,Smin+offsetSum,deltaS,deltaMax ) ;
                    for(j=0;j<PB103_NB;j++){
                        fprintf(stdout,"%d%c",values[j]+v0,(j==PB103_NB-1) ? ' ' : ',' ) ;
                    }
                    for(j=1;j<PB103_NB;j++){
                        fprintf(stdout,"%d%c",values[j]-values[j-1]-1,(j==PB103_NB-1) ? '\n' : '.' ) ;
                    }
                }
                int lg = 0 ;
                for(j=0;j<PB103_NB;j++){
                    lg+=sprintf(pbR->strRes+lg,"%2.2d",values[j]+v0) ;
                }
            }
        }
        is = PB103_NB-2 ;
        while(pondDelta[is] > delta && is){
            delta += pondDelta[is] * Delta[is];
            Delta[is-- ] = 0;
        }
        if(is == 0) {
            deltaS++ ;
            delta = deltaS  ;
            is = 1 ;
        } else {
            Delta[is]++ ; delta -= pondDelta[is] ;
        }
        Delta[PB103_NB-1] = delta / pondDelta[PB103_NB-1] ;
    }
    FreeCheckPath(chkP) ;
    FreeAlterPath(AltP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}
// version avec ponderation, avec parcours en tenant compte
// de l'ordre de toutes les ponderations a l'ordre PB103_NB-1
// Si c'est correct, on teste otus les cas possible en balayant les valeurs
// possible pour le dernier delta.
// Utilise CheckEqualityAlt pour PB103_NB-1,
// CheckEqualityPreN pour PB103_NB (avec isSup=1)


int PB103c(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    sum103_t values[PB103_NB] ;
    sum103_t Smin = 32000 ;
    sum103_t deltaMax = 20000 ;
    
    sum103_t pondDelta[PB103_NB] ;
    AlterPaths *AltP =GetAlterPath(PB103_NB/2) ;
    CheckPaths *chkP =GetCheckPath(PB103_NB,AltP,1) ;

    Smin = InitPondSum(pondDelta,PB103_NB ,AltP) ;
    sum103_t offsetSum = pondDelta[0]  ;
    
    if(pbR->isVerbose) {
        int i ;
        fprintf(stdout,"\t PB%sc Smin=%d,Dmin=%d Pond=",pbR->ident, Smin+offsetSum,Smin) ;
        for(i=0;i<PB103_NB;i++) { fprintf(stdout,"%d%c",pondDelta[i],(i==PB103_NB-1) ? '\n' : ' '); }
    }
    int j, is = 1 ;
    int deltaS = 0 ;
    
    values[0] = 0 ;
    sum103_t delta = deltaS ;
    sum103_t Delta[PB103_NB] ;
    for(j=0;j<PB103_NB-1;j++) Delta[j] = 0;
    Delta[PB103_NB-1] = deltaS ;
    while(deltaS<=deltaMax) {
        if(delta == 0 ){
            for(is=1;is<PB103_NB-1;is++) {
                values[is] = values[is-1]+Delta[is]+1 ;
            }
            if(CheckEqualityAlt(values,PB103_NB-1 ,AltP,0)) {
                values[PB103_NB-1] = values[PB103_NB-2]+1 ;
                sum103_t S = deltaS  ;
                while(S<=Smin) {
                    if(CheckEqualityPreN(values,PB103_NB,chkP))  {
                        sum103_t v0 = MinCheck(values,PB103_NB) ;
                        int j ;
                        Smin = S ;
                        deltaMax = S ;
                        if(pbR->isVerbose) {
                            fprintf(stdout,"\t PB%s S=%d Delta=%d DeltaMax=%d ",pbR->ident,Smin+offsetSum,deltaS,deltaMax ) ;
                            for(j=0;j<PB103_NB;j++){
                                fprintf(stdout,"%d%c",values[j]+v0,(j==PB103_NB-1) ? ' ' : ',' ) ;
                            }
                            for(j=1;j<PB103_NB;j++){
                                fprintf(stdout,"%d%c",values[j]-values[j-1]-1,(j==PB103_NB-1) ? '\n' : '.' ) ;
                            }
                        }
                        int lg = 0 ;
                        for(j=0;j<PB103_NB;j++){
                            lg+=sprintf(pbR->strRes+lg,"%2.2d",values[j]+v0) ;
                        }
                        break ;
                    }
                    values[PB103_NB-1] ++ ; S += pondDelta[PB103_NB-1] ;
                }
            }
        }
        is = PB103_NB-2 ;
        while(pondDelta[is] > delta && is){
            delta += pondDelta[is] * Delta[is];
            Delta[is-- ] = 0;
        }
        if(is == 0) {
            deltaS++ ;
            delta = deltaS  ;
            is = 1 ;
        } else {
            Delta[is]++ ; delta -= pondDelta[is] ;
        }
    }
    FreeCheckPath(chkP) ;
    FreeAlterPath(AltP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}
//
//
// Parcours en incrementant les delta[is) tt que somme pondere inferieur a dmin
// Et l'on ne developpe que si les ordres inferieurs sont valides
// Pas dans l'ordre de dmin croissant, mais a chque nouvelle valeur on tient compte de l'amelioration de dmin
// utilise CheckEqualityAlt pour is =3
// utilise CheckEqualityPreN aves isSup pour les ordres superieurs
//
int PB103d(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    
    sum103_t values[PB103_NB] ;
    sum103_t Smin = 32000 ;
    sum103_t pondDelta[PB103_NB] ;
    AlterPaths *AltP =GetAlterPath(PB103_NB/2) ;
    CheckPaths * chkPS[PB103_NB] ;
    { int i ; for(i=4;i<PB103_NB;i++) chkPS[i] = GetCheckPath(i+1,AltP,1) ; }
  
    Smin = InitPondSum(pondDelta,PB103_NB ,AltP) ;
    sum103_t offsetSum = pondDelta[0]  ;

    if(pbR->isVerbose) {
        int i ;
        fprintf(stdout,"\t PB%s Smin=%d,Dmin=%d Pond=",pbR->ident, Smin+offsetSum,Smin) ;
        for(i=0;i<PB103_NB;i++) { fprintf(stdout,"%d%c",pondDelta[i],(i==PB103_NB-1) ? '\n' : ' '); }
    }
        sum103_t S[PB103_NB] ;
        values[0] = 0 ; values[1] = 1 ; S[1] = 0 ;
        int is = 1 ;
        while(is>0) {
             if(S[is] <= Smin) {
                 if(is <= 3) {
                    if(is== 3 && !CheckEqualityAlt(values,is+1,AltP,0) ) {
                        values[is]++ ; S[is] += pondDelta[is] ;
                        continue ;
                    }
                } else if(is < PB103_NB) {
                    if(!CheckEqualityPreN(values,is+1,chkPS[is]) ) {
                        values[is]++ ; S[is] += pondDelta[is] ; continue;
                    } else if(is==PB103_NB-1) {
                        // new solution
                        sum103_t v0 = MinCheck(values,PB103_NB) ;
                        int j ;
                        Smin = S[is] ;
                        if(pbR->isVerbose) {
                            fprintf(stdout,"\t PB%s S=%d D=%d  ",pbR->ident,Smin+offsetSum,Smin ) ;
                            for(j=0;j<PB103_NB;j++){
                                fprintf(stdout,"%d%c",values[j]+v0,(j==PB103_NB-1) ? ' ' : ',' ) ;
                            }
                            for(j=1;j<PB103_NB;j++){
                                fprintf(stdout,"%d%c",values[j]-values[j-1]-1,(j==PB103_NB-1) ? '\n' : '.' ) ;
                            }
                        }
                        int lg = 0 ;
                        for(j=0;j<PB103_NB;j++){
                            lg+=sprintf(pbR->strRes+lg,"%2.2d",values[j]+v0) ;
                        }
                        values[is]++ ; S[is] += pondDelta[is] ; continue;
                    }
                }
                is++ ;
                S[is] = S[is-1] ;  values[is] = values[is-1] + 1 ;
            } else {
                is-- ;
                values[is]++ ; S[is] += pondDelta[is] ;
            }
        }
    { int i ; for(i=4;i<PB103_NB;i++) FreeCheckPath(chkPS[i]);  }
    FreeAlterPath(AltP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}





int PB103e(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    sum103_t values[PB103_NB] ;
    sum103_t Smin = 32000 ;
    sum103_t pondDelta[PB103_NB] ;
    AlterPaths *AltP =GetAlterPath(PB103_NB/2) ;
    HalfPaths * hlfP[PB103_NB] ;
    { int i ; for(i=4;i<PB103_NB;i++) hlfP[i] = GetHalfPath(i+1) ; }
    Smin = InitPondSum(pondDelta,PB103_NB ,AltP) ;
    sum103_t offsetSum = pondDelta[0]  ;
    //    Smin = 2773 - offsetSum ; // valeur optimum pour 10
    if(pbR->isVerbose) {
        int i ;
        fprintf(stdout,"\t PB%s Smin=%d,Dmin=%d Pond=",pbR->ident, Smin+offsetSum,Smin) ;
        for(i=0;i<PB103_NB;i++) { fprintf(stdout,"%d%c",pondDelta[i],(i==PB103_NB-1) ? '\n' : ' '); }
    }
    sum103_t S[PB103_NB] ;
    sum103_t SRev=0 ;
    values[0] = 0 ; values[1] = 1 ; S[1] = 0 ;
    int is = 1 ;
    while(is>0) {
        if(S[is] <= Smin || SRev <= Smin ) {
            if(is <= 3) {
                if(is== 3 && !CheckEqualityAlt(values,is+1,AltP,0) ) {
                    values[is]++ ; S[is] += pondDelta[is] ;
                    SRev += pondDelta[PB103_NB-is] ;
                    continue ;
                }
            } else if(is < PB103_NB) {
                if(!CheckEqualityPreH(values,hlfP[is]) ) {
                    values[is]++ ; S[is] += pondDelta[is] ;  SRev += pondDelta[PB103_NB-is] ;  continue;
                } else if(is==PB103_NB-1) {
                    int j ;
                    {// new solution
                        sum103_t v0 = MinCheck(values,PB103_NB) ;
                        if(S[is] <= Smin)  {
                            Smin = S[is] ;
                            if(pbR->isVerbose) {
                                fprintf(stdout,"\t PB%s S=%d D=%d  ",pbR->ident,Smin+offsetSum,Smin ) ;
                                for(j=0;j<PB103_NB;j++){
                                    fprintf(stdout,"%d%c",values[j]+v0,(j==PB103_NB-1) ? ' ' : ',' ) ;
                                }
                                for(j=1;j<PB103_NB;j++){
                                    fprintf(stdout,"%d%c",values[j]-values[j-1]-1,(j==PB103_NB-1) ? '\n' : '.' ) ;
                                }
                            }
                        }
                        // on va tester le reverse
                        if(SRev < Smin) {
                            sum103_t vRev[PB103_NB] ;
                            vRev[0]  = 0 ;
                            for(j=1;j<PB103_NB;j++){
                                vRev[j] = vRev[j-1] + values[PB103_NB-j]-values[PB103_NB-j-1] ;
                            }
                            {
                                sum103_t vR0 = MinCheck(vRev,PB103_NB) ;
                                if(CheckEqualityPreH(vRev,hlfP[is])) {
                                    fprintf(stdout,"\t PB%s [R] S=%d D=%d  ",pbR->ident,SRev+offsetSum,SRev ) ;
                                    for(j=0;j<PB103_NB;j++){
                                        fprintf(stdout,"%d%c",vRev[j]+vR0,(j==PB103_NB-1) ? ' ' : ',' ) ;
                                    }
                                    for(j=1;j<PB103_NB;j++){
                                        fprintf(stdout,"%d%c",vRev[j]-vRev[j-1]-1,(j==PB103_NB-1) ? '\n' : '.' ) ;
                                    }
  
                                    Smin = SRev ;
                                }
                            }
                        }

                        int lg = 0 ;
                        for(j=0;j<PB103_NB;j++){
                            lg+=sprintf(pbR->strRes+lg,"%2.2d",values[j]+v0) ;
                        }
                    }
                    values[is]++ ; S[is] += pondDelta[is] ; SRev += pondDelta[PB103_NB-is] ; continue;
                }
            }
            is++ ;
            S[is] = S[is-1] ;  values[is] = values[is-1] + 1 ;
        } else {
            SRev -= (values[is] - values[is-1] - 1) * pondDelta[PB103_NB-is] ;
            is-- ;
            values[is]++ ; S[is] += pondDelta[is] ; SRev += pondDelta[PB103_NB-is] ;
        }
    }
    { int i ; for(i=4;i<PB103_NB;i++) FreeHalfPath(hlfP[i]) ;  }
    FreeAlterPath(AltP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

int Cmp103f(const void *el1,const void *el2) {
    static int size = 0;
    if(el2==NULL) {
        size = ((int *)el1)[0] ;
        return size ;
    }
    return memcmp(el1,el2,size) ;
}

typedef struct DELTA {
    sum103_t val[PB103_NB] ;
    sum103_t deltas[PB103_NB] ;
    sum103_t S ;
    sum103_t Srev ;
    sum103_t vsMin ;
    int     IsMin ;
    int     nbDev ;
} DELTA ;

typedef struct DevD {
    DELTA D ;
    DELTA R ;
    int   isDR ;
    char History[PB103_NB] ;
} DevD ;



void InitLevel(DevD  *DD, int is, sum103_t pondDelta[PB103_NB],sum103_t Smin) {
    static sum103_t MinSumd[PB103_NB+1] ;
    int j ;
    sum103_t Sd = 0 ;
    for(j=0;j<is-1;j++) Sd += DD->D.deltas[j] ;
    if(MinSumd[is] == 0) {
        MinSumd[is] = Sd ;
     }
    if(MinSumd[is+1]) {
        for(j=0;j<is-1;j++) DD->R.deltas[j] = DD->D.deltas[is-2-j] ;
       DD->D.deltas[is-1]  = (MinSumd[is+1] - Sd  > DD->D.deltas[is-2] ) ? MinSumd[is+1] - Sd : DD->D.deltas[is-2] ;
        DD->R.deltas[is-1]  = (MinSumd[is+1] - Sd  > DD->R.deltas[is-2] ) ? MinSumd[is+1] - Sd : DD->R.deltas[is-2] ;
     } else {
        DD->D.deltas[is-1] = DD->D.deltas[is-2] ; // valeur minimum au delta precedent
        for(j=0;j<is-1;j++) DD->R.deltas[j] = DD->D.deltas[is-2-j] ;
        DD->R.deltas[is-1] = DD->R.deltas[is-2] ;
        
    }
    
    int idr ;
    for(idr=0;idr<2;idr++) {
        DELTA * Delt = idr  ? &DD->R : &DD->D ;
        Delt->val[0] = 0;
        Delt->S = Delt->Srev = 0 ;
        Delt->nbDev = 0 ;
        Delt->vsMin = 0 ;
        for(j=1;j<=is;j++) {
            Delt->S += Delt->deltas[j-1] * pondDelta[j] ;
            Delt->Srev += Delt->deltas[j-1] *pondDelta[is+1-j] ;
            Delt->val[j] = Delt->val[j-1] + Delt->deltas[j-1] + 1 ;
        }
    }
    if(is < PB103_NB -1){
        sum103_t delMin = (DD->D.deltas[is-1] <= DD->D.deltas[0]) ? DD->D.deltas[is-1] : DD->D.deltas[0] ;
        sum103_t sumP = 0 ;
        for(j=is+1;j<PB103_NB;j++) {
            sumP += pondDelta[j] ;
        }
        DD->D.S += delMin * sumP ;
        DD->D.Srev += delMin * sumP ;
        
        delMin = (DD->R.deltas[is-1] <= DD->R.deltas[0]) ? DD->R.deltas[is-1] : DD->R.deltas[0] ;
        DD->R.S += delMin * sumP ;
        DD->R.Srev += delMin * sumP ;
    
    }
    DD->isDR= 0 ;
    if(DD->D.S > Smin && DD->D.Srev > Smin) DD->isDR = 1 ;
    return ;
}



int CheckBestSolution(DevD *newDD , DevD *bestSolutions, int nbBest,sum103_t Smin) {
    int j ;
    sum103_t S ;
    sum103_t * deltas ;
    deltas = (newDD->isDR) ? newDD->R.deltas : newDD->D.deltas ;
    if(newDD->isDR) {
        S = (newDD->R.S <= newDD->R.Srev) ? newDD->R.S : newDD->R.Srev ;
    } else {
        S = (newDD->D.S <= newDD->D.Srev) ? newDD->D.S : newDD->D.Srev ;
    }
    if(S < Smin ) {
        nbBest = 0 ;
    }
    for(j=0;j<nbBest;j++) {
        if(memcmp(deltas,bestSolutions[j].D.deltas,(PB103_NB-1)*sizeof(newDD->D.deltas[0]))== 0
           || memcmp(deltas,bestSolutions[j].R.deltas,(PB103_NB-1)*sizeof(newDD->D.deltas[0]) )== 0) {
            return nbBest ;
        }
    }
    bestSolutions[nbBest] = *newDD ;
    DELTA * Dsrc = newDD->isDR  ? &bestSolutions[nbBest].R : &bestSolutions[nbBest].D ;
    DELTA * Ddst = newDD->isDR  ? &bestSolutions[nbBest].D : &bestSolutions[nbBest].R ;

    // oon met le reverse ds deltas
    for(j=0;j<PB103_NB-1;j++) Ddst->deltas[j] =  Dsrc->deltas[PB103_NB-2-j] ;
    Ddst->val[0] = 0 ;
    for(j=1;j<PB103_NB;j++) Ddst->val[j] = Ddst->val[j-1] + Ddst->deltas[j-1] +1 ;
    Ddst->S = Dsrc->Srev ;
    Ddst->Srev = Dsrc->S ;
    return ++nbBest ;
}



int CheckProgression(sum103_t * deltas , DevD *bestSolutions, int nbBest, int is) {
        if(nbBest == 0) return 1;
        if(memcmp(deltas,bestSolutions[0].D.deltas,is*sizeof(deltas[0]))== 0
           || memcmp(deltas,bestSolutions[0].R.deltas,is*sizeof(deltas[0]) )== 0) {
            return 0 ;
        }
        return 1;
 }

#define PB103_MAXBEST   10

#define MAX_DEV 5

// #define PRINT
int PB103f(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    
    sum103_t Smin = 32000 ;
    sum103_t pondDelta[PB103_NB] ;
    AlterPaths *AltP =GetAlterPath(PB103_NB/2) ;
    HalfPaths * hlfP[PB103_NB] ;
    { int i ; for(i=4;i<PB103_NB;i++)
        {
            hlfP[i] = GetHalfPath(i+1) ;
        }
    }
    Smin = InitPondSum(pondDelta,PB103_NB ,AltP) ;
    sum103_t offsetSum = pondDelta[0]  ;
    if(pbR->isVerbose) {
        int i ;
        fprintf(stdout,"\t PB%s Smin=%d,Dmin=%d Pond=",pbR->ident, Smin+offsetSum,Smin) ;
        for(i=0;i<PB103_NB;i++) { fprintf(stdout,"%d%c",pondDelta[i],(i==PB103_NB-1) ? '\n' : ' '); }
    }
    
    DevD DDBest[PB103_MAXBEST] ;
    
    sum103_t deltas0[PB103_NB] = {0,0,1,2} ;
    
    DevD  DD [PB103_NB] ;
    int is,j ;
    int pass ;
     for(pass=0;pass<2;pass++) {
 //       for(pass=0;pass<1;pass++) {
         int nbSol = 0;
        int max_dev = (pass == 0) ? MAX_DEV : 0xffff ;
        is = 4 ;
        for(j=0;j<is-1;j++) {
            DD[is].D.deltas[j] = deltas0[j] ;
        }
        InitLevel(DD+is,is,pondDelta,Smin) ;
        DD[is].D.IsMin = DD[is].R.IsMin = 1 ;
        strcpy(DD[is].History,"I") ;
        printf("****** PASS %d avec MAX_DEV = %d *****\n",pass,max_dev);
        while(is>3) {
            // tritement d'un element
            DELTA * Delt = (DD[is].isDR) ? &DD[is].R : &DD[is].D ;
#if defined(PRINT)
      //      if(pass==1)
            {
                int j ;
                printf("%d%c,%-4d,%-4d,%-3d,%-2d,%-2d ",is,DD[is].isDR ? 'R': 'D',Delt->S,Delt->Srev,Delt->nbDev,DD[is].D.vsMin,DD[is].R.vsMin) ;
                for(j=0;j<is;j++) printf("%d%c",Delt->deltas[j], (j==is-1) ? ' ' : '.')  ;
            }
#endif
            if( Delt->nbDev < max_dev &&  (Delt->S <= Smin || Delt->Srev <= Smin)) {
                if(CheckEqualityPreH(Delt->val,hlfP[is])) {
#if defined(PRINT)
      //              if(pass==1)
                    sum103_t Sd= 0; for(j=0;j<is;j++) Sd += Delt->deltas[j] ;
                        printf("+%d\n",Sd);
#endif
                    if(Delt->vsMin == 0) {
                        Delt->vsMin = Delt->deltas[is-1] ;
                        if(Delt->IsMin) {
                            printf("##%d%c,%-4d,%-4d,%-3d,%-2d,%-2d ",is,DD[is].isDR ? 'R': 'D',Delt->S,Delt->Srev,Delt->nbDev,DD[is].D.vsMin,DD[is].R.vsMin) ;
                            for(j=0;j<is;j++) printf("%d%c",Delt->deltas[j], (j==is-1) ? '\n' : '.')  ;
                        }
                    } else {
                        Delt->IsMin = 0 ;
                    }
                     Delt->nbDev++ ;
                    if(is == PB103_NB -1 ) {
                        int newNbSol = CheckBestSolution(DD+is,DDBest,nbSol,Smin) ;
                        if(Delt->S < Smin || Delt->Srev < Smin || newNbSol > nbSol) {
                            nbSol = newNbSol ;
                            DELTA * DeltBest = (DDBest[nbSol-1].D.S <= DDBest[nbSol-1].R.S) ? &DDBest[nbSol-1].D : &DDBest[nbSol-1].R ;
                            sum103_t v0 = MinCheck(DeltBest->val,PB103_NB) ;
                            printf("%s%c S=%d,%d ",DD[is].History,(DDBest[nbSol-1].D.S <= DDBest[nbSol-1].R.S) ? 'd' : 'r',  DeltBest->S+offsetSum,DeltBest->S) ;
                            for(j=0;j<=is;j++) printf("%d%c",DeltBest->val[j]+v0,(j==is) ? ' ' : ',') ;
                            for(j=1;j<=is;j++) printf("%d%c",DeltBest->deltas[j-1],(j==is) ? '\n' : '.') ;
                            Smin = DeltBest->S ;
                        }
                        if(DD[is].isDR) {
                            is-- ; continue ; // on passe D et R, c'est fini
                        } else {
                            DD[is].isDR = 1 ; continue ; // reste a traiter R
                        }
                    } else {
                        memcpy(DD[is+1].D.deltas,Delt->deltas,is*sizeof(Delt->deltas[0]) ) ;
                        DD[is+1].D.IsMin = DD[is+1].R.IsMin = DD[is].isDR ? DD[is].R.IsMin : DD[is].D.IsMin ;
                        strcpy(DD[is+1].History,DD[is].History) ;
                        DD[is+1].History[is-3] = (DD[is].isDR) ? 'R' : 'D' ;
                        DD[is+1].History[is-2] = 0 ;
                        Delt->val[is]++ ; Delt->deltas[is-1]++ ; Delt->S += pondDelta[is] ; Delt->Srev += pondDelta[1] ;
                        is ++ ; // on passer au niveau superieur
                        InitLevel(DD+is, is, pondDelta,Smin) ;
                        continue ;
                    }
                } else {
#if defined(PRINT)
         //           if(pass==1)
                        printf("\n") ;
#endif
                }
                // on
                Delt->val[is]++ ; Delt->deltas[is-1]++ ; Delt->S += pondDelta[is] ; Delt->Srev += pondDelta[1] ;
                continue ;
            } else {
#if defined(PRINT)
      //          if(pass==1)
                    printf("S\n") ;
#endif
                if(DD[is].isDR) {
                    is-- ; continue ; // on passe D et R, c'est fini
                } else {
                    DD[is].isDR = 1 ; continue ; // reste a traiter R
                }
            }
        }
    }
    
    { int i ; for(i=4;i<PB103_NB;i++) FreeHalfPath(hlfP[i]) ;  }
    FreeAlterPath(AltP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}


typedef struct DELTA_G {
    sum103_t val[PB103_NB] ;
    sum103_t deltas[PB103_NB] ;
    sum103_t S ;
    sum103_t Srev ;
    sum103_t   *ptCand ;
    int     nxtCand ;
    int     nbCand ;
    int     IsMin ;
    int     nbDev ;
} DELTA_G ;

typedef struct DevD_G {
    DELTA_G D ;
    DELTA_G R ;
    int   isDR ;
    char History[PB103_NB] ;
} DevD_G ;

GlobalPaths * glbP[PB103_NB] ;

#define PB103_MAXD  1000

int InitLevelG(DevD_G  *antDD,DevD_G  *DD, int is, sum103_t pondDelta[PB103_NB],sum103_t Smin, int maxCand) {
    int j ;
    DELTA_G * antDelt = (antDD->isDR) ? &antDD->R : &antDD->D ;
    for(j=0;j<is;j++) {
        DD->D.deltas[j] = antDelt->deltas[j] ;
        DD->R.deltas[j] = antDelt->deltas[is-1-j] ;
    }
    int idr ;
    for(idr=0;idr<2;idr++) {
        DELTA_G * Delt = idr  ? &DD->R : &DD->D ;
        Delt->val[0] = 0;
        Delt->S = Delt->Srev = 0 ;
        Delt->nbDev = 0 ;
        Delt->nxtCand = 0 ;
        Delt->IsMin = (antDelt->nxtCand == 0 && antDelt->IsMin ) ? 1 : 0 ;
        for(j=1;j<=is;j++) {
            Delt->S += Delt->deltas[j-1] * pondDelta[j] ;
            Delt->Srev += Delt->deltas[j-1] *pondDelta[is+2-j] ;
            Delt->val[j] = Delt->val[j-1] + Delt->deltas[j-1] + 1 ;
        }
        sum103_t sumP = 0 ;
        for(j=is+1;j<PB103_NB;j++) {
            sumP += pondDelta[j] ;
        }

        int maxDeltaS = (Smin - Delt->S) / sumP + 1 ;
        int maxDeltaSRev = (Smin - Delt->Srev) / (sumP - pondDelta[is+1] + pondDelta[1]) +1 ;

//        int maxDeltaS = (Smin - Delt->S) / pondDelta[is+1] + 1 ;
//        int maxDeltaSRev = (Smin - Delt->Srev) /  pondDelta[1]  +1 ;

        
        if(maxDeltaS < maxDeltaSRev ) maxDeltaS = maxDeltaSRev ;
        if(maxDeltaS > PB103_MAXD / 2) maxDeltaS = PB103_MAXD / 2 ;
        if(maxDeltaS < 0) {
            Delt->nbCand = 0 ;
        } else {
            Delt->nbCand = CheckEqualityPreG(Delt->val,glbP[is+1],Delt->ptCand, maxDeltaS,maxCand) ;
        }
     }
 /*   if(is < PB103_NB -2){
        sum103_t delMin = (DD->D.deltas[is-1] <= DD->D.deltas[0]) ? DD->D.deltas[is-1] : DD->D.deltas[0] ;
        sum103_t sumP = 0 ;
        for(j=is+2;j<PB103_NB;j++) {
            sumP += pondDelta[j] ;
        }
        DD->D.S += delMin * sumP ;
        DD->D.Srev += delMin * sumP ;
        
        delMin = (DD->R.deltas[is-1] <= DD->R.deltas[0]) ? DD->R.deltas[is-1] : DD->R.deltas[0] ;
        DD->R.S += delMin * sumP ;
        DD->R.Srev += delMin * sumP ;
        
    }*/
    if(DD->D.nbCand) {
        DD->isDR= 0 ;
        return DD->D.nbCand ;
    } else if(DD->R.nbCand) {
        DD->isDR = 1 ;
        return DD->R.nbCand ;
    }
    return 0 ;
}

int CheckBestSolutionG(DevD_G *newDD , DevD_G *bestSolutions, int nbBest,sum103_t Smin) {
    int j ;
    sum103_t S ;
    sum103_t * deltas ;
    deltas = (newDD->isDR) ? newDD->R.deltas : newDD->D.deltas ;
    if(newDD->isDR) {
        S = (newDD->R.S <= newDD->R.Srev) ? newDD->R.S : newDD->R.Srev ;
    } else {
        S = (newDD->D.S <= newDD->D.Srev) ? newDD->D.S : newDD->D.Srev ;
    }
    if(S < Smin ) {
        nbBest = 0 ;
    }
    for(j=0;j<nbBest;j++) {
        if(memcmp(deltas,bestSolutions[j].D.deltas,(PB103_NB-1)*sizeof(newDD->D.deltas[0]))== 0
           || memcmp(deltas,bestSolutions[j].R.deltas,(PB103_NB-1)*sizeof(newDD->D.deltas[0]) )== 0) {
            return nbBest ;
        }
    }
    bestSolutions[nbBest] = *newDD ;
    DELTA_G * Dsrc = newDD->isDR  ? &bestSolutions[nbBest].R : &bestSolutions[nbBest].D ;
    DELTA_G * Ddst = newDD->isDR  ? &bestSolutions[nbBest].D : &bestSolutions[nbBest].R ;
    
    // oon met le reverse ds deltas
    for(j=0;j<PB103_NB-1;j++) Ddst->deltas[j] =  Dsrc->deltas[PB103_NB-2-j] ;
    Ddst->val[0] = 0 ;
    for(j=1;j<PB103_NB;j++) Ddst->val[j] = Ddst->val[j-1] + Ddst->deltas[j-1] +1 ;
    Ddst->S = Dsrc->Srev ;
    Ddst->Srev = Dsrc->S ;
    return ++nbBest ;
}



// #define PRINT
int PB103g(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    
    //    sum103_t tmpDelta[50] ;
    sum103_t Smin = 32000 ;
    sum103_t pondDelta[PB103_NB] ;
    AlterPaths *AltP =GetAlterPath(PB103_NB/2) ;
    HalfPaths * hlfP[PB103_NB] ;
    DevD_G  DD [PB103_NB] ;
    {
        int i ; for(i=3;i<PB103_NB;i++){
            hlfP[i] = GetHalfPath(i+1) ;
            glbP[i] = GetGlobalPath(i+1) ;
        }
        for(i=0;i<PB103_NB;i++) {
            DD[i].D.ptCand = malloc(PB103_MAXD*sizeof(DD->D.ptCand[0])) ;
            DD[i].R.ptCand = malloc(PB103_MAXD*sizeof(DD->R.ptCand[0])) ;
        }
    }
    Smin = InitPondSum(pondDelta,PB103_NB ,AltP) ;
    sum103_t offsetSum = pondDelta[0]  ;
    if(pbR->isVerbose) {
        int i ;
        fprintf(stdout,"\t PB%s Smin=%d,Dmin=%d Pond=",pbR->ident, Smin+offsetSum,Smin) ;
        for(i=0;i<PB103_NB;i++) { fprintf(stdout,"%d%c",pondDelta[i],(i==PB103_NB-1) ? '\n' : ' '); }
    }
    
    DevD_G DDBest[PB103_MAXBEST] ;
    
    sum103_t deltas0[PB103_NB] = {0,0,1,2} ;
    
    int is,j ;
    int pass ;
    for(pass=0;pass<2;pass++) {
        //       for(pass=0;pass<1;pass++) {
        int nbSol = 0;
        int max_dev = (pass == 0) ? MAX_DEV : 0xffff ;
        is = 3 ;
        for(j=0;j<is;j++) {
            DD[is].D.deltas[j] = deltas0[j] ;
        }
        DD[is].isDR = 0 ;
        
        InitLevelG(DD+is,DD+is+1,is,pondDelta,Smin,max_dev) ;
        is++ ;
        DD[is].D.IsMin = DD[is].R.IsMin = 1 ;
        strcpy(DD[is].History,"I") ;
        printf("****** PASS %d avec MAX_DEV = %d *****\n",pass,max_dev);
        while(is>3) {
            if(DD[is].D.nxtCand >= DD[is].D.nbCand  && DD[is].R.nxtCand >= DD[is].R.nbCand ) {
                is-- ; continue ; // on a tout epuise
            } else if(DD[is].isDR && DD[is].R.nxtCand >= DD[is].R.nbCand ) {
                DD[is].isDR = 0 ;  // reste a traiter un D de plus
            } else if(!DD[is].isDR && DD[is].D.nxtCand >= DD[is].D.nbCand ) {
                DD[is].isDR = 1 ;
            }
            
            // tritement d'un element
            DELTA_G * Delt = (DD[is].isDR) ? &DD[is].R : &DD[is].D ;

            Delt->deltas[is-1] = Delt->ptCand[Delt->nxtCand] ;
            Delt->val[is] = Delt->val[is-1] + Delt->deltas[is-1] + 1 ;
            Delt->S += Delt->deltas[is-1] * pondDelta[is] ;
            Delt->Srev += Delt->deltas[is-1] * pondDelta[1] ;

#if defined(PRINT)
            //      if(pass==1)
            {
                int j ;
                printf("%d%c,%d/%d,%-4d,%-4d,%-3d ",is,DD[is].isDR ? 'R': 'D',Delt->nxtCand,Delt->nbCand, Delt->S,Delt->Srev,Delt->nbDev) ;
                for(j=0;j<is;j++) printf("%d%c",Delt->deltas[j], (j==is-1) ? ' ' : '.')  ;
            }
#endif
            
            if( Delt->nbDev < max_dev &&  (Delt->S <= Smin || Delt->Srev <= Smin)) {
//                if(Delt->nxtCand < Delt->nbCand)
                {
#if defined(PRINT)
                    //              if(pass==1)
                    sum103_t Sd= 0; for(j=0;j<is;j++) Sd += Delt->deltas[j] ;
                    printf("+%d\n",Sd);
#endif
                    if(Delt->IsMin && Delt->nxtCand == 0) {
                        printf("##%d%c,%-4d,%-4d,%-3d %d/%d ",is,DD[is].isDR ? 'R': 'D',Delt->S,Delt->Srev,Delt->nbDev,Delt->nxtCand,Delt->nbCand) ;
                            for(j=0;j<is;j++) printf("%d%c",Delt->deltas[j], (j==is-1) ? '\n' : '.')  ;
                    }
                    Delt->nbDev++ ;
                    if(is == PB103_NB -1 ) {
/*                        {
                            int j ;
                            printf("%d%c,%d/%d,%-4d,%-4d,%-3d ",is,DD[is].isDR ? 'R': 'D',Delt->nxtCand,Delt->nbCand, Delt->S,Delt->Srev,Delt->nbDev) ;
                            for(j=0;j<is;j++) printf("%d%c",Delt->deltas[j], (j==is-1) ? '\n' : '.')  ;
                        }
*/
                        int newNbSol = CheckBestSolutionG(DD+is,DDBest,nbSol,Smin) ;
                        if(Delt->S < Smin || Delt->Srev < Smin || newNbSol > nbSol) {
                            nbSol = newNbSol ;
                            DELTA_G * DeltBest = (DDBest[nbSol-1].D.S <= DDBest[nbSol-1].R.S) ? &DDBest[nbSol-1].D : &DDBest[nbSol-1].R ;
                            sum103_t v0 = MinCheck(DeltBest->val,PB103_NB) ;
                            printf("%s%c S=%d,%d ",DD[is].History,(DDBest[nbSol-1].D.S <= DDBest[nbSol-1].R.S) ? 'd' : 'r',  DeltBest->S+offsetSum,DeltBest->S) ;
                            for(j=0;j<=is;j++) printf("%d%c",DeltBest->val[j]+v0,(j==is) ? ' ' : ',') ;
                            for(j=1;j<=is;j++) printf("%d%c",DeltBest->deltas[j-1],(j==is) ? '\n' : '.') ;
                            Smin = DeltBest->S ;
                        }

                    } else {
                        strcpy(DD[is+1].History,DD[is].History) ;
                        DD[is+1].History[is-3] = (DD[is].isDR) ? 'R' : 'D' ;
                        DD[is+1].History[is-2] = 0 ;

                        if(InitLevelG(DD+is,DD+is+1, is, pondDelta,Smin, (is == PB103_NB -2) ? 1 : max_dev)) {
                            Delt->S -= Delt->deltas[is-1] * pondDelta[is] ;
                            Delt->Srev -= Delt->deltas[is-1] * pondDelta[1] ;
                            Delt->nxtCand++ ;
                            DD[is].isDR = 1 - DD[is].isDR ;
                            is ++ ; // on passer au niveau superieur
                            continue ;
                        }
                    }
                }
            } else {
#if defined(PRINT)
                printf("S\n") ;
#endif
            }
            Delt->S -= Delt->deltas[is-1] * pondDelta[is] ;
            Delt->Srev -= Delt->deltas[is-1] * pondDelta[1] ;
            Delt->nxtCand++ ;
            DD[is].isDR = 1 - DD[is].isDR ;
        }
    }
    
    { int i ; for(i=4;i<PB103_NB;i++) FreeHalfPath(hlfP[i]) ;  }
    FreeAlterPath(AltP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}



int Cmpint16(const void *el1, const void *el2) {
    return ((int16_t *)el1)[0] - ((int16_t *)el2)[0] ;
}

int PB105(PB_RESULT *pbR) {
    pbR->nbClock = clock()  ;
    int32_t * * tbSub = P105_GetData() ;
    int nt ;
    u_int32_t S =0 ;
    for(nt=0;tbSub[nt] != 0; nt++){
        int32_t * sub = tbSub[nt] ;
        int lg ;
        for(lg=0;sub[lg]!=0;lg++) ;
        heapsort(sub,lg,sizeof(sub[0]),Cmpint16) ;
        if(sub[0] >= MinCheck(sub,lg)  && CheckEquality(sub,lg) ) {
            int i;
            for(i=0;i<lg;i++) S += sub[i] ;
//            printf("%d ",nt);
        }
    }
    sprintf(pbR->strRes,"%d",S) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB106_ASK   12
int PB106(PB_RESULT *pbR) {
    pbR->nbClock = clock()  ;
    AlterPaths * altP = GetAlterPath(PB106_ASK/2) ;
    CheckPaths * chkP = GetCheckPath(PB106_ASK,altP,0) ;
    int S =0;
    int k ;
    for(k=2 ; 2*k <= PB106_ASK ; k++) {
        S += (chkP->nsumK[k-1]-1)*chkP->npermK[k-1];
    }
    sprintf(pbR->strRes,"%d",S) ;
    FreeAlterPath(altP);
    FreeCheckPath(chkP);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

