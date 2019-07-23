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

#include "euler_utils.h"
#include "faray_utils.h"
#include "PB151_200.h"
typedef struct PB151_STATE {
   int ns[4] ;
   int next[4] ;
   int size ;
   int nbSheet ;
} PB151_STATE ;

int PB151(PB_RESULT *pbR) {
   pbR->nbClock = clock() ;
   PB151_STATE st[270] ;
   int n5,n4,n3,n2 ;
   for(n5=0;n5<9;n5++) {
      for(n4=0;n4<5;n4++) {
         for(n3=0;n3<3;n3++) {
            for(n2=0;n2<2;n2++) {
               int size = n5+2*n4+4*n3+8*n2 ;
               if(size > 15 || size < 1 ) continue ;
               int index = n5+9*(n4+5*(n3+3*n2)) ;
               st[index].size = size ;
               st[index].ns[0] = n2 ; st[index].ns[1] = n3 ; st[index].ns[2] = n4 ; st[index].ns[3] = n5 ;
               st[index].nbSheet = n2+n3+n4+n5 ;
               int nbNext = 0 ;
               st[index].next[nbNext++] =  (n2 ==0) ? 0 : index - 135 + 45 + 9 + 1; // n2-- n3++ n4++ n5++
               st[index].next[nbNext++] =  (n3 ==0) ? 0 : index - 45  + 9 + 1; // n3-- n4++ n5++
               st[index].next[nbNext++] =  (n4 ==0) ? 0 : index -9 + 1; // n4-- n5++
               st[index].next[nbNext++] =  (n5 ==0) ? 0 : index -1 ; // n5--
           }
         }
      }
   }
   uint64_t prob[270] ;
   memset(prob, 0, 270 * sizeof(prob[0]));
   int index0 = 1+9*(1+5*(1+3));
   prob[index0]= 1;
   int n ;
   double alpha = 0 ;
   for(n=0;n<13;n++) {
      int size = 15 - n ;
      int i ;
      uint64_t probDen = 1;
      int denFactor = 0 ;
      for(i=0;i<270;i++) {
         if(st[i].size == size && prob[i] > 0)  denFactor |= 1 << st[i].nbSheet ;
      }
      for(i=8;i>1;i--) {
         if(denFactor & ( 1<< i)) probDen *= i / PGCD64(probDen,i) ;
      }
      for(i=0;i<270;i++) {
         if(st[i].size == size && prob[i] > 0)  {
            int is ;
            uint64_t den = prob[i] * probDen / st[i].nbSheet ;
            for(is=0;is<4;is++) {
               if(st[i].next[is]) {
                  prob[st[i].next[is]] +=  den * st[i].ns[is] ;
               }
            }
         }
      }
      uint64_t p = 0 ;
      for(i=0;i<270;i++) {
         if(st[i].size == size-1 && prob[i] > 0) {
            if (p==0) p = prob[i] ;
            else p = PGCD64(p,prob[i]);
            if(p==1) break ;
         }
      }
 //     printf("%d->%d : ",n,probDen) ;
      uint64_t sum = 0 , sum1 = 0 ;
      for(i=0;i<270;i++) {
         if(st[i].size == size-1 && prob[i]>0) {
            if(p>1)prob[i] /= p ;
 //           printf("P(%d.%d.%d.%d:%d)=%lld ",st[i].ns[0],st[i].ns[1],st[i].ns[2],st[i].ns[3],st[i].nbSheet, prob[i]);
            if(st[i].nbSheet==1) sum1 += prob[i] ;
            sum += prob[i] ;
         }
      }
      if(sum1) alpha += (double)sum1 / sum ;
 //     printf(" Sum%lld\n",sum);
   }
   
   
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%.6f",alpha) ;
   return 1 ;
}

typedef struct PB151a_STATE {
   int na[4] ; // number of sheet A2,A3,A4,A5
   int nbSheet ;  // total number of sheets
   uint64_t prob ;   // proba of the state
} PB151a_STATE ;

// number range for sheet
#define n2R   2
#define n3R   3
#define n4R   5
#define n5R   9

#define allR   (n2R*n3R*n4R*n5R)


int PB151a(PB_RESULT *pbR) {
   pbR->nbClock = clock() ;
   PB151a_STATE st[allR] ;
   int stateToRg[allR] ;
   int rgLevel[16] ;
   uint64_t sum[16] ;
   memset(stateToRg,0,allR*sizeof(stateToRg[0]));
   int ns = 0 ; //
   rgLevel[0] = ns ; // initialize first state A2=A3=A4=A5=1
   sum[0] = 1 ;
   st[ns].na[0] =  st[ns].na[1] =  st[ns].na[2] = st[ns].na[3] = 1 ;
   st[ns].nbSheet = st[ns].na[0]+st[ns].na[1]+st[ns].na[2]+st[ns].na[3] ;
   st[ns].prob = 1 ;
   ns++ ;
   rgLevel[1] = ns ;
   int nl,i ;
   uint64_t alphaN=0 ; // proba for one sheet alphaN/alphaD
   uint64_t alphaD=1 ;
   for(nl=0;nl<14;nl++) { // loop far a week
      int no,nn ;
      int denFactor = 0 ; // bit mask for number of sheets 1,2,3,4,5,6,7,8
       for(no=rgLevel[nl];no<rgLevel[nl+1];no++) {
          denFactor |= 1 << st[no].nbSheet ; // add bit
      }
     uint64_t probDen = 1 ; // product of active number of sheets
      for(i=8;i>1;i--) { // so  prob contribution to Ax  => probDen / nbSheet * nb(Ax)
         if(denFactor & ( 1<< i)) probDen *= i ;
      }
     for(no=rgLevel[nl];no<rgLevel[nl+1];no++) { // developpement for active states
         uint64_t den = st[no].prob * probDen / st[no].nbSheet ; // common factor
         int index = st[no].na[3]+n5R*(st[no].na[2]+n4R*(st[no].na[1]+n3R*st[no].na[0]));
         int ia ;
         for(ia=0;ia<4;ia++) { // loop on Ax
            if(st[no].na[ia]==0) continue ;
            int idNext ; // compute next hash(exact) index
            if(ia==3)  idNext = index - 1 ;
            else if(ia==2) idNext = index - n5R +1 ;
            else if(ia==1) idNext = index - n5R*n4R + n5R +1 ;
            else idNext = index - n5R*n4R*n3R + n5R*n4R + n5R +1 ;
            int isn =stateToRg[idNext] ;
            if(isn==0) { // newstate
               int ib ;
               isn = ns++ ; stateToRg[idNext] = isn ;
               memcpy(st[isn].na,st[no].na,4*sizeof(st[0].na[0])) ;
               st[isn].na[ia]-- ; for(ib=ia+1;ib<4;ib++) st[isn].na[ib]++ ;
               st[isn].nbSheet =st[no].nbSheet+2-ia ;
               st[isn].prob =0 ;
            }
            st[isn].prob  +=  den *  st[no].na[ia] ; // add contribution
         }
      }
      rgLevel[nl+2] = ns ;
      uint64_t p = 0 ; // search pgcd of active probas
      for(nn=rgLevel[nl+1];nn<rgLevel[nl+2];nn++) {
         if (p==0) p = st[nn].prob ;
         else p = PGCD64(p,st[nn].prob);
         if(p==1) break ;
      }
      sum[nl+1] = 0 ;
      uint64_t sum1 = 0 ;
      for(nn=rgLevel[nl+1];nn<rgLevel[nl+2];nn++) {
         if(p>1)st[nn].prob /= p ; // remove pgcd to keep smaller probas
//         printf("P(%d.%d.%d.%d:%d)=%lld ",st[nn].na[0],st[nn].na[1],st[nn].na[2],st[nn].na[3],st[nn].nbSheet,st[nn].prob );
         if(st[nn].nbSheet==1) sum1 += st[nn].prob ; // one sheet ?
         sum[nl+1] += st[nn].prob ;
      }
      if(nl<13 && sum1) { // add proba for one sheet
         uint64_t pgcdDS = PGCD64(alphaD,sum[nl+1]) ;
         alphaN = alphaN * (sum[nl+1]/pgcdDS) + sum1 * (alphaD/pgcdDS) ;
         alphaD *= (sum[nl+1]/pgcdDS) ;
       }
   }
   if(pbR->isVerbose)fprintf(stdout,"\t%s P=%lld/%lld=%.6f\n",pbR->ident,alphaN,alphaD, (double)alphaN/alphaD);
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%.6f",(double)alphaN/alphaD) ;
   return 1 ;
}



#define PB152_ASK 80

int PB152c_n(PB_RESULT *pbR,int max_n) ;
int PB152c(PB_RESULT *pbR) {
   return PB152c_n(pbR,PB152_ASK) ;
}



#define PB152_MAXN 320

typedef int64_t    sumType ;
typedef int32_t     constraintType ;
typedef int         indexType ;



#if !defined(INT128)
   typedef struct countType {
      u_int64_t   h ;
      u_int64_t l ;
   } countType ;
#define addCount(dst,src)   if( ( (dst).l = (dst).l + (src).l ) >= (src).l ) (dst).h += (src).h ; else (dst).h += (src).h + 1
#define int2count(low)   ((countType){0,(low)})
   #define l_count(c)       ((c).l)
   #define h_count(c)       ((c).h)
   #define isCountNull(c)     ((c).l==0  && (c).h==0 )
#  else
   typedef __int128   countType ;
   #define addCount(dst,src)  (dst) += (src)
   #define int2count(low)   ((countType)(low))
   #define l_count(c)       ((u_int64_t)(c))
   #define h_count(c)       ((int64_t)((c)>>64))
   #define isCountNull(c)     ((c)==0)
#endif
// ppcm*ppcm/k**2
typedef struct Element152_c {
    int     val ;           // k
    sumType weight ;        // contribution to 1/2 pondered by ppcm
} Element152_c ;


typedef struct hist152 {
    sumType     sum ;   // sigma(1/k**2) pondered
    countType count ;   // times sum has been observed
} hist152 ;

// comparison on sum
int CmpHisto(const void *el1,const void *el2){
    hist152 * h1 = (hist152 *)el1 ;
    hist152 * h2 = (hist152 *)el2 ;
    if(h1->sum > h2->sum) {
        return 1 ;
    } else if (h1->sum < h2->sum) {
        return -1 ;
    } else return 0 ;
}

// compress array of histos by merging histo with same sum (add counts)
int CompressHisto(hist152 *Histo, int nbHisto) {
    int nbHistoMerge = 0 ;
    qsort(Histo,nbHisto,sizeof(Histo[0]),CmpHisto);
    int ih,j;
    hist152 *sortHisto = Histo ;
    for(ih=0,nbHistoMerge=0;ih<nbHisto;) {
        sumType sum = Histo[ih].sum ;
        sortHisto[nbHistoMerge] = Histo[ih] ;
        for(j=ih+1;j<nbHisto && Histo[j].sum == sum;j++) {
 //          sortHisto[nbHistoMerge].count += Histo[j].count ;
            addCount(sortHisto[nbHistoMerge].count , Histo[j].count) ;
        }
        ih = j ;
        nbHistoMerge++ ;
    }
    return nbHistoMerge ;
}

// Power of p (prime) admissible for a development
// np design the max n  with p**n admissible
typedef struct Powp152 {
    constraintType p ;  // prime p
    constraintType powp ;   // p**n
    constraintType ppcmPowp ; // p**(np+1-n) usefull to cumulate ppcm
} Powp152 ;

// list of Powp for the maxValue of k
typedef struct ListPowp {
   int      max_n ;     // max value for ceofficient
    Powp152 Powp[100];
    int     nbPowp ;    // number of Powp
    int64_t den_ppcm ;  // ppcm of the whole powp (not square)
    int curPowp ;       // current powp to treat.
} listPowp ;

// for development for the set of elements(coefficients) corresponding to the same Powp
// As the sum must comply to modulus, the sum is decomposed in quotient (intSum) and remainder (modSum)
typedef struct sumLevel {
    sumType        intSum ;
    constraintType modSum ;
} sumLevel ;



// results of the development for  Powp
typedef struct LevelDev {
    indexType           nbSum ;     // number of resulting sums
    constraintType      constraint ;    // constraint (modulus = p*p)
    indexType           nbDiffMod ;     // number of different remainder
    sumLevel *          sumL ;      // array of resulting sums
    indexType *         indMod ;    // for each possible remainder index in sumL for the first
                                    // sumL with this remainder.
    sumType             maxSum ;    // max intSum obtain for the development
} LevelDev ;

// current context of development
typedef struct levelState {
    int         numLevel ;  // index of treated powp (with nb elements > 0)
    Element152_c  Elem[50] ;  // coefficients for current powp
    int         nbElem ;    // number of coefficients
    constraintType  constraint ;    // current constraint for the level (= p*p)
    int64_t     ppcm ;  // ppcm of elements for the level
    Powp152     PP ;    // current Powp
    int32_t     fact ;  // coefficient factor to convert sum from the precedent level (must apply fact*fact)
    int     computVal[PB152_MAXN] ; // history of k precedently used (to filter coefficients and use only once)
    int     nbComputVal ;   // number of k precedently used
    int64_t cumPpcm ;       // ppcm of precedent level ppcm's. (used to compute ppcm for the current level)
} levelState ;

// comparison of sumL, first by remainder modSum, secondly by increasing quotient
int CmpSumL(const void *el1,const void *el2){
    sumLevel * sumL1 = (sumLevel *)el1 ;
    sumLevel * sumL2 = (sumLevel *)el2 ;
    if(sumL1->modSum > sumL2->modSum) {
        return 1 ;
    } else if (sumL1->modSum < sumL2->modSum) {
        return -1 ;
    } else if (sumL1->intSum > sumL2->intSum) {
        return 1 ;
    } else if (sumL1->intSum < sumL2->intSum) {
        return -1 ;
    } else {
        return 0 ;
    }
}

LevelDev * Free_levelDev(LevelDev * Lv) {
    if(Lv) {
        free(Lv->sumL) ;
        free(Lv->indMod) ;
        free(Lv) ;
    }
    return NULL ;
}
// Compute all the partial sums for a powp.
// For nbElem (coefficients) => 2**nbElem partial sums.
// The sum are sorted and indexed by remainder (mod[constraint]) for faster access.
LevelDev * Cmp52_level (const Element152_c * Elem, int nbElem, int32_t modConstraint) {
    indexType nbSum = 1 << nbElem ;
    LevelDev * Lv = calloc(1,sizeof(Lv[0])) ;
    Lv->nbSum = nbSum ;
    Lv->constraint = modConstraint ;
    Lv->sumL = malloc((nbSum+1) * sizeof(Lv->sumL[0])) ;
    Lv->indMod = calloc(modConstraint,sizeof(Lv->indMod[0])) ;
    int i ;
    Lv->sumL[0].intSum = 0 ;
    Lv->sumL[0].modSum = 0 ;
    sumType MaxSum = 0 ;
    sumLevel * sumL = Lv->sumL ;
    for(i=0;i<nbElem;i++) { // add element by Element
        sumType intSum = Elem[i].weight / modConstraint ;
        constraintType modSum = (constraintType) (Elem[i].weight - intSum * modConstraint) ;
        int j,jmax = 1 << i ;
        for(j=0;j<jmax;j++) {
            sumL[j+jmax].intSum = sumL[j].intSum+intSum ;
            sumL[j+jmax].modSum = sumL[j].modSum+modSum ;
            if(sumL[j+jmax].modSum-modConstraint >= 0 ) { // avoid division
                sumL[j+jmax].modSum = sumL[j+jmax].modSum-modConstraint ;
                sumL[j+jmax].intSum++ ;
            }
            if(sumL[j+jmax].intSum  > MaxSum) MaxSum = sumL[j+jmax].intSum ;
         }
    }
    Lv->sumL[nbSum].intSum = 0 ;
    Lv->sumL[nbSum].modSum = modConstraint ;
    Lv->maxSum = MaxSum ;
    qsort(sumL,nbSum,sizeof(sumL[0]),CmpSumL) ; // sort by remainder,quotient
    Lv->nbDiffMod = 0 ;
    for(i=0;i<nbSum;) { // compute access index for each remainder present
        int j , nb ;
        constraintType mod = sumL[i].modSum ;
       Lv->indMod[mod] = i+1 ;
        Lv->nbDiffMod++ ;
        for(j=i+1;j<nbSum && sumL[j].modSum== mod;j++) ;
        i = j ;
    }
    return Lv ;
}

// update context levelState for the next Powp with nbElement (coef) > 0
// return 0 if last level
int nextLevelState(levelState *LS,listPowp *LP) {
    int k ;
    int oldLevel = LS->numLevel ;
    int nbElem=0 ;
    while(nbElem==0 && LP->curPowp < LP->nbPowp) { // loop on remaining powp
        Powp152 * newPowp = LP->Powp + LP->curPowp++ ;
        int64_t powp = newPowp->powp ;
        int64_t ppcm = LS->cumPpcm ;
        for(k=(int)powp;k<=LP->max_n;k+=powp) { // search element k multiple of powp
            int l ;
            for(l=0;l<LS->nbComputVal;l++) { // verify if k not precendently used.
                if(LS->computVal[l] == k) break ;
            }
            if(l<LS->nbComputVal) continue ;
            if((LP->den_ppcm % k) == 0) {   // check if k is admissible
                ppcm = ppcm * k  / PGCD64(ppcm,k) ; // update ppcm
                LS->Elem[nbElem++].val = k ; // store k
            }
        }
        if(nbElem > 0) { // powp has one admissible element
            // update the levelState
            LS->constraint = newPowp->p * newPowp->p ;  // constraint
           
            LS->fact = (int32_t)(ppcm * LS->PP.p)/ LS->ppcm;    // fact from precedent level
            LS->cumPpcm = ppcm/PGCD64(newPowp->ppcmPowp,ppcm) ; // update cumulative ppcm
            LS->nbElem = nbElem ;
            LS->ppcm = ppcm ;   // ppcm for the level
            LS->PP = *newPowp ; // valid powp
            int ie ;
            for(ie=0;ie<nbElem;ie++) {  // compute weight of each element with ppcm
                LS->Elem[ie].weight = (ppcm / LS->Elem[ie].val)*(ppcm / LS->Elem[ie].val) ;
                LS->computVal[LS->nbComputVal++] = LS->Elem[ie].val ;
            }
            LS->numLevel++ ; // increment level number (can be decorralated qith powp number, due to powp with no new lement
        }
    }  ;
    return LS->numLevel-oldLevel ;
}

