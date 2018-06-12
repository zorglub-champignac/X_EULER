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
#include "p102_data.h"
#include "p105_data.h"
#include "p107_data.h"
#include "p122_data.h"

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

int PB102(PB_RESULT *pbR) {
    pbR->nbClock = clock()  ;
    
    const Triangle * TR =  P102_GetData() ;
    int i ;
    int nbOK = 0;
    for(i=0;i<PB102_NBT;i++) {
        int32_t A = - TR[i].yb*TR[i].xc +TR[i].yc*TR[i].xb ;
        int32_t B = - TR[i].yc*TR[i].xa +TR[i].ya*TR[i].xc ;
        if(A > 0 && B > 0) {
            int32_t C = - TR[i].ya*TR[i].xb +TR[i].yb*TR[i].xa ;
            if(C > 0) nbOK++ ;
        } else if(A < 0 && B < 0) {
            int32_t C = - TR[i].ya*TR[i].xb +TR[i].yb*TR[i].xa ;
            if(C < 0) nbOK++ ;
        }
    }
    sprintf(pbR->strRes,"%d",nbOK) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

/*
double t = r * log10((1+sqrt(5))/2) + log10(1/sqrt(5))
long L = (long)Math.pow(10, t - (long)t + 8)
*/
static int CmpUint32(const void *el1, const void *el2) {
    return ((int32_t *)el1)[0] - ((int32_t *)el2)[0] ;
}
#define FACT9   362880
int PB104(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int k ;
     u_int32_t PanDigital[FACT9] ;
    u_int8_t perm[9] = {1,2,3,4,5,6,7,8,9} ;
    int is=0 ;
    do {
        PanDigital[is++] = 10*(10*(10*(10*(10*(10*(10*(10*perm[0]+perm[1])+perm[2])+perm[3])+perm[4])+perm[5])+perm[6])+perm[7])+perm[8] ;
    } while (NextPermut(perm,9) >= 0) ;
    u_int32_t F0=1 ;
    u_int32_t F1=1 ;
    k = 2 ;
    while(1) {
        k++ ;
        F0=(F0+F1) % 1000000000 ;
        if((F0 % 9 ) == 0 ) {
           if(bsearch(&F0, PanDigital, FACT9,sizeof(u_int32_t), CmpUint32) != NULL) {
                // approximation by PHY**k / sqrt(5)
                double logFk = k * log10((1+sqrt(5))/2) - log10(sqrt(5)) ;
                u_int32_t highDigits = (u_int32_t)pow(10,logFk - (u_int32_t)logFk +8 ) ;
                if(bsearch(&highDigits, PanDigital, FACT9,sizeof(u_int32_t), CmpUint32) != NULL) {
                    printf(" F(%d) is Double Pandigit\n",k) ;
                    break ;
                }
            }
        }
        u_int32_t tmp = F0 ;
        F0=F1 ;
        F1=tmp ;
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%d",k) ;
    return 1 ;
}




typedef struct Edge {
    u_int16_t   cost;
    u_int16_t   Vbeg ;
    u_int16_t   Vend ;
} Edge ;

typedef struct vertexTree { // tree for a vertex
    u_int16_t numTree ; // tree number.
    u_int16_t nxtVertex ; // link to nxt vertex in the tree
    
} nodeTree ;


int CmpEdge( const void *edg1, const void *edg2) {
    return ((Edge *)edg1)[0].cost - ((Edge *)edg2)[0].cost ;
}
int PB107(PB_RESULT *pbR) {
    pbR->nbClock = clock()  ;
    Edge EDG[PB107_SIZE*PB107_SIZE] ;
    u_int16_t treeLength[PB107_SIZE+1] ;
    nodeTree VtoT[PB107_SIZE] ; // association node -> tree
    u_int16_t nbTree = 0 ;
    int i, j ;
    const u_int16_t * cost = P107_GetData();
    u_int16_t nbEdge = 0 ;
    for(i=0;i<PB107_SIZE;i++) { // get the non null edge
        for(j=i+1;j<PB107_SIZE;j++) {
            if(cost[i*PB107_SIZE+j]){
                EDG[nbEdge].Vbeg = i ;
                EDG[nbEdge].Vend = j ;
                EDG[nbEdge].cost = cost[i*PB107_SIZE+j] ;
                nbEdge++ ;
            }
        }
    }
    treeLength[nbTree++] = 1 ; // special tree with one node
    for(i=0;i<PB107_SIZE;i++) { //each node in is own tree
        VtoT[i].numTree = 0 ; // can be the same tree as this tree is never used
        VtoT[i].nxtVertex = i ;
    }
    heapsort(EDG,nbEdge,sizeof(EDG[0]),CmpEdge) ; // sort edges by cost
    int indSortEdg = 0 ;
    int maxLength = 0 ;
    u_int32_t savingCost = 0 ;
    do {    // loop to build tree , adding the min cost edge
        Edge curEdg = EDG[indSortEdg++] ;
        u_int16_t begT= VtoT[curEdg.Vbeg].numTree ;
        u_int16_t endT =VtoT[curEdg.Vend].numTree ;
        if(begT && endT && begT == endT) {
            savingCost += curEdg.cost ; // the edge address the same tree => loop, forbiddeen
            continue ;
        }
        if(begT == 0 && endT == 0 ) { // nodes not in e tree => simple tree with 2 nodees
                treeLength[nbTree] = 2 ;
                if(maxLength < 2) maxLength = 2 ;
                VtoT[curEdg.Vbeg].numTree = VtoT[curEdg.Vend].numTree = nbTree ;
                VtoT[curEdg.Vbeg].nxtVertex = curEdg.Vend ;
                VtoT[curEdg.Vend].nxtVertex = curEdg.Vbeg ;
            nbTree++ ;
                continue;
        } else { // two trees, rattach the shortest to the longuest
            if(treeLength[begT] > treeLength[endT] ) { // permutation end <->beg
                u_int16_t tmp = endT ;
                endT = begT ;
                begT= tmp ;
                tmp = curEdg.Vbeg ;
                curEdg.Vbeg = curEdg.Vend ;
                curEdg.Vend = tmp ;
            }
          // rattach beg tree to end tree
            u_int16_t nxtEnd = VtoT[curEdg.Vend].nxtVertex ;
            u_int16_t nxtBeg = curEdg.Vbeg ;
            VtoT[curEdg.Vend].nxtVertex = nxtBeg ;
            u_int16_t antBeg ;
            do { // loop on the vertexes of beg tree
                antBeg= nxtBeg ;
                VtoT[nxtBeg].numTree = endT ;
                nxtBeg = VtoT[nxtBeg].nxtVertex ;
            } while(nxtBeg != curEdg.Vbeg) ;
            VtoT[curEdg.Vbeg].numTree = endT ;
            VtoT[antBeg].nxtVertex = nxtEnd ;
            treeLength[endT] += treeLength[begT] ;
            if(maxLength < treeLength[endT]) maxLength = treeLength[endT] ;
            continue ;
        }
    } while(maxLength != PB107_SIZE) ;
    while(indSortEdg<nbEdge) { // add the remaining edge to cost saving
        savingCost += EDG[indSortEdg++].cost ;
    }
    sprintf(pbR->strRes,"%d",savingCost) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

// les solutions sont du type <=> xy = n (x+y)
// n = d1 x d2 x d3 avec  d2 <= d3 et d2^d3 pour assurer l'unicite
// x= d1 x d3 x (d2+d3) ;  y = x= d1 x d2 x (d2+d3)
// Ex n=4
// d1=1 d2=1 d3=4 => x=20 y=5  ;100 = 4 x(20+5)
// d1=2 d2=1 d3=2 => x=12 y=6   ;72 = 2 x (6+12)
// d1=4 d2=1 d3=1 => x=8  y=8  ; 64 = 4 x (8+8)
//
// plus astucieux
// 1/(n+a) + 1/(n+b) = 1/n <=> (n+a)(n+b)=n(2n+a+b) <=>ab = n2
// donc c'est le (nombre de diviseurs de n**2)/2 + 1. 


int PB108(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int d1,d2,d3 ;
    int n ;
    int nbSol = 0 ;
    int nbSolM=0 ;
    for(n=4;;n++) {
        nbSol = 0 ;
        for(d2=1;d2*d2<=n;d2++) {
            int n1 = n / d2 ;
            if(n != d2*n1) continue ;
            int g =PGCD(d2,n1);
            for(d1=g;d1*d2 <= n1 ;d1 += g) {
                d3 = n1/d1 ;
                if(n1 == d1*d3) {
                    if( PGCD(d2,d3) == 1)
                    {
//                           printf("(%d,%d)",d1*d2*(d2 + d3),d1*d3*(d2 + d3)) ;
                        nbSol++ ;
                    }
                }
            }
        }
        if(nbSol > nbSolM) {
            printf("%d=>%d \n",n,nbSol );
            nbSolM = nbSol ;
            if(nbSol > 1000) break ;
        }
    }
 
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%d",n) ;
    return 1 ;
}

#define PB108_ALPHAM    60
#define PB110_MINS      4000000
#define PB108_MINS      1000
int PB110(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int alpha[PB108_ALPHAM+1] ;
    int primes[PB108_ALPHAM] = { 2,3,5,7,11,13,17,19,23,29
                                ,31,37,41,43,47,53,59,61,67,71
                                ,73,79,83,89,97,101,103,107,109,113
                                ,127,131,137,139,149,151,157,163,167,173
                                ,179,181,191,193,197,199,211,223,227,229
                                ,233,239,241,251,257,263,269,271,277,281} ;
    int minS = (strcmp(pbR->ident,"110") == 0) ? PB110_MINS : PB108_MINS ;
    int sumA ;
     u_int64_t nMin = 1L << PB108_ALPHAM ;
    for(sumA=2;(1L<<sumA) <= nMin ;sumA++) {
        //      printf("\nSum(alpha)=%d ",sumA) ;
        int nbA,nbAmax ;
         u_int64_t n=1 ;
        for(nbAmax=0;nbAmax<=sumA;nbAmax++) {
            n *= primes[nbAmax] ;
            if(n> nMin) break ;
        }
        u_int64_t nbS = 1 ;
        for(nbA=1;nbA<nbAmax;nbA++) {
            alpha[nbA] = 1 ;
            nbS *= 3 ;
            if((2*(sumA-nbA)+1)*nbS > 2*minS) break ;
        }
        alpha[--nbA] = 0 ;
        alpha[0] = sumA-nbA+1 ;
        while(1) {
            int j ;
            int ia;
            u_int64_t nbS = 1 ;
            for(ia=0;ia<nbA;ia++) {
                nbS *= 2*alpha[ia]+1 ;
            }
            u_int64_t nbSol = (nbS+1)/2 ;
//            for(ia=0;ia<nbA;ia++) printf("%c%d",(ia==0)? '\n' : '.' ,alpha[ia]) ; printf(" nbS=%lld",nbSol) ;
           if(nbSol > minS ) {
                u_int64_t n=1;
                for(ia=nbA-1;ia>=0;ia--) {
                    for(j=alpha[ia];j>0;j--) {
                        n *= primes[ia] ;
                    }
                    if(n > nMin) break ;
                }
                if(n < nMin) {
                    nMin = n ;
                    if(pbR->isVerbose) {
                        fprintf(stdout,"\t PB%s SumA=%d Alpha=",pbR->ident,sumA);
                        for(ia=0;ia<nbA;ia++) fprintf(stdout,"%c%d",(ia==0)? ' ' : '.' ,alpha[ia]) ;
                        printf(" NbSol=%lld,n=%lld\n",nbSol,nMin) ;
                    }
                }
                break ;
            } else {
                int sumR ;
                for(sumR=1,j=nbA-1;j>=0;j--) {
                    sumR += alpha[j] ; // sumR to dispatch betwwen the remaining alpha's
                    if(sumR <= (nbAmax-j)*(alpha[j]-1)) {
                        sumR -= alpha[j] ;
                        alpha[j]-- ;
                        int k= j+1 ;
                        while(sumR > 0) {
                            alpha[k] = (sumR > alpha[j]) ? alpha[j] : sumR ;
                            sumR -= alpha[k++] ;
                        }
                        nbA = k ;
                        alpha[k] = 0 ;
                        break ;
                    }
                }
                if(j<0)    break ; // no successor
            }
        }
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",nMin) ;
    return 1 ;
}

// le parcours des solutions peut se faire de la façon suivante
// Pour le dernier coup c'est un parmi les 21 doubles (D1..D20 + D25) => 21
// Pour les cas a 2 ou 1 coups : il faut pour le coup supllementaire un aprmi les 62 cas (S1..S20 S25,  D1..D20 +D25 T1..T20) + le cas vide
// => 21 x (62+1)
// Pour les cas a trois coup, pour ne pas doublonner les coups et 2 (Parmi les 62) il suffir de les classer donc 62+61+...+1 = (62x63)/2
// => 21 x 62x63)/2
// donc un total de 21 ( 63+62x63/2) = 21x63x(62+2)/2= 21x63x32.

int cmpVal(const void *e1,const void *e2) {
    return ((int *)e1)[0] - ((int *)e2)[0] ;
}
#define PB109_MAX   100
int PB109(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int nb =0;
    int val[63],D[21];
    val[0] = 0 ;
    int i,j,k, is=1 ;
    for(i=1;i<=20;i++) {
        D[i-1] = 2*i ;
        val[is++]= i;
        val[is++] = 2*i ;
        val[is++] = 3*i ;
    }
    val[is++] = 25 ; val[is++] = 50 ; D[20] = 50 ;
    qsort(val,63,sizeof(val[0]),cmpVal) ;
    for(i=0;i<21;i++) {
        for(j=0;j<63;j++) {
            if(D[i]+val[j] >= PB109_MAX) break ;
            for(k=j;k<63;k++) {
                if(D[i]+val[j]+val[k] < PB109_MAX) nb++ ;
            }
        }
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%d",nb) ;
    return 1 ;
}

#define PB111_MAXP  400000
#define PB111_NBD   10
int PB111(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    CTX_PRIMETABLE * ctxP  ;
    if((ctxP = Gen_tablePrime(PB111_MAXP)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    const T_prime * tbPrime = GetTbPrime(ctxP);
    u_int32_t pow10[PB111_NBD+1] = { 1 } ;
    int nd,k,id;
    pow10[PB111_NBD] = pow10[0] ;
    for(k=1;k<PB111_NBD;k++) {
        pow10[k] = pow10[k-1] * 10 ;
        pow10[PB111_NBD] += pow10[k] ;
    }
    u_int64_t S ;
    int nbDOk = 0 ;
    S = 0 ;
    int nbD[10] = {0,0,0,0,0,0,0,0,0,0} ;
    for(nd=1;nbDOk < 10 ;nd++) {
        u_int8_t noD[PB111_NBD] ;
        u_int8_t valNoD[PB111_NBD] ;
        for(k=0;k<nd;k++) {noD[k]=k ; valNoD[k]= 0 ;}
        do {
            u_int32_t Ndig = pow10[PB111_NBD] ;
            for(k=0;k<nd;k++) {
                Ndig -= pow10[noD[k]] ;
            }
            int is = nd-1 ;
            while (is >=0) {
                u_int64_t NnoDig = 0 ;
                for(k=0;k<nd;k++) NnoDig += (u_int64_t) pow10[noD[k]] * valNoD[k] ;
                {
                    for(id=0;id<10;id++) {
                        if(nbD[id] && nbD[id] < nd) continue ;
                        u_int64_t N = (u_int64_t) Ndig * id + NnoDig ;
                        if(N>pow10[PB111_NBD-1] && Is_Prime(N,tbPrime)) {
                            if(nbD[id] == 0) {
                                nbD[id] = nd ;
                                nbDOk++ ;
                            }
                            S += N ;
                        }
                    }
                }
                while(is >= 0 && valNoD[is] == 9) {
                    valNoD[is]= 0 ; is-- ;
                }
                if(is >= 0) { valNoD[is]++ ; is = nd-1 ; }
            }
        } while(NextSub(noD,nd,PB111_NBD)>=0 );
    }
    Free_tablePrime(ctxP);
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",S) ;
    return 1 ;
}


// the searched proportion is PB112_PERCENT / (PB112_PERCENT + 1)
// can be 999999999 for a proportion af 1/one billion no bouncy
#define PB112_PERCENT   99
#define PB112_MAXDIG    21
// #define PB112_DEBUG
// struct to count the different number categories
// for increasing and decreasing number differentiation of the numbers
// by the leading digit.
// Also, the "constant numbers" are not included in incr[] and decr[]
// to avoid multiples counts.
// Leading '0' is only counted for recursion.
typedef struct CountB {
    int64_t Incr[10] ;  // increasing by leading digit (constant excluded)
    int64_t Decr[10] ;  // increasing by leading digit (constant excluded)
    int64_t Bouncy0 ;   // Bouncy with '0' as leading digit
    int64_t BouncyN0 ;  // Bouncy with no '0' as leading digit
    int64_t Const ;     // Constant numbers (same digit)
// cumulative counters for recursion
    int64_t SumI ;
    int64_t SumD ;
    int64_t SumB ;
    int64_t SumC ;
} CountB ;

// info is only a comment
// nbDigJoker is only a commodity to print the last digits
void CountB_print(CountB *CB,char *info, int nbDigJoker) {
    int64_t SI = CB->SumI;
    int64_t SD = CB->SumD ;
    int64_t SB = CB->SumB ;
    int64_t SC = CB->SumC ;
    int64_t ST = SI+SD+SB -SC ; // verification
    char * joker=".................9" ;
#if defined(PB112_DEBUG)
    int j ;
    int64_t I = CB->Const  ; for(j=1;j<10;j++) I += CB->Incr[j] ;
    int64_t D = CB->Const  ; for(j=1;j<10;j++) D += CB->Decr[j] ;
    int64_t B = CB->BouncyN0 ;
    int64_t T = I+D+B - CB->Const ;
    printf(" %s%s => %6lld I +%6lld D + %10lld B T=%10lld Cum: %6lld C %6lld I +%6lld D +%10lld B T=%10lld Perc=%.6lf\n"
           ,info,joker+17-nbDigJoker,I,D,B,T,SC,SI,SD,SB,ST, ((double)100.0*SB)/ST) ;
#else
    printf(" %s%s =>  %6lld C %6lld I +%6lld D +%10lld B T=%10lld Perc=%.8lf\n"
           ,info,joker+17-nbDigJoker,SC,SI,SD,SB,ST, (((double)100.0)*SB)/ST);
#endif
}

// initialise newC with oldC
// reset the current counters, tansmit cumulatives counters
void CountB_Init(CountB *oldC, CountB *newC) {
    newC->SumI = oldC->SumI ;
    newC->SumD = oldC->SumD ;
    newC->SumB = oldC->SumB ;
    newC->SumC = oldC->SumC ;
    memset(newC->Incr,0,sizeof(newC->Incr));
    memset(newC->Decr,0,sizeof(newC->Decr));
    newC->Bouncy0 = newC->BouncyN0 = newC->Const = 0 ;
}

#define BIT_INC 1   // increasing
#define BIT_DEC 2   // decreasing
#define BIT_CONST 4 // increasing and decreasing

// add leading digits chain to statistics
//
void CountB_AddHead(char *digits, CountB *oldC, CountB *newC) {
    // a chain can be increasing and decreasing (constant chain)
    // check the status (increasing, decreasing of the leading digits chain)
    int status = BIT_INC | BIT_DEC | BIT_CONST ;
    int idH = digits[0] - '0' ; // leading digit of the added digit chain
    int idT = idH ;             // last digit of the chain, to compute butting
    int is ;
    for(is=0;digits[is] != 0; is++) {
        idT = digits[is] - '0' ;
        if(is > 0) {
            if(digits[is] > digits[is-1]) status &= ~(BIT_DEC|BIT_CONST) ;
            else if(digits[is] < digits[is-1]) status &= ~(BIT_INC|BIT_CONST) ;
        }
    }
    int ida; // leader digit of the precedent stats
    // deltaBouncy will be attributed to Boncy0 or BouncyN0 depending on idH (leading digt of the added chain)
    int64_t deltaBouncy = oldC->Bouncy0 + oldC->BouncyN0 ;
    if(oldC->Const) {
        for(ida=0;ida<10;ida++) {
            if(ida < idT) {  // T > A
                if(status & BIT_DEC) newC->Decr[idH] += oldC->Decr[ida] + 1;
                else  deltaBouncy += oldC->Decr[ida] + 1 ;
                deltaBouncy += oldC->Incr[ida] ;
            }else if (ida > idT) {  // T < A
                if(status & BIT_INC)  newC->Incr[idH] += oldC->Incr[ida]  + 1 ;
                else deltaBouncy += oldC->Incr[ida] + 1 ;
                deltaBouncy += oldC->Decr[ida] ;
            } else { // T == A
                if(status & BIT_DEC) newC->Decr[idH] += oldC->Decr[ida] + ((status & BIT_CONST) ? 0 : 1)  ;
                else deltaBouncy += oldC->Decr[ida]   ;
                if(status & BIT_INC) newC->Incr[idH] += oldC->Incr[ida]  + ((status & BIT_CONST) ? 0 : 1) ;
                else  deltaBouncy += oldC->Incr[ida]   ;
                if(! (status & (BIT_INC|BIT_DEC)))  deltaBouncy++ ;
            }
        }
    } else { // special case for one digit numbers  to avoid problems with constant numbers
        if(status & BIT_DEC) newC->Decr[idH] += oldC->Decr[idT] + ((status & BIT_CONST) ? 0 : 1)  ;
        else deltaBouncy += oldC->Decr[idT]   ;
        if(status & BIT_INC) newC->Incr[idH] += oldC->Incr[idT]  + ((status & BIT_CONST) ? 0 : 1) ;
        else  deltaBouncy += oldC->Incr[idT]   ;
        if(! (status & (BIT_INC|BIT_DEC)))  deltaBouncy++ ;
    }
    if(idH==0) newC->Bouncy0 += deltaBouncy ;
    else { // accumulation
        newC->BouncyN0 += deltaBouncy ;
        newC->SumB += deltaBouncy ;
        newC->SumI += newC->Incr[idH] + ((status & BIT_CONST) ? 1  : 0) ;
        newC->SumD += newC->Decr[idH] + ((status & BIT_CONST) ? 1  : 0) ;
        if(status & BIT_CONST) {
            newC->Const++ ;
            newC->SumC++ ;
        }
    }
}


int PB112(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    CountB CB[PB112_MAXDIG+1] ; // to store statistic for numbers  10**(nd-1) : 9, 99, 999, 9999,
    char prefix[PB112_MAXDIG+1] ;
    int id=1,nd,j ;
    int64_t n ;
    CountB CBwork ; // 2 counts so when depassing the purpose, rolling back one step
    CountB CBnext ;
    memset(&CB[0],0,sizeof(CB[0])) ;
    CountB_Init(CB,&CBwork) ;
    // search the power of 10 reaching a superior proportion of bouncy
    for(nd=1;nd<PB112_MAXDIG-1;nd++) {
         CountB_AddHead("0",CB+nd-1,&CBwork) ; // no print, only for recursion
         CBnext=CBwork ;
         for(id=1;id<10;id++) { // add the
            sprintf(prefix,"%d",id) ;
            CountB_AddHead(prefix,CB+nd-1,&CBnext) ;
            if(PB112_PERCENT * (CBnext.SumI+CBnext.SumD-CBnext.SumC) <= CBnext.SumB) break ;
            CountB_print(&CBnext,prefix,nd-2) ;
            CBwork = CBnext ; // valid power, next turn.
        }
        printf("\n");
        if(id != 10) {  break ; // upper bound reached
        } else {
            CB[nd] = CBwork ;
            CountB_Init(CB+nd,&CBwork) ;
        }
     }
    n = id ; // leading digit for the search, CBwork contains statistic for numbers <= [id]9999999
    while(--nd > 0) { // loop to search lower digits
        n *=10 ;
         for(j=0;j<10;j++) { // search the digit (upper bound)
             int isLess = 0 ;
            sprintf(prefix,"%lld",n+j) ;
             CountB_Init(&CBwork,&CBnext) ;
             CountB_AddHead(prefix, CB+nd-1,&CBnext) ;
             if(PB112_PERCENT * (CBnext.SumI+CBnext.SumD-CBnext.SumC) < CBnext.SumB) {
                 isLess = 1; // upper bound found, => next lower dgit
             }
             printf(" n %c ",isLess ? '<' : ' ') ; CountB_print(&CBnext,prefix,nd-2) ;
             if(isLess) break ;
             CBwork = CBnext ; // valid digit, next turn.
        }
        n += j ; // add the valid digit
    }
    n = CBwork.SumI+CBwork.SumD + CBwork.SumB - CBwork.SumC ;
    int64_t nbBouncy = CBwork.SumB ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s Under n=%lld exactly only %lld are not bouncy numbers\n",pbR->ident,n,n-nbBouncy) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",n) ;
    return 1 ;
}


#define PB113_NBDIG    100
// #define PB113_DEBUG
// struct to count the different number categories
// for increasing and decreasing number differentiation of the numbers
// by the leading digit.
// Also, the "constant numbers" are not included in incr[] and decr[]
// to avoid multiples counts.
// Leading '0' is only counted for recursion.
typedef struct CountNoB {
    int64_t Incr[10] ;  // increasing by leading digit (constant excluded)
    int64_t Decr[10] ;  // increasing by leading digit (constant excluded)
    int64_t Const ;     // Constant numbers (same digit)
    // cumulative counters for recursion
    int64_t SumI ;
    int64_t SumD ;
    int64_t SumC ;
} CountNoB ;

// info is only a comment
void CountNB_print(CountNoB *CNB,char *info) {
    int64_t SI = CNB->SumI;
    int64_t SD = CNB->SumD ;
    int64_t SC = CNB->SumC ;
 #if defined(PB113_DEBUG)
    int j ;
    int64_t I = CNB->Const  ; for(j=1;j<10;j++) I += CNB->Incr[j] ;
    int64_t D = CNB->Const  ; for(j=1;j<10;j++) D += CNB->Decr[j] ;
    int64_t C = CNB->Const ;
    
    printf(" %s =>  %6lld C %6lld I +%6lld D T=%6lld Cum: %6lld C %6lld I +%6lld D  NB=%10lld\n"
           ,info,C,I,D,I+D-C,SC,SI,SD,SI+SD-SC) ;
#else
    printf(" %s =>  %6lld C %6lld I +%6lld D\n"
           ,info,SC,SI,SD);
#endif
}

// initialise newC with oldC
// reset the current counters, tansmit cumulatives counters
void CountNB_Init(CountNoB *oldC, CountNoB *newC) {
    newC->SumI = oldC->SumI ;
    newC->SumD = oldC->SumD ;
    newC->SumC = oldC->SumC ;
    memset(newC->Incr,0,sizeof(newC->Incr));
    memset(newC->Decr,0,sizeof(newC->Decr));
    newC->Const = 0 ;
}

#define BIT_INC 1   // increasing
#define BIT_DEC 2   // decreasing
#define BIT_CONST 4 // increasing and decreasing

// add leading digits chain to statistics
//
void CountNB_AddHead(char *digits, CountNoB *oldC, CountNoB *newC) {
    // a chain can be increasing and decreasing (constant chain)
    // check the status (increasing, decreasing of the leading digits chain)
    int status = BIT_INC | BIT_DEC | BIT_CONST ;
    int idH = digits[0] - '0' ; // leading digit of the added digit chain
    int idT = idH ;             // last digit of the chain, to compute butting
    int is ;
    for(is=0;digits[is] != 0; is++) {
        idT = digits[is] - '0' ;
        if(is > 0) {
            if(digits[is] > digits[is-1]) status &= ~(BIT_DEC|BIT_CONST) ;
            else if(digits[is] < digits[is-1]) status &= ~(BIT_INC|BIT_CONST) ;
        }
    }
    int ida; // leader digit of the precedent stats
    if(oldC->Const) {
        for(ida=0;ida<10;ida++) {
            if(ida < idT) {  // T > A
                if(status & BIT_DEC) newC->Decr[idH] += oldC->Decr[ida] + 1;
            }else if (ida > idT) {  // T < A
                if(status & BIT_INC)  newC->Incr[idH] += oldC->Incr[ida]  + 1 ;
            } else { // T == A
                if(status & BIT_DEC) newC->Decr[idH] += oldC->Decr[ida] + ((status & BIT_CONST) ? 0 : 1)  ;
                if(status & BIT_INC) newC->Incr[idH] += oldC->Incr[ida]  + ((status & BIT_CONST) ? 0 : 1) ;
            }
        }
    } else { // special case for one digit numbers  to avoid problems with constant numbers
        if(status & BIT_DEC) newC->Decr[idH] += oldC->Decr[idT] + ((status & BIT_CONST) ? 0 : 1)  ;
        if(status & BIT_INC) newC->Incr[idH] += oldC->Incr[idT]  + ((status & BIT_CONST) ? 0 : 1) ;
    }
    if(idH!=0) { // accumulation
        newC->SumI += newC->Incr[idH] + ((status & BIT_CONST) ? 1  : 0) ;
        newC->SumD += newC->Decr[idH] + ((status & BIT_CONST) ? 1  : 0) ;
        if(status & BIT_CONST) {
            newC->Const++ ;
            newC->SumC++ ;
        }
    }
}


int PB113(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    char prefix[10] ;
    int id=1,nd ;
    CountNoB CNBwork ; // 2 counts so when depassing the purpose, rolling back one step
    CountNoB CNBnext ;
    memset(&CNBwork,0,sizeof(CNBwork)) ;
    CountNB_Init(&CNBwork,&CNBnext) ;
    // search the power of 10 reaching a superior proportion of bouncy
    for(nd=1;nd<=PB113_NBDIG;nd++) {
        for(id=0;id<10;id++) { // add the
            sprintf(prefix,"%d",id) ;
            CountNB_AddHead(prefix,&CNBwork,&CNBnext) ;
        }
#if defined(PB113_DEBUG)
        char info[30] ;
        sprintf(info,"under 10**%d",nd) ;
        CountNB_print(&CNBnext,info) ;
#endif
        
        CNBwork = CNBnext ;
        CountNB_Init(&CNBwork,&CNBnext) ;
    }
    nd-- ;
    int64_t noBouncy = CNBwork.SumI+CNBwork.SumD-CNBwork.SumC ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s Under 10**%d not bouncy=%lld (+I=%lld +D=%lld -C=%lld)\n",
                               pbR->ident,nd,noBouncy,CNBwork.SumI,CNBwork.SumD,CNBwork.SumC) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",noBouncy) ;
    return 1 ;
}


#define PB114_LEN   50
typedef struct State114 {
    u_int64_t B ;
    u_int64_t R1 ;
    u_int64_t R2 ;
    u_int64_t R3 ;
} State114;


int PB114(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    State114 St[PB114_LEN+1] ;
    // init
    St[0].B = 1 ;
    St[0].R1 = St[0].R2= St[0].R3 = 0 ;
    int nt;
    for(nt=1;nt<PB114_LEN+1 ;nt++ ) {
        St[nt].B = St[nt-1].B + St[nt-1].R3 ;
        St[nt].R2 = St[nt-1].R1 ;
        St[nt].R3 = St[nt-1].R3 + St[nt-1].R2 ;
        St[nt].R1 = St[nt-1].B ;
    }
    nt-- ;
    u_int64_t nbStates = St[nt].B+St[nt].R3 ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s NnState=%lld=(%lld-B %lld %lld %lld-R3)\n",
                               pbR->ident, nbStates,St[nt].B,   St[nt].R1,St[nt].R2,St[nt].R3) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",nbStates) ;
    return 1 ;
}

/* On consider la matrice M (matrice de transition)
 1 0 0 1
 1 0 0 0
 0 1 0 0
 0 0 1 1
 
                                 1
                                 0
                                 0
 Le resultat est (1 0 0 1 ) M**n 0
 
 le produit final est pour ne retenir que les B er R3
 
 */

#define PB114_SIZE  4
#define PB114_MAXPOW2    6

#define I2(i,j)   ((i)*PB114_SIZE+(j))
#define I3(ip,i,j)   ((ip)*PB114_SIZE*PB114_SIZE+(i)*PB114_SIZE+(j))

int PB114a(PB_RESULT *pbR) {
    int i,j,k,ip ;
    u_int64_t M[PB114_SIZE*PB114_SIZE] ;
    u_int64_t Mpow2[PB114_MAXPOW2*PB114_SIZE*PB114_SIZE] ;
    u_int64_t V[PB114_SIZE] , antV[PB114_SIZE] ;
    memset(M,0,sizeof(M)) ;
    memset(V,0,sizeof(V)) ;
    V[0] = 1 ;
    M[I2(0,0)] = 1 ; M[I2(0,PB114_SIZE-1)] = 1 ;
    M[I2(PB114_SIZE-1,PB114_SIZE-1)] = 1 ;
    for(i=1;i<PB114_SIZE;i++)M[I2(i,i-1)] = 1 ;
    memcpy(Mpow2,M,sizeof(M)) ;
    for(ip=1;ip<PB114_MAXPOW2;ip++) {
        for(i=0;i<PB114_SIZE;i++) {
            for(j=0;j<PB114_SIZE;j++) {
                u_int64_t S =0 ;
                for(k=0;k<PB114_SIZE;k++) {
                    S += Mpow2[I3(ip-1,i,k)] * Mpow2[I3(ip-1,k,j)] ;
                }
                Mpow2[I3(ip,i,j)] = S ;
            }
        }
    }
    for(ip=0;ip<PB114_MAXPOW2;ip++) {
        if((1<<ip) & PB114_LEN) {
            // puissance presente, on mutliplie V par M**(1<<ip))
            memcpy(antV,V,sizeof(V)) ;
            for(i=0;i<PB114_SIZE;i++) {
                u_int64_t S =0 ;
                for(k=0;k<PB114_SIZE;k++) {
                    S += Mpow2[I3(ip,i,k)] * antV[k] ;
                }
                V[i] = S ;
            }
            
        }
    }
    u_int64_t nbStates = V[0]+V[PB114_SIZE-1] ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s NnState=%lld=(%lld-B  %lld-R3)\n",
                               pbR->ident, nbStates,V[0],V[PB114_SIZE-1]) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",nbStates) ;
    return 1 ;
    
    
}


#define PB115_SIZE  51
#define PB115_MAX   1000000
int PB115(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    u_int64_t St0[PB115_SIZE],St1[PB115_SIZE] ;
    u_int64_t * antSt, *curSt ;
    // init
    memset(St0,0,sizeof(St0)) ;
    St0[0] =  1 ;
    antSt = St0 ;
    curSt = St1 ;
    int nt;
    for(nt=1;;nt++) {
        int i ;
        curSt[0] = antSt[0] + antSt[PB115_SIZE-1] ;
        for(i=1;i<PB115_SIZE-1;i++) {
            curSt[i] = antSt[i-1] ;
        }
        curSt[PB115_SIZE-1] = antSt[PB115_SIZE-2] + antSt[PB115_SIZE-1] ;
        u_int64_t nbStates = curSt[0] + curSt[PB115_SIZE-1] ;
        if(nbStates > PB115_MAX) break ;
        u_int64_t *tmp = antSt ;
        antSt = curSt ;
        curSt = tmp ;
     }
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s n=%d\n",
                               pbR->ident, nt) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%d",nt) ;
    return 1 ;
}


typedef struct State116 {
    u_int64_t B ;
    u_int64_t T1 ;
    u_int64_t T2 ;
    u_int64_t T3 ;
    u_int64_t T4 ;
} State116;


#define PB116_LEN 50
int PB116(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    State116 StR0 ,StR1 , *ptRcur = &StR0 , *ptRant = &StR1 ;
    State116 StG0 ,StG1 , *ptGcur = &StG0  , *ptGant = &StG1 ;
    State116 StB0 ,StB1 , *ptBcur = &StB0 , *ptBant = &StB1 ;
    // init
    StR0.B = 1 ;
    StR0.T1 = StR0.T2 = 0 ;
    StG0.B = 1 ;
    StG0.T1 = StG0.T2 = StG0.T3 = 0 ;
    StB0.B = 1 ;
    StB0.T1 = StB0.T2 = StB0.T3 = StB0.T4 = 0 ;
    
    int nt;
    for(nt=0;nt<PB116_LEN ;nt++ ) {
        State116 * tmp  = ptRant ;
        ptRant = ptRcur ;
        ptRcur = tmp ;
  
        tmp  = ptGant ;
        ptGant = ptGcur ;
        ptGcur = tmp ;

        tmp  = ptBant ;
        ptBant = ptBcur ;
        ptBcur = tmp ;

        
        ptRcur->B = ptRant->B + ptRant->T2 ;
        ptRcur->T1 = ptRant->B + ptRant->T2 ;
        ptRcur->T2 = ptRant->T1;

        ptGcur->B = ptGant->B + ptGant->T3 ;
        ptGcur->T1 = ptGant->B + ptGant->T3 ;
        ptGcur->T2 = ptGant->T1;
        ptGcur->T3 = ptGant->T2;

        ptBcur->B = ptBant->B + ptBant->T4 ;
        ptBcur->T1 = ptBant->B + ptBant->T4 ;
        ptBcur->T2 = ptBant->T1;
        ptBcur->T3 = ptBant->T2;
        ptBcur->T4 = ptBant->T3;
    }
    u_int64_t nbStates = ptRcur->B  + ptRcur->T2 + ptGcur->B  + ptGcur->T3 + ptBcur->B  + ptBcur->T4 -3 ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s NnState=%lld R=%lld G=%lld B=%lld\n",
                               pbR->ident, nbStates,ptRcur->B  + ptRcur->T2 -1 ,ptGcur->B  + ptGcur->T3 -1 , ptBcur->B  + ptBcur->T4 -1) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",nbStates) ;
    return 1 ;
}


typedef struct State117 {
    u_int64_t B ;
    u_int64_t R1 ;
    u_int64_t R2 ;
    u_int64_t G1 ;
    u_int64_t G2 ;
    u_int64_t G3 ;
    u_int64_t B1 ;
    u_int64_t B2 ;
    u_int64_t B3 ;
    u_int64_t B4 ;
} State117;


#define PB117_LEN 50
int PB117(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    State117 St0 ,St1 , *ptCur = &St0 , *ptAnt = &St1 ;
    // init
    memset(&St0,0,sizeof(St0)) ;
    St0.B = 1 ;
    
    int nt;
    for(nt=0;nt<PB117_LEN ;nt++ ) {
        State117 * tmp  = ptAnt ;
        ptAnt = ptCur ;
        ptCur = tmp ;
       
        
        ptCur->B = ptAnt->B + ptAnt->R2 + ptAnt->G3 + ptAnt->B4 ;
        ptCur->R1 = ptCur->G1 = ptCur->B1 = ptAnt->B + ptAnt->R2 + ptAnt->G3 + ptAnt->B4 ;
        ptCur->R2 = ptAnt->R1;
        ptCur->G2 = ptAnt->G1;
        ptCur->G3 = ptAnt->G2;
        ptCur->B2 = ptAnt->B1;
        ptCur->B3 = ptAnt->B2;
        ptCur->B4 = ptAnt->B3;
    }
    u_int64_t nbStates = ptCur->B  + ptCur->R2 + ptCur->G3 + ptCur->B4  ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s NnState=%lld \n",
                               pbR->ident, nbStates) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%lld",nbStates) ;
    return 1 ;
}
#define PB118_MAXDPRIME 50000
#define PB118_MAXP  100000000
typedef struct DPrime {
    u_int32_t P ;       // value
    u_int32_t mask ;    // binary mask for used digits
} DPrime ;

