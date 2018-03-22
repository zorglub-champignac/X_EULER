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
    if(pbR->isVerbose) fprintf(stdout,"\t PB%0.3d S",pbR->pbNum);
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

#define PB103_NB   7
#define PB103_MAX_DELTA   300
/*
int CheckEquality(int *v) {
    int k ;
    for(k=2;2*k<=PB103_NB;k++) {
        int j ;
        u_int8_t arr[PB103_NB] ;
        for(j=0;j<PB103_NB;j++)arr[j] = j ;
        do {
            int D = 0 ;
            for(j=0;j<k;j++){
                D += v[arr[j]] - v[arr[j+k]] ;
                if( D == 0) {
                    return 0 ;
                }
            }
        } while(NextArrangement(arr,2*k,PB103_NB) >= 0) ;
    }
    return 1;
}
*/
int CheckEquality(int *v) {
    int k ;
    for(k=2;2*k<=PB103_NB;k++) {
        int j ;
        u_int8_t perm2[PB103_NB] ;
        for(j=0;j<2*k;j++)perm2[j] = j ;
        do {
            int S = 0 ;
            for(j=0;j<2*k;j++){
                S += v[perm2[j]] ;
            }
            if (S & 1) continue ; // pas divisible par 2
            S  /= 2 ;
            u_int8_t perm[PB103_NB] ;
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
        } while(NextSub(perm2,2*k,PB103_NB) >= 0) ;
    }
    return 1;
}


int MinCheck(int * v) {
    int minV0 = 1 ;
    int j ;
    for(j=0;j<(PB103_NB-1)/2;j++){
        minV0 += v[PB103_NB-1-j] - v[j+1] ;
    }
    return minV0 ;
}

int Check(int *v) {
    if(v[0] < MinCheck(v)) return 0 ;
    return CheckEquality(v);
}


int PB103(PB_RESULT *pbR) {
    pbR->nbClock = clock() - pbR->nbClock ;

    int values[PB103_NB] ;
    int vmin ;
    int isNotFound = 1;
    int minV0 = 10000000 ;
    static int MinSmin[] = {1,3,4,9,21,51,115,255,567} ;
    static int MinV0[] ={1,1,1,2,3,6,11,20,39} ;
//    int Smin = 255 + 39 * PB103_NB  + 1  ;
//    int Smin = 567 + 78 * PB103_NB  + 1  ;
    int Smin = 1000000 ;

    int AntMinV0 ;
/*    {
        u_int8_t sub[4] = {0,1,2,3} ;
        do {
            printf("%d,%d,%d,%d ",sub[0],sub[1],sub[2],sub[3]) ;
        }  while(NextSub(sub,4,6) >= 0) ;
    }
 */
//    for(vmin=19;vmin<30 && isNotFound ;vmin++)
    if(PB103_NB*sizeof(MinV0[0]) <= sizeof(MinV0)) {
        AntMinV0 = MinV0[PB103_NB-1] * PB103_NB ;
    } else {
        AntMinV0 = (((PB103_NB-1)/2) * ((PB103_NB-1)/2)) *  PB103_NB ;
    }
    for(vmin = 0;isNotFound;)
    {
        values[0] = vmin ;
        int j ;
        int is = PB103_NB - 2 ;
        int deltaS = 0 ;
        int delta = deltaS ;
        int Delta[PB103_NB] ;
        for(j=0;j<PB103_NB;j++) Delta[j] = 0;
        Delta[PB103_NB-1] = deltaS ;
        while(1) {
            int v0, S=0 ;
            { int j ;
                for(j=1;j<PB103_NB;j++) {
                    values[j] = values[j-1]+Delta[j]+1 ; S += values[j] ;
                }
                // car v0+v1+v2+v3 > v6+v5+v4 => S > 2*(v6+v5+v4) > (PB103_NB-1)/(PB103_NB) * deltaS ;
//                if(deltaS*(PB103_NB-1) > Smin * PB103_NB) { isNotFound = 0; printf("DeltaS=%d\n",deltaS); break ; }
                // S = deltaS + 7 * v0 et v0 > (v6-v3)+(v5-v2)+(v4-v1) >= ((PB103_NB-1)/2) * ((PB103_NB-1)/2))
//                if(deltaS > Smin - PB103_NB * ((PB103_NB-1)/2) * ((PB103_NB-1)/2)) { isNotFound = 0; printf("DeltaS=%d\n",deltaS); break ; }
                if(deltaS > Smin - AntMinV0 )  { isNotFound = 0; printf("DeltaS=%d\n",deltaS); break ; }
                //               for(j=0;j<PB103_NB;j++)printf("%d%c",values[j],(j==PB103_NB-1) ? ' ' : ',' ) ;
                v0 = MinCheck(values) ;
                S += PB103_NB * v0 ;
                if(S < Smin && CheckEquality(values))  {
                    int j ;
                    Smin = S ;
                    if(v0 < minV0) minV0= v0 ;
                    printf("S=%d,Delta=%d,minv0=%d,DeltaMax=%d ",S,deltaS,minV0,Smin - AntMinV0) ;
                    int lg = 0 ;
                    for(j=0;j<PB103_NB;j++){
                        printf("%d%c",values[j]+v0,(j==PB103_NB-1) ? '\n' : ',' ) ;
                        lg+=sprintf(pbR->strRes+lg,"%2.2d",values[j]+v0) ;
                    }
  //              isNotFound = 0 ;
  //              break ;
                }
            }
            

            while(PB103_NB-is > delta){
                delta += (PB103_NB-is) * Delta[is] ;
                Delta[is] = 0;
                if(--is == 0 || PB103_NB-is > deltaS) break ;
            }
            if(is == 0) {
                if((deltaS & 15)==0)printf("[%d]", deltaS) ;
                delta += (PB103_NB-1) * Delta[1] ;
                Delta[1] = 0;
                deltaS++ ;
                delta = deltaS  ;
                Delta[PB103_NB-1] = delta ;
                is = PB103_NB-2 ;

     //           delta = deltaS ;
            } else if(PB103_NB-is <= delta) {
                Delta[is]++ ; delta -= PB103_NB-is ;
                Delta[PB103_NB-1] = delta ;
                is = PB103_NB-2 ;
            } else {
                deltaS++ ;
                delta = deltaS  ;
                Delta[PB103_NB-1] = delta ;
                is = PB103_NB-2 ;
            }
        }
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}