// decreasing order
int CmpOrderByPowp(const void *el1,const void *el2) {
    return (int) ((Powp152 *)el2)[0].powp - (int) ((Powp152 *)el1)[0].powp ;
}
// reoder powp by decreasing powp (and not p,pwop )
void REORDER152 (listPowp *LP) {
     qsort(LP->Powp+LP->curPowp,LP->nbPowp-LP->curPowp,sizeof(LP->Powp[0]),CmpOrderByPowp) ;
}
// construct the list of admissible powp's for maxvalue for k
int ComputeListPowP(listPowp *LP, int maxValue){
    // maxValue must be <= 300
    int Primes[] = { 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,119,127,131,137,139,149,151,157,161} ;
    LP->den_ppcm = 1 ;
    LP->max_n = maxValue ;
    int ip,p ;
#define MAX152_NP   16
    int64_t invSqare[MAX152_NP], sumInv[1<<MAX152_NP] ;
    for(ip=0;(p=Primes[ip])<=maxValue/2;ip++) ; // seach first prime above maxValue/2 (other not admissible)
    int nbPowp = 0 ;
    for(;--ip >=0;) {
        p = Primes[ip] ;
        int powp,np ;
        for(powp=p;powp*p<=maxValue/2;) powp = powp * p ; // search max power of p candidate
        int notFound = 1 ;
        for(;powp>1 && notFound; powp /= p) {// search by decreasing power of the first admissible
            np = maxValue/powp ;
            int nbInv = 0 ;
            int sqp = p*p ;
            if(p != 2 && np>= powp-1) np = powp-1 ;
            if(p==2 && np>= 2*powp-1) np =2*powp -1 ;
            if(np>MAX152_NP) { printf("FATAL ERROR np=%d too big for powp=%d\n",np,powp) ; }
            int i ;
            for(i=1;i<=np;i++) { // for each i (k=i) search inverse for k*k mod[p*p]
                if((i % p) == 0) continue ;
                int i2 = (i*i) % sqp ;
                int inv_i2, ninv ; // search 1/(i*i) with if a power of i*i
                for(inv_i2 = 1; (ninv = (inv_i2*i2) % sqp )!=1;inv_i2=ninv) ;
                invSqare[nbInv++] = inv_i2 ; // store 1/(i*i) mod[p*p]
            }
            sumInv[0] = 0 ;
            for(i=0;i<nbInv && notFound ;i++) { // search a combinaison sum of inverse == 0 mod[p*p]
                int j,jmax = 1 << i ;
                for(j=0;j<jmax && notFound;j++) {
                    sumInv[j+jmax] = (sumInv[j]+invSqare[i]) % sqp ;
                    if(sumInv[j+jmax]==0) notFound = 0 ;
                }
            }
            
        }
        if(!notFound) { // existing combinaison sum
            powp *= p ; // add p*powp and all divisor to list of admissible powp
            LP->den_ppcm *= powp ; printf("x%d",powp) ;
            constraintType ppcmPowp = p ;
            for(;powp > 1;powp /= p) {
                LP->Powp[nbPowp].powp = powp ;
                LP->Powp[nbPowp].p = p ;
                LP->Powp[nbPowp++].ppcmPowp = ppcmPowp ; ppcmPowp *= p ;
            }
        }
    }
    LP->curPowp = 0 ;
    LP->nbPowp = nbPowp ;
    printf("=%llu [%llu] \n",(u_int64_t)LP->den_ppcm,(u_int64_t)(LP->den_ppcm *  LP->den_ppcm/2) );
    return LP->nbPowp ;
}

#define PB152_SIZE_HISTO    20480000



int PB152c_n(PB_RESULT *pbR,int max_n) {
    pbR->nbClock = clock() ;
    listPowp LP ;
    ComputeListPowP(&LP, max_n) ;
    levelState LS ;
    LS.nbComputVal = 0 ; LS.numLevel = 0 ;
    LS.computVal[LS.nbComputVal++] = 2 ; // 1/2**2 is mandatory, so remove it
    LS.constraint = 1; LS.ppcm = 2 ;   LS.PP.p = 1;    LS.PP.powp = 2;
    LS.PP.ppcmPowp = 2;  LS.cumPpcm = 1; LS.fact = 1 ;
   countType nbSolTot = int2count(0) ;
    sumType sumFinal = 1 ;

    int nbHisto0 = 0 ;
    hist152 * Histo0 = malloc(1*sizeof(Histo0[0])) ;
    Histo0[nbHisto0].sum = sumFinal ;
   Histo0[nbHisto0++].count = int2count(1);
    int64_t nbCountS0 = 0 ;
    sumType sumMax ;
    LevelDev * Lv ;
    countType * countS0 = NULL ;
    sumType offsetS0 = 0 ;
#define M_IN_COUNT   1
#define M_OUT_COUNT  2
#define M_OUT_HISTO  0
    int mode = 0 ; // in and out by histo

    while(nextLevelState(&LS, &LP)) {
        int curLevel = LS.numLevel ;
        countType * countS1 = NULL ;
        hist152 * Histo1 = NULL ;
        constraintType constraint = LS.constraint ;
        int nbElem = LS.nbElem ;
        Lv = Cmp52_level(LS.Elem,nbElem,constraint) ;
        int64_t factS = LS.fact*LS.fact ;
        sumType maxLv = Lv->maxSum ;
        int nbHisto1 = 0 , nbNul = 0 ;
        int ih,ihMax ;
        if(mode & M_OUT_COUNT ) {
             ihMax = (indexType) nbCountS0 ;
        } else {
            Histo1 = malloc(PB152_SIZE_HISTO*sizeof(Histo1[0])) ;
               offsetS0 = Histo0[0].sum ;
               ihMax = nbHisto0 ;
               sumMax =  Histo0[nbHisto0-1].sum ;
        }
        sumMax =  (sumMax*factS)/constraint  ;
        int64_t nbCountS1 =(sumMax-offsetS0*factS/constraint+Lv->maxSum+1) ;
        if(nbCountS1-1 > sumMax) nbCountS1 =  sumMax+1 ;
        sumType offsetS1 = sumMax - (nbCountS1-1) ;
        if((mode == M_OUT_HISTO)  && nbCountS1 < 2 * nbHisto0 - 10  ) {
            mode = M_OUT_COUNT ; printf("Swap to mode OUT_COUNT at level %d\n",curLevel) ;
        }
        printf("%.3f Level=%d,Elems=[%d...%d] Mod[%d],ppcm=%lld,nbEl=%d,MaxLv=%lld,fact=%d HMax=%lld ExpHout=%lld Hin=%d"
              ,(clock() - pbR->nbClock) / (float) CLOCKS_PER_SEC
              ,curLevel,LS.Elem[0].val,LS.Elem[nbElem-1].val,constraint
              ,LS.ppcm,nbElem,maxLv,LS.fact ,sumMax,nbCountS1 , ihMax) ;
        if(mode & M_OUT_COUNT ) {
            countS1 = calloc(nbCountS1,sizeof(countS1[0])) ;
        }
        u_int64_t nout = 0 ;
        for(ih=0;ih<ihMax;ih++) {
            sumType sum  ;  countType count ;
            if(mode & M_IN_COUNT ) {
                count = countS0[ih] ;
                sum = (offsetS0+ih) ;
            } else {
                count = Histo0[ih].count ;
                sum = Histo0[ih].sum ;
            }
            sum *= factS ;
            sumType intSum = sum / constraint ;
            constraintType modSum = (constraintType) (sum - intSum * constraint) ;
           if(Lv->indMod[modSum]>0) {
                int32_t ind ;
                for(ind = Lv->indMod[modSum]-1 ; Lv->sumL[ind].modSum == modSum;ind++) {
                    sumType newSum = (intSum - Lv->sumL[ind].intSum) ;
//                   if(newSum < 0) { nbNul++ ;break ;} // sum exeeds 1/2
                    nout++ ;
                    if(mode & M_OUT_COUNT ) {
                        int nc = (indexType)(newSum - offsetS1) ;
                        addCount(countS1[nc],count) ;
                    } else {
                        Histo1[nbHisto1].sum = newSum ;
                        Histo1[nbHisto1++].count = count ;
                    }
                }
            }
            if(mode == M_OUT_HISTO ) {
                if(nout > nbCountS1 ) { // test swap to count mode ?
                    mode = M_OUT_COUNT ; ih=-1; nout=0 ;
                    printf("***");
                    countS1 = calloc(nbCountS1,sizeof(countS1[0])) ;
                    continue ;
                } else if( nbHisto1 > PB152_SIZE_HISTO - Lv->nbSum || ih==nbHisto0-1) {
                    // need compression ? end of level, or histo buffer out almost full
                    nbHisto1 = CompressHisto(Histo1,nbHisto1) ;
                }
            }
        } // end loop ih on precedent active sums
       Lv = Free_levelDev(Lv);
        if(mode == M_OUT_HISTO ) {
            nbHisto0 = nbHisto1 ;
            free(Histo0) ;
            Histo0 = Histo1 ;
        } else {
            if(mode == M_OUT_COUNT) {
                REORDER152 (&LP);
                mode |= M_IN_COUNT ;
            }
            nbCountS0 = nbCountS1 ;
            offsetS0 = offsetS1 ;
            free(countS0);
            countS0 = countS1 ;
        }
        if(mode == M_OUT_HISTO ) {
            printf(" Hout=%lld/->%d \n",nout,nbHisto0) ;

#if defined(PB152_DEBUG)
            printf("(%lld,%lld)...(%lld,%lld)\n",(u_int64_t)(Histo0[0].sum),l_count(Histo0[0].count),(u_int64_t)Histo0[nbHisto0-1].sum,l_count(Histo0[nbHisto0-1].count));
            if(nbHisto0 < 50) {
                for(ih=0;ih<nbHisto0;ih++) printf("(%lld,%lld)",(u_int64_t)(Histo0[ih].sum),l_count(Histo0[ih].count));
                printf("\n");
            }
#endif
        } else {
           printf("=%d+%d(0)  Hout=%lld/%d\n",(indexType)(ihMax-nbNul),nbNul,nout,(indexType)nbCountS0);
#if defined(PB152_DEBUG)
           if(nbCountS0 < 50) {
                int i,i0 ;
                for(i0=0,i=0;i<nbCountS0 && i0 < 100;i++) { if(!isCountNull(countS0[i]))  { printf("[%d,%lld]",i,l_count(countS0[i])); i0++ ; } }
                printf("\n");
            }
#endif
        }
    }
    if(mode & M_OUT_COUNT) {
       int i ;
//       nbSolTot = countS0[(int64_t)-offsetS0] ;
       nbSolTot = countS0[0] ;
        free(countS0);
    } else {
        int i ;
        for(i=0;i<nbHisto0;i++) {
           if(Histo0[i].sum == 0 ) { addCount(nbSolTot,Histo0[i].count) ; }
        }
        free(Histo0) ;
    }
    
    pbR->nbClock = clock() - pbR->nbClock ;
# define POW10_9  1000000000LL
    if(h_count(nbSolTot) || l_count(nbSolTot) >= POW10_9 * POW10_9) {
       u_int64_t nbH10 = h_count(nbSolTot) / POW10_9 ;
       u_int64_t nbHr = h_count(nbSolTot) % POW10_9 ;
       u_int64_t nbL10 = l_count(nbSolTot) / POW10_9 ;
       u_int64_t nbLr = l_count(nbSolTot) % POW10_9 ;
       u_int64_t p64_10 = (1LL << (64-9)) / (POW10_9/(1<<9)) ;
       u_int64_t p64r = ((1LL << (64-9)) - p64_10 * (POW10_9/(1<<9))) << 9 ;
       // (nbH10 * POW10_9 + nbHr) * (P64_10 * POW10_9 + p64r) + (nbL10 * POW10_9 + nbLr)
      u_int64_t nbLow = (nbHr * p64r + nbLr) ;
      u_int64_t nbHig = (nbLow / POW10_9) + nbH10 * p64r + nbHr * p64_10 + POW10_9 * nbH10 * p64_10 + nbL10;
      nbLow -= (nbLow / POW10_9) * POW10_9 ;
 
        if(pbR->isVerbose) fprintf(stdout,"\t PB%s 1/2=sigma(1/n**2 <1<n<=%d has %lld%lld sol\n",pbR->ident,max_n,nbHig,nbLow);
        snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld%lld",nbHig,nbLow) ;
    } else {
        if(pbR->isVerbose) fprintf(stdout,"\t PB%s 1/2=sigma(1/n**2 <1<n<=%d has %lld sol \n",pbR->ident,max_n,l_count(nbSolTot));
        snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",l_count(nbSolTot)) ;
    }
    return 1 ;
}




typedef struct Node152_a {
    int  level ;        // level for the node
    indexType elem ;              // element in the level for the node
    constraintType sumConstraint   ;   // cumulates sum modulus the constraint
    sumType sum ;                       // cumulated sum from the beginning
} Node152_a ;

typedef void (*CBsol152) (void *ctx,hist152 hist) ;


typedef struct Element152_a {
   int val ;                           // int for denominator
   sumType weight ;                    // contribution to 1/2
   constraintType weightConstraint ;   // weight modulus the constraint
} Element152_a ;

// a context of development for a powp
typedef struct level_a {
   int         numLevel ;  // index of treated powp (with nb elements > 0)
   Element152_a  Elem[50] ;  // coefficients for current powp
   int         nbElem ;    // number of coefficients
   constraintType  constraint ;    // current constraint for the level (= p*p)
   Powp152     PP ;    // current Powp
} level_a ;

// whole set of development of powp
typedef struct levelsGlobal {
   level_a  LS[100] ;
   int     nbLevel ;
   int     computVal[PB152_MAXN] ; // history of k precedently used (to filter coefficients and use only once)
   int     nbComputVal ;   // number of k precedently used
   
} levelsGlobal ;



int PB152a_n(PB_RESULT *pbR,int max_n) ;
int PB152a(PB_RESULT *pbR) {
   return PB152a_n(pbR,PB152_ASK);
}


// update context levelsGlobal for the next Powp with nbElement (coef) > 0
// return 0 if last level
int nextLevelsGlobal(levelsGlobal *LG,listPowp *LP) {
   int k ;
   int oldLevel = LG->nbLevel ;
   level_a * LS = LG->LS+oldLevel ;
   int nbElem=0 ;
   while(nbElem==0 && LP->curPowp < LP->nbPowp) { // loop on remaining powp
      Powp152 * newPowp = LP->Powp + LP->curPowp++ ;
      int64_t powp = newPowp->powp ;
      int64_t ppcm = LP->den_ppcm ;
      for(k=(int)powp;k<=LP->max_n;k+=powp) { // search element k multiple of powp
         int l ;
         for(l=0;l<LG->nbComputVal;l++) { // verify if k not precendently used.
            if(LG->computVal[l] == k) break ;
         }
         if(l<LG->nbComputVal) continue ;
         if((LP->den_ppcm % k) == 0) {   // check if k is admissible
            LS->Elem[nbElem++].val = k ; // store k
         }
      }
      if(nbElem > 0) { // powp has one admissible element
         // update the levelState
         LS->constraint = newPowp->ppcmPowp * newPowp->ppcmPowp ;  // constraint
         
         LS->nbElem = nbElem ;
         LS->PP = *newPowp ; // valid powp
         int ie ;
         for(ie=0;ie<nbElem;ie++) {  // compute weight of each element with ppcm
            u_int64_t weight = (ppcm / LS->Elem[ie].val)*(ppcm / LS->Elem[ie].val)  ;
            
            LS->Elem[ie].weight = weight  ;
            LS->Elem[ie].weightConstraint = weight % LS->constraint ;
            
            LG->computVal[LG->nbComputVal++] = LS->Elem[ie].val ;
         }
         LS->numLevel = LG->nbLevel ; // increment level number (can be decorralated qith powp number, due to powp with no new lement
         LG->nbLevel++ ;
      }
   }  ;
   return LG->nbLevel-oldLevel ;
}