int PB118(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;

    CTX_PRIMETABLE * ctxP  ;
    if((ctxP = Gen_tablePrime(Sqrt32(PB118_MAXP))) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    const T_prime * tbPrime = GetTbPrime(ctxP);

    DPrime *tbDigPrime = malloc(PB118_MAXDPRIME*sizeof(tbDigPrime[0])) ;
    Decomp * DeC = DecompAlloc(9) ;
    int nbSub ;
    int indDPrime[10] ;
    int nbDPrime[10] ;
    int id = 1 ;
    int ip = 0 ;
    // primes with one digit
    indDPrime[id] = ip ;
    tbDigPrime[ip].P = 2 ;
    tbDigPrime[ip++].mask = 1<<(2-1) ;
    tbDigPrime[ip].P = 3 ;
    tbDigPrime[ip++].mask = 1<<(3-1) ;
    tbDigPrime[ip].P = 5 ;
    tbDigPrime[ip++].mask = 1<<(5-1) ;
    tbDigPrime[ip].P = 7 ;
    tbDigPrime[ip++].mask = 1<<(7-1) ;
    nbDPrime[id] = ip - indDPrime[id] ;
    for(id=2;id<8;id++) {
        // primes with (id+1) different digits
        u_int8_t sub[9] ;
        int k ;
        indDPrime[id] = ip ;
        for(k=0;k<9;k++) sub[k] = k ;
        do {
            if(sub[id-1] & 1) continue ; // avoid even number
            int N=0 ;
            for(k=0;k<id;k++) N = 10*N + sub[k] + 1 ;
            if(Is_Prime32(N,tbPrime)) {
                tbDigPrime[ip].P = N ;
                int mask = 0 ;
                for(k=0;k<id;k++) mask |= 1 << sub[k] ;
                tbDigPrime[ip++].mask = mask  ;
            }
        } while (NextArrangement(sub,id,9) >= 0) ;
        nbDPrime[id] = ip - indDPrime[id] ;
    }
    { // case 8 digits (id=7) : imposes to the excluded digit to be prime (so a solution)
        u_int8_t sub[9] ;
        int k ;
        indDPrime[id] = ip ;
        for(k=0;k<8;k++) sub[k] = k ;
        do {
            int ipExclus = 1 ; // minus one, 2,3,5,7
            while(ipExclus) {
                int N=0 ;
                for(k=0;k<id;k++) {
                    N = 10*N + ((sub[k] == ipExclus) ? 9 :  (sub[k] + 1) )  ;
                }
                if(Is_Prime32(N,tbPrime)) {
                    tbDigPrime[ip].P = N ;
                    int mask = 0 ;
                    for(k=0;k<id;k++) mask |= 1 << ((sub[k] == ipExclus) ? 8 :  (sub[k]) )  ;
                    tbDigPrime[ip++].mask = mask  ;
                }
                if(ipExclus == 1) ipExclus = 2;
                else if (ipExclus == 2) ipExclus = 4;
                else if (ipExclus == 4) ipExclus = 6 ;
                else ipExclus = 0 ;
            }
        } while (NextPermut(sub,8) >= 0) ;
        nbDPrime[id] = ip - indDPrime[id] ;
        id++ ;
    }
    
 
    //no solution as permutation of nine digits is a multiple of 3
    indDPrime[id] = ip ;
    nbDPrime[id] = 0 ;
    nbSub = nbDPrime[8] ; // as 8 digits are solution
    if(pbR->isVerbose) {
            printf("\t PB%s +%-5d form:8.1\n",pbR->ident,nbSub) ;
    }
    do { // loop on decomposition of 9 as a sum (number of digits of eeach number)
        int j ;
        int indFree[9] ;
        indFree[0] = 0 ;
        int is = 0 ;
        int mask = 0 ;
        int oldNbSub = nbSub ;
        if(DeC->val[0] > 7) continue ; // 9 and 8 digits
        while(is>=0) { // loop to fill the decomposition (tree exploration)
            if(indFree[is] < nbDPrime[DeC->val[is]]) {
                if(mask & tbDigPrime[indDPrime[DeC->val[is]]+indFree[is]].mask ) {
                    indFree[is]++ ; continue ; // mask conflit ?
                }
                mask |= tbDigPrime[indDPrime[DeC->val[is]]+indFree[is]].mask ;
                indFree[is]++ ;
                if(is == DeC->nbVal - 1) {
                    nbSub++ ; // remove last digit in the mask
                    mask ^= tbDigPrime[indDPrime[DeC->val[is]]+indFree[is]-1].mask ;
                } else {
                    is++ ;
                    if(DeC->val[is] == DeC->val[is-1]) { // same length, must be different
                        indFree[is] = indFree[is-1] ;
                    } else {
                        indFree[is] = 0 ;
                    }
                }
            } else { // no more candidate
                indFree[is] = 0 ;
                is-- ;
                if(is>=0) mask ^= tbDigPrime[indDPrime[DeC->val[is]]+indFree[is]-1].mask ;
            }
        }
        if(pbR->isVerbose) {
            if(nbSub > oldNbSub) {
                printf("\t PB%s +%-5d form:",pbR->ident,nbSub-oldNbSub) ;
                for(j=0;j<DeC->nbVal;j++) printf("%d%c",DeC->val[j],(j== DeC->nbVal -1) ? '\n' : '.');
            }
        }
    } while(DecompNext(DeC) >= 0) ;
    DecompFree(DeC) ;
    Free_tablePrime(ctxP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%d",nbSub) ;
    return 1 ;
}

#define PB119_MAX   30
#define PB119_NBD_MAX 20
int PB119(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int nd,nb = 0 ;
    u_int64_t s ;
    u_int64_t Pow[PB119_NBD_MAX*9];
    u_int64_t Pow10 ;
    u_int64_t Pfind[2*PB119_MAX] ;
    for(s=2;s<PB119_NBD_MAX*9;s++){
        Pow[s] = s*s ;
    }
    int antNb = 0 ;
    for(nd=3,Pow10=1000;nb<PB119_MAX;nd++ , Pow10 *= 10) {
        antNb = nb ;
        for(s=2;s<=9*PB119_NBD_MAX;s++){
            u_int64_t pow  ;
            for(pow=Pow[s];pow < Pow10 ; pow *= s){
                u_int64_t p1 = pow  ;
                int s1 = 0 , np =0 ;
                while(p1) {
                    s1 += p1 % 10 ;
                    np++ ;
                    if(s1 > s) break ;
                    p1 /= 10 ;
                }
                if(s1 == s) {
                    Pfind[++nb] = pow  ;
                    if(pbR->isVerbose) fprintf(stdout,"\t PB%s %lld\t=\t%lld**%d\n",pbR->ident,pow,s,np) ;
                }
            }
            Pow[s] = pow ;
            
        }
    }
    while(antNb < PB119_MAX) {
        int in ;
        antNb++ ;
        for(in=antNb+1; in<=nb;in++) {
            if(Pfind[antNb] > Pfind[in]) {
                u_int64_t tmp = Pfind[antNb] ;
                Pfind[antNb] = Pfind[in] ;
                Pfind[in] = tmp ;
            }
        }
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s The %dth is %lld\n",pbR->ident,antNb,Pfind[antNb]) ;
    sprintf(pbR->strRes,"%lld",Pfind[antNb]) ;
    return 1 ;
}


int PB120(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int SumR = 0 ;

    pbR->nbClock = clock() - pbR->nbClock ;
    
    SumR += 2*3 ;
    int n4 ;
    for(n4=4;n4<1000;n4 += 4) {
        SumR += n4 *(n4-2) + (n4+1)*(n4) + (n4+2)*(n4) + (n4+3)*(n4+2) ;
    }
    SumR += n4 * (n4-2) ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s Sum of rest=%d\n",pbR->ident,SumR) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    sprintf(pbR->strRes,"%d",SumR) ;
    return 1 ;
}

#define PB121_NBTURN    15
// On calcule par recurrence Pn(x) = Sigma(P(k,n) x**k ) avec P(k,n) = nb cas avec k bleu
// on a la recurrence Pn(x) = Pn-1(x) * (x+n)
// le nombre total  de cas est Factoriel(n+1)
// Il restec ensuite a sommer les cas favorables.
int PB121(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    
    u_int64_t Cf[PB121_NBTURN+1] ;
    u_int64_t fact = 2;
    Cf[0] = 1 ;
    Cf[1] = 1 ;
    int n,k ;
    // Pn(x) = Pn-1(x) * (x+n)
    for(n=2;n<=PB121_NBTURN;n++) {
        fact *= n+1 ;
        u_int64_t ck = Cf[0] ;
        Cf[0] = ck * n ;
        Cf[n]= 0 ;
        for(k=1;k<=n;k++) {
            u_int64_t tmp = ck + n * Cf[k] ;
            ck = Cf[k] ;
            Cf[k] = tmp ;
        }
    }
    u_int64_t sumCkMin = 0 ;
    for(k=PB121_NBTURN;k>PB121_NBTURN/2;k--) {
        sumCkMin += Cf[k] ;
    }
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s Sum=%lld (%lld/%lld)\n",pbR->ident,fact/sumCkMin,fact,sumCkMin) ;
    sprintf(pbR->strRes,"%d",(int) (fact/sumCkMin)) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB122_MAX   100000
// #define PB122_MAX   15000
#define PB122_NBCHAIN   2000000000
#define PB122_CHK 1
#define PB122_MAXLEVEL  64
typedef struct Chain122 {
    u_int32_t   n ;
    u_int32_t   antCh ;
} Chain122 ;
int PB122(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int SumR = 0 ;
    
    Chain122 * tbCH = malloc(sizeof(tbCH[0])*PB122_NBCHAIN) ;
    u_int8_t MinM[PB122_MAX+1] ;
    int nxtCH = 0 ;
    int antCH = 0 ;
    int nbMulMax = 1 ;
    int nbFind = 0 ;
    memset(MinM,0,sizeof(MinM)) ;
    while((1<<nbMulMax) <= PB122_MAX) nbMulMax++ ;
    nbMulMax = 2 * (nbMulMax - 1 ) ; // decomposition binaire (calculer les puissances, les sommer)
    MinM[2] = 1 ;
    nbFind = 2 ;
    int nm ;
    tbCH[nxtCH].n = 1 ;
    tbCH[nxtCH].antCh = nxtCH ;
    nxtCH++ ;
    for(nm=1;nbFind < PB122_MAX;nm++) {
        int k ;
        int curCH = nxtCH ;
        for(k=antCH;k<curCH;k++) {
            Chain122 endCH = tbCH[k] ;
            int  iCh = k ;
            while(1) {
                 int n = endCH.n + tbCH[iCh].n ;
                if ((n <= PB122_MAX) && (MinM[n] == 0 || MinM[n] >= nm)) {
 //                   printf("(%d->%d)",n,endCH.n) ;
                    tbCH[nxtCH].n = n ;
                    tbCH[nxtCH++].antCh = k ;
                    if(MinM[n] == 0) {
                        MinM[n] = nm ;
                        nbFind++ ;
                    }
                    if(iCh==0 && endCH.antCh > 0) {
                        tbCH[nxtCH].n = endCH.n - tbCH[endCH.antCh].n +1 ;
                        tbCH[nxtCH++].antCh =  tbCH[endCH.antCh].antCh ;
                        tbCH[nxtCH].n = n ;
                        tbCH[nxtCH].antCh = nxtCH-1 ; nxtCH++ ;
                    }
                }
                if(iCh) {
                    iCh = tbCH[iCh].antCh ;
                } else {
                    break ;
                }
            } ;
        }
        printf("%d,%d ",nxtCH,nbFind) ;
        antCH = curCH ;
    }
    int i,sumM = 0 ;
    for(i=1;i<=PB122_MAX;i++) sumM +=  MinM[i] ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s Sum([1 %d]=%d, %d nodes Max=%d\n",pbR->ident,PB122_MAX,sumM,nxtCH,nm) ;
    sprintf(pbR->strRes,"%d",sumM) ;
    free(tbCH);
    pbR->nbClock = clock() - pbR->nbClock ;
#if PB122_CHK
    {
        const int32_t * MinRef = P122_GetData() ;
        int nbErr = 0 ;
        int iMax = ( PB122_MAX < P122_REFLG ) ? PB122_MAX : P122_REFLG ;
        for(i=1;i<=iMax;i++) {
            if(MinM[i] != MinRef[i]) {
                nbErr ++;
                printf("([%d]=%d exp=%d)\n",i,MinM[i],MinRef[i]);
            }
                
        }
        printf("CHECK again http://oeis.org/A003313/b003313.txt %d errors\n",nbErr);
    }
#endif
    
    return 1 ;
}


int PB122a(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    Chain122 tbCH[30] ;
    int isDeltaOne[30] ;
    u_int8_t MinM[PB122_MAX+1] ;
    int nbMulMax = 1 ;
    int nbFind = 0 ;
    int iOut= 0 ;
 //   memset(MinM,0,sizeof(MinM)) ;
    int i ;
    for(i=0;i<PB122_MAX+1;i++) MinM[i] = 0 ;
    while((1<<nbMulMax) <= PB122_MAX) nbMulMax++ ;
    nbMulMax += 2 ;
    //    nbMulMax = 2 * (nbMulMax - 1 )-1 ; // decomposition binaire (calculer les puissances, les sommer)
    MinM[1] = 0 ;
    MinM[2] = 1 ;
    nbFind = 2 ;
    for(;nbFind<PB122_MAX;nbMulMax++) {
        int is = 1 ;
        tbCH[is].n = 1 ;
        tbCH[is].antCh = 1 ;
        while(is > 0) {
           if(is>=nbMulMax) { is-- ; continue ;}
           if(tbCH[is].antCh < 1) {
               is-- ; continue;
           }
          int n = tbCH[is].n + tbCH[tbCH[is].antCh].n ;
//          iOut++ ; printf("%d(%d)=%d+%d%c",n,is,tbCH[is].n,tbCH[tbCH[is].antCh].n,(iOut & 7) ? ' ' : '\n') ;
          tbCH[is].antCh-- ;
            if(n <= PB122_MAX) {
               if(MinM[n] == 0 || MinM[n] >= is ) {
                    if(MinM[n] == 0) {
                        nbFind++ ;
                        if(nbFind >= PB122_MAX) {
                            MinM[n] = is ;
                            break ;
                        }
                    }
                    MinM[n] = is ;
                    if(n < PB122_MAX){
                        tbCH[++is].n = n ;
                        tbCH[is].antCh = is ;
                    }
                }
            }
        }
        printf("%d->%d ",nbMulMax,nbFind);
    }
    int sumM = 0 ;
    for(i=1;i<=PB122_MAX;i++) sumM +=  MinM[i] ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s Sum([1 %d]=%d \n",pbR->ident,PB122_MAX,sumM) ;
    sprintf(pbR->strRes,"%d",sumM) ;
    pbR->nbClock = clock() - pbR->nbClock ;
#if PB122_CHK
    {
        const int32_t * MinRef = P122_GetData() ;
        int nbErr = 0 ;
        int iMax = ( PB122_MAX < P122_REFLG ) ? PB122_MAX : P122_REFLG ;
        for(i=1;i<=iMax;i++) {
            if(MinM[i] != MinRef[i]) {
                nbErr ++;
                printf("([%d]=%d exp=%d)\n",i,MinM[i],MinRef[i]);
            }
            
        }
        printf("CHECK again http://oeis.org/A003313/b003313.txt %d errors\n",nbErr);
    }
#endif
    return 1 ;
}

typedef struct Chain122b {
    u_int32_t   n ;
    u_int32_t   n2 ;
    u_int32_t   antCh1 ;
    u_int32_t   antCh2 ;
} Chain122b ;

int PB122b(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    Chain122b tbCH[30] ;
    u_int8_t MinM[PB122_MAX+1] ;
    int nbMulMax = 1 ;
    int nbFind = 0 ;
    int iOut= 0 ;
    //   memset(MinM,0,sizeof(MinM)) ;
    int i ;
    for(i=0;i<PB122_MAX+1;i++) MinM[i] = 0 ;
    while((1<<nbMulMax) <= PB122_MAX) nbMulMax++ ;
    nbMulMax += 1 ;
    //    nbMulMax = 2 * (nbMulMax - 1 )-1 ; // decomposition binaire (calculer les puissances, les sommer)
    MinM[1] = 0 ;
    MinM[2] = 1 ;
    nbFind = 2 ;
    
    MinM[1] = 0 ;
    MinM[2] = 1 ;
    nbFind = 2 ;
    for(;nbFind<PB122_MAX;nbMulMax++) {
        int is = 1 ;
        tbCH[is].n = 1 ;
        tbCH[is].antCh1 = 1 ;
        while(is > 0) {
            if(is>=nbMulMax) { is-- ; continue ;}
            if(tbCH[is].antCh1 < 1) {
                is-- ; continue;
            }
            int n = tbCH[is].n + tbCH[tbCH[is].antCh1].n ;
            //          iOut++ ; printf("%d(%d)=%d+%d%c",n,is,tbCH[is].n,tbCH[tbCH[is].antCh].n,(iOut & 7) ? ' ' : '\n') ;
            tbCH[is].antCh1-- ;
            if(n <= PB122_MAX) {
                if(MinM[n] == 0 || MinM[n] >= is ) {
                    if(MinM[n] == 0) {
                        nbFind++ ;
                        if(nbFind >= PB122_MAX) {
                            MinM[n] = is ;
                            break ;
                        }
                    }
                    MinM[n] = is ;
                    if(n < PB122_MAX){
                        tbCH[++is].n = n ;
                        tbCH[is].antCh1 = is ;
                    }
                }
            }
        }
        printf("%d->%d ",nbMulMax,nbFind);
    }

    
    
    
    nbMulMax -=2 ;
    
//    for(;nbFind<PB122_MAX;nbMulMax++)
    {
        tbCH[0].n = 0 ;
        tbCH[0].antCh1 = tbCH[0].antCh2 = 0 ;
        int is = 1 ;
        tbCH[is].n = 1 ;
        tbCH[is].antCh1 = is ;
        tbCH[is].antCh2 = 0 ;
        tbCH[is].n2 = tbCH[is].n ;
        while(is > 0) {
            if(is>=nbMulMax) { is-- ; continue ;}
            if(tbCH[is].antCh1 < tbCH[is].antCh2 + 1) {
                if(tbCH[is].antCh2 < is-1 ) {
//                if(tbCH[is].antCh2 < is-1 && tbCH[is].antCh2 < 2 ) {
//                if(tbCH[is].antCh2 < is-1 && tbCH[is].antCh2 < 100 ) {
                    tbCH[is].antCh1 = is ;
                    tbCH[is].antCh2++ ;
                    tbCH[is].n2 = tbCH[is].n + tbCH[tbCH[is].antCh2].n  ;
                } else {
                    is-- ; continue;
                }
            }
//            int n = tbCH[is].n+tbCH[tbCH[is].antCh1].n + tbCH[tbCH[is].antCh2].n ;
            int n = tbCH[is].n2 + tbCH[tbCH[is].antCh1].n  ;
//            iOut++ ; if(n<=PB122_MAX)printf("%d(%d)=%d+%d%c",n,is,tbCH[tbCH[is].antCh1].n,tbCH[tbCH[is].antCh2].n,(iOut & 7) ? ' ' : '\n') ;
            tbCH[is].antCh1-- ;
            if(n <= PB122_MAX) {
                if((MinM[n] >= is && tbCH[is].antCh2 == 0) || MinM[n] >= is+1  ) {
                    if(tbCH[is].antCh2 == 0) {
                        if(is < MinM[n]) {
                            MinM[n] = is ;
                            printf("\n%d(%d)->",n,is);
                            int j ;
                            for(j=is;j>0;j--) printf("%d ",tbCH[j].n) ;
                        }
                    } else {
                        if(is+1 < MinM[n]) {
                            MinM[n] = is+1 ;
                            printf("\n%d(%d)->*%d %d ",n,is+1,tbCH[tbCH[is].antCh1+1].n, tbCH[tbCH[is].antCh2].n );
                            int j ;
                            for(j=is;j>0;j--) printf("%d ",tbCH[j].n) ;
                        }
                    }
                    if(n < PB122_MAX){
                        if(tbCH[is].antCh2 == 0) {
                            tbCH[++is].n = n ;
                            tbCH[is].antCh1 = is ;
                            tbCH[is].antCh2 = 0 ;
                            tbCH[is].n2 = n ;
                        } else {
                            tbCH[is+1].n = tbCH[tbCH[is].antCh1+1].n + tbCH[tbCH[is].antCh2].n ;
                            tbCH[is+1].antCh1 = is ;
                            tbCH[is+1].antCh2 = is ;
                            is++ ;
                            
                            tbCH[++is].n = n ;
                            tbCH[is].antCh1 = is ;
                            tbCH[is].n2 = n ;
                            tbCH[is].antCh2 = 0 ;
                        }
                    }

                }
            }
        }
        printf("%d->%d ",nbMulMax,nbFind);
    }
    int sumM = 0 ;
    for(i=1;i<=PB122_MAX;i++) sumM +=  MinM[i] ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s Sum([1 %d]=%d \n",pbR->ident,PB122_MAX,sumM) ;
    sprintf(pbR->strRes,"%d",sumM) ;
    pbR->nbClock = clock() - pbR->nbClock ;
#if PB122_CHK
    {
        const int32_t * MinRef = P122_GetData() ;
        int nbErr = 0 ;
        int iMax = ( PB122_MAX < P122_REFLG ) ? PB122_MAX : P122_REFLG ;
        for(i=1;i<=iMax;i++) {
            if(MinM[i] != MinRef[i]) {
                nbErr ++;
                printf("([%d]=%d exp=%d)\n",i,MinM[i],MinRef[i]);
            }
            
        }
        printf("CHECK again http://oeis.org/A003313/b003313.txt %d errors\n",nbErr);
    }
#endif
    return 1 ;
}