int PB152a_n(PB_RESULT *pbR,int max_n) {
    u_int64_t den_ppcm = 1  ;
    int nbSol = 0 ;
    pbR->nbClock = clock() ;
   listPowp LP ; // list of power of p admissible
   levelsGlobal LG ; // context for levels
   ComputeListPowP(&LP, max_n) ; // compute powp
   den_ppcm = LP.den_ppcm ;
   LG.nbLevel =0 ;
   LG.nbComputVal=0 ;
   LG.computVal[LG.nbComputVal++] = 2 ;
   // compute all the level
   while(nextLevelsGlobal(&LG,&LP) ) {} //compute coefficients all the powp

    printf("=%lld [%lld] nbLevel%d\n",LP.den_ppcm,LP.den_ppcm*LP.den_ppcm/2,LG.nbLevel );
    Node152_a nod[PB152_MAXN] ; // nodes for tree walk
    int is ;
//   is =0 ; nod[is].sum = den_ppcm*den_ppcm/2 - Elem[0].weight ;  // car 2 obligatoire
    is =0 ; nod[is].sum = den_ppcm*den_ppcm/4 ;  // car 2 obligatoire
    nod[is].sumConstraint = 0 ;
    nod[is].level = -1 ; // special case for 1/2**2
   is++ ;
    nod[is].level = 0 ;
    nod[is].elem = 0 ;

   while(is>=1) {
      if(nod[is-1].sum == 0) {
         nbSol++ ;
      }
      while (is > 0) { //loop : try to advance by developing current node
         if(nod[is-1].sum > 0 && nod[is].level < LG.nbLevel) { // precedent node OK ?
            int ie ;
            level_a *curLevel = LG.LS+nod[is].level ;
            for(ie=nod[is].elem; ie<curLevel->nbElem ; ie++) { // search a valid element
               if( curLevel->Elem[ie].weight <= nod[is-1].sum ) {
                  nod[is].elem = ie ;
                  nod[is].sum = nod[is-1].sum-curLevel->Elem[ie].weight ;
                  if(nod[is].level == nod[is-1].level) {
                     nod[is].sumConstraint = nod[is-1].sumConstraint - curLevel->Elem[ie]. weightConstraint;
                     if(nod[is].sumConstraint < 0) nod[is].sumConstraint += curLevel->constraint ;
                  } else {
                     nod[is].sumConstraint = nod[is].sum % curLevel->constraint  ;
                  }
                  break ;
               }
            }
            if(ie < curLevel->nbElem) { // found an admissible element
               is++ ; nod[is].elem = nod[is-1].elem+1 ; nod[is].level=nod[is-1].level ;
               break ; // continue on current level
            } else if(nod[is-1].sumConstraint == 0){
               nod[is].level++ ; // current level exhausted, try next level
               nod[is].elem = 0 ;
               break ; // continue on next level
            }
         } // end ob branch (last level, or sum< 0, must rewind
         
         while( --is > 0) { // rewind the tree
            if(nod[is].elem+1 <  LG.LS[nod[is].level].nbElem) {
               nod[is].elem++ ; break ;
            } else if ( nod[is-1].sumConstraint == 0) {
               nod[is].level++ ; nod[is].elem = 0 ;  break ;
            }
         }
      } // end loop try to advance
   } // end global loop
    pbR->nbClock = clock() - pbR->nbClock ;
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s 1/2=sigma(1/n**2 <1<n<=%d has %d sol \n",pbR->ident,max_n,nbSol);

    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbSol) ;
    return 1 ;
}

int PB152_n(PB_RESULT *pbR,int max_n) ;

int PB152(PB_RESULT *pbR) {
   return PB152_n(pbR,PB152_ASK);
}


typedef struct Node152 {
   indexType elem ;              // element in the level for the node
   sumType sum ;                 // cumulated sum from the beginning
} Node152 ;

typedef struct Element152 {
   int     val ;           // k
   sumType weight ;        // contribution to 1/2 pondered by ppcm
   sumType cumToEnd ;
} Element152 ;


int PB152_n(PB_RESULT *pbR,int max_n) {
    int nbSol = 0 ;
    pbR->nbClock = clock() ;
   listPowp LP ; // list of power of p admissible
   levelsGlobal LG ; // context for levels
   ComputeListPowP(&LP, max_n) ; // compute powp
   u_int64_t den_ppcm = LP.den_ppcm  ;

   Node152 nod[PB152_MAXN] ;
   Element152 Elem[PB152_MAXN] ;
   int k ;
   int nbElem = 0 ;
   for(k=2;k<=max_n;k++) { // compute the weight for each k retained
        if((den_ppcm % k) == 0 ) {
            Elem[nbElem].weight = (den_ppcm / k)*(den_ppcm / k) ;
            Elem[nbElem++].val = k ;
        }
    }
    sumType cum = 0 ;   // cumulative to the end, so we can filter the partail sums too low.
    for(k=nbElem-1;k>=0;k--) {
        cum += Elem[k].weight ;
        Elem[k].cumToEnd = cum ;
    }
    printf("=%lld [%lld] nbNum=%d\n",den_ppcm,den_ppcm*den_ppcm/2,nbElem );
    int is ;
    is =0 ;
    nod[is].sum = den_ppcm*den_ppcm/2 - Elem[0].weight ;
    nod[is].elem = 0 ;
    is++ ;
    nod[is].sum = nod[is-1].sum - Elem[1].weight ;
    nod[is].elem = 1 ;
   while(is>=1) { // tree walk do develop the sums
      if(nod[is].sum > 0) {
         is++ ;
         nod[is].elem = nod[is-1].elem + 1;
         nod[is].sum = nod[is-1].sum - Elem[nod[is].elem].weight ;
      } else {
         if(nod[is].sum == 0) {
            nbSol++ ;
         }
         while (is > 0) {
            nod[is].sum += Elem[nod[is].elem].weight ;
            int ie ;
            for(ie=nod[is].elem+1;Elem[ie].cumToEnd >= nod[is].sum &&  ie<nbElem ;ie++) {
               if( Elem[ie].weight <= nod[is].sum) break ;
            }
            if(ie<nbElem && Elem[ie].cumToEnd>= nod[is].sum ) {
               nod[is].elem = ie ; nod[is].sum -= Elem[ie].weight ;
               break ;
            }
            is-- ;
         }
      }
   }
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbSol) ;
    return 1 ;
}

#define PB153_NB    100000000
//#define PB153_NB    1000000000
#define SumN(N)   ((N)*((N)+1)/2)

// compute Sigma (nb/i)*i i=1,... nb
int64_t SigmaInvInt(int64_t nb) {
   int64_t i ;
   int64_t Inb0 = nb ;
   int64_t S = 0 ;
   for(i=1;i<Inb0;i++) {
      int64_t Inb1 = nb/(i+1) ;
      S += Inb0*i + i*(Inb1+1+Inb0)*(Inb0-Inb1)/2 ;
      Inb0 = Inb1 ;
   }
   if(i == Inb0) S += i * Inb0 ;
   return S ;
}



// #define PB153_NB    25

// compute Sigma x for x^2+y^2 <= N x>0, y>0
// on decoupe en 3 zones
/*. ...
 |  Z3   .
 |---------.
 |       . |  .
 |    Z1 . |   .
 |  .      | Z2 .
 |_________|____.
*/
int64_t NbxPointsInCirle(int64_t N) {
   int64_t sqrN2 =  Sqrt64(N/2) ;
   int64_t sqrN = Sqrt64(N) ;
   int64_t S = SumN(sqrN2)*sqrN2 ; // Z1
   int64_t i ;
   for(i=sqrN2+1;i<=sqrN;i++) {
      int64_t sqrY = Sqrt64(N-i*i) ;
      S += i * sqrY     // Z2
         + SumN(sqrY);  // Z3
   }
   return S ;
}




void PB153_compCircle(int64_t N,int64_t n1,int64_t * tbCircleLow,int64_t * tbCircleHigh,int64_t * tbPrimeLow,int64_t * tbPrimeHigh) {
   int64_t i ;
   tbCircleLow[0]=tbCircleHigh[0]=tbPrimeLow[0]=tbPrimeHigh[0]=0 ;
   for(i=1;i<=n1;i++) {
      tbCircleLow[i] = NbxPointsInCirle(i);
      tbCircleHigh[i] = NbxPointsInCirle(N/i);
   }
   // compute small values
   for(i=1;i<=n1;i++) {
      int64_t S = tbCircleLow[i]  ;
      int64_t j ;
      // remove pgcd(x,y)=j
      for(j=2;j*j<=i;j++) S -= j * tbPrimeLow[i/(j*j)] ;
      tbPrimeLow[i] = S ;
   }
   // compute big values N/i  (ascending order => i descending , for recursion)
   for(i=n1;i>0;i--) {
      int64_t i1 = N/i ;
      int64_t S =  tbCircleHigh[i]  ;
      int64_t j ;
      for(j=2;j*j<=i1;j++) {
         int64_t j1 = i1 / (j*j) ;
         if(j1 > n1) { // H or L ?
            // i1/(j*j) = (N/i)/(j*j) = N/(i*j*j)
            S -= j * tbPrimeHigh[i*j*j] ;
         } else {
            S -= j * tbPrimeLow[j1] ;
         }
      }
      tbPrimeHigh[i] = S ;
   }
}



int PB153c(PB_RESULT *pbR) {
   pbR->nbClock = clock() ;
   int64_t n1=Sqrt64(PB153_NB);
   int64_t * tbPrimeLow = malloc((n1+1)*sizeof(tbPrimeLow[0]));
   int64_t * tbPrimeHigh = malloc((n1+1)*sizeof(tbPrimeHigh[0]));
   int64_t * tbCircleLow = malloc((n1+1)*sizeof(tbCircleLow[0]));
   int64_t * tbCircleHigh = malloc((n1+1)*sizeof(tbCircleHigh[0]));
   

   int32_t i ;
   PB153_compCircle(PB153_NB,n1,tbCircleLow,tbCircleHigh,tbPrimeLow,tbPrimeHigh) ;

   int64_t S =  SigmaInvInt(PB153_NB);
   for(i=1;i<n1;i++) {
      S += 2 * (tbPrimeHigh[i] - tbPrimeHigh[i+1])* SigmaInvInt(i) ;
   }
   S += 2 * (tbPrimeHigh[n1] - tbPrimeLow[PB153_NB/(n1+1)]) * SigmaInvInt(n1) ;
   for(i=1;i<PB153_NB/n1;i++) {
      S += 2* (tbPrimeLow[i] - tbPrimeLow[i-1])* SigmaInvInt(PB153_NB/i)  ;
   }

   //  sbt=SBT_free(sbt);
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",S) ;
   return 1 ;
}


int PB153b(PB_RESULT *pbR) {
   pbR->nbClock = clock() ;
   int n1=Sqrt32(PB153_NB);
   int32_t *tbSquare = malloc((n1+2)*sizeof(tbSquare[0]));
   int32_t i ;
   int64_t *lowNb=calloc(n1+1,sizeof(lowNb[0])) ;
   int64_t *highNb=calloc(n1+1,sizeof(highNb[0])) ;
   int64_t S = SigmaInvInt(PB153_NB) + 2*SigmaInvInt(PB153_NB/2) ;
   for(i=0;i<=n1+1;i++) tbSquare[i] = i*i;
   
   FRACTrec FRrec ;
   FRC_init(&FRrec,n1 , (FRACTRED){0,1}, (FRACTRED){1,1}) ;
   int32_t norm ;
   do {
      norm = tbSquare[FRrec.fr1.n]+tbSquare[FRrec.fr1.d] ;
      if(norm <=n1) {
         highNb[norm] += FRrec.fr1.n+FRrec.fr1.d ;
      } else {
         lowNb[PB153_NB/norm] += FRrec.fr1.n+FRrec.fr1.d ;
      }
   } while(FRC_getNext(&FRrec)) ;
   for(i=1;i<=n1;i++) {
      if(lowNb[i]) S += 2*SigmaInvInt(i)*lowNb[i] ;
      if(highNb[i])  S += 2*SigmaInvInt(PB153_NB/i)*highNb[i] ;
    }
   free(tbSquare) ; free(lowNb) ;
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",S) ;
   return 1 ;
}


int PB153a(PB_RESULT *pbR) {
   pbR->nbClock = clock() ;
   int n1=Sqrt32(PB153_NB);
   int32_t *tbSquare = malloc((n1+2)*sizeof(tbSquare[0]));
   int64_t *lowNb=calloc(n1+1,sizeof(lowNb[0])) ;
   int64_t *highNb=calloc(n1+1,sizeof(highNb[0])) ;

   int32_t i ;
   int64_t S = SigmaInvInt(PB153_NB) + 2*SigmaInvInt(PB153_NB/2) ;
   for(i=0;i<=n1+1;i++) tbSquare[i] = i*i;
   SBTree * sbt =  SBT_alloc() ;
   FRACTRED fr0, fr1 ;
   fr0.d=1 ; fr0.n=0 ;
   fr1.d=1 ; fr1.n=1 ;
   SBT_init(sbt,fr0,fr1);
   int32_t norm ;
    while(SBT_getNext(sbt, n1)){
      norm = tbSquare[sbt->fr1.n]+tbSquare[sbt->fr1.d] ;
       if(norm <=n1) {
          highNb[norm] += sbt->fr1.n+sbt->fr1.d ;
       } else {
          lowNb[PB153_NB/norm] += sbt->fr1.n+sbt->fr1.d ;
       }

   }
   for(i=1;i<=n1;i++) {
      if(lowNb[i]) S += 2*SigmaInvInt(i)*lowNb[i] ;
      if(highNb[i]) S += 2*SigmaInvInt(PB153_NB/i)*highNb[i] ;
   }

   sbt=SBT_free(sbt);
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",S) ;
   return 1 ;
}

int PB153(PB_RESULT *pbR) {
   pbR->nbClock = clock() ;
   int n1=Sqrt32(PB153_NB);
   int32_t *tbSquare = malloc((n1+2)*sizeof(tbSquare[0]));
   int32_t i,j ;
   int64_t S = SigmaInvInt(PB153_NB) + 2*SigmaInvInt(PB153_NB/2) ;
   printf("S=%lld ",S);
   for(i=0;i<=n1+1;i++) tbSquare[i] = i*i;
   for(i=1;i<=n1;i++) {
      int32_t norm ;
      int antNb = 1 ;
      int64_t delta = 1 ;
      for(j=1;(j < i) && ((norm=tbSquare[i]+tbSquare[j]) <= PB153_NB) ;j++) {
         if(PGCD(i,j) != 1) continue ;
         int nb = PB153_NB / norm ;
         if(nb != antNb) {
            antNb = nb ;
            delta = SigmaInvInt(nb) ;
         }
//         printf("(%d,%d)->%lld",j+1,i+1,delta);
         S += 2*delta*(i+j) ;
      }
   }
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",S) ;
   return 1 ;
}

#define PB154_EXP 200000
int PB154a(PB_RESULT *pbR) {
   pbR->nbClock = clock() ;
   int32_t *exp2 = calloc(PB154_EXP+1,sizeof(exp2[0]));
   int32_t *exp5 = calloc(PB154_EXP+1,sizeof(exp5[0]));
   int16_t *sexp2 = malloc((PB154_EXP+1) *sizeof(sexp2[0]));
   int16_t *sexp5 = malloc((PB154_EXP+1)*sizeof(sexp5[0]));
   int32_t pow2 ;
   for(pow2=2;pow2<=PB154_EXP; pow2 *= 2) {
      int i ;
      for(i=pow2; i<=PB154_EXP;i+=pow2 ) {
         exp2[i]++ ;
      }
   }
   int32_t pow5 ;
   for(pow5=5;pow5<=PB154_EXP; pow5 *= 5) {
      int i ;
      for(i=pow5; i<=PB154_EXP;i+=pow5 ) {
         exp5[i]++ ;
      }
   }
   int i ;
   for(i=1;i<=PB154_EXP;i++){ exp2[i] += exp2[i-1] ; exp5[i] += exp5[i-1];}
   int32_t min2=0, min5=0 ;
   int32_t max2=-100, max5=-100 ;
   for(i=0;i<=PB154_EXP;i++){
      sexp2[i] = (int16_t) (exp2[i] -  i) ;
      if(sexp2[i]< min2) { min2 = sexp2[i] ;  }
      else if(sexp2[i]> max2) { max2 = sexp2[i] ;}
      sexp5[i] = (int16_t) (4*exp5[i] - i);
      if(sexp5[i]< min5) { min5 = sexp5[i] ;  }
      else if(sexp5[i]> max5) { max5 = sexp5[i] ;}
//      sexp5[i] = sexp5[i] *256 + sexp2[i] ;
   }
   
   int64_t S = 0 ;
   int32_t p2 = sexp2[PB154_EXP]-12 , p5=sexp5[PB154_EXP]-48 ;
//   p5 = sexp5[PB154_EXP] -12 -48*256 ;
   //   for(i=0;i<1000;i++) printf("%d ",exp2[i]+6);
   printf("Pow2=%d,[%d,%d] pow5=%d,[%d,%d]\n",p2,min2,max2,p5,min5,max5);
   int a,b ;
  
   // a < b < c ; c = p-a-b
   for(a=0;a<=PB154_EXP;a++) {
      int c = PB154_EXP - 2*a -1 ;
      for(b=a+1;2*b<PB154_EXP-a;b++,c--) {
         if(sexp5[b]+sexp5[c]<=p5-sexp5[a] && sexp2[b]+sexp2[c]<= p2-sexp2[a]  ) S++ ;
       }
   }

   
   S *= 6 ;
   // a=b
   for(a=0;2*a<=PB154_EXP;a++) {
      if(2*sexp2[a]+sexp2[PB154_EXP-2*a]<= p2 && 2*sexp5[a]+sexp5[PB154_EXP-2*a]<=p5 ) S += 3 ;
//     if( 2*sexp5[a]+sexp5[PB154_EXP-2*a]<=p5 ) S += 3 ;
   }
   
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",S) ;
   return 1 ;
}


int PB154(PB_RESULT *pbR) {
   pbR->nbClock = clock() ;
   int32_t *exp2 = calloc(PB154_EXP+1,sizeof(exp2[0]));
   int32_t *exp5 = calloc(PB154_EXP+1,sizeof(exp5[0]));
   int32_t pow2 ;
   for(pow2=2;pow2<=PB154_EXP; pow2 *= 2) {
      int i ;
      for(i=pow2; i<=PB154_EXP;i+=pow2 ) {
         exp2[i]++ ;
      }
   }
   int32_t pow5 ;
   for(pow5=5;pow5<=PB154_EXP; pow5 *= 5) {
      int i ;
      for(i=pow5; i<=PB154_EXP;i+=pow5 ) {
         exp5[i]++ ;
      }
   }
   int i ;
   for(i=1;i<=PB154_EXP;i++){ exp2[i] += exp2[i-1] ; exp5[i] += exp5[i-1];}
   int32_t min2=0, min5=0 ;
   int32_t max2=-100, max5=-100 ;
  for(i=0;i<=PB154_EXP;i++){
      exp2[i] -=  i ;
      if(exp2[i]< min2) { min2 = exp2[i] ;  }
      else if(exp2[i]> max2) { max2 = exp2[i] ;}
      exp5[i] = 4*exp5[i] - i;
      if(exp5[i]< min5) { min5 = exp5[i] ;  }
      else if(exp5[i]> max5) { max5 = exp5[i] ;}
   }

   int64_t S = 0 ;
   int32_t p2 = exp2[PB154_EXP]-12 , p5=exp5[PB154_EXP]-48 ;
//   for(i=0;i<1000;i++) printf("%d ",exp2[i]+6);
   printf("Pow2=%d,[%d,%d] pow5=%d,[%d,%d]\n",p2,min2,max2,p5,min5,max5);
   int a,b ;
   // a < b < c ; c = p-a-b
   for(a=0;a<=PB154_EXP;a++) {
      for(b=a+1;2*b<PB154_EXP-a;b++) {
         if(exp5[a]+exp5[b]+exp5[PB154_EXP-a-b]<=p5 && exp2[a]+exp2[b]+exp2[PB154_EXP-a-b]<= p2  ) S++ ;
      }
   }
   S *= 6 ;
   // a=b
   for(a=0;2*a<=PB154_EXP;a++) {
      if(2*exp2[a]+exp2[PB154_EXP-2*a]<= p2 && 2*exp5[a]+exp5[PB154_EXP-2*a]<=p5 ) S += 3 ;
   }
   
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",S) ;
   return 1 ;
}

#define PB155_NBC 18
typedef struct Capacity {
   int32_t  num;
   int32_t  den ;
} Capacity ;

static inline Capacity IParal(Capacity cap1,Capacity cap2) {
   Capacity capP ;
   capP.num = cap1.den * cap2.den ;
   capP.den = cap1.num * cap2.den + cap2.num * cap1.den ;
   int32_t g = PGCD(capP.den, capP.num);
   if(g > 1) {
      capP.den /= g ;
      capP.num /= g ;
   }
   return capP ;
}

static inline Capacity SerialI1(Capacity cap1,Capacity cap2) {
   Capacity serP ;
   serP.num = cap1.den * cap2.num ;
   serP.den = cap1.den * cap2.den + cap2.num * cap1.num ;
   int32_t g = PGCD(serP.den, serP.num);
   if(g > 1) {
      serP.den /= g ;
      serP.num /= g ;
   }
   return serP ;
}
static inline Capacity SerialI2(Capacity cap1,Capacity cap2) {
   Capacity serP ;
   serP.num = cap1.num * cap2.den ;
   serP.den = cap1.num * cap2.num + cap2.den * cap1.den ;
   int32_t g = PGCD(serP.den, serP.num);
   if(g > 1) {
      serP.den /= g ;
      serP.num /= g ;
   }
   return serP ;
}
static inline Capacity SerialI12(Capacity cap1,Capacity cap2) {
   Capacity serP ;
   serP.num = cap1.num * cap2.num ;
   serP.den = cap1.den * cap2.num + cap2.den * cap1.num ;
   int32_t g = PGCD(serP.den, serP.num);
   if(g > 1) {
      serP.den /= g ;
      serP.num /= g ;
   }
   return serP ;
}



//#define HASH_CP(cp) (PB155_MAXD*(cp).den+(cp).num)
#define HASH_CP(cp) ((cp).den*((cp).den+1)/2+(cp).num)
int PB155(PB_RESULT *pbR) {
   int maxDen ;
   {
      int F0 = 1, F1=1 ;
      int k ;
      for(k=2;k<=PB155_NBC;k++) {    int F2 = F0+F1 ;    F0=F1;  F1=F2 ;   }
      maxDen = F1 ;

   }
//   int maxDen = PB155_MAXD ;
   pbR->nbClock = clock() ;
   int i;
   Capacity cp ;
   Capacity *caps = malloc((maxDen+1)*(maxDen+2)/2*sizeof(caps[0]));
   int32_t indCs[PB155_NBC+2];
   int is = 0 ;
   uint8_t * isFound = calloc((maxDen+1)*(maxDen+2)/2,sizeof(isFound[0])) ;
   indCs[1] = 0 ;
   cp = (Capacity) { 1, 1} ;
   caps[is++]  = cp  ; isFound[HASH_CP(cp)]=1 ;
   indCs[2] = is ;
   caps[is++] = cp= IParal(caps[0],caps[0]); isFound[HASH_CP(cp)]=1 ;
   int SS =  is ;
   indCs[3]=is ;
   int32_t hcp ;
   for(i=3;i<=PB155_NBC;i++) {
      int k ;
      indCs[i] = is ;
      {
         int k2 ;
         Capacity cp1 = (Capacity) { 1, 1} ;
         for(k2=indCs[i-1];k2<indCs[i];k2++) {
               Capacity cp = IParal(cp1,caps[k2]) ;
               if(isFound[hcp=HASH_CP(cp)] == 0) { caps[is++] = cp ; isFound[hcp]=1 ; }
               cp = SerialI1(cp1,caps[k2]) ; if(isFound[hcp=HASH_CP(cp)] == 0) { caps[is++] = cp ; isFound[hcp]=1 ; }
               cp = SerialI2(cp1,caps[k2]) ; if(isFound[hcp=HASH_CP(cp)] == 0) { caps[is++] = cp ; isFound[hcp]=1 ; }
         }

      }
      for(k=2;2*k<=i;k++) {
        int k1,k2 ;
         for(k1=indCs[k];k1<indCs[k+1];k1++) {
            for(k2=(2*k==i) ? k1 : indCs[i-k];k2<indCs[i-k+1];k2++) {
                Capacity cp = IParal(caps[k1],caps[k2]) ;
               if( cp.den > cp.num) {
                  if(isFound[hcp=HASH_CP(cp)] == 0) { caps[is++] = cp ; isFound[hcp]=1 ; }
               } else if(cp.den < cp.num)  {
                  int32_t tmp = cp.den ;
                  cp.den = cp.num ;
                  cp.num = tmp ;
                  if(isFound[hcp=HASH_CP(cp)] == 0) { caps[is++] = cp ; isFound[hcp]=1 ; }
               }
               cp = SerialI1(caps[k1],caps[k2]) ; if(isFound[hcp=HASH_CP(cp)] == 0) { caps[is++] = cp ; isFound[hcp]=1 ; }
               cp = SerialI2(caps[k1],caps[k2]) ; if(isFound[hcp=HASH_CP(cp)] == 0) { caps[is++] = cp ; isFound[hcp]=1 ; }
               cp = SerialI12(caps[k1],caps[k2]) ; if(isFound[hcp=HASH_CP(cp)] == 0) { caps[is++] = cp ; isFound[hcp]=1 ; }
            }
         }
         
      }
      indCs[i+1] = is  ;
      SS +=   indCs[i+1] - indCs[i] ;
      printf("%d->s=%d,T=%d\n ",i,indCs[i+1]-indCs[i],2*SS-1);
   }
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",2*SS-1) ;
   
   return 1 ;
}

#define PB156_EXP    19

static inline int64_t nbxd(int64_t n,int digit) {
   int64_t pow10 = 1 ;
   int64_t nb = -n ;
   while(n >= pow10) {
      int64_t powNext = 10 * pow10 ;
      int64_t iq = (n / powNext) ;
      nb  += pow10*iq  ;
      iq = n - powNext * iq - digit* pow10 + 1 ; // n % 10*pow10 - (digit*pow10-1)
      if(iq > 0) {
         if(iq>pow10) {
            nb += pow10 ;
         } else {
            nb += iq ;
         }
      }
      pow10 = powNext ;
   }
   return nb ;
}


// f(10**10+n,d)=f(n,d)+10**10
// so if f(n,d)==n => f(10**10+n,d) == 10**10+n
// we can sho that no solution > 10**11

int PB156(PB_RESULT *pbR) {
   pbR->nbClock = clock() ;
   int digit ;
   int64_t is, Sd=0 ;
   for(digit=1;digit<=9;digit++){
      int nbF = 0 ;
      int64_t Si = 0 ;
      for(is=1;is<10000000000;){
         int64_t exps =nbxd(is,digit) ;
         if(exps > 0) {
            is += exps ;
         }  else if(exps< 0) {
            is += (-exps+9)/10 ;
         } else {
            Si += is ; nbF++ ;
            is++ ;
         }
            
      }
      if(digit > 1) nbF++ ;
      Sd += digit*Si+(digit-1)*digit/2*nbF*10000000000 ;
      if(pbR->isVerbose) fprintf(stdout,"\t PB%s S(%d)=%lld\n",pbR->ident,digit,digit*Si+(digit-1)*digit/2*nbF*10000000000);
   }
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%llu",Sd) ;
   return 1 ;
}

#define PB157_EXP 9
int PB157(PB_RESULT *pbR) {
   pbR->nbClock = clock() ;
   int64_t nbSol = 0 ;
   int pow2,pow5 ;
   int exp ;
   for(exp=1;exp<=PB157_EXP;exp++){
      int aIs1 ;
      for(aIs1=0;aIs1<=1;aIs1++) {
         int a,b;
         int N,D ;
         for(pow2=1-aIs1;pow2<=exp;pow2++) {
            if(aIs1==1) {
               a=1;
               b=1 << pow2 ; // 2**pow2b
            } else {
               a= 1 << pow2 ;
               b = 5 ;
            }
            for(pow5=1-aIs1;pow5<=exp;pow5++,b *=5) {
               N = a+b ;
               D = a*b ;
               int pow2r = exp - pow2 + 1 ;
               int pow5r = exp - pow5 + 1 ;
               {
                  int N1 ;
                  while(N1=N/2, N==2*N1) {
                     pow2r++ ; N = N1 ;
                  }
                  while(N1=N/5, N==2*N1) {
                     pow5r++ ; N = N1 ;
                  }
               }
              // divison for N
               int d ,nbDiv = 0;
               for(d=1;d*d<N;d++) {
                  if((N % d) == 0) nbDiv += 2 ;
               }
               if((N % d)==0) nbDiv++ ;
               nbSol += nbDiv * pow2r * pow5r ;
   //            printf("[%d,%d]->%d ",a,b, nbDiv * pow2r * pow5r);
            }
         }
      }
   }
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%llu",nbSol) ;
   return 1 ;
}

// par symetrie il suffit de trouver les permutations 1..n avec un seul increment Kn
// par position du 0 en 1ere éieme ... nieme on a K(n) = C(0,n-1)+C(1,n-1)+..(C(n-2,n-1)+K(n-1)
//  = 2 << (n-1) - 1 + K(n-1)
// => K(n) = 2 << n - (n+1)
// on multiplie apres par C(n,26)

int PB158(PB_RESULT *pbR) {
   pbR->nbClock = clock() ;
   uint64_t C =  26 * 25 /2;
   uint64_t F = C ;
   int n ;
   for(n=3;n<20;n++) {
      C = C * (26-n+1) / n ;
      uint64_t Fnext = C *  ((1<<n) -n-1);
      if(pbR->isVerbose) fprintf(stdout,"\t PB%s F(%d)=%lld\n",pbR->ident, n,Fnext);
      if(Fnext<F) break ;
      else F = Fnext ;
   }
     pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%llu",F) ;
   return 1 ;
}

#define PB159_MAX 1000000
#define mod9(p)   ((p) % 9)


int PB159(PB_RESULT *pbR) {
   int32_t S = 0 ;
   pbR->nbClock = clock() ;
   int sqM = Sqrt32(PB159_MAX)+1;
   
   CTX_PRIMETABLE * ctxP  ;
   if((ctxP = Gen_tablePrime(sqM)) == NULL) {
      fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
      return 0 ;
   }
   const T_prime * tbPrime = GetTbPrime(ctxP);
   int nbPrime = GetNbPrime(ctxP) ;
   int lastP = tbPrime[nbPrime-1] ;
   int8_t *modP = malloc(nbPrime*sizeof(modP[0])) ;
   int i ;
   for(i=0;i<nbPrime;i++) {
      int p = tbPrime[i] ;
      modP[i] = mod9(p) ;
//      printf("%d->%d ",p,modP[i]) ;
   }
   int8_t mod[9] ;
   for(i=2;i<PB159_MAX;i++) {
      int i1=i ;
      int ip,p ;
      memset(mod,0,sizeof(mod)) ;
      for(ip=0;i1>1 && ip < nbPrime ;ip++) {
         p = tbPrime[ip] ;
         if(p>i1) break ;
         while((i1 % p)==0) {
            i1 /= p ;
            mod[modP[ip]]++ ;
         }
      }
      if(i1 > 1) {
         mod[mod9(i1)]++ ;
      }
//    6 and 9 no possible value
      int s = mod[1]+ 5*mod[5]+7*mod[7]+8*mod[8] ;
      if(mod[3]) {
         s += (mod[3]/2) * 9 ;
         mod[3] &= 1 ;
      }
      if(mod[4]) {
         if(mod[4]<=mod[2]) {
            s += mod[4]*8 ;
            mod[2] -= mod[4] ;
         } else {
            s += mod[2]*8 + (mod[4]-mod[2])*4 ;
            mod[2] = 0 ;
         }
      }
      s += (mod[2]/3)*8 ;
      mod[2]-= 3*(mod[2]/3) ;
      if(mod[3] && mod[2]) {
         s += 6  ;
         mod[2]-- ;
      } else if(mod[3]) s+= 3 ;
      s += mod[2]*2  ;
      S += s ;
   }
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",S) ;
   return 1 ;
}

int PB159a(PB_RESULT *pbR) {
   int32_t S = 0 ;
   pbR->nbClock = clock() ;
   
   int8_t *maxDrs = malloc(PB159_MAX*sizeof(maxDrs[0])) ;
   int i,m9 ;
   
   for(m9=1;m9<=9;m9++) {
      for(i=m9;i<PB159_MAX;i+=9) {
         maxDrs[i]= m9 ;
      }
   }
   for(i=2;i<PB159_MAX;i++) {
      S += maxDrs[i] ;
      int j,k ;
      for(j=2*i,k=2; j<PB159_MAX;k++,j+=i) {
         int d = maxDrs[i]+maxDrs[k] ;
         if(d > maxDrs[j])maxDrs[j] = d ;
      }
   }
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",S) ;
   return 1 ;
}

int PB159b(PB_RESULT *pbR) {
   int32_t S = 0 ;
   pbR->nbClock = clock() ;
   
   int8_t *maxDrs = malloc(PB159_MAX*sizeof(maxDrs[0])) ;
   int i,m9 ;
   
   for(m9=1;m9<=9;m9++) {
      for(i=m9;i<PB159_MAX;i+=9) {
         maxDrs[i]= m9 ;
      }
   }
   int sqM = Sqrt32(PB159_MAX)+1;
   int *next = malloc(sqM*sizeof(next[0]));
   for(i=0;i<sqM;i++)next[i]= i ;
   int imax,n,m,k,j ;
   for(imax=1,n=9;n<PB159_MAX;n*=2) {
      m=2*n; if(m>=PB159_MAX) m=PB159_MAX-1;
      while((imax+1)*(imax+1)<=m) imax++;
      for(i=2;i<=imax;i++) {
         for(k=i*(j=next[i]);k<=m;j++,k+=i) {
            int d = maxDrs[i]+maxDrs[j] ;
            if(maxDrs[k]<d) maxDrs[k]=d;
         }
         next[i]=j;
      }
   }
   for(i=2;i<PB159_MAX;i++) S+=maxDrs[i];
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",S) ;
   return 1 ;
}

#define PB160_FACT   1000000000000LL
int PB160(PB_RESULT *pbR) {
   int32_t S = 0 ;
   pbR->nbClock = clock() ;
   int64_t pow2=2 ;
   int64_t exp2 = 0 ;
   while(pow2<=PB160_FACT) {
      exp2 += PB160_FACT / pow2 ;
      pow2 *=2 ;
   }
   int64_t pow5=5 ;
   int64_t exp5 = 0 ;
   while(pow5<=PB160_FACT) {
      exp5 += PB160_FACT / pow5 ;
      pow5 *=5 ;
   }
   printf("Exp2=%lld Exp5=%lld \n",exp2,exp5) ;
   
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",S) ;
   return 1 ;
}

int64_t Pow(int64_t a,int exp) {
   int64_t pow = 1;
   while(exp > 0) { pow *= a ; exp-- ; }
   return pow ;
}
int PB162(PB_RESULT *pbR) {
   int64_t S = 0 ;
   pbR->nbClock = clock() ;
   int len ;
   for(len=16;len>=3;len--) {
      S += 15*Pow(16,len-1) // any
      -Pow(15,len) // without 0
      -2*14*Pow(15,len-1) // without 1 or A
      +2*Pow(14,len) // without 01 or 0A
      +13*Pow(14,len-1) // without 1A
      -Pow(13,len) ; // without 01A
   }
   
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%llX",S) ;
   return 1 ;
}


#define I(i,j) ((i)*4+(j))

int CmpV(const void *e1,const void *e2){
   const uint64_t * u1= e1 ;
   const uint64_t * u2= e2 ;
   if(*u1 > *u2) return 1 ;
   else if (*u1 != *u2) return -1;
   else return 0 ;

}
int PB166(PB_RESULT *pbR) {
   int64_t N = 0 ;
   char * raw1[4] = { " X . . ."," . X . ."," . . X ."," . . . X" } ;
   char *raw[16] = {" . . . ."," X . . ."," . X . ."," X X . .",
      " . . X ."," X . X ."," . X X ."," X X X .",
      " . . . X"," X . . X"," . X . X"," X X . X",
      " . . X X"," X . X X"," . X X X"," X X X X"
   } ;
   pbR->nbClock = clock() ;
   
   uint64_t S = 0 ;
   {
      int ai,bi,ci,di,ei,fi,gi,hi,ii,ji,ki,li,mi,ni,oi,pi, Si ;
      for(ai=0;ai<=9;ai++) {
         for(bi=0;bi<=9;bi++) {
            for(ci=0;ci<=9;ci++) {
               for(di=0;di<=9;di++) {
                  Si = ai+bi+ci+di ;
                  for(fi=0;fi<=9;fi++) {
                     ni = Si -bi -fi ;
                     if(ni < 0) break ;
                     for(ji=0;ji<=9;) {
 //                       ni = Si - bi -fi - ji ;
                        if(ni<0) break ;
                        if(ni > 9) { ji += ni - 9 ; ni=9 ; continue ; }
                        
                        //  oi = Si - mi -ni -pi ; mi = bi + ci -pi
                        oi = Si -bi -ci -ni ;
                        if(oi < 0 || oi > 9) { ji++ ; ni-- ;  continue ; }
                        for(pi=0;pi<=9;pi++) {
                           ki = Si - ai -fi -pi ;
                           if(ki < 0) break ;
                           if(ki>9) { pi += ki -10 ; continue ; }
                           // oi = Si - mi -ni -pi  et oi = Si - ci -gi -ki et gi = Si - di -ji - mi
                           // => 2*mi = Si -ni -pi -di -ji +ci +ki
                           mi = bi + ci -pi ;
                           if(mi < 0) break ;
                           if(mi > 9) { pi += mi -10 ; continue ; }
                           gi = Si - di -ji - mi ;
                           if(gi > 9) break ;
                           if(gi< 0) continue ;
                           for(ei=0;ei<=9;ei++) {
                              ii = Si - ai - ei -mi  ;
                              if(ii<0) break ;
                              hi = Si -ei -fi -gi ;
                              if(hi < 0) break ;
                              if(ii>9 || hi > 9) continue ;
                              li = Si -di -hi -pi ;
                              if(li > 9) break ;
                              if(li < 0) continue ;
                              S++ ;
                           }
                        }
                        ji++ ; ni-- ;
                     }
                  }
                  
               }
            }
         }
      }
      printf("S=%lld \n",S) ;
   }
   
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",S) ;
   return 1 ;
   
   uint64_t j0,j1,j2,j3 ;
   int i0,i1,i2,i3 ;
   uint32_t x1[4]= {1,16,256,4096} ;
   uint64_t pat[12] ;
   int np = 0 ;
   for(i0=0;i0<4;i0++) {
      j0 = x1[i0] ;
      for(i1=0;i1<4;i1++) {
         j1 = x1[i1] ;
         for(i2=0;i2<4;i2++) {
            j2 = x1[i2] ;
            for(i3=0;i3<4;i3++) {
               j3 = x1[i3] ;
               int c0 = (j0 & 0xf) + (j1 & 0xf) + (j2 & 0xf) + (j3 & 0xf) ;
               if(c0 != 1) continue ;
               int c1 = (j0 & 0xf0) + (j1 & 0xf0) + (j2 & 0xf0) + (j3 & 0xf0) ;
               if(c1 != 0x10) continue ;
               int c2 = (j0 & 0xf00) + (j1 & 0xf00) + (j2 & 0xf00) + (j3 & 0xf00) ;
               if(c2 != 0x100) continue ;
               int d0 = (j0 & 0xf) + ((j1 & 0xf0)>>4) + ((j2 & 0xf00)>>8) + ((j3 & 0xf000)>>12) ;
               if(d0 != 1) continue ;
               int d1 = (j3 & 0xf) + ((j2 & 0xf0)>>4) + ((j1 & 0xf00)>>8) + ((j0 & 0xf000)>>12) ;
               if(d1==1) {
                  printf("%s\n%s\n%s\n%s\n\n",raw[(j0 & 1) + ((j0 & 0x10)>>3) + ((j0 & 0x100)>>6) + ((j0 & 0x1000)>>9)]
                         ,raw[(j1 & 1) + ((j1 & 0x10)>>3) + ((j1 & 0x100)>>6) + ((j1 & 0x1000)>>9)]
                         ,raw[(j2 & 1) + ((j2 & 0x10)>>3) + ((j2 & 0x100)>>6) + ((j2 & 0x1000)>>9)]
                         ,raw[(j3 & 1) + ((j3 & 0x10)>>3) + ((j3 & 0x100)>>6) + ((j3 & 0x1000)>>9)]);
                  uint64_t v = j0 + (j1<<16) + (j2<<32) + (j3<<48) ;
                  pat[np++] = v ;
               }
            }

         }
      }
   }
   uint32_t x2[6]= { 0x11,0x101,0x110,0x1001,0x1010,0x1100} ;
   for(i0=0;i0<6;i0++) {
      j0 = x2[i0] ;
      for(i1=0;i1<6;i1++) {
         j1 = x2[i1] ;
         for(i2=0;i2<6;i2++) {
            j2 = x2[i2] ;
            for(i3=0;i3<6;i3++) {
               j3 = x2[i3] ;
               int c0 = (j0 & 0xf) + (j1 & 0xf) + (j2 & 0xf) + (j3 & 0xf) ;
               if(c0 != 2) continue ;
               int c1 = (j0 & 0xf0) + (j1 & 0xf0) + (j2 & 0xf0) + (j3 & 0xf0) ;
               if(c1 != 0x20) continue ;
               int c2 = (j0 & 0xf00) + (j1 & 0xf00) + (j2 & 0xf00) + (j3 & 0xf00) ;
               if(c2 != 0x200) continue ;
               int d0 = (j0 & 0xf) + ((j1 & 0xf0)>>4) + ((j2 & 0xf00)>>8) + ((j3 & 0xf000)>>12) ;
               if(d0 != 2) continue ;
               int d1 = (j3 & 0xf) + ((j2 & 0xf0)>>4) + ((j1 & 0xf00)>>8) + ((j0 & 0xf000)>>12) ;
               if(d1==2) {
                  uint64_t v = j0 + (j1<<16) + (j2<<32) + (j3<<48) ;
                  int nbv1=0 ;
                  int k ; for(k=0;k<np;k++) {if((v & pat[k]) == pat[k]) nbv1++ ; }
                  if(nbv1==2)printf("Sum \n") ;
                  else {
                     pat[np++] = v ;
                     printf("%s\n%s\n%s\n%s\n\n",raw[(j0 & 1) + ((j0 & 0x10)>>3) + ((j0 & 0x100)>>6) + ((j0 & 0x1000)>>9)]
                            ,raw[(j1 & 1) + ((j1 & 0x10)>>3) + ((j1 & 0x100)>>6) + ((j1 & 0x1000)>>9)]
                            ,raw[(j2 & 1) + ((j2 & 0x10)>>3) + ((j2 & 0x100)>>6) + ((j2 & 0x1000)>>9)]
                            ,raw[(j3 & 1) + ((j3 & 0x10)>>3) + ((j3 & 0x100)>>6) + ((j3 & 0x1000)>>9)],v);
                  }
               }
            }
         }
      }
   }
   int i;
   int nbBycase[16] ;
   int indexByCase[16][8] ;
   for(i=0;i<16;i++) {
      nbBycase[i]=0 ;
      uint64_t ib = 1LL <<(4*i) ;
      int j ;
      for(j=0;j<np;j++) {
         if (pat[j] & ib) {
            indexByCase[i][nbBycase[i]++] = j ;
         }
      }
      printf("%d->",i); for(j=0;j<nbBycase[i];j++) printf(" %d",indexByCase[i][j]) ;
      printf("\n");
   }
   /*
    0-> 0 1 8 9
    1-> 2 3 8 10
    2-> 4 5 9 11
    3-> 6 7 10 11
    4-> 4 6 8 10
    5-> 5 7 10 11
    6-> 0 2 8 9
    7-> 1 3 9 11
    8-> 2 7 9 11
    9-> 1 4 8 9
    10-> 3 6 10 11
    11-> 0 5 8 10
    12-> 3 5 10 11
    13-> 0 6 9 11
    14-> 1 7 8 10
    15-> 2 4 8 9
   */
   int n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11;
   for(i=0;i<np;i++) printf("%016llx\n",pat[i]) ;
   printf("\n");
#define MAX1(maxi,nj)  if(9-(nj)<maxi) maxi = 9-(nj)
   int nb9 = 0 ;
  
   uint64_t *tbV = malloc(70000000*sizeof(tbV[0])) ;
   for(n0=0; n0<=9;n0++) {
      for(n1=0;n1<=9-n0;n1++) {
         for(n2=0;n2<=9-n0;n2++) {
            int maxn3 = 9 -n1;
            MAX1(maxn3,n2);
             for(n3=0;n3<=maxn3;n3++) {
               int maxn4 = 9 -n1;
               MAX1(maxn4,n2) ;
               for(n4=0;n4<=maxn4;n4++) {
                  int maxn5 = 9 -n4;
                  MAX1(maxn5,n0) ; MAX1(maxn5,n3) ;
                  for(n5=0;n5<=maxn5;n5++) {
                     int maxn6 = 9 -n4;
                     MAX1(maxn6,n0) ; MAX1(maxn6,n3) ;
                     for(n6=0;n6<=maxn6;n6++) {
                        int maxn7 = 9 -n6;
                        MAX1(maxn7,n5) ; MAX1(maxn7,n2) ;  MAX1(maxn7,n1) ;
                        if(n0 && n3 && n4) maxn7 = 0 ;
                        for(n7=0;n7<=maxn7;n7++) {
                           int maxn8 = 9-n0-n1;
                           MAX1(maxn8,n2+n3) ; MAX1(maxn8,n1+n4) ;  MAX1(maxn8,n0+n5) ;
                           MAX1(maxn8,n4+n6) ; MAX1(maxn8,n0+n2) ;  MAX1(maxn8,n1+n7) ; MAX1(maxn8,n2+n4) ;
                           maxn8 = 0 ;
                          for(n8=0;n8<=maxn8;n8++) {
                             int maxn9 = 9-n0-n1-n8;
                             MAX1(maxn9,n4+n5) ; MAX1(maxn9,n0+n2+n8) ;  MAX1(maxn9,n1+n3) ;
                             MAX1(maxn9,n2+n7) ; MAX1(maxn9,n0+n6) ;  MAX1(maxn9,n2+n4+n8) ;
                               maxn9 = 0 ;
                             if(n3 && n7 && n8) maxn9 = 0 ;
                             for(n9=0;n9<=maxn9;n9++) {
                                int maxn10 = 9-n2-n3-n8;
                                MAX1(maxn10,n6+n7) ; MAX1(maxn10,n4+n6+n8) ;  MAX1(maxn10,n5+n7) ;
                                MAX1(maxn10,n3+n6) ; MAX1(maxn10,n3+n5) ;  MAX1(maxn10,n1+n7+n8) ;
                                maxn10 = 0 ;
                                if(n9) maxn10=0 ;
                                for(n10=0;n10<=maxn10;n10++) {
                                   int maxn11 = 9-n4-n5-n9;
                                   MAX1(maxn11,n6+n7+n10) ; MAX1(maxn11,n5+n7+n10) ;  MAX1(maxn11,n1+n3+n9) ;
                                   MAX1(maxn11,n2+n7+n9) ; MAX1(maxn11,n3+n6+n10) ;  MAX1(maxn11,n3+n5+n10) ; MAX1(maxn11,n0+n6+n9) ;
                                   if(n8) maxn11 = 0 ;
                                   if(n0 && n4 && n10)maxn11 = 0 ;
                                   maxn11 = 0 ;
                                   for(n11=0;n11<=maxn11;n11++) {
                                      uint64_t v = n0*pat[0]+n1*pat[1]+n2*pat[2]+n3*pat[3]+n4*pat[4]+n5*pat[5]
                                      +n6*pat[6]+n7*pat[7]+n8*pat[8]+n9*pat[9]+n10*pat[10]+n11*pat[11] ;
                                      if(v==0x9999999999999999LL){ nb9++ ; printf("%d%d%d%d,%d%d%d%d,%d%d,%d%d ",n0,n3,n4,n7,n1,n2,n5,n6,n8,n11,n9,n10) ; }
//                                      if(v==0x2222222222222222LL){ nb9++ ; printf("%d%d%d%d,%d%d%d%d,%d%d,%d%d ",n0,n3,n4,n7,n1,n2,n5,n6,n8,n11,n9,n10) ; }
//                                      if(v==0x3333333333333333LL){ nb9++ ; printf("%d%d%d%d,%d%d%d%d,%d%d,%d%d ",n0,n3,n4,n7,n1,n2,n5,n6,n8,n11,n9,n10) ; }
                                      tbV[N++] = v ;
                                   }
                                }
                             }
                          }
                       }
                     }
                  }
               }
            }
         }
      }
   }

  int64_t nbEq= 0 ;
  qsort(tbV,N,sizeof(tbV[0]),CmpV) ;
   for(i=1;i<N;i++) {
      if(tbV[i-1] == tbV[i]) nbEq++ ;
   }
   printf("N=%lld nbEQ=%d\n",N,nbEq);

   return 1 ;
}


int PB164(PB_RESULT *pbR) {
   pbR->nbClock = clock() ;
   uint64_t n00=0,n01=0,n02=0,n03=0,n04=0,n05=0,n06=0,n07=0,n08=0,n09=0 ;
   uint64_t n10=1,n11=1,n12=1,n13=1,n14=1,n15=1,n16=1,n17=1,n18=1 ;
   uint64_t n20=1,n21=1,n22=1,n23=1,n24=1,n25=1,n26=1,n27=1;
   uint64_t n30=1,n31=1,n32=1,n33=1,n34=1,n35=1,n36=1;
   uint64_t n40=1,n41=1,n42=1,n43=1,n44=1,n45=1;
   uint64_t n50=1,n51=1,n52=1,n53=1,n54=1;
   uint64_t n60=1,n61=1,n62=1,n63=1;
   uint64_t n70=1,n71=1,n72=1;
   uint64_t n80=1,n81=1;
   uint64_t n90=1;
   int i ;
   for(i=3;i<=20;i++) {
      uint64_t m00 = n00+n10+n20+n30+n40+n50+n60+n70+n80+n90 ;
      uint64_t m01 = n00+n10+n20+n30+n40+n50+n60+n70+n80 ;
      uint64_t m02 = n00+n10+n20+n30+n40+n50+n60+n70 ;
      uint64_t m03 = n00+n10+n20+n30+n40+n50+n60 ;
      uint64_t m04 = n00+n10+n20+n30+n40+n50 ;
      uint64_t m05 = n00+n10+n20+n30+n40 ;
      uint64_t m06 = n00+n10+n20+n30 ;
      uint64_t m07 = n00+n10+n20 ;
      uint64_t m08 = n00+n10 ;
      uint64_t m09 = n00 ;

      uint64_t m10 = n01+n11+n21+n31+n41+n51+n61+n71+n81 ;
      uint64_t m11 = n01+n11+n21+n31+n41+n51+n61+n71 ;
      uint64_t m12 = n01+n11+n21+n31+n41+n51+n61 ;
      uint64_t m13 = n01+n11+n21+n31+n41+n51 ;
      uint64_t m14 = n01+n11+n21+n31+n41 ;
      uint64_t m15 = n01+n11+n21+n31 ;
      uint64_t m16 = n01+n11+n21 ;
      uint64_t m17 = n01+n11 ;
      uint64_t m18 = n01 ;

      uint64_t m20 = n02+n12+n22+n32+n42+n52+n62+n72 ;
      uint64_t m21 = n02+n12+n22+n32+n42+n52+n62 ;
      uint64_t m22 = n02+n12+n22+n32+n42+n52 ;
      uint64_t m23 = n02+n12+n22+n32+n42 ;
      uint64_t m24 = n02+n12+n22+n32 ;
      uint64_t m25 = n02+n12+n22 ;
      uint64_t m26 = n02+n12 ;
      uint64_t m27 = n02 ;

      uint64_t m30 = n03+n13+n23+n33+n43+n53+n63 ;
      uint64_t m31 = n03+n13+n23+n33+n43+n53 ;
      uint64_t m32 = n03+n13+n23+n33+n43 ;
      uint64_t m33 = n03+n13+n23+n33 ;
      uint64_t m34 = n03+n13+n23 ;
      uint64_t m35 = n03+n13 ;
      uint64_t m36 = n03 ;

      uint64_t m40 = n04+n14+n24+n34+n44+n54 ;
      uint64_t m41 = n04+n14+n24+n34+n44 ;
      uint64_t m42 = n04+n14+n24+n34 ;
      uint64_t m43 = n04+n14+n24 ;
      uint64_t m44 = n04+n14 ;
      uint64_t m45 = n04 ;

      uint64_t m50 = n05+n15+n25+n35+n45 ;
      uint64_t m51 = n05+n15+n25+n35 ;
      uint64_t m52 = n05+n15+n25 ;
      uint64_t m53 = n05+n15 ;
      uint64_t m54 = n05 ;

      uint64_t m60 = n06+n16+n26+n36 ;
      uint64_t m61 = n06+n16+n26 ;
      uint64_t m62 = n06+n16 ;
      uint64_t m63 = n06 ;

      uint64_t m70 = n07+n17+n27 ;
      uint64_t m71 = n07+n17 ;
      uint64_t m72 = n07 ;

      uint64_t m80 = n08+n18 ;
      uint64_t m81 = n08 ;

      uint64_t m90 = n09 ;

      n00=m00 ; n01=m01 ;n02=m02 ;n03=m03 ;n04=m04 ;n05=m05 ;n06=m06 ;n07=m07 ;n08=m08 ;n09=m09 ;
      n10=m10 ; n11=m11 ;n12=m12 ;n13=m13 ;n14=m14 ;n15=m15 ;n16=m16 ;n17=m17 ;n18=m18 ;
      n20=m20 ; n21=m21 ;n22=m22 ;n23=m23 ;n24=m24 ;n25=m25 ;n26=m26 ;n27=m27 ;
      n30=m30 ; n31=m31 ;n32=m32 ;n33=m33 ;n34=m34 ;n35=m35 ;n36=m36 ;
      n40=m40 ; n41=m41 ;n42=m42 ;n43=m43 ;n44=m44 ;n45=m45 ;
      n50=m50 ; n51=m51 ;n52=m52 ;n53=m53 ;n54=m54 ;
      n60=m60 ; n61=m61 ;n62=m62 ;n63=m63 ;
      n70=m70 ; n71=m71 ;n72=m72 ;
      n80=m80 ; n81=m81 ;
      n90=m90 ;
   }
   uint64_t S = n00+n01+n02+n03+n04+n05+n06+n07+n08+n09
     + n10+n11+n12+n13+n14+n15+n16+n17+n18
     + n20+n21+n22+n23+n24+n25+n26+n27
     + n30+n31+n32+n33+n34+n35+n36
     + n40+n41+n42+n43+n44+n45
     + n50+n51+n52+n53+n54
     + n60+n61+n62+n63
     + n70+n71+n72
     + n80+n81
     + n90 ;
   pbR->nbClock = clock() - pbR->nbClock ;
   snprintf(pbR->strRes, sizeof(pbR->strRes),"%llu",S) ;
   return 1 ;
}


// nb(tiles) = 4*n*p with n>p
// so count multiples k*p from k=p+1 to k<= N/p
#define PB173_NB    1000000
int PB173(PB_RESULT *pbR) {
    int N = PB173_NB/4 ;
    pbR->nbClock = clock() ;
    int n ;
    int64_t S = 0 ;
    S += N-1 ; // product 1xn
    for(n=2;n*(n+1)<=N;n++) {
        S += N/n - n ;
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",S) ;
    return 1 ;
}

#define PB174_NB    1000000
// #define PB174_NB    100

int PB174(PB_RESULT *pbR) {
   int N = PB174_NB/ 4 ;
    pbR->nbClock = clock() ;
    uint8_t *nbProd=malloc((N+1)*sizeof(nbProd[0])) ;
    int n ;
    for(n=2;n<=N;n++) { nbProd[n] = 1 ; }
    for(n=2;n*n<N;n++) {
        int np ;
        for(np=n*(n+1);np<=N;np+=n) { nbProd[np]++ ; }
    }
    int S = 0 ;
    for(n=2;n<=N;n++) { if(nbProd[n]<=10)S++ ;  }
    free(nbProd) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",S) ;
    return 1 ;
}
// #define PB179_NB    1000
#define PB179_NB    10000000

int PB179(PB_RESULT *pbR) {
    int N =  PB179_NB ;
    pbR->nbClock = clock() ;
    uint16_t *nbDiv=malloc((N+2)*sizeof(nbDiv[0])) ;
    int n,d ;
    nbDiv[2] = nbDiv[3] = 2 ;
    for(n=4;n<=N;n+=2) { // d=2
        nbDiv[n] = 4 ;
        nbDiv[n+1] = 2 ;
    }
    int dMax = Sqrt32(N);
    for(d=3;d<=dMax;d++) { // d > 2
        int np = d*d ;
        nbDiv[np]++ ;
        for(np += d ;np<=N;np+=d) {
            nbDiv[np] += 2 ;
            
        }
    }
    int S = 0 ;
    for(n=2;n<N;n++) {
        if(nbDiv[n]==nbDiv[n+1] ) {
            S++ ;
        }
        
    }
    free(nbDiv) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",S) ;
    return 1 ;
}

int PB179c(PB_RESULT *pbR) {
    int N =  PB179_NB ;
    pbR->nbClock = clock() ;
    int dMax = Sqrt32(N) ;
    int sizeCache = 16384 ;
    sizeCache += sizeCache & 1 ; // sizeCache even
    uint16_t *nbDiv=malloc((sizeCache)*sizeof(nbDiv[0])) ; // number of divisor
    int32_t *indNxtFactor = malloc((dMax+1)*sizeof(indNxtFactor[0])) ; // indice of nxt multiple
    int n,d ;
    int offset = 6 ; // offset for the current cache pof nbDiv
    for(d=3;d<=dMax;d++) { // if n = d1*d2 d1<=d2
        indNxtFactor[d] = d*d - offset ;
    }
    int nbDivAnt = 2 ; // n=5
    int S = 1 ; // (2,3)
    for(;offset < N;offset += sizeCache ) { // loop, by moving the cache
         for(n=0;n<sizeCache;n+=2) { // special case for d=2
            nbDiv[n] = 4 ; // n even
            nbDiv[n+1] = 2 ; // n odd
        }
        int indCacheMax = sizeCache ;
        if(offset+sizeCache >N+1) { // check if last cache move
            indCacheMax = N+1 - offset ;
        }
        for(d=3;d<=dMax;d++) { // d=3,...dMax
            int np = indNxtFactor[d] ;
            if(np < indCacheMax) { //
                if(np == d*d - offset ) { // square
                    nbDiv[np]++ ; np += d ;
                }
                for(;np<indCacheMax;np += d ) {
                    nbDiv[np] += 2 ; // add product, 2 divisors
                }
            }
            indNxtFactor[d] = np - sizeCache  ;
        }
        for(n=0;n< indCacheMax ; n++) { // check nbDiv[n] == nbDiv[n-1]
            if(nbDiv[n]==nbDivAnt) S++ ;
            nbDivAnt = nbDiv[n] ;
        }
    }
    free(nbDiv) ; free(indNxtFactor) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",S) ;
    return 1 ;
}


int PB179a(PB_RESULT *pbR) {
    int N =  PB179_NB ;
    pbR->nbClock = clock() ;
    uint16_t *nbDiv=malloc((N+1)*sizeof(nbDiv[0])) ;
    int n ;
    // pow of 2
    for(n=2;n<=N;n++) {
        nbDiv[n] = 2 - (n&1);
    }
    int pow2 ;
    for(pow2=4;pow2<=N;pow2 *=2){
        for(n=pow2;n<=N;n+=pow2) nbDiv[n]++ ;
    }
    int p ;
    int maxP = Sqrt32(N) ;
    for(p=3;p<=N;p+=2) {
        if(nbDiv[p]>1) continue ;
        int k ;
        for(n=p,k=1;n<=N;n+=p, k++) {
            if(k<p) {
                nbDiv[n] *= 2 ;
            } else {
                k = 0 ;
            }
        }
        if(p<=maxP) {
            int powp ,exp1 ;
            int maxPowp = N/ p ;
            for(powp=p*p,exp1=3;;exp1++,powp *=p) {
                for(n=(int)powp,k=1;n<=N;n+=(int)powp, k++) {
                    if(k<p) {
                        nbDiv[n] *= exp1 ;
                    } else {
                        k = 0 ;
                    }
                }
                if(powp>maxPowp) break ;
            }
        }
    }
    int S = 0 ;
    for(n=2;n<N;n++) {
       if(nbDiv[n]==nbDiv[n+1] ) {
            S++ ;
        }
        
    }
    free(nbDiv) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",S) ;
    return 1 ;
}

int PB179b(PB_RESULT *pbR) {
    int N =  PB179_NB ;
    pbR->nbClock = clock() ;
    uint16_t *pDivMax=malloc((N+1)*sizeof(pDivMax[0])) ;
    int n ;
    
    pDivMax[1] = 1 ;
    for(n=2;n<=N;n++) { // case d= 2
        pDivMax[n] = (n&1) ? 0 : 2 ;
    }
    int p ;
    int maxP = Sqrt32(N) ;
    // search max prime divisor for each n
    for(p=3;p<=maxP;p+=2) { // loop for p even
        if(pDivMax[p]) continue ;
        for(n=p;n<=N;n+=p) { // multiple of
            pDivMax[n] = p ;
        }
    }
    for(n=3;n<=N;n++) {
        if(n == pDivMax[n] || (pDivMax[n] == 0) ) {
            // n , case n< maxP and n > maxP
            pDivMax[n] = 2 ;
        } else {
            p = pDivMax[n] ;
            int d,d1,exp1 ; // search max pow of p
            for(d=n/p,exp1=2;d1=d/p,d==d1*p;d = d1) {
                exp1++ ;
            }
            pDivMax[n] = pDivMax[d]*exp1 ; // as d and p prime
        }
    }
    int S = 0 ;
    for(n=2;n<N;n++) {
        if(pDivMax[n]==pDivMax[n+1] ) {
            S++ ;
        }
    }
    free(pDivMax) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",S) ;
    return 1 ;
}

#define PB181_NB 60
#define PB181_NW 40
#define indBW(nb,nw)    ((nb)*(PB181_NW+1)+(nw))

#define P2(nb,nw)   xP2[indBW((nb),(nw))]
#define PW2(nb,nw)   xPW2[indBW((nb),(nw))]
#define M2(nb,nw)   xM2[indBW((nb),(nw))]

// first compute number of decomposition for one color
// (developpment for Prod(1/(1-x**n)) = (1+x+x**2+x**3+...) (1+x**2+x**4+x**6...) ...(1+x**n+x**2n+x**3n...)
// with Pentagonal number theorem (Euler)
// For 2 colors it is the developpement P2(x,y) = Prod(1/1-x**m . y**n) (n,m != (0,0) )
// the trick is to remark that :
// the elements of a decomposition are in 2 separate class.
//  a) nb(B) >= nb(W)
//  b) nb(B) < nb(W)
// Un element of class a) can be decomposed in BW and B, so the class is generated by P2(x,xy)
// Class B is generated by PW2(xy,y) = Prod(1/1-x**m . y**n) (n >0 , m >= 0 )
// so first compute PW2(x,y) by DP (see PB181a) and then
// compute P2(x,y) = P2(x,xy) * PW2(xy,y)
// the gain
int PB181(PB_RESULT *pbR) {
    u_int64_t xP1[PB181_NB+1] ;
    u_int64_t xP2[(1+PB181_NB)*(1+PB181_NW)] ;
    u_int64_t xPW2[(1+PB181_NW)*(1+PB181_NW)] ;

    int m,n ;
    pbR->nbClock = clock() ;
    xP1[0] = xP1[1] = 1 ;
    for(n=2;n<=PB181_NB;n++) {
        int k  ,Pk = 1  ;
        u_int64_t P = 0 ;
        for(k=1; Pk <= n ; Pk += 3*k+1, k++ ) {
            if(k&1) {
                P += xP1[n - Pk] ;
                // car P-k = Pk+k
                if(Pk + k <= n ) P += xP1[n - Pk - k ] ;
            } else {
                P -= xP1[n - Pk] ;
                if(Pk + k <= n ) P -= xP1[n - Pk - k ] ;
            }
        }
        xP1[n] = P ;
    }
    
    for (m = 0; m <= PB181_NW; m++) {
        for (n = 0; n <= PB181_NW-m; n++) {
            PW2(m,n)=0 ;
        }
    }
    
    PW2(0,0)=1 ;
    for (m = 0; m < PB181_NW; m++) {
        // n=1 to force a W
        for (n = 1 ; n <= PB181_NW-m; n++) {
            int i,j ;
            PW2(m,n)++;
            for (i = m; i < PB181_NW; i++) {
                // n+1 for because j-èn must be strictly positive
                for (j = n+1; j <= PB181_NW-i; j++) {
                    PW2(i,j) += PW2(i - m,j - n);
                }
            }
        }
    }
    int nb ;
    for(nb=1;nb<=PB181_NB;nb++) {
        P2(nb,0) = xP1[nb] ;
        if(nb<=PB181_NW) P2(0,nb)=xP1[nb] ;
    }
    P2(0,0) = 1 ;

    for(m=1;m<=PB181_NB;m++) {
        int maxN = (m < PB181_NW ) ? m : PB181_NW ;
        for(n=1;n<=maxN;n++) {
            int m2,n1;
            u_int64_t P = P2(m-n,n) ;
            for(n1=0;n1<n;n1++) {
               for(m2=0;m2+n1<n;m2++) {
                  P += P2(m-m2-n1,n1)*PW2(m2,n-n1-m2) ;
               }
            }
            P2(m,n) = P ;
            if(m<=PB181_NW) P2(n,m) = P ;
        }
    }
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%llu",P2(PB181_NB,PB181_NW)) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}


// Partition of multiset
// par recurrence sur la taille des sous-ensembles de (m+n) elements (mB,nW)
// que l'on ajoute aux denombrement des partitions ne contenant que des sous-ensembles de taille inferieure.
int PB181a(PB_RESULT *pbR) {
    u_int64_t xP2[(1+PB181_NB)*(1+PB181_NW)] ;
    int m,n;
    pbR->nbClock = clock() ;
    for (m = 0; m <= PB181_NB; m++) {
        for (n = 0; n <= PB181_NW; n++) {
            P2(m,n)=0 ;
        }
    }
    P2(0,0)=1 ;
    for (n = 0; n <= PB181_NW; n++) {
        for (m = (n) ? 0 : 1 ; m <= PB181_NB; m++) {
            int i,j ;
            for (j = n; j <= PB181_NW; j++) {
                for (i = m; i <= PB181_NB; i++) {
                    P2(i,j) += P2(i - m,j - n);
                }
            }
        }
    }
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%llu",P2(PB181_NB,PB181_NW)) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

//#define PB187_MAX   2000000000
#define PB187_MAX   100000000
int PB187(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    CTX_PRIMETABLE * ctxP  ;
    if((ctxP = Gen_tablePrime(PB187_MAX/2)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    const T_prime * tbPrime = GetTbPrime(ctxP);
    int nbPrime = GetNbPrime(ctxP) ;
    int nbFind =0 ;
    int i,j;
    nbFind += nbPrime ; // 2 * pi
    for(i=1;i<nbPrime;i++) {
        int maxPj = PB187_MAX/ tbPrime[i] ;
        for(j=i;tbPrime[j] <= maxPj;j++)  ;
        nbFind += j-i ;
    }
    Free_tablePrime(ctxP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbFind) ;
    return 1 ;
}
int PB187a(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    CTX_PRIMETABLE * ctxP  ;
    if((ctxP = Gen_tablePrime(PB187_MAX/2)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    const T_prime * tbPrime = GetTbPrime(ctxP);
    int nbPrime = GetNbPrime(ctxP) ;
    int nbFind =0 ;
    int i;
    int maxPi = Sqrt32(PB187_MAX) ;
    int pi ;
    nbFind += nbPrime ; // 2 * pi
 //   printf("%d ",nbPrime);

    int j = nbPrime - 1 ;
    for(i=1;(pi=tbPrime[i])<= maxPi ;i++) {
        int maxPj = PB187_MAX/ tbPrime[i] ;
        for(;tbPrime[j] > maxPj;j--)  ;
 //       printf("%d ",j+1);
        nbFind += j-i+1 ;
    }
    Free_tablePrime(ctxP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbFind) ;
    return 1 ;
}
//
// implantation de l'algo d'euler pour caluler PI(x)  S(x)= x - Sigma(X/pi) +  Sigma(X/pi pj) ...
// S(x) = PI(x) - PI(sqrt(x))+ 1 ; les pi <= sqrt(x)
// n'est rentable qua partir de 10**9
int PB187b(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    CTX_PRIMETABLE * ctxP  ;
    int maxPi = Sqrt32(PB187_MAX)+1 ;
    if((ctxP = Gen_tablePrime(maxPi)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    const T_prime * tbPrime = GetTbPrime(ctxP);
    int nbPrime = GetNbPrime(ctxP) ;
    int nbFind =0 ;
    int i;
    int Pi ;
 //   nbFind += nbPrime ; // 2 * pi,
    int nbM = nbPrime -1 ;
    for(i=0;i<nbPrime ;i++) {
        Pi=tbPrime[i] ;
        int invPi = PB187_MAX/ Pi ;
        // on veut calculer pi(invPi)
        int PIinvPi = invPi ;
        int sqInvPi = Sqrt32(invPi) ;
        for(;tbPrime[nbM]>sqInvPi;nbM--) ;
        const T_prime *pt1,*pt2,*pt3,*pt4,*pt5,*pt6,*pt7,*pt8 ;
        const T_prime *ptnbM = tbPrime+nbM ;
        int32_t p1,p2,p3,p4,p5,p6,p7 ;
        
        for(pt1=tbPrime;pt1<=ptnbM && (p1=  invPi / *pt1);pt1++) {
            PIinvPi -= p1 ;
            for(pt2=pt1+1;pt2<=ptnbM &&(p1 >= *pt2);pt2++) {
                PIinvPi += (p2 = p1 / *pt2 )  ;
                for(pt3=pt2+1;pt3<=ptnbM &&(p2 >= *pt3);pt3++) {
                    PIinvPi -=  (p3  = p2 / *pt3)  ;
                    for(pt4=pt3+1;pt4<=ptnbM &&(p3 >= *pt4);pt4++) {
                        PIinvPi +=  (p4 = p3 / *pt4) ;
                        for(pt5=pt4+1;pt5<=ptnbM && (p4 >= *pt5);pt5++) {
                            PIinvPi -=  (p5 = p4 / *pt5 ) ;
                            for(pt6=pt5+1;pt6<=ptnbM && (p5 >= *pt6);pt6++) {
                                PIinvPi += (p6 = p5 / *pt6 )  ;
                                for(pt7=pt6+1;pt7<=ptnbM && (p6 >= *pt7);pt7++) {
                                    PIinvPi -=  (p7 = p6 / *pt7 ) ;
                                    for(pt8=pt7+1;pt7<=ptnbM && (p7 >= *pt8);pt8++) {
                                        PIinvPi +=  p7 / *pt8 ;                                     }
                                }
                            }
                        }
                    }
                }
            }
        }
 //       printf("PI(%d)=%d\n",invPi,PIinvPi+nbM);
        nbFind += PIinvPi+nbM - i  ; // -i +i PI(i)
    }
    Free_tablePrime(ctxP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbFind) ;
    return 1 ;
}

#define PB188_VAL   1777
#define PB188_EXP   1855
#define PB188_EXP10 8

// use Euler's totient function
// if a^n = 1  a**(EPhi(n)) =1 Mod[n]
//EPhi(n) = n Prod (1-1/p)  p divide n.
// For m0= n = 10**8 Ephi(10**8) = 10**8 x 1/2 x 4/5 = 2**8 * 5**8 x 2/5 = 2**9 * 5**7 = 20000000
// Donc le plus petit m1 tel que a**m1=1 [m0] est un diviseur de 20000000, on trouve m1=1250000
// En suite on cherche le plus petit des diviseur de Ephi[125000] = 1250000 * 4/5 = 1000000
// tel que a**m2=1 [m1]
//
u_int64_t modPow(int64_t exp,u_int64_t mod) {
    u_int64_t modPOW2 = PB188_VAL ;
    u_int64_t answer = (exp & 1) ? (PB188_VAL % mod) : 1 ;
    int i ;
    for(i=1;exp >= (1LL<<i) ;i++) {
        modPOW2 = (modPOW2 * modPOW2) % mod ;
        if( (1LL<<i) & exp) {
            answer = (answer * modPOW2 ) % mod  ;
        }
    }
    return answer ;
}
int PB188(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int nbMod =0 ;
    u_int64_t Pn,Dn ;
    u_int64_t Pn1,Dn1,rop ;
    int nbPow2 = 2 ;
    int nbPow5 = 0 ;
    Pn1= 0 ;
    Pn = 1<<nbPow2 ;
    Dn1=1 ;
    Dn= modPow (Dn1,Pn) ;
    rop= modPow (Pn1,Pn) ;
    fprintf(stdout,"(%d,%d) 1777**%lld %% %lld =%lld  ; 1777**%lld %% %lld = %lld \n",nbPow2,nbPow5,Pn1,Pn,rop,Dn1,Pn,Dn);

    while(nbPow2 < PB188_EXP10 || nbPow5 < PB188_EXP10) {
        Dn1=Dn ;
        Pn1=Pn ;
        nbPow5++ ;
        int deltaPow2 = (nbPow2+4 < PB188_EXP10) ? 4 : PB188_EXP10-nbPow2 ;
        nbPow2 += deltaPow2 ;
        Pn = Pn * 5 * (1<<deltaPow2) ;
        Dn=modPow(Dn,Pn) ;
        nbMod++ ;
        rop=modPow(Pn1,Pn) ;
        fprintf(stdout,"(%d,%d) 1777**%lld %% %lld =%lld  ; 1777**%lld %% %lld = %lld \n",nbPow2,nbPow5,Pn1,Pn,rop,Dn1,Pn,Dn);
        
    }
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s Exp=%d (%d)digits=%lld \n",pbR->ident,nbMod,PB188_EXP10,Dn);
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%llu",Dn % 100000000 ) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}






#define PB191_LEN   30
#define PB191_NBLL  12
typedef enum LL2 {
  OO_0,OA_0,AO_0,AA_0,OO_1,OA_1,AO_1,AA_1,LA,LO,AL,OL
} LL2 ;


int PB191(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbChains[PB191_NBLL]  ,newNb[PB191_NBLL];
    int i ;
    nbChains[OO_0]=1; nbChains[OA_0]=1;nbChains[AO_0]=1;nbChains[AA_0]=1;
    nbChains[OO_1]=0; nbChains[OA_1]=0;nbChains[AO_1]=0;nbChains[AA_1]=0;
    nbChains[LA]=1; nbChains[AL]=1;nbChains[LO]=1;nbChains[OL]=1;
    for(i=2;i<PB191_LEN;i++) {
        newNb[OO_0] = nbChains[OO_0] + nbChains[AO_0] ;
        newNb[OO_1] = nbChains[OO_1] + nbChains[AO_1] + nbChains[LO] ;

        newNb[AA_0] = nbChains[OA_0] ;
        newNb[AA_1] = nbChains[OA_1] + nbChains[LA] ;

        newNb[OA_0] = nbChains[OO_0] + nbChains[AO_0] ;
        newNb[OA_1] = nbChains[OO_1] + nbChains[AO_1] + nbChains[LO] ;

        newNb[AO_0] = nbChains[AA_0] + nbChains[OA_0] ;
        newNb[AO_1] = nbChains[AA_1] + nbChains[OA_1] + nbChains[LA] ;
        
        newNb[LA] = nbChains[AL] + nbChains[OL];
        newNb[LO] = nbChains[AL] + nbChains[OL] ;

        newNb[AL] = nbChains[OA_0] + nbChains[AA_0];
        newNb[OL] = nbChains[OO_0] + nbChains[AO_0];

        memcpy(nbChains,newNb,sizeof(nbChains)) ;
    }
    int64_t nbTot = 0;
    for(i=0;i<PB191_NBLL;i++) {
        nbTot += nbChains[i] ;
    }
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s NbChains=%lld\n",pbR->ident,nbTot) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbTot) ;
    return 1 ;
}

typedef enum LL2a {
    A0_0,A0_1,A1_0,A1_1,A2_0,A2_1
} LL2a ;

#define PB191_NBLLa  6

int PB191a(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbChains[PB191_NBLLa]  ,newNb[PB191_NBLLa];
    int i ;
    for(i=0;i<PB191_NBLLa;i++) nbChains[i] = 0 ;
    nbChains[A0_0]=1;
    for(i=0;i<PB191_LEN;i++) {
        newNb[A0_0] = nbChains[A0_0] + nbChains[A1_0] + nbChains[A2_0] ; //  rajout O
        newNb[A0_1] = nbChains[A0_1] + nbChains[A1_1] + nbChains[A2_1]    // rejout O
            +  nbChains[A0_0] + nbChains[A1_0] + nbChains[A2_0] ; // rajout L
        newNb[A1_0] = nbChains[A0_0] ;
        newNb[A1_1] = nbChains[A0_1] ;
        newNb[A2_0] = nbChains[A1_0] ;
        newNb[A2_1] = nbChains[A1_1] ;
        memcpy(nbChains,newNb,sizeof(nbChains)) ;
    }
    int64_t nbTot = 0;
    for(i=0;i<PB191_NBLLa;i++) {
        nbTot += nbChains[i] ;
    }
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s NbChains=%lld\n",pbR->ident,nbTot) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbTot) ;
    return 1 ;
}


#define PB192_MAXN  100000
#define PB192_PREC    1000000000000LL
//#define PB192_PREC  1000000000000LL
#define PB192_HALF  1000LL
//#define PB192_HALF  1000000LL
//#define PB192_PREC  100LL

#define HIGH_PREC_192   0


int PB192(PB_RESULT *pbR) {
    int32_t N , a0, a2;
    pbR->nbClock = clock()  ;
    int64_t Sum = 0 ;
    for(N=2,a0=1,a2=4;N<=PB192_MAXN;N++) {
        int32_t n , d ;
        int64_t p0,q0,p1,q1,p2,q2,pk,qk ;
        int32_t a ;
        if(N == a2) { // a2 = (a0+1)*(a0+1)
            a0++ ;
            a2 += 2*a0 + 1 ; continue ;
        }
        // compute the convergent for sqrt(N)
        // in place with 3 consecutives p0/q0 p1/q1 p2/q2
        a = a0 ; d=1 ;  n = 0 ; // so k0 =(int) srqt(N)
        p1=1 ; q1=0;
        p2=a ; q2 = 1 ;
        do {
            n = d * a - n ;
            d = (N - n*n) / d ;
            a = (a0+n) / d ;
            p0 = p1 ;  p1 = p2 ;  p2 = a*p1 + p0  ;
            q0 = q1 ; q1 = q2 ;   q2 =a*q1 + q0 ;
        } while( q2 <= PB192_PREC) ;
        // p2/q2 exceed precision. p1/q1 is the last convergent OK
        // must test if pk/qk = (p0+k*p1)/(q0+k*q1) is better
        // with k max value not ot excced precision
       int64_t k = (PB192_PREC - q0) / q1 ;
        if(k == 0) {
            pk = p1 ;
            qk = q1 ;
        } else {
            pk = p0 + k * p1 ;
            qk = q0 + k * q1 ;
            if(2*k < a) {
                pk = p1 ;
                qk = q1 ;
            } else if( 2*k==a){
                // compute the remaining convergent np1/nq1 , np2/nq2
                // and compare to q1/q0 ( q(n)/q(n-1) )
                // comparaison depend on parity as convergents alternate.
                n = d * a - n ;
                d = (N - n*n) / d ;
                a = (a0+n) / d ;
                int64_t np1 = 1 , nq1 = 0 ;
                int64_t np2 = a , nq2 = 1 ;
                int is = 1 ;
                do {
                    n = d * a - n ;
                    d = (N- n*n) / d ;
                    a = (a0+n) / d ;
                    
                    int64_t tmp = np1 ;
                    np1 = np2 ;
                    np2 = a*np2 + tmp ;
                    
                    tmp = nq1 ;
                    nq1 = nq2 ;
                    nq2 =a*nq2 + tmp ;
                    
                    is = -is ; // parity
                } while((np2*q0-nq2*q1) * is < 0 ) ; // test loop on (n,d) = (k0,1)first couple
                if(is < 0) {
                    pk = p1 ;
                    qk = q1 ;
                }
            }
        }
        Sum += qk ;
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",Sum);
    return 1 ;
}

 #define PB193_MAX   1125899906842624LL
// #define PB193_MAX   64LL

int PB193(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    CTX_PRIMETABLE * ctxP  ;
    int maxP = (int) Sqrt64(PB193_MAX)+1 ;
    if((ctxP = Gen_tablePrime(maxP)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    const T_prime * tbPrime = GetTbPrime(ctxP);
    int nbPrime = GetNbPrime(ctxP) ;
    int nbM = nbPrime ;
    const T_prime *ptnbM = tbPrime+nbM ;
    const T_prime *pt1,*pt2,*pt3,*pt4,*pt5,*pt6,*pt7,*pt8 ;
    int64_t nbSqrMult = 0 ;
    int64_t P2,P3,P4,P5,P6,P7,P8 ;
    for(pt1=tbPrime;pt1<ptnbM ;pt1++) {
        int64_t P1 = PB193_MAX / ( *pt1 * (int64_t) *pt1)  ;
        nbSqrMult += P1 ;
        for(pt2=pt1+1;pt2<ptnbM && (P2=*pt2 * (int64_t) *pt2 ) <= P1 ;pt2++) {
            P2 = P1 / P2 ;
            nbSqrMult -= P2 ;
            for(pt3=pt2+1;pt3<ptnbM && (P3=*pt3 * (int64_t) *pt3 ) <= P2 ;pt3++) {
                P3 = P2 / P3 ;
                nbSqrMult += P3 ;
                for(pt4=pt3+1;pt4<ptnbM && (P4=*pt4 * (int64_t) *pt4 ) <= P3 ;pt4++) {
                    P4 = P3 / P4 ;
                    nbSqrMult -= P4 ;
                    for(pt5=pt4+1;pt5<ptnbM && (P5=*pt5 * (int64_t) *pt5 ) <= P4 ;pt5++) {
                        P5 = P4 / P5 ;
                        nbSqrMult += P5 ;
                        for(pt6=pt5+1;pt6<ptnbM && (P6=*pt6 * (int64_t) *pt6 ) <= P5;pt6++) {
                            P6 = P5 / P6 ;
                            nbSqrMult -= P6 ;
                            for(pt7=pt6+1;pt7<ptnbM  && (P7=*pt7 * (int64_t) *pt7 ) <= P6 ;pt7++) {
                                P7 = P6 / P7 ;
                                nbSqrMult += P7 ;
                                for(pt8=pt7+1;pt8<ptnbM && (P8=*pt8 * (int64_t) *pt8 ) <= P7 ;pt8++) {
                                    P8 = P7 / P8 ;
                                    nbSqrMult -= P8 ;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    int64_t nbFind = PB193_MAX - nbSqrMult ;
 
    Free_tablePrime(ctxP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbFind) ;
    return 1 ;
}

int PB193a(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    CTX_PRIMETABLE * ctxP  ;
    int maxP = (int) Sqrt64(PB193_MAX) ;
    if((ctxP = Gen_tablePrime(maxP)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    const T_prime * tbPrime = GetTbPrime(ctxP);
    int nbPrime = GetNbPrime(ctxP) ;
    int64_t *tbPrime2 = malloc((nbPrime+1)*sizeof(tbPrime2[0])) ;
    int i;
    for(i=0;i<nbPrime;i++) {
        tbPrime2[i] = tbPrime[i]*(int64_t)tbPrime[i] ;
    }
    tbPrime2[i] = tbPrime2[i-1] ; // on duplique le derneir element pour eviter un test
    Free_tablePrime(ctxP) ;

    int64_t *ptnbM = tbPrime2+nbPrime ;
    int64_t *pt1,*pt2,*pt3,*pt4,*pt5,*pt6,*pt7,*pt8 ;
    int64_t nbSqrMult = 0 ;
    int64_t P2,P3,P4,P5,P6,P7,P8 ;
    for(pt1=tbPrime2;pt1<ptnbM ;pt1++) {
        int64_t P1 = PB193_MAX / *pt1  ;
        nbSqrMult += P1 ;
        for(pt2=pt1+1; *pt2 <= P1 ;pt2++) {
            P2 = P1 / *pt2 ;
            nbSqrMult -= P2 ;
            for(pt3=pt2+1; *pt3  <= P2 ;pt3++) {
                P3 = P2 / *pt3 ;
                nbSqrMult += P3 ;
                for(pt4=pt3+1;*pt4  <= P3 ;pt4++) {
                    P4 = P3 / *pt4 ;
                    nbSqrMult -= P4 ;
                    for(pt5=pt4+1;*pt5  <= P4 ;pt5++) {
                        P5 = P4 / *pt5 ;
                        nbSqrMult += P5 ;
                        for(pt6=pt5+1;*pt6  <= P5;pt6++) {
                            P6 = P5 / *pt6 ;
                            nbSqrMult -= P6 ;
                            for(pt7=pt6+1;*pt7  <= P6 ;pt7++) {
                                P7 = P6 / *pt7 ;
                                nbSqrMult += P7 ;
                                for(pt8=pt7+1;*pt8  <= P7 ;pt8++) {
                                    P8 = P7 / *pt8 ;
                                    nbSqrMult -= P8 ;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    int64_t nbFind = PB193_MAX - nbSqrMult ;
    free(tbPrime2) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbFind) ;
    return 1 ;
}

 // #define PB195_MAXR   100


// #define PB195_MAXR   10000

#define PB195_MAXR  1053779

//#define PB195_MAXR 10000000
// parametrage triangle primitif m^n (premiers)
// a=m(3m+2n) ; b = (m+n)("m+n)
// c = sqrt(a*a+b*b-axb) = 3m*m+3*mxn+n*n
// R = sqrt(3)/2 * m * (m+n) si n%3 != 0
// si n= 3*p
// a/3 , b/3 est primitif
// R = 1/(2*sqrt(3)) * m * (m+3p)

int PB195(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    double R =  PB195_MAXR * 2 / sqrtl(3.0);
    double R3 = PB195_MAXR * 2 * sqrtl(3.0) ;
    int64_t nbSol = 0 ;
    int m,mr ;
    int mMax = sqrt(R3)+1 ;
     for(m=1;m<=mMax;m++) {
         int n ;
          for(n=1; (mr=m*(n+m))<=R3 ;n++) {
             if(PGCD(m,n) > 1) continue ;
             if((n % 3) != 0) {
                 nbSol += R / mr ;
             } else {
                 nbSol += R3 / mr ;
             }
        }
    }
    if(pbR->isVerbose) fprintf(stdout,"\tPB%s Nbsol=%lld\n",pbR->ident ,nbSol) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbSol) ;
    return 1 ;
}

int PB195a(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    double R =  PB195_MAXR * 2 / sqrtl(3);
    double R3 = PB195_MAXR * 2 * sqrtl(3) ;
    int64_t nbSol = 0 ;
    int m,n,mr ;
    int mMax = sqrt(R3)+1 ;
    int nMax = R3/4 + 2 ; // for m>=4
    u_int8_t *isNotPrime_mn = calloc(nMax,sizeof(isNotPrime_mn[0])) ;
    for(n=1;(mr=n+1)<=R3;n++) { nbSol += (n % 3) ? R/mr : R3/mr ; } // m=1
    for(n=1;(mr=2*n+4)<=R3;n +=2) { nbSol += (n % 3) ? R/mr : R3/mr ; } // m=2
    for(n=1;(mr=3*n+9)<=R;n++) { if( (n%3) != 0 ) nbSol += R/mr ; } // m=3
    for(m=4;m<=mMax;m++) { // m >= 4
        nMax =R3/m - m ; // m*(m+n)=R3
        nbSol += R/(m*(m+1)) ; // n=1
        for(n=2;n<=nMax;n++){
            if(isNotPrime_mn[n]) { isNotPrime_mn[n] = 0 ; continue ; }
            if(n <= m) {
                if( (m % n) == 0 ) { // n divisor of m
                    int np ; // invalidate multiple on n
                    for(np = 2*n; np<=nMax;np+=n) isNotPrime_mn[np] = 1 ;
                    continue ;
                }
            }
            // n is prime with m
            mr = m * (m+n) ;
            nbSol += (n % 3) ? R/mr : R3/mr ;
        }
    }
    if(pbR->isVerbose) fprintf(stdout,"\tPB%s Nbsol=%lld\n",pbR->ident ,nbSol) ;
    free(isNotPrime_mn) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbSol) ;
    return 1 ;
}



#define PB198_MAXQ  100000000
#define PB198_MIND  100

int PB198e(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-PB198_MIND)/2 ; // ajout de 1/2k pour k=51,52,...,99,100
    int d0Max = (int32_t)Sqrt64(PB198_MAXQ/2) ;
    FRACTRED fr0 ;
    nbA += (PB198_MAXQ/2 - PB198_MIND) / (PB198_MIND*PB198_MIND) ;
    int nbLoop = 0 ;
    for(fr0.d=PB198_MIND+1;fr0.d<=d0Max;fr0.d++) {
        for(fr0.n=1;PB198_MIND*fr0.n<=fr0.d;fr0.n++) {
            nbLoop++ ;
            FRACTRED fr1 =Besout(fr0);
            if(fr1.d*fr0.n-fr1.n*fr0.d != -1) continue ;
            int diff = PB198_MAXQ/2/fr0.d - fr1.d ;
            nbA += diff / fr0.d  ;
 //           {  int nb = diff / fr0.d  ; int i ;  for(i=1;i<=nb;i++) printf("%d/%d->%d/%d\n",fr0.n,fr0.d,fr1.n+i*fr0.n,fr1.d+i*fr0.d);       }
            int d = -fr1.d + fr0.d ;
               diff = PB198_MAXQ/2/fr0.d - d ;
            nbA += diff / fr0.d  ;
//          {   int nb = diff / fr0.d  ; int i ; for(i=1;i<=nb;i++) printf("%d/%d->%d/%d\n",fr0.n,fr0.d,n+i*fr0.n,d+i*fr0.d); }
        }
    }
    if(pbR->isVerbose) fprintf(stdout,"\tPB%s S=%lld,Version with Besout d0<%d n0/d0<%d loops=%d\n",pbR->ident,nbA,d0Max,PB198_MIND,nbLoop) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbA) ;
    return 1 ;
}

int PB198f(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-PB198_MIND)/2 ; // ajout de 1/2k pour k=51,52,...,99,100
    int d0Max = (int32_t)Sqrt64(PB198_MAXQ/2) ;
    printf("D0max=%d\n",d0Max);
    nbA += (PB198_MAXQ/2 - PB198_MIND) / (PB198_MIND*PB198_MIND) ;
    
    SBTree *sbt = SBT_alloc() ;
    FRACTRED fr0 = {0,1} ;
    FRACTRED fr1= {1,PB198_MIND} ;
    SBT_init(sbt,fr0,fr1) ;
    int nbLoop = 0 ;
    while(sbt->indS > 0  ) {
        nbLoop++ ;
        if(sbt->fr0.d + sbt->fr1.d <=d0Max ) {
            SBT_ValidNxt(sbt,1) ;
        } else {
            if(sbt->fr0.d <=d0Max && sbt->fr0.n ) {
 //               printf("%d/%d->%d/%d :",sbt->fr0.n,sbt->fr0.d,sbt->fr1.n,sbt->fr1.d);
                int d0 = sbt->fr0.d ;  int n0 = sbt->fr0.n ;  int d1 = sbt->fr1.d ;    int n1 = sbt->fr1.n ;
                if(d0<d1) {
                    int q = n1/n0 ;
                    d1 -= q * d0 ;
                    n1 -= q * n0 ;
                }
                int diff = PB198_MAXQ/2/d0 -  d1 ;
                nbA += diff / d0 ;
//              {  int nb = diff /  d0  ; int i ;  for(i=1;i<=nb;i++) printf("%d/%d ",n1+i*n0,d1+i*d0); printf(";") ; }
                d1 = -d1 + d0 ;
                n1 = -n1 + n0 ;
                diff = PB198_MAXQ/2/d0 - d1 ;
                nbA += diff / d0 ;
  //            { int nb = diff / d0 ; int i ; for(i=1;i<=nb;i++) printf("%d/%d ",n1+i*n0,d1+i*d0); printf("\n");}
            }
            SBT_ValidNxt(sbt,0) ;
        }
    }
    if(pbR->isVerbose) fprintf(stdout,"\tPB%s S=%lld,Version with stack(%d) d0<%d loops=%d\n",pbR->ident,nbA,sbt->sizeStack,d0Max,nbLoop) ;
    SBT_free(sbt);
    
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbA) ;
    return 1 ;
}

#define PB198_Nend     1
#define PB198_Dend     PB198_MIND

int PB198g(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    
    int64_t nbA = 0 ;
    
    nbA = 0 ;
    nbA += (PB198_MAXQ-PB198_MIND)/2 ; // add 1/2k for k=51,52,...,99,100,... 50000000
    int N = (int32_t)Sqrt64(PB198_MAXQ/2) ;
    nbA += (PB198_MAXQ/2 - PB198_MIND) / (PB198_MIND*PB198_MIND) ; // add (1/100 + k/(1+k*100) for (100*(1+k*100) <= 50000000
 
    
    int nbLoop = 0 ;
    int n0, n, d0, d ;
    n0 = n = 1 ;
    d0 = N+1 ;  d = N ;
//    d0 = Sqrt32(PB198_MAXQ/4) + 1 ; d =d0-1 ;
    int n_end = 1 ;
    int d_end = PB198_MIND ;
    //  satisfait besout n x d0 - d * n0 = 1
    do {
        nbLoop++ ;
        int a = (N+d0)/d ; // on cherche d = a * d - d0 le plus grand possible
        int tmp = d ;
        d = a * d - d0 ;
        d0 = tmp ;
        tmp = n ;
        n = a * n - n0 ; // n = a * n - n0 ;
        n0 = tmp ;
//               printf("%d/%d->%d/%d :",n0,d0,n,d);

        {
            int d1 = d ;    int n1 = n ;
            int diff = (PB198_MAXQ/2)/d0 ;
            if(d0<d1) {
                int q = n1/n0 ;
                d1 -= q * d0 ;
                n1 -= q * n0 ;
            }
            
            if(diff >= d0+d1) nbA += (diff-d1) / d0 ;
//              {  int nb = diff /  d0  ; int i ;  for(i=1;i<=nb;i++) printf("%d/%d ",n1+i*n0,d1+i*d0); printf(";") ; }
            d1 = -d1 + d0 ;
            n1 = -n1 + n0 ;
            
            if(diff >= d0+d1) nbA += (diff-d1) / d0 ;
//            { int nb = diff / d0 ; int i ; for(i=1;i<=nb;i++) printf("%d/%d ",n1+i*n0,d1+i*d0); printf("\n");}
            
        }
 //      } while(d >= n* PB198_MIND) ;
    } while(d != d_end || n != n_end ) ;
     if(pbR->isVerbose) fprintf(stdout,"\tPB%s S=%lld,Version  direct with d0<%d loops=%d\n",pbR->ident,nbA,N,nbLoop) ;
    
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbA) ;
    return 1 ;
}


/*
static inline int PB198CB(int d0,int d1) {
    if ( d1 <= PB198_MAXQ/2*PB198_MIND  && d0*(int64_t)d1 <= PB198_MAXQ/2) return 1 ;
    else return 0 ;
}
*/


int PB198(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += PB198_MIND/2+ (PB198_MAXQ)/2 - PB198_MAXQ/(2*PB198_MIND) ;

    
    SBdTree *sbdt = SBdT_alloc() ;
    SBdT_init(sbdt,1, PB198_MIND) ;
    int nbLoop = 0 ;
    while(sbdt->indS > 0 ) {
        nbLoop++ ;
         if(sbdt->d1 <=PB198_MAXQ/(2*PB198_MIND) && sbdt->d0*(int64_t)sbdt->d1 <= PB198_MAXQ/2) {
 //            printf("%d->%d ",sbdt->d0,sbdt->d1) ;
            nbA++ ;
            if(SBdT_ValidNxt(sbdt,1)==0) {
                if(pbR->isVerbose) fprintf(stdout,"\tPB%s ERROR REALLOC SBT(%d)\n",pbR->ident,sbdt->sizeStack) ;
                SBdT_free(sbdt);
                return 0 ;
            }
        } else {
           SBdT_ValidNxt(sbdt,0) ;
        }
    }
    if(pbR->isVerbose) fprintf(stdout,"\tPB%s S=%lld,Version with stack(%d) (den only) loops=%d\n",pbR->ident,nbA-1,sbdt->sizeStack,nbLoop) ;
    SBdT_free(sbdt);
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbA-1) ;
    return 1 ;
}





// Impementation of Stern-Brocot Tree

int PB198a(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-PB198_MIND)/2 ; //
    FRACTRED fr0 = {1,2} , fr1 = {1,1} ; ;
    
    SBTree *sbt = SBT_alloc() ;
    SBT_init(sbt,fr0, fr1) ;
    int nbLoop = 0 ;
    while(sbt->indS > 0 ) {
        nbLoop++ ;
        int64_t dd = 2 * (int64_t) sbt->fr0.n * sbt->fr1.n ;
        int k = (int)((int64_t)(Sqrt64(dd*PB198_MAXQ+1)-sbt->fr0.n * (int64_t)sbt->fr1.d - sbt->fr1.n * (int64_t)sbt->fr0.d)/dd) - PB198_MIND +2 ;
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
    }
    if(pbR->isVerbose) fprintf(stdout,"\tPB%s S=%lld,Version with stack(%d) (Fract[1/2,1]) loops=%d\n",pbR->ident,nbA-1,sbt->sizeStack,nbLoop) ;

    SBT_free(sbt);
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbA) ;
    return 1 ;
}


static int loopPB198b = 0 ;
int PB198bCB(FRACTRED fr0, FRACTRED fr1) {
    loopPB198b++ ;
    int64_t dd = 2 * (int64_t) fr0.n * fr1.n ;
    int k = (int)((int64_t)(Sqrt64(dd*PB198_MAXQ+1)-fr0.n * (int64_t)fr1.d - fr1.n * (int64_t)fr0.d)/dd) - PB198_MIND +2 ;
    if(k>0) return k ;
    else return 0 ;
}

int PB198b(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-PB198_MIND)/2 ; //
    FRACTRED fr0 = {1,2} , fr1 = {1,1} ;
    nbA += STBrcv(fr0,fr1,PB198bCB) ;
    if(pbR->isVerbose) fprintf(stdout,"\tPB%s S=%lld,Version recursive (Fract[1/2,1]) loops=%d\n",pbR->ident,nbA-1,loopPB198b) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbA) ;
    return 1 ;
}





int PB198c(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-PB198_MIND)/2 ; //
    int i ;
    int iMax= Sqrt32(PB198_MAXQ) ;
    SBdTree *sbdt = SBdT_alloc() ;
    int nbLoop = 0;
    for(i=PB198_MIND;i<iMax;i++) {
        SBdT_init(sbdt,i,i+1) ;
        while(sbdt->indS > 0) {
            nbLoop++ ;
            if (sbdt->d0*(int64_t)sbdt->d1 <= PB198_MAXQ/2) {
                nbA++ ;
                SBdT_ValidNxt(sbdt,1) ;
            } else {
                SBdT_ValidNxt(sbdt,0) ;
            }
        }
    }
    if(pbR->isVerbose) fprintf(stdout,"\tPB%s S=%lld,Version stack(%d) Den[%d %d] loops=%d\n",pbR->ident,nbA-1,sbdt->sizeStack,PB198_MIND,iMax,nbLoop) ;
    SBdT_free(sbdt);
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbA) ;
    return 1 ;
}


static int loopPB198d = 0 ;
static inline int PB198dCB(int d0,int d) {
    loopPB198d++ ;
    if (d0*(int64_t)d <= PB198_MAXQ/2) return 1 ;
    else return 0 ;
}

int PB198d(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int64_t nbA = 0 ;
    nbA += (PB198_MAXQ-PB198_MIND)/2 ; //
    int i ;
    int iMax= Sqrt32(PB198_MAXQ) ;
    for(i=PB198_MIND;i<iMax;i++) {
        nbA += STBrcvDen(i,i+1,PB198dCB) ;
    }
    if(pbR->isVerbose) fprintf(stdout,"\tPB%s S=%lld,Version recursive Den[%d %d] loops=%d\n",pbR->ident,nbA-1,PB198_MIND,iMax,loopPB198d) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbA) ;
    return 1 ;
}



#define PB199_NBITER    10

typedef struct T199_CIRCLE {
    int nb ;
    int type ;
    double  q1 ;
    double  q2 ;
    double  q3 ;
} T199_CIRCLE ;

#define T199_T111 1
#define T199_T112 2
#define T199_T123 3


int PB199(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int n,na ;
    int nbMax = na = 2 ; // T111 + T112
    for(n=0;n<PB199_NBITER;n++) {
        na *= 3 ;
        nbMax += na ;
    }
    double S = 1 ;
    int indTbyNiv[PB199_NBITER+1] ;
    int nbTbyNiv[PB199_NBITER+1] ;
    T199_CIRCLE *TC199 = malloc(nbMax*sizeof(TC199[0])) ;
    na = 0 ;
    indTbyNiv[0] = na ;
    TC199[na].type = T199_T111 ;
    TC199[na].nb = 1 ;
    TC199[na].q1 = TC199[na].q2 = TC199[na].q3 = 1/(2*sqrt(3.0)-3) ;
    S -= 3 * (2*sqrt(3.0)-3)*(2*sqrt(3.0)-3) ;
    na++ ;
    TC199[na].type = T199_T112 ;
    TC199[na].nb = 3 ;
    TC199[na].q1 = TC199[na].q2 =  1/(2*sqrt(3.0)-3) ;
    TC199[na].q3 = -1.0 ;
    na++ ;
    nbTbyNiv[0] = na - indTbyNiv[0] ;
    for(n=0;n<PB199_NBITER;n++) {
        int no = indTbyNiv[n] ;
        int na = no + nbTbyNiv[n] ;
        indTbyNiv[n+1] = na ;
        while(no<indTbyNiv[n+1]){
            double q ;
            if(TC199[no].type== T199_T123) {
                q = TC199[no].q1 + TC199[no].q2 + TC199[no].q3 + 2*sqrt( TC199[no].q1 * TC199[no].q2 + TC199[no].q1 * TC199[no].q3 + TC199[no].q2 * TC199[no].q3) ;
                TC199[na].type = T199_T123 ;
                TC199[na].nb = TC199[no].nb ;
                TC199[na].q1  = TC199[no].q1 ;
                TC199[na].q2  = TC199[no].q2 ;
                TC199[na].q3 = q ;
                na++ ;

                TC199[na].type = T199_T123 ;
                TC199[na].nb = TC199[no].nb ;
                TC199[na].q1  = TC199[no].q1 ;
                TC199[na].q2  = TC199[no].q3 ;
                TC199[na].q3 = q ;
                na++ ;

                TC199[na].type = T199_T123 ;
                TC199[na].nb = TC199[no].nb ;
                TC199[na].q1  = TC199[no].q2 ;
                TC199[na].q2  = TC199[no].q3 ;
                TC199[na].q3 = q ;
                na++ ;

                
            } else if(TC199[no].type== T199_T112) {
                q = 2*TC199[no].q1 + TC199[no].q3 + 2*sqrt( TC199[no].q1 * (TC199[no].q1+2*TC199[no].q3) ) ;
                TC199[na].type = T199_T112 ;
                TC199[na].nb = TC199[no].nb ;
                TC199[na].q1 = TC199[na].q2 = TC199[no].q1 ;
                TC199[na].q3 = q ;
                na++ ;

                TC199[na].type = T199_T123 ;
                TC199[na].nb = 2*TC199[no].nb ;
                TC199[na].q1  = TC199[no].q1 ;
                TC199[na].q2  = TC199[no].q3 ;
                TC199[na].q3 = q ;
                na++ ;

            } else { // T199_T111
                q = TC199[no].q1 * (3 + 2 *sqrt(3)) ;
                 TC199[na].type = T199_T112 ;
                TC199[na].nb = 3 * TC199[no].nb ;
                TC199[na].q1 = TC199[na].q2 = TC199[no].q1 ;
                TC199[na].q3 = q ;
                na++ ;
            }
            S -= TC199[no++].nb / (q*q) ;
       }
        nbTbyNiv[n+1] = na - indTbyNiv[n+1] ;
        if(pbR->isVerbose) fprintf(stdout,"\tPB%s %d -> S=%.8f\n",pbR->ident,n,S) ;
     }
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%.8f",S) ;
    return 1 ;
}






