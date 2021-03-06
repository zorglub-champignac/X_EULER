//
//  PB051_100.c
//  X_euler
//
//  Created by Jeannot on 13/03/2018.
//  Copyright © 2018 Jeannot. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <assert.h>


#include "PB051_100.h"

#include "faray_utils.h"

#include "p054_data.h"

#define PB051_MAXP 1000000
#define PB051_SQMAXP 3200
#define PB051_MAXGDIG   7
#define PB051_MINLGSUI  2
#define PB051_MAXLGSUI  8

typedef struct ListDeltas {
    int nb;         // nombre de delta possibles
    int dt[1<<(PB051_MAXGDIG-1)] ;  // valeurs des deltas
} ListDeltas ;

int PB051(PB_RESULT *pbR) {
    ListDeltas  L_delta[1<<(PB051_MAXGDIG-1)] ;
    CTX_PRIMETABLE * ctxP  ;
    int minSuit = PB051_MINLGSUI ;
    T_prime p ;
    pbR->nbClock = clock()  ;
    if( PB051_SQMAXP*PB051_SQMAXP< 10*PB051_MAXP) {
        fprintf(stdout,"\t PB%s Need more Prime %d < %lld \n",pbR->ident,PB051_SQMAXP,Sqrt64(10*PB051_MAXP));
        return 0 ;
        
    }
    if((ctxP = Gen_tablePrime(PB051_SQMAXP)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    const T_prime * tbPrime = GetTbPrime(ctxP);
    {
        int i ;
        int deltas[1<<(PB051_MAXGDIG-1)] ;
        int  lg = 0 , pow10 = 1 ;
        L_delta[0].nb = 0 ;
        deltas[lg++] = 0 ;
        for(i=1; i<PB051_MAXGDIG;i++ , lg *= 2) {
            int j;
            pow10 *= 10 ;
            for(j=0;j<lg;j++) {
                int nPat = lg+j ;
                deltas[nPat] = deltas[j] +pow10 ;
                int k ;
                L_delta[nPat].nb = 0 ;
                for(k=1;k<=nPat;k++) {
                    if((k & nPat) == k) { // k est-il contenu dans le pattern nPat
                        L_delta[nPat].dt[L_delta[nPat].nb++] = deltas[k] ;
                    }
                }
                
            }
        }
    }
    
    int incP ;
    for(p=5,incP=2;p<PB051_MAXP;p+=incP , incP = 6 - incP) {
        uint8_t occurDig[10] ;
        int k ;
        if(!Is_Prime(p,tbPrime)) continue ;
        memset(occurDig,0,10 - minSuit) ;
        {
            int p1 ;
            uint8_t bit = 1; // on extrait les digits en sautant le premier (poids faible)
            for(p1=(int) (p/10);p1 != 0; p1 /= 10 , bit <<= 1 ) {
                uint8_t dg = p1 % 10 ;
                if(dg < 10 - minSuit) { // on construi le pattern pour de digit
                    occurDig[dg] |= bit ;
                }
            }
        }
        for(k=0;k<10 - minSuit;k++){ // on traite les patterns des digits 0, 1, ... pour lesquels le minimum est valide
            if(occurDig[k]) {
                ListDeltas * L_d = L_delta +  occurDig[k] ; // liste des deltas pour le pattern
                int tbP[10] ;
                int id ;
                for(id=0;id<L_d->nb;id++) { // listes des deltas .
                    int j,nbP ;
                    tbP[0]= (int) p ;
                    nbP = 1;
                    int delta = L_d->dt[id]  ;
                    for(j=1;j<10-k;j++) {
                        if(Is_Prime(p+j*delta,tbPrime)) tbP[nbP++] = (int)(p+j*delta) ;
                    }
                    if(nbP >= minSuit) {
                        if(pbR->isVerbose) {
                            int i; T_prime p1 ;
                            char pattern[PB051_MAXGDIG];
                            for(i=0, p1=p;p1!=0;i++ , p1 /= 10, delta /= 10) {
                                if((delta % 10) == 1) {
                                    pattern[i] = '*' ;
                                } else {
                                    pattern[i] = (p1 % 10) + '0' ;
                                }
                            }
                            fprintf(stdout,"\t PB%s ",pbR->ident);
                            while(i-- > 0) fprintf(stdout,"%c",pattern[i]);
                            fprintf(stdout,"->[%d] ",nbP);
                            for(i=0;i<nbP;i++) fprintf(stdout,"%d%c",tbP[i],(i==nbP-1) ? '\n' : ',');
                        }
                        minSuit = nbP + 1 ;
                        if(minSuit > PB051_MAXLGSUI ) {
                            Free_tablePrime(ctxP);
                            snprintf(pbR->strRes, sizeof(pbR->strRes),"%u",p) ;
                            pbR->nbClock = clock() - pbR->nbClock ;
                            return 1 ;
                        }
                    }
                }
            }
        }
    }
    Free_tablePrime(ctxP);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 0 ;
}

#define PB052_MAXD  10
// pour que x et 6x ait le m nb de digit x commence par 1
// de ce fait il y a au moins 6 digits differents
// donc x est >= 123456
int PB052(PB_RESULT *pbR) {
    int i;
    pbR->nbClock = clock()  ;
    int nbDig = 6 ;
    uint8_t digX[PB052_MAXD+1] = { 6,5,4,3,2,1,0,0,0,0,0} ;
    while(nbDig < PB052_MAXD) {
        int k ;
        uint8_t digXsort[PB052_MAXD] ;
        uint8_t digXxk[PB052_MAXD+1] ;
        memcpy(digXsort,digX,nbDig);
        HeapSortUint8(digXsort,nbDig) ;
        for(k=6;k>1;k--) {
            int i ;
            for(i=0;i<nbDig;i++) {
            }
            // on ajoute digXxk + digX
            digXxk[0] = 0 ;
            for(i=0;i < nbDig ;i++) {
                digXxk[i] += k * digX[i];
                digXxk[i+1] = 0 ;
                if(digXxk[i] >= 10) {
                    digXxk[i+1] =  digXxk[i] / 10;
                    digXxk[i] = digXxk[i] % 10 ;
                }
            }
            if(digXxk[nbDig])  break ;
            HeapSortUint8(digXxk,nbDig) ;
            if(memcmp(digXsort,digXxk,nbDig) != 0)  break ;
        }
        if(k==1) {
            int i ;
            uint32_t n =0 ;
            for(i=1;i<=nbDig;i++) { n = n*10 + digX[nbDig-i] ; }
            if(pbR->isVerbose)  fprintf(stdout,"\t PB%s %d,%d,%d,%d,%d,%d \n",pbR->ident,n,2*n,3*n,4*n,5*n,6*n) ;
            snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",n);
            pbR->nbClock = clock() - pbR->nbClock ;
            return 1 ;
        }
        if(digXxk[nbDig]) {
            nbDig++ ;
            for(i=0;i<=6;i++) {digX[i] = 6 -i ;}
            for(;i<nbDig;i++) digX[i] = 1 ;
            printf(" nbDig=%d",nbDig) ;
            
        } else {
            digX[0] ++ ;
            for(i=0;digX[i]>=10;i++) {
                digX[i] -= 10 ;
                digX[i+1]++ ;
            }
        }
        
    }
    
    pbR->nbClock = clock() - pbR->nbClock ;
    return 0 ;
}

int PB052a(PB_RESULT *pbR) {
    pbR->nbClock = clock()  ;
    int nbDig = 6 ;
    uint32_t n = 123456 ;
    while(nbDig < PB052_MAXD) {
        int i,k ;
        uint32_t N1 ;
        uint8_t digXsort[PB052_MAXD] ;
        uint8_t digXxk[PB052_MAXD+1] ;
        memset(digXxk,0,sizeof(digXxk)) ;
        for(i=0,N1=n;N1 != 0; i++) {
            digXsort[i] = N1 % 10 ; N1 /= 10 ;
        }
        HeapSortUint8(digXsort,nbDig) ;
        for(k=6;k>1;k--) {
            for(i=0,N1=n*k;N1 != 0; i++) {
                digXxk[i] = N1 % 10 ; N1 /= 10 ;
            }
            if(digXxk[nbDig])  break ;
            HeapSortUint8(digXxk,nbDig) ;
            if(memcmp(digXsort,digXxk,nbDig) != 0)  break ;
        }
        if(k==1) {
            if(pbR->isVerbose)  fprintf(stdout,"\t PB%s %d,%d,%d,%d,%d,%d \n",pbR->ident,n,2*n,3*n,4*n,5*n,6*n) ;
            snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",n);
            pbR->nbClock = clock() - pbR->nbClock ;
            return 1 ;
        }
        if(digXxk[nbDig]) {
            n = 0 ;
            nbDig++ ;
            for(i=nbDig;i>=6;i--) { n = 10*n + 1 ; }
            for(;i>=0;i--) {n = 10*n + 6 - i ;}
            printf(" nbDig=%d",nbDig) ;
        } else {
            n++ ;
        }
        
    }
    
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}


#define PB053_MAXN  100
#define PB053_MINV  1000000
int PB053(PB_RESULT *pbR) {
    pbR->nbClock = clock()  ;
    int32_t n, n2,r,Cr  ;
    int32_t nbSup = 0;
    for(n=2,n2=0,r=0,Cr=1;n<=PB053_MAXN;n++) {
        if((n & 1) == 0 ){
            n2++ ;
        }
        // calculate C(r,n)= (C(r,n-1) * n) / (n-r)
        Cr = (Cr * n) / (n-r) ;
        if( Cr > PB053_MINV ) {
            do {
                // calculate C(r-1,n) = (C(r,n) * r)/(n-r+1)
                Cr = (Cr * r)/(n-r+1) ;
                r-- ;
            } while(Cr > PB053_MINV);
        } else {
            while(r < n2) {
                // calculate C(r+1,n) = (C(r,n) * (n-r))/(r+1)
                int32_t nCr = (Cr * (n-r)) /(r+1) ;
                if(nCr > PB053_MINV) break ;
                Cr = nCr ;
                r++ ;
            }
        }
        if(r < n2) {
            nbSup += n-1 - 2*r ;
        }
    }
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbSup);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}


#define PB054_NB    1000

// warning reverse order
int Cmp_Card(const void * c0, const void *c1) {
    return ((Card *)c1)[0].h - ((Card *)c0)[0].h ;
}
#define PB054_HC    1
#define PB054_1P    2
#define PB054_2P    3
#define PB054_BR    4
#define PB054_SU    5
#define PB054_CO     6
#define PB054_FU     7
#define PB054_SQ    8
#define PB054_FL   9
#define PB054_FR   10








void Get_Value(Card *hand, char *Value) {
    qsort(hand,5,sizeof(hand[0]),Cmp_Card) ;
    if(hand[0].c == hand[1].c && hand[0].c == hand[2].c && hand[0].c == hand[3].c && hand[0].c == hand[4].c) {
        // couleur ou autre
        if(hand[0].h == hand[1].h+1 && hand[0].h == hand[2].h+2 && hand[0].h == hand[3].h+3 && hand[0].h == hand[4].h+4) {
            if(hand[0].h == 14) {
                Value[0] = PB054_FR ; // quinte flush royale
                Value[1] = 0 ;
            } else {
                Value[0] = PB054_FL ; // qinte flush , on compte la hauteur
                Value[1] = hand[0].h ;
                Value[2] = 0 ;
            }
        } else {
            Value[0] = PB054_CO ; // couleur, on compte toutes les huateurs
            Value[1] = hand[0].h ;
            Value[2] = hand[1].h ;
            Value[3] = hand[2].h ;
            Value[4] = hand[3].h ;
            Value[5] = hand[4].h ;
            Value[6] = 0 ;
        }
    } else if (hand[0].h == hand[1].h+1 && hand[0].h == hand[2].h+2 && hand[0].h == hand[3].h+3 && hand[0].h == hand[4].h+4) {
        Value[0] = PB054_SU ; // suite, on compte toutes les hauteurs
        Value[1] = hand[0].h ;
        Value[2] = 0 ;
    } else if (hand[0].h == hand[1].h || hand[1].h == hand[2].h || hand[2].h == hand[3].h || hand[3].h == hand[4].h) {
        // au moins une paire
        if (hand[0].h == hand[1].h && hand[1].h == hand[2].h && hand[2].h == hand[3].h ) {
            Value[0] = PB054_SQ ; // carre
            Value[1] = hand[0].h ;
            Value[2] = hand[4].h ;
            Value[3] = 0 ;
        } else if (hand[1].h == hand[2].h && hand[2].h == hand[3].h && hand[3].h == hand[4].h) {
            Value[0] = PB054_SQ ; // carre
            Value[1] = hand[1].h ;
            Value[2] = hand[0].h ;
            Value[3] = 0 ;
        } else if ( hand[0].h == hand[1].h && hand[1].h == hand[2].h) {
            if(hand[3].h == hand[4].h) {
                Value[0] = PB054_FU ; // Full
                Value[1] = hand[0].h ;
                Value[2] = hand[3].h ;
                Value[3] = 0 ;
            } else {
                Value[0] = PB054_BR ; // brelan
                Value[1] = hand[0].h ;
                Value[2] = hand[3].h ;
                Value[3] = hand[4].h ;
                Value[4] = 0 ;
            }
        } else if ( hand[1].h == hand[2].h && hand[2].h == hand[3].h) {
            Value[0] = PB054_BR ; // brelan
            Value[1] = hand[1].h ;
            Value[2] = hand[0].h ;
            Value[3] = hand[4].h ;
            Value[4] = 0 ;
        } else if ( hand[2].h == hand[3].h && hand[3].h == hand[4].h) {
            if(hand[0].h == hand[1].h) {
                Value[0] = PB054_FU ; // Full
                Value[1] = hand[2].h ;
                Value[2] = hand[0].h ;
                Value[3] = 0 ;
            } else {
                Value[0] = PB054_BR ; // brelan
                Value[1] = hand[2].h ;
                Value[2] = hand[0].h ;
                Value[3] = hand[1].h ;
                Value[4] = 0 ;
            }
        } else if ( hand[0].h == hand[1].h && hand[2].h == hand[3].h) {
            Value[0] = PB054_2P ; // double paire
            Value[1] = hand[0].h ;
            Value[2] = hand[2].h ;
            Value[3] = hand[4].h ;
            Value[4] = 0 ;
        } else if ( hand[0].h == hand[1].h && hand[3].h == hand[4].h) {
            Value[0] = PB054_2P ; // double paire
            Value[1] = hand[0].h ;
            Value[2] = hand[3].h ;
            Value[3] = hand[2].h ;
            Value[4] = 0 ;
        } else if ( hand[1].h == hand[2].h && hand[3].h == hand[4].h) {
            Value[0] = PB054_2P ; // double paire
            Value[1] = hand[1].h ;
            Value[2] = hand[3].h ;
            Value[3] = hand[0].h ;
            Value[4] = 0 ;
        } else if ( hand[0].h == hand[1].h) {
            Value[0] = PB054_1P ; // paire
            Value[1] = hand[0].h ;
            Value[2] = hand[2].h ;
            Value[3] = hand[3].h ;
            Value[4] = hand[4].h ;
            Value[5] = 0 ;
        } else if ( hand[1].h == hand[2].h) {
            Value[0] = PB054_1P ; // paire
            Value[1] = hand[1].h ;
            Value[2] = hand[0].h ;
            Value[3] = hand[3].h ;
            Value[4] = hand[4].h ;
            Value[5] = 0 ;
        } else if ( hand[2].h == hand[3].h) {
            Value[0] = PB054_1P ; // paire
            Value[1] = hand[2].h ;
            Value[2] = hand[0].h ;
            Value[3] = hand[1].h ;
            Value[4] = hand[4].h ;
            Value[5] = 0 ;
        }else if ( hand[3].h == hand[4].h) {
            Value[0] = PB054_1P ; // paire
            Value[1] = hand[3].h ;
            Value[2] = hand[0].h ;
            Value[3] = hand[1].h ;
            Value[4] = hand[2].h ;
            Value[5] = 0 ;
        } else {
            printf("BIG PROBLEM") ;
            Value[0] = 0 ;
        }





        
    } else {
        Value[0] = PB054_HC ; // rien, on compte toutes les cartes
        Value[1] = hand[0].h ;
        Value[2] = hand[1].h ;
        Value[3] = hand[2].h ;
        Value[4] = hand[3].h ;
        Value[5] = hand[4].h ;
        Value[6] = 0 ;
        
    }
}


// #define PB054_DEBUG

int PB054(PB_RESULT *pbR) {
#if defined(PB054_DEBUG)
    char * valHand[] = { "?" ,"Card" , "Pair", "2Pairs" , "Brelan", "Suite", "Couleur" , "Full", "Carré", "Quinte" , "Quinte_Flush" ,"Quinte_Royale" } ;
    char valCard[]="??23456789TJQKA" ;
    char valColor[] = "CDHS" ;
    
#endif
    pbR->nbClock = clock()  ;
    const Play * PL = P054_GetData() ;
    int nbWinP0 =0 ;
    int i ;
    for(i=0;i<PB054_NB;i++) {
        Play curPlay = PL[i] ;
#if defined(PB054_DEBUG)
        int k ;
        for(k=0;k<5;k++) printf("%c%c ",valCard[curPlay.Hand0[k].h],valColor[curPlay.Hand0[k].c]) ;
        printf("\t");
        for(k=0;k<5;k++) printf("%c%c ",valCard[curPlay.Hand1[k].h],valColor[curPlay.Hand1[k].c]) ;
#endif
        char Value0[10]  ;
        char Value1[10] ;
        Get_Value(curPlay.Hand0,Value0) ;
        Get_Value(curPlay.Hand1,Value1) ;
        int issue = strcmp(Value0,Value1) ;
#if defined(PB054_DEBUG)
        printf("%s",valHand[Value0[0]]) ; for(k=1;Value0[k];k++) printf("%c%c",(k==1) ? '(' : ',' , valCard[Value0[k]] ) ;
        printf(")\t%c\t",(issue > 0) ? '>' : '<');
        printf("%s",valHand[Value1[0]]) ; for(k=1;Value1[k];k++) printf("%c%c",(k==1) ? '(' : ',' , valCard[Value1[k]] ) ;
        printf(")\n");
#endif
       
        if(issue > 0){
            nbWinP0++ ;
//            printf("W1(%d)",i) ;
        } else if(issue == 0) {
            printf("Egalite %d\n",i);
        }
    }
    
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbWinP0);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}


#define PB055_MAXN  10000
#define PB055_MAXITER   50
#define PB055_MAXDIGIT  PB055_MAXITER+5

int PB055(PB_RESULT *pbR) {
    pbR->nbClock = clock()  ;
    int32_t n  ;
    int32_t nbLychrel = 0;
    for(n=1;n<PB055_MAXN;n++) {
        uint8_t Dig0[PB055_MAXDIGIT] ;
        uint8_t Dig1[PB055_MAXDIGIT] ;
        uint8_t *pDigCur ;
        uint8_t *pDigAnt ;
        
        int nbDig,k,n1  ;
        for(nbDig=0,n1=n;n1 != 0;) {
            Dig0[nbDig++] = n1 % 10 ;
            n1 /= 10 ;
        }
        pDigCur = Dig0 ;
        pDigAnt = Dig1 ;
        for(k=0;k<PB055_MAXITER;k++) {
            int i ;
            uint8_t carry = 0 ;
            {
                uint8_t *tmp = pDigCur;
                pDigCur = pDigAnt ;
                pDigAnt = tmp ;
            }
            for(i=0;i<nbDig;i++) {
                pDigCur[i] = pDigAnt[i]+pDigAnt[nbDig-i-1] + carry ;
                if(pDigCur[i] >= 10) {
                    pDigCur[i] -= 10 ; carry = 1 ;
                } else {
                    carry = 0 ;
                }
            }
            if( carry) pDigCur[nbDig++] = 1 ;
            for(i=0;2*i<nbDig;i++) {
                if(pDigCur[i] != pDigCur[nbDig-i-1]) break ;
            }
            if(2*i>=nbDig) break ;
        }
        if(k==PB055_MAXITER) {
            nbLychrel++ ;
        }
    }
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbLychrel);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}



#define PB057_N  1000
#define PB057_MAXDIGIT  PB057_N+5

typedef struct FractCont {
    uint8_t *pt1 ;
    uint8_t *pt ;
    int nbDig ;
}  FractCont ;
int PB057(PB_RESULT *pbR) {
    int n ;
    int nbNsupD = 0 ;
    pbR->nbClock = clock()  ;
    uint8_t DigN1[PB057_MAXDIGIT] ;
    uint8_t DigN0[PB057_MAXDIGIT] ;
    uint8_t DigD1[PB057_MAXDIGIT] ;
    uint8_t DigD0[PB057_MAXDIGIT] ;
    FractCont FC[2] ; // numerateur indice 0, denominateur indice 1
    FC[0].nbDig = 1 ;
    FC[0].pt = DigN0 ;
    FC[0].pt1 = DigN1 ;
    FC[0].pt[0] = 1 ;
    FC[0].pt1[0] = 1 ;
    
    FC[1].nbDig = 1 ;
    FC[1].pt = DigD0 ;
    FC[1].pt1 = DigD1 ;
    FC[1].pt[0] = 1 ;
    FC[1].pt1[0] = 0 ;
    for(n=1;n<=PB057_N;n++) {
        int i ;
        int k ;
        for(k=0;k<2;k++) {
            uint8_t *tmp = FC[k].pt1 ;
            FC[k].pt1  = FC[k].pt ; FC[k].pt = tmp ;
            // N(n+1) = 2*N(n) + N(n-1)
            int carry = 0 ;
            for(i=0;i<FC[k].nbDig;i++) {
                FC[k].pt[i] += 2 * FC[k].pt1[i] + carry ;
                if(FC[k].pt[i] >= 10) {
                    carry = FC[k].pt[i] / 10 ;
                    FC[k].pt[i] = FC[k].pt[i] % 10 ;
                } else {
                    carry = 0 ;
                }
            }
            if(carry) {
                FC[k].pt1[FC[k].nbDig] = 0 ;
                FC[k].pt[FC[k].nbDig++] = carry ;
            }
        }
        if(FC[0].nbDig > FC[1].nbDig) {
            nbNsupD++ ;
        }
        
    }
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbNsupD);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

// N = 2n+1
// DownRight N*N
// DownLeft  N*(N-1)+1
// UpLeft    N*(N-2)+2
// UpRight   N*(N-3)+3
// NbPoint = 4n+1
#define PB058_MAXN      100000
int PB058(PB_RESULT *pbR) {
    CTX_PRIMETABLE * ctxP  ;
    int N ;
    int nbPoint, nbPrime , DL  ;
    pbR->nbClock = clock()  ;
    if((ctxP = Gen_tablePrime(PB058_MAXN)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    nbPoint = 1 ;
    nbPrime = 0 ;
    DL = 1 ;
    const T_prime * tbPrime =GetTbPrime(ctxP);
    for(N=3;N< PB058_MAXN;N += 2 ) {
        nbPoint += 4 ;
        DL += 4*N - 6 ;
        if(Is_Prime(DL,tbPrime)) {
            nbPrime++ ;
        }
        if(Is_Prime(DL-N+1,tbPrime)) {
            nbPrime++ ;
        }
        if(Is_Prime(DL-2*N+2,tbPrime)) {
            nbPrime++ ;
        }
        if(10*nbPrime < nbPoint) break ;
    }
    Free_tablePrime(ctxP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    if(N<PB058_MAXN) {
        snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",N);
        return 1 ;
    } else {
        return 0 ;
    }
}

static uint8_t PB59_encrypted[] = {
    79,59,12,2,79,35,8,28,20,2,3,68,8,9,68,45,0,12,9,67,68,4,7,5,23,27,1,21,79,85,78,79,85,71,38,10,71,27,12,2,79,6,2,8,13,9,1,13,9,8,68,
    19,7,1,71,56,11,21,11,68,6,3,22,2,14,0,30,79,1,31,6,23,19,10,0,73,79,44,2,79,19,6,28,68,16,6,16,15,79,35,8,11,72,71,14,10,3,79,12,2,79,
    19,6,28,68,32,0,0,73,79,86,71,39,1,71,24,5,20,79,13,9,79,16,15,10,68,5,10,3,14,1,10,14,1,3,71,24,13,19,7,68,32,0,0,73,79,87,71,39,1,71,
    12,22,2,14,16,2,11,68,2,25,1,21,22,16,15,6,10,0,79,16,15,10,22,2,79,13,20,65,68,41,0,16,15,6,10,0,79,1,31,6,23,19,28,68,19,7,5,19,79,12,
    2,79,0,14,11,10,64,27,68,10,14,15,2,65,68,83,79,40,14,9,1,71,6,16,20,10,8,1,79,19,6,28,68,14,1,68,15,6,9,75,79,5,9,11,68,19,7,13,20,79,8,
    14,9,1,71,8,13,17,10,23,71,3,13,0,7,16,71,27,11,71,10,18,2,29,29,8,1,1,73,79,81,71,59,12,2,79,8,14,8,12,19,79,23,15,6,10,2,28,68,19,7,22,
    8,26,3,15,79,16,15,10,68,3,14,22,12,1,1,20,28,72,71,14,10,3,79,16,15,10,68,3,14,22,12,1,1,20,28,68,4,14,10,71,1,1,17,10,22,71,10,28,19,6,
    10,0,26,13,20,7,68,14,27,74,71,89,68,32,0,0,71,28,1,9,27,68,45,0,12,9,79,16,15,10,68,37,14,20,19,6,23,19,79,83,71,27,11,71,27,1,11,3,68,2,
    25,1,21,22,11,9,10,68,6,13,11,18,27,68,19,7,1,71,3,13,0,7,16,71,28,11,71,27,12,6,27,68,2,25,1,21,22,11,9,10,68,10,6,3,15,27,68,5,10,8,14,
    10,18,2,79,6,2,12,5,18,28,1,71,0,2,71,7,13,20,79,16,2,28,16,14,2,11,9,22,74,71,87,68,45,0,12,9,79,12,14,2,23,2,3,2,71,24,5,20,79,10,8,27,
    68,19,7,1,71,3,13,0,7,16,92,79,12,2,79,19,6,28,68,8,1,8,30,79,5,71,24,13,19,1,1,20,28,68,19,0,68,19,7,1,71,3,13,0,7,16,73,79,93,71,59,12,
    2,79,11,9,10,68,16,7,11,71,6,23,71,27,12,2,79,16,21,26,1,71,3,13,0,7,16,75,79,19,15,0,68,0,6,18,2,28,68,11,6,3,15,27,68,19,0,68,2,25,1,21,
    22,11,9,10,72,71,24,5,20,79,3,8,6,10,0,79,16,8,79,7,8,2,1,71,6,10,19,0,68,19,7,1,71,24,11,21,3,0,73,79,85,87,79,38,18,27,68,6,3,16,15,0,17,
    0,7,68,19,7,1,71,24,11,21,3,0,71,24,5,20,79,9,6,11,1,71,27,12,21,0,17,0,7,68,15,6,9,75,79,16,15,10,68,16,0,22,11,11,68,3,6,0,9,72,16,71,29,
    1,4,0,3,9,6,30,2,79,12,14,2,68,16,7,1,9,79,12,2,79,7,6,2,1,73,79,85,86,79,33,17,10,10,71,6,10,71,7,13,20,79,11,16,1,68,11,14,10,3,79,5,9,11,
    68,6,2,11,9,8,68,15,6,23,71,0,19,9,79,20,2,0,20,11,10,72,71,7,1,71,24,5,20,79,10,8,27,68,6,12,7,2,31,16,2,11,74,71,94,86,71,45,17,19,79,16,
    8,79,5,11,3,68,16,7,11,71,13,1,11,6,1,17,10,0,71,7,13,10,79,5,9,11,68,6,12,7,2,31,16,2,11,68,15,6,9,75,79,12,2,79,3,6,25,1,71,27,12,2,79,
    22,14,8,12,19,79,16,8,79,6,2,12,11,10,10,68,4,7,13,11,11,22,2,1,68,8,9,68,32,0,0,73,79,85,84,79,48,15,10,29,71,14,22,2,79,22,2,13,11,21,
    1,69,71,59,12,14,28,68,14,28,68,9,0,16,71,14,68,23,7,29,20,6,7,6,3,68,5,6,22,19,7,68,21,10,23,18,3,16,14,1,3,71,9,22,8,2,68,15,26,9,6,1,
    68,23,14,23,20,6,11,9,79,11,21,79,20,11,14,10,75,79,16,15,6,23,71,29,1,5,6,22,19,7,68,4,0,9,2,28,68,1,29,11,10,79,35,8,11,74,86,91,68,52,
    0,68,19,7,1,71,56,11,21,11,68,5,10,7,6,2,1,71,7,17,10,14,10,71,14,10,3,79,8,14,25,1,3,79,12,2,29,1,71,0,10,71,10,5,21,27,12,71,14,9,8,1,3,
    71,26,23,73,79,44,2,79,19,6,28,68,1,26,8,11,79,11,1,79,17,9,9,5,14,3,13,9,8,68,11,0,18,2,79,5,9,11,68,1,14,13,19,7,2,18,3,10,2,28,23,73,79,
    37,9,11,68,16,10,68,15,14,18,2,79,23,2,10,10,71,7,13,20,79,3,11,0,22,30,67,68,19,7,1,71,8,8,8,29,29,71,0,2,71,27,12,2,79,11,9,3,29,71,60,11,
    9,79,11,1,79,16,15,10,68,33,14,16,15,10,22,73
    
} ;

#define PB059_MAXASCII  128
int PB059(PB_RESULT *pbR) {
    uint16_t HIST[PB059_MAXASCII*3] ;
    int i;
    int Sum = 0 ;
    pbR->nbClock = clock()  ;
    memset(HIST,0,sizeof(HIST));
    for(i=0;i<sizeof(PB59_encrypted);i++) {
        HIST[ (i % 3) * PB059_MAXASCII + PB59_encrypted[i]]++ ;
    }
    for(i=0;i<3;i++) {
        int j, jmax = 0 ;
        int max = 0 ;
        for(j=0;j<PB059_MAXASCII;j++) {
            if(HIST[j+i*PB059_MAXASCII]>max) {
                max = HIST[j+i*PB059_MAXASCII] ;
                jmax = j ;
            }
        }
        jmax ^= ' ' ;
        for(j=i;j<sizeof(PB59_encrypted);j += 3) {
            PB59_encrypted[j] ^= jmax ;
            Sum += PB59_encrypted[j] ;
        }
    }
    if(pbR->isVerbose)fprintf(stdout,"\t PB%s \"%30.30s ...\"\n",pbR->ident,PB59_encrypted) ;
    
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",Sum);
    return 1 ;
}


#define PB060_MAXP      100000



int PB060(PB_RESULT *pbR) {
    static uint8_t *isP1P2 ;
    
    static int NPMAX,NP ;
    CTX_PRIMETABLE * ctxP  ;
    int32_t *pow10 ;
    int32_t maxS, minS = 0 ;
    pbR->nbClock = clock()  ;
    if((ctxP = Gen_tablePrime(PB060_MAXP)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    const T_prime * tbPrime = GetTbPrime(ctxP) ;
    uint32_t nbPrime = GetNbPrime(ctxP) ;
    NPMAX = GetNbPrime(ctxP)-1 ;
    int32_t maxP = (int32_t)tbPrime[nbPrime-1] ;
    {   // on limite la valeur max des nb premiers a tester pour que la concat de 2 d'entre eux
        // soit testable avec la table calculee
        int32_t pow10 = 10 ;
        while(pow10 < PB060_MAXP) pow10 *= 10 ;
        uint64_t maxP2 = maxP * (uint64_t) maxP ;
        while(tbPrime[NP] * (uint64_t) (pow10 + 1) >= maxP2 ) NPMAX-- ;
    }
    
    NP = NPMAX ;
    maxS = (int32_t)tbPrime[NP-1] ;
    if(pbR->isVerbose)fprintf(stdout,"\t PB%s NPMAX=%d,maxS=%d,maxP=%d\n",pbR->ident,NP,maxS,maxP);
    pow10 = calloc(NP, sizeof(pow10[0])) ;
    {
        int i ;
        for(i=0;i<NP;i++) {
            int32_t Pi = (int32_t) tbPrime[i] ;
            int32_t ip10 = 10 ;
            while(ip10< Pi) ip10 *= 10 ;
            pow10[i] = ip10 ;
        }
    }
    {
        isP1P2 = calloc(NP*NP,sizeof(isP1P2[0])) ;
        // parcours arborescent jusqu'a atteindre la profondeur 5
        int index[5],i ;
        uint64_t P[5],SP ;
        if(minS==0) maxS = (int32_t) tbPrime[NP-1] ;
        
        for(i=0,index[i]=0,SP=0;i>=0;) {
            int j , isOK ;
            int ip = index[i] ;
            isOK = 1 ;
            // borne sur l'index et test que la somme P[k] < maxS avec P[k] croissant
            if(ip >=NP || SP+(5-i)*(P[i]=tbPrime[ip]) > maxS ) {
                if(--i >= 0) { SP -= P[i] ; index[i]++ ; }
                continue ;
            } else {
                for(j=0;j<i;j++) {
                    int jp = index[j] ; // test compatibilite P[j] et le nouveau P[i]
                    // test non calcule (zero , sinon 1 pour non compat et 2 pour compat
                    if(isP1P2[NP*jp+ip]==0)  { isP1P2[NP*jp+ip] = Is_Prime2(P[i]+P[j]*pow10[ip],P[j]+P[i]*pow10[jp],tbPrime) ? 2 : 1 ; }
                    if(isP1P2[NP*jp+ip]==1)  {
                        isOK = 0; break ;
                    }
                }
            }
            if(isOK) {
                if(i == 4) {
                    if(pbR->isVerbose)fprintf(stdout,"\t PB%s  %lld = %lld + %lld + %lld + %lld + %lld\n",pbR->ident,SP+P[4],P[0],P[1],P[2],P[3],P[4]);
                    if(SP+P[4] < maxS) {
                        minS = maxS =(int32_t) (SP+P[4]) ;
                    }
                    index[i]++ ;
                } else {
                    SP += P[i] ;
                    index[i+1] = index[i]+1 ;
                    i++ ;
                }
            } else {
                index[i]++ ;
            }
        }
        free(isP1P2) ;
    }
    free(pow10);
    Free_tablePrime(ctxP) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    if(minS) {
        snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",minS) ;
        return 1 ;
    } else {
        return 0 ;
    }
}



// maximum compris entre 1010 et 9999
// P(s,n) = (s-2)n(n-1)/2 + n
// P(s+1,n) = P(s,n) + n(n-1)/ 2 ;

//  nombre de polygones differents
#define PB061_NS       6

// max de valeurs pour le gros polygone (choisi comme point de depart de la boucle)
#define PB061_MAXPX0     100
// nombre de prefixe differents (en fait entre 10 et 99) Majorant
#define PB061_NBPREF    100
// majorant stric du nombre de polygone d'un meme type pouvent tomber sur un prefixe
#define PB061_MAXBYPREF 4

// hors le premier type de polygone (permTyp=PB061_NS-1 choisi,s=0 où toutes les valeurs sont stockees a la suite)
// pour les autres on indexe les polygones par prefixe,typePoly et si polygones plusieurs ont
// le meme index on les mets a la suite terminee par un zero (Hyp : max conflits < PB061_MAXBYPREF)
// de ce fait on adresse directement la tete de liste par la macro IND(pref,s)
#define IND(pref,s) (PB061_MAXPX0+ ((pref)*PB061_NS+(s)) * PB061_MAXBYPREF)

typedef uint16_t  T_Polygonal;
int PB061(PB_RESULT *pbR) {
    int n,s ;
    int nbSol = 0 ;
    int isOnlyFirst = 0 ;
    int Smin ;
    T_Polygonal  *T_values;
    ;
    char *     nameT[6] = { "Tria","Squa","Pent","Hexa","Hept","Octo" } ;
    
    pbR->nbClock = clock()  ;
    
    {
        uint32_t T0 = 0, Ts ;
        T_values = calloc(PB061_MAXPX0+(PB061_NS*PB061_MAXBYPREF*PB061_NBPREF),sizeof(T_values[0])) ;
        for(n=0;T0 < 10000 ;n++ , T0 += n) {
            for(s=0,Ts=T0;s<PB061_NS;s++ ,Ts += (n*(n-1))/2) {
                if(Ts>=1010 && Ts<10000 && ((Ts % 100) >= 10) ) {
                    // index par, prefixe et type=s
                    int ind = (s==PB061_NS-1) ? 0 : IND(Ts/100,s);
                    while(T_values[ind]) ind++ ; // on cherche la premiere valeur non nulle
                    if(s ==PB061_NS-1) {
                        T_values[ind] = Ts ;
                    }else {
                        if(ind >= IND(Ts/100,s)+ PB061_MAXBYPREF-1) {
                            fprintf(stdout,"\t PB%s Too many identical prefix(%d) pour Type %d\n",pbR->ident, Ts/100,s) ;
                            return 0 ;
                        }
                        T_values[ind] = Ts % 100 ; // on ne stocke que le suffixe
                    }
                }
            }
            
        }
    }
    Smin = 9999 * PB061_NS;
    {
        int inds[PB061_NS] ;
        uint8_t permTyp[PB061_NS] ;
        T_Polygonal T[PB061_NS] ;
        T_Polygonal  pref0 = 0 ;
        permTyp[0] = PB061_NS-1 ;
        for(s=1;s<PB061_NS;s++) permTyp[s] = s-1 ;
        inds[0] = 0 ;
        for(s=0;s>=0;){
            int isOKs = 0 ;
            if((T[s] = T_values[inds[s]++])) {
                if(s==0) {
                    pref0 = T[s] / 100 ;
                    T[s] = T[s] % 100 ;
                }
                isOKs = 1;
            }
            if(isOKs) {
                if(s < PB061_NS-2) {
                    s++ ;
                    inds[s] = IND(T[s-1],permTyp[s]) ;
                    
                    continue ;
                } else { // fin de boucle on verifie le rebouclage
                    int indf ;
                    uint8_t typf = permTyp[PB061_NS-1] ;
                    for(indf=IND(T[s],typf);T_values[indf] ; indf++) {
                        if(T_values[indf] == pref0) {
                            T[PB061_NS-1] = pref0 ;
                            {
                                int32_t S =0 ; int i ;
                                T_Polygonal Tc[PB061_NS] ;
                                for(i=0;i<PB061_NS;i++) {
                                    Tc[i] = (i==0) ? (100*pref0+T[0]) : (100*T[i-1]+T[i]) ;
                                    S += Tc[i] ;
                                }
                                nbSol++ ;
                                if(S < Smin) {
                                    Smin = S ;
                                    if(pbR->isVerbose){
                                        fprintf(stdout,"\t PB%s S=%d,Nb=%d ",pbR->ident,S,nbSol);
                                        if(PB061_NS>6) {
                                            for(i=0;i<PB061_NS;i++) fprintf(stdout,"%d(%d)%c",permTyp[i],Tc[i],(i==PB061_NS-1) ? '\n' :' ') ;
                                        }else {
                                            for(i=0;i<PB061_NS;i++) fprintf(stdout,"%s(%d)%c",nameT[permTyp[i]],Tc[i],(i==PB061_NS-1) ? '\n' :' ') ;
                                            
                                        }
                                        
                                    }
                                }
                            }
                        }
                    }
                    isOKs= 0 ;
                }
            }
            if(!isOKs) { // on doit decrementer s
                if(s==0 || (isOnlyFirst && nbSol)) {
                    break ;
                }
                int i ;
                uint8_t tmp ;
                for(i=s+1;i<PB061_NS;i++) {
                    if(permTyp[i] > permTyp[s]) {
                        tmp = permTyp[s] ;
                        permTyp[s] = permTyp[i] ;
                        permTyp[i] = tmp ;
                        inds[s] = IND(T[s-1],permTyp[s]) ;
                        break ;
                    }
                }
                if(i==PB061_NS) {
                    tmp = permTyp[s] ;
                    for(i=s+1;i<PB061_NS;i++) {
                        permTyp[i-1] = permTyp[i] ;
                    }
                    permTyp[PB061_NS-1] = tmp ;
                    s-- ;
                }
                
            }
        }
    }
    free(T_values);
    if(pbR->isVerbose){
        fprintf(stdout,"\t PB%s S=%d,Nb=%d\n",pbR->ident,Smin,nbSol);
    }
    if(nbSol > 0) snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",Smin);
    pbR->nbClock = clock() - pbR->nbClock ;
    return (nbSol > 0) ;
}

static uint8_t * digCube = NULL ;

int CmpIndexPB62(const void *pt1,const void *pt2) {
    int cmp ;
    const uint8_t * st1 = digCube + ((int32_t *)pt1)[0] ;
    const uint8_t * st2 = digCube + ((int32_t *)pt2)[0] ;
    cmp = strcmp((char *)st1,(char *)st2) ;
    if(cmp != 0) { // pou les garder dans l'ordre initial en cas d'egalite
        return cmp ;
    } else return (int) (st1 - st2) ;
}

#define PB062_NBASK 5
#define PB062_MAX   100000
#define PB062_MAXD  18+1 // 3x5
// (racine cubique de 10 x 100000)+1
#define PB062_S1     215443
// (racine cubique de 100 x 100000)+1
#define PB062_S2     464157
#define PB062_S3     999999
#define PB062_SMIN  6   // 100
#define PB062_SMAX  12  // 10000

int PB062(PB_RESULT *pbR) {
    
    uint64_t n , cube , bestCub=0 ;
    int i,pow, nbDig, nbEqualMax = 1 ;
    int32_t * index = NULL ;
    pbR->nbClock = clock() ;
    uint32_t seuil[] = {
        0   , 3     ,  5
        , 10    , 22    , 47
        ,100    ,216    ,465
        ,1000   ,2155   ,4642
        ,10000  ,21545  ,46416
        ,100000 } ;
    {
        for(i=1,pow=100000;pow>1;pow /= 10) {
            seuil[i++] = (PB062_S1 / pow) + 1;
            seuil[i++] = (PB062_S2 / pow) + 1;
            seuil[i++] = (PB062_S3 / pow) + 1 ;
        }
    }
    for(nbDig=0,i=0;i<seuil[PB062_SMIN];i++) {
        if(i == seuil[nbDig]) nbDig++ ;
    }
    for(n=seuil[PB062_SMIN], cube=1000000 ;n<seuil[PB062_SMAX];n++) {
        uint8_t * ptDig ;
        if(n==seuil[nbDig]) {
            nbDig++ ;
            digCube = malloc((nbDig+1)* (seuil[nbDig]-seuil[nbDig-1])) ;
            index = malloc((seuil[nbDig]-seuil[nbDig-1])*sizeof(index[0])) ;
        }
        index[n - seuil[nbDig-1]] = (int32_t) (n - seuil[nbDig-1])* (nbDig+1) ;
        ptDig = digCube + (n - seuil[nbDig-1]) * (nbDig+1) ;
        snprintf((char *)ptDig,nbDig+1,"%lld",cube);
        HeapSortUint8(ptDig,nbDig);
        if(n+1==seuil[nbDig]) {
            uint8_t *ant,*cur ;
            int nbSim = 1;
            qsort(index,seuil[nbDig]-seuil[nbDig-1],sizeof(index[0]),CmpIndexPB62);
            for(i=1,cur=digCube+index[0];i<seuil[nbDig]-seuil[nbDig-1];i++) {
                ant = cur ;
                cur = digCube+index[i] ;
                if(strcmp((char *)ant,(char *)cur)== 0) {
                    nbSim++;
                }else {
                    if(nbSim>nbEqualMax) {
                        int k ;
                        nbEqualMax = nbSim ;
                        bestCub = seuil[nbDig-1]+index[i-nbSim]/(nbDig+1) ;
                        bestCub = bestCub *bestCub * bestCub ;
                        if(pbR->isVerbose){
                            fprintf(stdout,"\t PB%s %d %lld ",pbR->ident, nbEqualMax,bestCub);
                            for(k=0;k<nbSim;k++)fprintf(stdout,"%d%c",seuil[nbDig-1]+index[i-nbSim+k]/(nbDig+1) ,(k==nbSim-1) ? '\n' : ' ' );
                        }
                    }
                    nbSim = 1 ;
                }
                
            }
            free(digCube) ;
            if(nbEqualMax == PB062_NBASK) break ;
        }
        cube += 3*n*(n+1)+1 ;
    }
    
    pbR->nbClock = clock() - pbR->nbClock ;
    if(nbEqualMax == PB062_NBASK){
        snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",bestCub) ;
        return 1 ;
    } else   return 0 ;
}


// si  10**(n-1) <= a**n < 10 **n
// en passant au log10 :  n-1 <= log10(a) < n
// en divisant par n : 1-1/n <= log10(a) < 1
// donc : a < 10 et 1 -log10(a) <= 1/n <=> 1 / (1 - log10(a)) >= n
int PB063(PB_RESULT *pbR) {
    uint16_t nb = 0 ;
    int a;
    pbR->nbClock = clock()  ;
    for(a=1;a<10; a++) {
        nb += (int) ( 1 / ( 1 - log10((double)a) ) ) ;
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nb);
    return 1 ;
}


int PB064(PB_RESULT *pbR) {
    int32_t N , k0, k02, n , d ;
    int NbImpair = 0 ;
    pbR->nbClock = clock()  ;
    for(N=2,k0=1,k02=4;N<10000;N++) {
        int i ;
        if(N == k02) { // k02 = (k0+1)*(k0+1)
            k0++ ;
            k02 += 2*k0 + 1 ; continue ;
        }
        n = k0 ; d=1 ; i = 0 ; // so k0 =(int) srqt(N)
        do {
            d = (N - n * n) / d ; // quite easy to recursively show that division is exact
            n = k0 - ( (k0 + n) % d ) ;
            i++ ;
        } while(d !=1 || n != k0) ; // test loop on (n,d) = (k0,1)first couple
        if(i&1) NbImpair++ ;
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",NbImpair);
    return 1 ;
}

#define PB065_NITER         100
#define PB065_MAXDIGIT  (PB065_NITER+10)

#define PB065_N0    1
#define PB065_N1    2

#define PB065_D0    0
#define PB065_D1    1

typedef struct FractCont16 {
    uint16_t *pt1 ;
    uint16_t *pt ;
    int nbDig ;
}  FractCont16 ;
int PB065(PB_RESULT *pbR) {
    int n ;
    pbR->nbClock = clock()  ;
    uint16_t DigN1[PB065_MAXDIGIT] ;
    uint16_t DigN0[PB065_MAXDIGIT] ;
    uint16_t DigD1[PB065_MAXDIGIT] ;
    uint16_t DigD0[PB065_MAXDIGIT] ;
    FractCont16 FC[2] ; // numerateur indice 0, denominateur indice 1
    FC[0].nbDig = 1 ;
    FC[0].pt = DigN0 ;
    FC[0].pt1 = DigN1 ;
    FC[0].pt[0] = PB065_N1 ;
    FC[0].pt1[0] = PB065_N0 ;
    
    FC[1].nbDig = 1 ;
    FC[1].pt = DigD0 ;
    FC[1].pt1 = DigD1 ;
    FC[1].pt[0] = PB065_D1 ;
    FC[1].pt1[0] = PB065_D0 ;
    for(n=2;n<=PB065_NITER;n++) {
        int i ;
        int k ;
        int a = (n % 3) ? 1 : 2*(n/3) ;
        for(k=0;k<2;k++) { // numerateur et denominateur
            uint16_t *tmp = FC[k].pt1 ;
            // pt->N(n) pt1->N(n->1)
            FC[k].pt1  = FC[k].pt ; FC[k].pt = tmp ;
            // pt1->N(n) pt->N(n-1) => N(n+1)
            // N(n+1) = a*N(n) + N(n-1)
            int carry = 0 ;
            for(i=0;i<FC[k].nbDig;i++) {
                FC[k].pt[i] += a * FC[k].pt1[i] + carry ;
                if(FC[k].pt[i] >= 10) {
                    carry = FC[k].pt[i] / 10 ;
                    FC[k].pt[i] = FC[k].pt[i] % 10 ;
                } else {
                    carry = 0 ;
                }
            }
            while(carry) {
                FC[k].pt1[FC[k].nbDig] = 0 ;
                FC[k].pt[FC[k].nbDig++] = carry % 10 ;
                carry /= 10 ;
            }
        }
    }
    {
        int k,SumD = 0;
        for(k=0;k<FC[0].nbDig;k++) SumD += FC[0].pt[k] ;
        snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",SumD);
        
    }
    
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}


#include "p067_data.h"
#define PB067_SIZE  100
#define PB067_TSIZE ((PB067_SIZE*(PB067_SIZE+1))/2)
int PB067(PB_RESULT *pbR) {
    
    int ic,ir ;
    pbR->nbClock = clock() ;
    const uint8_t * p067_data = P067_GetData() ;
    int vals[PB067_TSIZE] ;
    /* on recopie le tableau */
    {
        for(ic=0; ic <PB067_TSIZE;ic++) vals[ic] = p067_data[ic] ;
    }
    // on commernce a l'avant derniere ligne
    for(ir=PB067_SIZE-2;ir>=0;ir--) {
        int ic0 = (ir*(ir+1))/2 ;
        for(ic=0;ic<=ir;ic++) {
            int icnr = ic0+ir+1+ic ;
            vals[ic0+ic] += (vals[icnr] > vals[icnr+1]) ? vals[icnr] : vals[icnr+1] ;
        }
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",vals[0]) ;
    return 1 ;
}

// pour avoir 16 digits il faut que le 10 soit exterieur (sinon compte 2 fois donc 17 digits)
// comme on commence par le plus petit exterieur les chiffres exterieur sont 6,7,8,9,10
// et donc les chiffres interieurs 1,2,3,4,5
// Le total = sum(ext) +2 sum(int) = 40 + 2x15 = 70, donc le total sur une ligne est 70/5=14
// Pour completer le 6 seul couple possible 3,5.
// Pour completer le 10 seul couple possible 1,3
// Donc 3 partege entre 6 et 10, donc 6 et 10 sont voisin.
// 6,5,3 > 6,3,5 donc le 10 est le voisin apres le 6. Et l'on a place 6,5,3,10,1
// Pour completer le 9, puisque le 3 est pris par 6 et 10 il ne reste que 1,4
// Comme le 1 est place , le 9 est apres le 10.
// Il ne reste a placer en int que le 2, qui conduit donc en externe a placer le 8 puis le 4.
// Les lignes sont donc :
// 6,5,3 , 10,3,1, 9,1,4, 8,4,2, 7,2,5

int PB068(PB_RESULT *pbR) {
    
    
    pbR->nbClock = clock() ;
    strcpy(pbR->strRes,"6531031914842725") ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB069_MAXN  1000000
int PB069a(PB_RESULT *pbR) {
    int32_t N = PB069_MAXN ;
    int32_t *phi = malloc(N * sizeof(phi[0])) ;
    int i, nBest = 2 ;
    pbR->nbClock = clock() ;
    for(i=0;i<N;i++) phi[i]=i ;
    if(pbR->isVerbose){
        fprintf(stdout,"\t PB%sa ",pbR->ident) ;
    }
    for(i=2;i<N;i++) {
        if(phi[i] == i) { // nouveau nombre premier
            int np ;
            for(np=i;np<N;np+= i) {
                phi[np] = phi[np]/i * (i-1) ;
            }
        }
        if(i*(uint64_t)phi[nBest] > phi[i]*(uint64_t)nBest) {
            if(pbR->isVerbose)fprintf(stdout,"(%d,%d)",i,phi[i]);
            nBest = i ;
        }
    }
    if(pbR->isVerbose){
        fprintf(stdout,"\n\t PB%sa best n/phi %.2f for %d\n",pbR->ident,((float) nBest)/phi[nBest] ,nBest) ;
    }
    
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nBest);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}


// comme n/phi(n) = prod (Pi/(Pi-1) ) = prod(1/ (1 -1/Pi))
// les plus grande valeurs sont pour le maximum de Pi, et pour chacun la plus petite valeur
// Donc on cherche 2x3x5x7x11... xPi < PB069_MAXN
int PB069b(PB_RESULT *pbR) {
    int32_t N = PB069_MAXN ;
    int32_t P = 1 ;
    int32_t phi = 1 ;
    pbR->nbClock = clock() ;
    if(pbR->isVerbose){
        fprintf(stdout,"\t PB%sb ",pbR->ident) ;
    }
    // ss optimiser les nbre premiers
    int p ;
    for(p=2;p*P < N ;p++) {
        int d,d2,isPrime ;
        for(d=2,d2=4,isPrime=1; d2<=p;d++) {
            if((p % d )== 0) { isPrime = 0 ; break ; }
            d2 += 2*d+1 ;
        }
        if(isPrime) { P *= p ; phi *= p-1 ; }
    }
    
    if(pbR->isVerbose){
        fprintf(stdout,"\t PB%sb best n/phi %.2f for %d\n",pbR->ident,((float) P)/phi ,P) ;
    }
    
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",P);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}


#define PB070_MAXN  10000000
int PB070(PB_RESULT *pbR) {
    int32_t N = PB070_MAXN ;
    int32_t *phi = malloc(N * sizeof(phi[0])) ;
    int i, nBest = 2 ;
    pbR->nbClock = clock() ;
    for(i=0;i<N;i++) phi[i]=i ;
    for(i=2;i<N;i++) {
        if(phi[i] == i) { // nouveau nombre premier
            int np ;
            for(np=i;np<N;np+= i) {
                phi[np] = phi[np]/i * (i-1) ;
            }
        }
        if(i*(uint64_t)phi[nBest] < phi[i]*(uint64_t)nBest) {
            unsigned char str_i[10], str_phi[10] ;
            int lg = snprintf((char *)str_i,sizeof(str_i),"%d",i) ;
            HeapSortUint8(str_i,lg);
            lg=snprintf((char *) str_phi,sizeof(str_phi),"%d",phi[i]);
            HeapSortUint8(str_phi,lg);
            if(strcmp((char *)str_i,(char *)str_phi)== 0) {
                if(pbR->isVerbose)fprintf(stdout,"(%d,%d)",i,phi[i]);
                nBest = i ;
            }
        }
    }
    if(pbR->isVerbose){
        fprintf(stdout,"\t PB%s best n/phi %.6f for %d phi=%d\n",pbR->ident,((float) nBest)/phi[nBest] ,nBest,phi[nBest]) ;
    }
    
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nBest);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}
//
// on utilise le fait que la solution est
// Pi*Pj < N  avec (1-1/Pi)*(1-1/Pj) maximum <=> Pi/(Pi-1)*Pj/(Pj-1) minimum
// La solution est proche de sqrt(N) donc on filtre mieux en partant de la
int PB070a(PB_RESULT *pbR) {
    int32_t N = PB070_MAXN ;
    int32_t nSqrt = (int32_t) Sqrt64(N) ;
    CTX_PRIMETABLE * ctxP  ;
    pbR->nbClock = clock()  ;
    if((ctxP = Gen_tablePrime(N/2+1)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    int i,j,jmax, nBest = 2 , phiBest = 1 ;
    pbR->nbClock = clock() ;
    uint32_t nbPrime = GetNbPrime(ctxP) ;
    const T_prime * tbPrime = GetTbPrime(ctxP) ;
    for(i=0;i<nbPrime && (tbPrime[i] < nSqrt) ;i++) ;
    // on continue la boucle sur les Pi (croissants) et jmax est la valeur maxi pour que Pi*Pj < N)
    for(jmax=i-1;i<nbPrime;i++){
        int Pi = (int)tbPrime[i] ;
        int Pj ;
        int n,phi ;
        for(j=jmax;j>=0; j--) {
            if( (n=Pi*(Pj=(int)tbPrime[j])) >= N) { jmax-- ; continue ; }
            if(n*(uint64_t) phiBest < (phi=(Pi-1)*(Pj-1)) * (uint64_t) nBest) {
                unsigned char str_n[10], str_phi[10] ;
                int lg = snprintf((char *)str_n,sizeof(str_n),"%d",Pi*Pj) ;
                HeapSortUint8(str_n,lg);
                lg=snprintf((char *) str_phi,sizeof(str_phi),"%d",phi);
                HeapSortUint8(str_phi,lg);
                if(strcmp((char *)str_n,(char *)str_phi)== 0) {
                    nBest = n ;
                    phiBest = (Pi-1)*(Pj-1);
                    //                    if(pbR->isVerbose)fprintf(stdout,"(%d,%d)",nBest,phiBest);
                }
            } else {
                // a Pi fixe, on ne peut que degrader en decroissant j
                break ;
            }
        }
    }
    if(pbR->isVerbose){
        fprintf(stdout,"\t PB%s best n/phi %.6f for %d phi=%d\n",pbR->ident,((float) nBest)/phiBest ,nBest,phiBest) ;
    }
    
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nBest);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB070_MAX 100000
// facile de voir que l'on a n(k)*d(k-1)-d(k)*n(k-1) = 1
// donc si n(k) = 3 et d(k) =7
// 3 d(k-1) - 7 n(k-1) = 1
// 3x5-7x2=1
// donc d(k-1) = 5 + 7xi et n(k-1) = 2 + 3xi
// i = (1000000 - 5) / 7
// et n = 2 + 3*i
int PB071(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int i = (1000000 - 5) / 7 ;
    int d = 5 + 7 * i ;
    int n = 2 + 3 * i ;
    if(pbR->isVerbose){
        fprintf(stdout,"\t PB%s The fraction before 3/7 is %d/%d\n",pbR->ident,n,d) ;
    }
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",n);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB072_MAXN  1000000
int PB072(PB_RESULT *pbR) {
    int32_t N = PB072_MAXN ;
    int32_t *phi = malloc((N +1)* sizeof(phi[0])) ;
    uint64_t S = 0 ;
    int i ;
    pbR->nbClock = clock() ;
    for(i=0;i<=N;i++) phi[i]=i ;
    for(i=2;i<=N;i++) {
        if(phi[i] == i) { // nouveau nombre premier
            int np ;
            for(np=i;np<=N;np+= i) {
                phi[np] = phi[np]/i * (i-1) ;
            }
        }
        S += phi[i] ;
    }
    
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",S);
    free(phi) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}


#define PB073_MAXN  12000

#define PB073_N     1
#define PB073_D     3

#define PB073_Nend     1
#define PB073_Dend     2

// version calculant directement une par une les valeurs
int PB073(PB_RESULT *pbR) {
    int32_t N = PB073_MAXN ;
    int nb = 0 ;
    int d ;
    int n ;
    pbR->nbClock = clock() ;
    int d_end=PB073_Dend ;
    int n_end=PB073_Nend ;
    FRACTRED fr1 = {PB073_N,PB073_D} ;
    FRACTRED fr0 = IBesout(fr1) ;
    d = fr1.d ; n = fr1.n ;
    int d0 = fr0.d ;
    int n0 = fr0.n ;
    //  satisfait besout n x d0 - d * n0 = 1
    do {
        int a = (N+d0)/d ; // searcd d = a * d - d0 biggest
        int tmp = d ;
        d = a * d - d0 ;
        d0 = tmp ;
        tmp = n ;
        n = a * n - n0 ; // n = a * n - n0 ;
        n0 = tmp ;
//        printf("%d/%d ",n,d) ;
        nb++ ;
        // besout  n x d0 - d * n0 = 1 is transmitted
        // (a *n - n0) *d - (a*d - d0) * n = n x d0 - d * n0 = 1;
    } while(d != d_end || n != n_end ) ;
    
    
    
    
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nb-1);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}
// version by Stern-Brocot Tree with stack
int PB073a(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    FRACTRED fr0 = {1,3} , fr1 = {1,2} ; ;
    
    SBTree *sbt = SBT_alloc() ;
    int nb = 0 ;
    SBT_init(sbt,fr0, fr1) ;
    while(sbt->indS > 0 ) {
        if(sbt->fr0.d + sbt->fr1.d <= PB073_MAXN) {
            if(SBT_ValidNxt(sbt,1)==0) {
                if(pbR->isVerbose) fprintf(stdout,"\tPB%s ERROR REALLOC SBT(%d)\n",pbR->ident,sbt->sizeStack) ;
                SBT_free(sbt);
                return 0 ;
            }
        } else {
            nb++ ;
            SBT_ValidNxt(sbt,0) ;
        }
    }
    SBT_free(sbt);
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nb-1) ;
    return 1 ;
}
// version by Stern-Brocot Tree recursive
static int nb_pb073b = 0 ;
int PB073bCB(FRACTRED fr0, FRACTRED fr1) {
    if(fr0.d+fr1.d<=PB073_MAXN) {
        return 1 ;
    }    else {
        nb_pb073b++ ;
        return 0 ;
    }
}

int PB073b(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    FRACTRED fr0 = {1,3} , fr1 = {1,2} ; ;
    STBrcv(fr0,fr1,PB073bCB) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nb_pb073b-1) ;
    return 1 ;
}
// version by Stern-Brocot Tree recursive qith only denominators
static int nb_pb073c = 0 ;
int PB073cCB(int d0, int d1) {
    if(d0+d1<=PB073_MAXN) {
        return 1 ;
    }    else {
        nb_pb073c++ ;
        return 0 ;
    }
}

int PB073c(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    STBrcvDen(3,2,PB073cCB) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nb_pb073b-1) ;
    return 1 ;
}



typedef uint32_t TY_FACT   ;


static TY_FACT FactDig[10] = { 1,1,2,6,24,120,720,5040,40320,362880 } ;

void FactVal(TY_FACT *valCur) {
    TY_FACT val = valCur[0] ;
    if(val == 169 ) {
        valCur[1] = FactDig[1] + FactDig[6] + FactDig[9] ; // 363601
        valCur[2] = 2 * FactDig[3] + 2 * FactDig[6] + FactDig[0] + FactDig[1] ;
        return ;
        
    }else if( val == 871 || val == 872) {
        valCur[1] = FactDig[8] + FactDig[7] + FactDig[val % 10] ;
        return ;
    } else {
        TY_FACT q;
        TY_FACT newVal , curval = val ;
        for(newVal=0, q=curval/10 ;curval ;curval = q, q=curval /10 ){
            newVal += FactDig[curval - 10 * q] ;
        }
        if(newVal == val) {
            return ;
        }
        valCur[1] = newVal ;
        FactVal(valCur+1);
    }
}

// ne retourne que la longueur, pas les valeurs
#define FACT_MEM    1
#if FACT_MEM
#define FACT_MEM_SIZE (1<<19)
static uint16_t lgF[FACT_MEM_SIZE] ;
static uint16_t privFactLgMem(uint16_t lg,TY_FACT valInit,TY_FACT val) {
    if(val < FACT_MEM_SIZE ) {
        if(lgF[val]) {
            if(valInit < FACT_MEM_SIZE) lgF[valInit] = lg+lgF[val] ;
            return lg+lgF[val] ;
        } else if (val == 169) {
            lgF[val] = 3 ;
            if(valInit < FACT_MEM_SIZE) lgF[valInit] = lg+lgF[val] ;
            return lg+lgF[val] ;
        } else if ( val == 871 || val == 872) {
            lgF[val] = 2 ;
            if(valInit < FACT_MEM_SIZE) lgF[valInit] = lg+lgF[val] ;
            return lg+lgF[val] ;
        }
    }
    {
        TY_FACT q;
        TY_FACT newVal, valcur = val ;
        for(newVal=0  ;valcur > 9 ;valcur = q ){
            q=valcur/10 ;
            newVal += FactDig[valcur - 10 * q] ;
        }
        newVal +=FactDig[valcur] ;
        if(newVal == val) {
            if(val < FACT_MEM_SIZE ) lgF[val] = 1 ;
            return lg+1 ;
        }
        return privFactLgMem(lg+1,valInit,newVal);
    }
    
}
#else

static uint16_t privFactLgH3(uint16_t lg,TY_FACT val2,TY_FACT val1,TY_FACT val) {
    TY_FACT q;
    TY_FACT newVal, valcur = val ;
    for(newVal=0 ;valcur > 9 ;valcur = q ){
        q=valcur/10 ;
        newVal += FactDig[valcur - 10 * q] ;
    }
    newVal += FactDig[valcur] ;
    if(val == newVal || newVal == val1 || newVal == val2 ) {
        return lg+1 ;
    }
    return privFactLgH3(lg+1,val1,val,newVal);
}

static uint16_t privFactLgH2(uint16_t lg,TY_FACT val1,TY_FACT val) {
    TY_FACT q;
    TY_FACT newVal, valcur = val ;
    if(val == 169) {
        return lg +3 ;
    }
    for(newVal=0 ;valcur > 9 ;valcur = q ){
        q=valcur/10 ;
        newVal += FactDig[valcur - 10 * q] ;
    }
    newVal += FactDig[valcur] ;
    if(val == newVal || newVal == val1  ) {
        return lg+1 ;
    }
    return privFactLgH2(lg+1,val,newVal);
}

#endif
static uint16_t privFactLg(uint16_t lg,TY_FACT val) {
    if(val == 169 ) {
        return lg + 3 ;
    } else if ( val == 871 || val == 872) {
        return lg + 2 ;
    } else {
        TY_FACT q;
        TY_FACT newVal, valcur = val ;
        for(newVal=0 ;valcur > 9 ;valcur = q ){
            q=valcur/10 ;
            newVal += FactDig[valcur - 10 * q] ;
        }
        newVal += FactDig[valcur] ;
        if(val == newVal) {
            return lg+1 ;
        }
        return privFactLg(lg+1,newVal);
    }
}


static uint16_t FactLg(uint32_t val) {
#if FACT_MEM
    return privFactLgMem(0,val,val) ;
#else
    return privFactLg(0,val) ;
    //    return privFactLgH3(0,0,0,val) ;
    //    return privFactLgH2(0,0,val) ;
#endif
}

#define PB074_MAX_VALUE 1000000
#define PB074_PRINT 0
#define PB074_LGCOUNT   60
int PB074(PB_RESULT *pbR) {
    TY_FACT k ;
    TY_FACT kBest = 1 ;
    int bestLg = 0 ;
    int nbCount = 0 ;
    pbR->nbClock = clock() ;
    for(k=1;k<PB074_MAX_VALUE;k++) {
        uint16_t lg = FactLg(k) ;
        if(lg > bestLg ) {
            bestLg = lg ;
            kBest = k ;
        }
        if(lg==PB074_LGCOUNT ) {
            nbCount++ ;
        }
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    if(pbR->isVerbose)fprintf(stdout,"\t PB%s  lg=%d seen %d first for %d\n"
                              ,pbR->ident
                              ,bestLg,nbCount,kBest);
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbCount);
#if PB074_PRINT
    {
        TY_FACT *chainVal = malloc((bestLg+1)*sizeof(chainVal[0])) ;
        chainVal[0] = kBest ;
        FactVal(chainVal);
        
        {
            int i ;
            for(i=0 ; i<bestLg ; i++) {
                printf("%d%s",chainVal[i], (i==bestLg-1) ? "\n" : "->" );
            }
        }
        free(chainVal);
    }
#endif
    return 1 ;
}

//
// methode differente on remarque que l'on peut permuter les digits pour obtenir la meme longueur
// donc on cherche le calcul des longueurs uniquement pour le representant a digit croissant
int PB074a(PB_RESULT *pbR) {
    int nbCount = 0 ;
    pbR->nbClock = clock() ;
    int d1,d2,d3,d4,d5,d6 ;
    int dhist[10] = {0,0,0,0,0,0,0,0,0,0} ;
    if(pbR->isVerbose)fprintf(stdout,"\t PB%s ",pbR->ident);
    for(d1=0;d1<=9;d1++) {
        dhist[d1]++ ;
        for(d2=d1;d2<=9;d2++) {
            dhist[d2]++ ;
            for(d3=d2;d3<=9;d3++) {
                dhist[d3]++ ;
                for(d4=d3;d4<=9;d4++) {
                    dhist[d4]++ ;
                    for(d5=d4;d5<=9;d5++) {
                        dhist[d5]++ ;
                        for(d6=d5;d6<=9;d6++) {
                            dhist[d6]++ ;
                            int k = d1*100000+d2*10000+d3*1000+d4*100+d5*10+d6 ;
                            if(privFactLg(0,k) == PB074_LGCOUNT) {
                                int i,npermut = 1;
                                for(i=2;i< 6-dhist[0];i++) { // (nbdig-1)!
                                    npermut *= i ;
                                }
                                if(dhist[1]) { // rempacement des 1 par des zeros sauf le 1 leader
                                    npermut = (npermut * (1 << (dhist[1]-1)) )* (12-2*dhist[0]-dhist[1]) ;
                                } else {
                                    npermut *= (6 -dhist[0])  ;
                                }
                                for(i=2;i<=9;i++) {
                                    if(dhist[i] > 1) npermut /= FactDig[dhist[i]] ;
                                }
                                nbCount += npermut ;
                                if(pbR->isVerbose)fprintf(stdout," +Permut(%d)=%d ",k,npermut);
                            }
                            dhist[d6]-- ;
                        }
                        dhist[d5]-- ;
                    }
                    dhist[d4]-- ;
                }
                dhist[d3]-- ;
            }
            dhist[d2]-- ;
        }
        dhist[d1]-- ;
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    if(pbR->isVerbose)fprintf(stdout,"\n\t PB%s seen %d \n"
                              ,pbR->ident,nbCount);
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbCount);
#if PB074_PRINT
    {
        TY_FACT *chainVal = malloc((bestLg+1)*sizeof(chainVal[0])) ;
        chainVal[0] = kBest ;
        FactVal(chainVal);
        
        {
            int i ;
            for(i=0 ; i<bestLg ; i++) {
                printf("%d%s",chainVal[i], (i==bestLg-1) ? "\n" : "->" );
            }
        }
        free(chainVal);
    }
#endif
    return 1 ;
}


#define PB075_MAXN  1500000
// pour m > n > 0
// a = (m**2 - n**2)*k ; b = 2*m*n*k ; c = (m**2 + n**2)*k ;
// donc S/2 = (m**2 + m*n)*k = m * (m+n) * k
int PB075(PB_RESULT *pbR) {
    int32_t N = PB075_MAXN/2 ;
    pbR->nbClock = clock() ;
    int32_t m,mSqrt = (int32_t) Sqrt64(N);
    int32_t * nbPyth = calloc(N+1 ,sizeof(nbPyth[0])) ;
    uint32_t nb = 0 ;
    
    for(m=2;m<mSqrt;m++) {
        int32_t n ;
        int32_t L = m*m ;
        int32_t H = L ;
        for(n=1;n<m;n++) {
            L += m ;
            H += 2*n - 1 ;
            if( ((n&1) ^(m& 1)) == 0) continue ; // on saute si parite identique (pour l'unicite)
            {
                int kH,kL ;
                for(kL=L,kH=H;kL <=N;kL+= L, kH += H) {
                    if(nbPyth[kL] == 0) {
                        nbPyth[kL] = kH  ;
                    } else if(nbPyth[kL] != kH ) {
                        nbPyth[kL] = -1 ;
                    }
                }
            }
        }
    }
    int i ;
    for(i=1;i<=N;i++) {
        if(nbPyth[i]>0) {
            nb++ ;
        }
    }
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nb);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB076_MAXN  100
#define IND76(k,n) ((PB076_MAXN)*(n-1)+(k-1))
int PB076(PB_RESULT *pbR) {
    int32_t Ekn[(PB076_MAXN)*(PB076_MAXN)] ;
    int n ;
    pbR->nbClock = clock() ;
    
    Ekn[IND76(1,1)] = 1 ;
    for(n=2;n<=PB076_MAXN;n++) {
        int k ;
        Ekn[IND76(1,n)] = 1 ;
        for(k=2;k<=n/2;k++) {
            // decompostion de n en k nombre.
            // soit la decomposition contient au mois un 1, et donc en l'enlevant on tombe sur Ekn(k-1,n-1)
            // soit la decomposition ne contient pas de 1, et l'on peut enlever 1 a chaque element Ekn(k,n-k)
            // Le deuxieme cas ne peut se produire que si k <= n/2
            Ekn[IND76(k,n)] = Ekn[IND76(k-1,n-1)] + Ekn[IND76(k,n-k)] ;
        }
        for(;k<=n;k++) {
            Ekn[IND76(k,n)] = Ekn[IND76(k-1,n-1)]  ;
        }
    }
    int k,S ; // on saute k=1 car somme en au moins 2 elements demande
    for(k=2,S=0;k<=PB076_MAXN;k++) S += Ekn[IND76(k,PB076_MAXN)] ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",S);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

// base sur le theoreme pentagonal d'euler
// P(n) = Sum(k){ (-1)**(k+1) P(n - (k(3k-1))/2) }
int PB076a(PB_RESULT *pbR) {
    int32_t Pn[PB076_MAXN+1] ;
    int n ;
    pbR->nbClock = clock() ;
    Pn[0] = Pn[1] = 1 ;
    for(n=2;n<=PB076_MAXN;n++) {
        int k , P = 0 ,Pk = 1  ;
        for(k=1; Pk <= n ; Pk += 3*k+1, k++ ) {
            if(k&1) {
                P += Pn[n - Pk] ;
                // car P-k = Pk+k
                if(Pk + k <= n ) P += Pn[n - Pk - k ] ;
            } else {
                P -= Pn[n - Pk] ;
                if(Pk + k <= n ) P -= Pn[n - Pk - k ] ;
            }
        }
        Pn[n] = P ;
    }
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",Pn[PB076_MAXN]-1);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB077_MAXN  100
#define PB077_NB_ASK    5000
// calculate E(k,n) où k represente le kime nombre premier Pk  et 0 <= n < Maxn
// E(kn) represente le nombre de decomposition n'utilisant que des premiers <= Pk
// On calcule par recurrence sur k  ( E(0,n) vaut 1 si n pair 0 sinon )
// Ensuite E(k,n) = Sum (i=0, ...) E(k-1, n - i * Pk).
int PB077(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    CTX_PRIMETABLE *ctxP ;
    if((ctxP = Gen_tablePrime(PB077_MAXN)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    uint32_t nbPrime = GetNbPrime(ctxP) ;
    const T_prime * tbPrime = GetTbPrime(ctxP) ;
    
    int32_t *Epn = calloc(nbPrime*PB077_MAXN,sizeof(Epn[0])) ;
    int ip,k ;
    int p = 2 ;
    for(k=0;2*k<PB077_MAXN;k++) {
        Epn[2*k] = 1 ; Epn[2*k+1] = 0 ;
    }
    
    for(ip=1;ip<nbPrime;ip++) {
        p = (int)tbPrime[ip] ;
        int np ;
        for(np=0; np < PB077_MAXN; np+=p) {
            for(k=np;k < PB077_MAXN;k++) {
                Epn[ip * PB077_MAXN + k] += Epn[(ip-1)*PB077_MAXN+k-np] ;
            }
        }
    }
    int kmin ;
    for(kmin=0;kmin<PB077_MAXN;kmin++) {
        //        printf("%d ",Epn[(ctxP->nbPrime-1)*PB077_MAXN + kmin]) ;
        if(Epn[(nbPrime-1)*PB077_MAXN + kmin]-1 > PB077_NB_ASK) {
            if(pbR->isVerbose) fprintf(stdout,"\t PB%s %d a %d decompositions en premiers\n"
                                       ,pbR->ident,kmin,Epn[(nbPrime-1)*PB077_MAXN + kmin]-1);
            break ;
        }
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    free(Epn) ;
    Free_tablePrime(ctxP) ;
    if(kmin<PB077_MAXN){
        snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",kmin);
        return 1 ;
    } else {
        return 0 ;
    }
}



// voir PB076a
// meme calcul avec la valeur exacte de Pn[n] (sans -1)
#define PB078_MAX   100000
#define PB078_DIV   1000000
int PB078(PB_RESULT *pbR) {
    int32_t * Pn= malloc((PB078_MAX+1) *sizeof(Pn[0])) ;
    int n ;
    pbR->nbClock = clock() ;
    Pn[0] = Pn[1] = 1 ;
    for(n=2;n<=PB078_MAX;n++) {
        int k ;
        int32_t Pk = 1  ;
        int32_t P = 0 ;
        for(k=1; Pk <= n ; Pk += 3*k+1, k++ ) {
            if(k&1) {
                P += Pn[n - Pk] ;
                // car P-k = Pk+k
                if(Pk + k <= n ) P += Pn[n - Pk - k ] ;
            } else {
                P -= Pn[n - Pk] ;
                if(Pk + k <= n ) P -= Pn[n - Pk - k ] ;
            }
        }
        P =  P % PB078_DIV ;
        //        if (P < 0) P += PB078_DIV ;
        Pn[n] =  P ;
        if(P == 0) break ;
    }
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",n);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#include "p081_data.h"



#define MATDYN(i,j)  ((i)*(sizeM+2)+(j))
#define MATDYN_PC  1000000000
#define MATDYN_PD  1000000000

int ProgDyn_Mat(int sizeM,const uint16_t * cout,const int *startCase, const int *endCase,const int *Neighbour ) {
    uint32_t   *antDist = malloc((sizeM+2)*(sizeM+2)*sizeof(antDist[0])) ;
    uint32_t   *newDist = malloc((sizeM+2)*(sizeM+2)*sizeof(newDist[0])) ;
    
    uint32_t   *Cout = malloc((sizeM+2)*(sizeM+2)*sizeof(Cout[0])) ; ;
    
    int i,j ;
    for(j=0;j<=sizeM+1;j++) {
        Cout[MATDYN(0,j)] = Cout[MATDYN(sizeM+1,j)] = MATDYN_PC ;
        newDist[MATDYN(0,j)] = newDist[MATDYN(sizeM+1,j)] = MATDYN_PD ;
        antDist[MATDYN(0,j)] = antDist[MATDYN(sizeM+1,j)] = MATDYN_PD ;
    }
    for(i=1;i<=sizeM;i++) {
        Cout[MATDYN(i,0)] = Cout[MATDYN(i,sizeM+1)] = MATDYN_PC ;
        newDist[MATDYN(i,0)] = newDist[MATDYN(i,sizeM+1)] = MATDYN_PD ;
        antDist[MATDYN(i,0)] = antDist[MATDYN(i,sizeM+1)] = MATDYN_PD ;
        for(j=1;j<=sizeM;j++) {
            Cout[MATDYN(i,j)] = cout[i*sizeM+j-sizeM-1] ;
            newDist[MATDYN(i,j)] = MATDYN_PD ;
        }
    }
    // debut
    while(*startCase) {
        newDist[*startCase] = Cout[*startCase] ;
        startCase++ ;
    }
    int isMod = 0;
    do {
        isMod = 0 ;
        uint32_t   * tmp = antDist ;
        antDist = newDist ;
        newDist = tmp ;
        for(i=1;i<=sizeM;i++) {
            int ic = MATDYN(i,1) ;
            for(j=1;j<=sizeM;j++) {
                int k ;
                uint32_t bestD ;
                int cout_ic = Cout[ic] ;
                bestD = antDist[ic] - cout_ic ;
                for(k=0;Neighbour[k];k++) {
                    if(antDist[ic+Neighbour[k]] < bestD) { bestD = antDist[ic+Neighbour[k]] ; isMod = 1 ; }
                }
                newDist[ic++] = bestD + cout_ic ;
            }
        }
    } while(isMod) ;
    uint32_t minD = newDist[*endCase];
    
    while(*++endCase){
        if(newDist[*endCase] < minD) {
            minD = newDist[*endCase] ;
        }
    }
    free(Cout); free(antDist); free(newDist);
    return minD;
}


#define P081_SIZE  80

int PB081(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int neigh[] = { -1 , -P081_SIZE-2 , 0  } ;
    int start[] = { P081_SIZE+3 , 0 } ;
    int end[] = { (P081_SIZE+2)*P081_SIZE+ P081_SIZE  , 0 } ;
    const uint16_t * p81_data = P081_GetData() ;
    int best = ProgDyn_Mat(P081_SIZE,p81_data,start,end,neigh) ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",best);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

int PB082(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int neigh[] = { -1 , -P081_SIZE-2 , +P081_SIZE+2, 0  } ;
    int i ;
    int start[P081_SIZE+1] ;
    int end[P081_SIZE+1] ;
    const uint16_t * p81_data = P081_GetData() ;
    for(i=1;i<=P081_SIZE;i++){
        start[i-1] = i*(P081_SIZE+2) + 1 ;
        end[i-1] = i*(P081_SIZE+2) + P081_SIZE ;
    }
    start[P081_SIZE] = end[P081_SIZE] = 0 ;
    int best = ProgDyn_Mat(P081_SIZE,p81_data,start,end,neigh) ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",best);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

int PB083(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int neigh[] = { 1, -1 , -P081_SIZE-2 , +P081_SIZE+2, 0  } ;
    int start[] = { P081_SIZE+3 , 0 } ;
    int end[] = { (P081_SIZE+2)*P081_SIZE+ P081_SIZE  , 0 } ;
    const uint16_t * p81_data = P081_GetData() ;
    int best = ProgDyn_Mat(P081_SIZE,p81_data,start,end,neigh) ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",best);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

//#define PB084_NBFACE    6
#define PB084_NBFACE    4

#define PB084_NBCASE    40
#define PB084_NBITER    100
#define JAIL_CASE   10
#define GOTO_JAIL_CASE   30
#define C1_CASE  11
#define E3_CASE  24
#define H2_CASE  39
#define R1_CASE  5

int PB084(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    double MatTr[PB084_NBCASE*PB084_NBCASE] ;
    double DoubleDice[2*PB084_NBFACE] ;
    double oldProb[PB084_NBCASE] , CaseProb[PB084_NBCASE] ;
    int i,j,d,n ;
    DoubleDice[0] = 0 ;
    for(i=1;i<=PB084_NBFACE;i++) {
        DoubleDice[i]=DoubleDice[2*PB084_NBFACE-i] = i / (((double)PB084_NBFACE) * PB084_NBFACE) ;
    }
    for(i=0;i<PB084_NBCASE*PB084_NBCASE;i++) MatTr[i] = 0 ;
    // Dice deplacement
    for(i=0;i<PB084_NBCASE;i++) {
        for(d=0;d<2*PB084_NBFACE;d++) {
            MatTr[PB084_NBCASE*i +  ( (i+PB084_NBCASE-d-1)  % PB084_NBCASE) ] = DoubleDice[d] ;
        }
    }
    // correction Triple double
    for(i=0;i<PB084_NBCASE;i++) {
        for(j=0;j<PB084_NBCASE;j++) {
            if(MatTr[PB084_NBCASE*i+j] > 0 && ((j+i) & 1) == 0 && j !=JAIL_CASE) {
                MatTr[PB084_NBCASE*i+j] -= 1/(36.0*36.0) ;
                MatTr[PB084_NBCASE*JAIL_CASE+j] += 1/(36.0*36.0) ;
            }
        }
    }
   
     // Correction GTJ
    for(j=0;j<PB084_NBCASE;j++) {
        MatTr[JAIL_CASE*PB084_NBCASE+j] += MatTr[GOTO_JAIL_CASE*PB084_NBCASE+j] ;
        MatTr[GOTO_JAIL_CASE*PB084_NBCASE+j] = 0 ;
    }
    // CH correction 7 , 22 , 36
    // doit etre avant car il peut y avoir CH(36)- -3case --> CC(33)
    int CHcase[3] = {7,22,36} ;
    
    for(n=0;n<3;n++) {
        int ch = CHcase[n] ;
        int ch_next[10] = { 0, JAIL_CASE,C1_CASE,E3_CASE,H2_CASE,R1_CASE,0,0,0,ch-3} ;
        ch_next[6]=ch_next[7] = (n==0) ? 15 : ((n==1) ? 25 : 5 ) ;
        ch_next[8] = (n==1) ? 28 : 12 ;

        for(j=0;j<PB084_NBCASE;j++) {
            int next ;
            for(next=0;next<10;next++) {
                MatTr[ch_next[next]*PB084_NBCASE+j] += MatTr[ch*PB084_NBCASE+j] / 16.0 ;
            }
            MatTr[ch*PB084_NBCASE+j] *= 6.0/16.0 ;
        }
    }
    // CC correction case 02, 17, 33
    int CCcase[3] = { 2,17,33} ;
    for(n=0;n<3;n++) {
        int cc = CCcase[n] ;
        int cc_next[2] = { 0, JAIL_CASE} ;

        for(j=0;j<PB084_NBCASE;j++) {
            int next ;
            for(next=0;next<2;next++) {
                MatTr[cc_next[next]*PB084_NBCASE+j] += MatTr[cc*PB084_NBCASE+j] / 16.0 ;
            }
            MatTr[cc*PB084_NBCASE+j] *= 14.0/16.0 ;
        }
    }
   
    for(i=0;i<PB084_NBCASE;i++) CaseProb[i] = 0 ;
    CaseProb[0] = 1 ;
    for(n=0;n<PB084_NBITER;n++) {
        memcpy(oldProb,CaseProb,sizeof(CaseProb)) ;
        for(i=0;i<PB084_NBCASE;i++) {
            double sum = 0 ;
            for(j=0;j<PB084_NBCASE;j++) {
                sum += MatTr[PB084_NBCASE*i +j] * oldProb[j] ;
            }
            CaseProb[i] = sum ;
        }
    }
    int best=0,best1=0,best2=0 ;
    double prBest=0.0, prBest1=0.0, prBest2=0.0;
    for(i=0;i<PB084_NBCASE;i++) {
        if(CaseProb[i] > prBest) {
            prBest2= prBest1 ; prBest1 = prBest ; prBest = CaseProb[i] ;
            best2=best1 ; best1= best ; best = i ;
        } else if( CaseProb[i] > prBest1) {
            prBest2= prBest1 ; prBest1 =  CaseProb[i] ;
            best2=best1 ; best1= i ;
        } else if( CaseProb[i] > prBest2) {
            prBest2=  CaseProb[i] ;
            best2 = i ;
        }
    }
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s P[%02d]=%.5f P[%02d]=%.5f P[%02d]=%.5f\n"
                               ,pbR->ident,best,prBest,best1,prBest1,best2,prBest2);
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%02d%02d%02d",best,best1,best2);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB084_NBCASE_3   (PB084_NBCASE*3)
#define IND84(di,i,dj,j)    ( ( ((di)*PB084_NBCASE+(i)) * PB084_NBCASE_3 ) + (dj)*PB084_NBCASE+(j) )
#define djtodi(dj)  (((dj) >= 2) ? 2 : ((dj)+1))
int PB084a(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    double MatTr[PB084_NBCASE_3*PB084_NBCASE_3] ;
    double DoubleDice[2*PB084_NBFACE] ;
    double oldProb[PB084_NBCASE_3] , CaseProb[PB084_NBCASE_3] ;
    int i,d,dj,j,n ;
    DoubleDice[0] = 0 ;
    for(i=1;i<=PB084_NBFACE;i++) {
        DoubleDice[i]=DoubleDice[2*PB084_NBFACE-i] = i / (((double)PB084_NBFACE) * PB084_NBFACE) ;
    }
    for(i=0;i<PB084_NBCASE_3*PB084_NBCASE_3;i++) MatTr[i] = 0 ;
    // Dice deplacement
    for(dj=0;dj<3;dj++) {
        int di = djtodi(dj) ;
        for(j=0;j<PB084_NBCASE;j++) {
            for(d=0;d<2*PB084_NBFACE;d++) {
                int i = (j+d+1)  % PB084_NBCASE ;
                MatTr[IND84(0,i,dj,j)] += DoubleDice[d] ;
                if(d & 1) { // double possibility
                    MatTr[IND84(0,i,dj,j)] -= 1/36.0 ;
                    if(dj < 2) {
                        MatTr[IND84(di,i,dj,j)] += 1/36.0 ;
                    } else {
                        MatTr[IND84(0,JAIL_CASE,dj,j)] += 1/36.0 ;
                    }
                }
            }
        }
    }

    // Correction GTJ
    for(dj=0;dj<3;dj++) {
        int di = djtodi(dj) ;
        for(j=0;j<PB084_NBCASE;j++) {
            MatTr[IND84(di,JAIL_CASE,dj,j)] +=  MatTr[IND84(di,GOTO_JAIL_CASE,dj,j)];
            MatTr[IND84(di,GOTO_JAIL_CASE,dj,j)] = 0 ;
            if(di)MatTr[IND84(0,JAIL_CASE,dj,j)] +=  MatTr[IND84(0,GOTO_JAIL_CASE,dj,j)];
            MatTr[IND84(0,GOTO_JAIL_CASE,dj,j)] = 0 ;
        }
    }

    // CH correction 7 , 22 , 36
    int CHcase[3] = {7,22,36} ;
    
    for(n=0;n<3;n++) {
        int ch = CHcase[n] ;
        
        for(dj=0;dj<3;dj++) {
            int di = djtodi(dj) ;
            int ch_next[10] = { 0, JAIL_CASE,C1_CASE,E3_CASE,H2_CASE,R1_CASE,0,0,0,ch-3} ;
            ch_next[6]=ch_next[7] = (n==0) ? 15 : ((n==1) ? 25 : 5 ) ;
            ch_next[8] = (n==1) ? 28 : 12 ;
            for(j=0;j<PB084_NBCASE;j++) {
                int next ;
                for(next=0;next<10;next++) {
                    MatTr[IND84(di,ch_next[next],dj,j)] += MatTr[IND84(di,ch,dj,j)] / 16.0;
                    if(di)MatTr[IND84(0,ch_next[next],dj,j)] += MatTr[IND84(0,ch,dj,j)] /16.0 ;
                }
                MatTr[IND84(di,ch,dj,j)] *=  6.0 / 16.0;
                if(di)MatTr[IND84(0,ch,dj,j)] *=  6.0 /16.0 ;
                //                MatTr[ch*PB084_NBCASE+j] *= 6.0/16.0 ;
            }
        }
    }


    // CC correction case 02, 17, 33
    int CCcase[3] = { 2,17,33} ;
    for(n=0;n<3;n++) {
        int cc = CCcase[n] ;
        for(dj=0;dj<3;dj++) {
            int di = djtodi(dj) ;
            int cc_next[2] = { 0, JAIL_CASE} ;

            for(j=0;j<PB084_NBCASE;j++) {
                int next ;
                for(next=0;next<2;next++) {
                    MatTr[IND84(di,cc_next[next],dj,j)] += MatTr[IND84(di,cc,dj,j)] / 16.0 ;
                    if(di)MatTr[IND84(0,cc_next[next],dj,j)] += MatTr[IND84(0,cc,dj,j)] / 16.0;
                }

                MatTr[IND84(di,cc,dj,j)] *= 14.0/16.0 ;
                if(di)MatTr[IND84(0,cc,dj,j)] *= 14.0/16.0 ;
            }
        }
    }
/*
    for(j=0;j<PB084_NBCASE_3;j++) {
        double sum = 0;
        for(i=0;i<PB084_NBCASE_3;i++) {
            sum += MatTr[j+i*PB084_NBCASE_3] ;
//            printf("%.3f ",MatTr[j+i*PB084_NBCASE_3]) ;
        }
        printf("%.3f ",sum);
        
    }
*/

    
    for(i=0;i<PB084_NBCASE_3;i++) CaseProb[i] = 0 ;
    CaseProb[0] = 1 ;
    for(n=0;n<PB084_NBITER;n++) {
        memcpy(oldProb,CaseProb,sizeof(CaseProb)) ;
        for(i=0;i<PB084_NBCASE_3;i++) {
            double sum = 0 ;
            for(j=0;j<PB084_NBCASE_3;j++) {
                sum += MatTr[PB084_NBCASE_3*i +j] * oldProb[j] ;
            }
            CaseProb[i] = sum ;
        }
    }
    int best=0,best1=0,best2=0 ;
    double prBest=0.0, prBest1=0.0, prBest2=0.0;
    for(i=0;i<PB084_NBCASE;i++) {
        CaseProb[i]+= CaseProb[i+PB084_NBCASE]+CaseProb[i+2*PB084_NBCASE] ;
        if(CaseProb[i] > prBest) {
            prBest2= prBest1 ; prBest1 = prBest ; prBest = CaseProb[i] ;
            best2=best1 ; best1= best ; best = i ;
        } else if( CaseProb[i] > prBest1) {
            prBest2= prBest1 ; prBest1 =  CaseProb[i] ;
            best2=best1 ; best1= i ;
        } else if( CaseProb[i] > prBest2) {
            prBest2=  CaseProb[i] ;
            best2 = i ;
        }
    }
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s P[%02d]=%.8f P[%02d]=%.8f P[%02d]=%.8f\n"
                               ,pbR->ident,best,prBest,best1,prBest1,best2,prBest2);
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%02d%02d%02d",best,best1,best2);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB084b_MULT2    (16)
#define PB084b_MULT1    (16*PB084b_MULT2)
#define PB084b_MULT0    (PB084_NBFACE*PB084_NBFACE*PB084b_MULT1)

int PB084b(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int MatTr[PB084_NBCASE_3*PB084_NBCASE_3] ;
    int DoubleDice[2*PB084_NBFACE] ;
    uint32_t oldProb[PB084_NBCASE_3] , CaseProb[PB084_NBCASE_3] ;
    int i,d,dj,j,n ;
    DoubleDice[0] = 0 ;
    for(i=1;i<=PB084_NBFACE;i++) {
        DoubleDice[i]=DoubleDice[2*PB084_NBFACE-i] = i * PB084b_MULT0 / (PB084_NBFACE * PB084_NBFACE);
    }
    for(i=0;i<PB084_NBCASE_3*PB084_NBCASE_3;i++) MatTr[i] = 0 ;
    // Dice deplacement
    for(dj=0;dj<3;dj++) {
        int di = djtodi(dj) ;
        for(j=0;j<PB084_NBCASE;j++) {
            for(d=0;d<2*PB084_NBFACE;d++) {
                int i = (j+d+1)  % PB084_NBCASE ;
                MatTr[IND84(0,i,dj,j)] += DoubleDice[d] ;
                if(d & 1) { // double possibility
                    MatTr[IND84(0,i,dj,j)] -= PB084b_MULT1 ;
                    if(dj < 2) {
                        MatTr[IND84(di,i,dj,j)] += PB084b_MULT1 ;
                    } else {
                        MatTr[IND84(0,JAIL_CASE,dj,j)] += PB084b_MULT1 ;
                    }
                }
            }
        }
    }
    
    // Correction GTJ
    for(dj=0;dj<3;dj++) {
        int di = djtodi(dj) ;
        for(j=0;j<PB084_NBCASE;j++) {
            MatTr[IND84(di,JAIL_CASE,dj,j)] +=  MatTr[IND84(di,GOTO_JAIL_CASE,dj,j)];
            MatTr[IND84(di,GOTO_JAIL_CASE,dj,j)] = 0 ;
            if(di)MatTr[IND84(0,JAIL_CASE,dj,j)] +=  MatTr[IND84(0,GOTO_JAIL_CASE,dj,j)];
            MatTr[IND84(0,GOTO_JAIL_CASE,dj,j)] = 0 ;
        }
    }
    
    // CH correction 7 , 22 , 36
    int CHcase[3] = {7,22,36} ;
    
    for(n=0;n<3;n++) {
        int ch = CHcase[n] ;
        
        for(dj=0;dj<3;dj++) {
            int di = djtodi(dj) ;
            int ch_next[10] = { 0, JAIL_CASE,C1_CASE,E3_CASE,H2_CASE,R1_CASE,0,0,0,ch-3} ;
            ch_next[6]=ch_next[7] = (n==0) ? 15 : ((n==1) ? 25 : 5 ) ;
            ch_next[8] = (n==1) ? 28 : 12 ;
            for(j=0;j<PB084_NBCASE;j++) {
                int next ;
                for(next=0;next<10;next++) {
                    MatTr[IND84(di,ch_next[next],dj,j)] += MatTr[IND84(di,ch,dj,j)] / 16 ;
                    if(di)MatTr[IND84(0,ch_next[next],dj,j)] += MatTr[IND84(0,ch,dj,j)] /16 ;
                }
                MatTr[IND84(di,ch,dj,j)] =  (6*MatTr[IND84(di,ch,dj,j)]) / 16;
                if(di)MatTr[IND84(0,ch,dj,j)] =  (6*MatTr[IND84(0,ch,dj,j)]) /16 ;
                //                MatTr[ch*PB084_NBCASE+j] *= 6.0/16.0 ;
            }
        }
    }
    
    
    // CC correction case 02, 17, 33
    int CCcase[3] = { 2,17,33} ;
    for(n=0;n<3;n++) {
        int cc = CCcase[n] ;
        for(dj=0;dj<3;dj++) {
            int di = djtodi(dj) ;
            int cc_next[2] = { 0, JAIL_CASE} ;
            
            for(j=0;j<PB084_NBCASE;j++) {
                int next ;
                for(next=0;next<2;next++) {
                    MatTr[IND84(di,cc_next[next],dj,j)] += MatTr[IND84(di,cc,dj,j)] / 16 ;
                    if(di)MatTr[IND84(0,cc_next[next],dj,j)] += MatTr[IND84(0,cc,dj,j)] / 16;
                }
                
                MatTr[IND84(di,cc,dj,j)] = (14*MatTr[IND84(di,cc,dj,j)])/16 ;
                if(di)MatTr[IND84(0,cc,dj,j)] = (14*MatTr[IND84(0,cc,dj,j)])/16 ;
            }
        }
    }
    
     for(j=0;j<PB084_NBCASE_3;j++) {
         int sum = 0;
         for(i=0;i<PB084_NBCASE_3;i++) {
             sum += MatTr[j+i*PB084_NBCASE_3] ;
         }
         if(sum != PB084b_MULT0) printf("Sum[%d]=%d(exp%d)",j,sum,PB084b_MULT0);
     
     }
    
#define PB084_PREC  16384
    
    for(i=0;i<PB084_NBCASE_3;i++) CaseProb[i] = PB084_PREC ;
//    CaseProb[0] = PB084b_MULT0*16 ;
    for(n=0;n<PB084_NBITER;n++) {
        memcpy(oldProb,CaseProb,sizeof(CaseProb)) ;
        for(i=0;i<PB084_NBCASE_3;i++) {
            int sum = 0 ;
            for(j=0;j<PB084_NBCASE_3;j++) {
                sum += MatTr[PB084_NBCASE_3*i +j] * oldProb[j] ;
            }
            CaseProb[i] = (sum+PB084b_MULT0/2)/PB084b_MULT0 ;
        }
    }
    int best=0,best1=0,best2=0 ;
    int prBest=0, prBest1=0, prBest2=0;
    for(i=0;i<PB084_NBCASE;i++) {
        CaseProb[i]+= CaseProb[i+PB084_NBCASE]+CaseProb[i+2*PB084_NBCASE] ;
        if(CaseProb[i] > prBest) {
            prBest2= prBest1 ; prBest1 = prBest ; prBest = CaseProb[i] ;
            best2=best1 ; best1= best ; best = i ;
        } else if( CaseProb[i] > prBest1) {
            prBest2= prBest1 ; prBest1 =  CaseProb[i] ;
            best2=best1 ; best1= i ;
        } else if( CaseProb[i] > prBest2) {
            prBest2=  CaseProb[i] ;
            best2 = i ;
        }
    }
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s P[%02d]=%.8f P[%02d]=%.8f P[%02d]=%.8f\n"
                               ,pbR->ident,best,prBest/(float)(PB084_NBCASE_3*PB084_PREC),best1,prBest1/(float)(PB084_NBCASE_3*PB084_PREC),best2,prBest2/(float)(PB084_NBCASE_3*PB084_PREC));
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%02d%02d%02d",best,best1,best2);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}



// number of rectangles is NB(n,p) = n(n+1)/2 x p(p+1)/2
// search n,p nearest 2000000
// Loop on Tn and Tp so Tn*Tp <= 2000000 < Tn(Tp+1)
// use recursion to compute Tn*tp - 2000000

#define PB085_NB    2000000
int PB085(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int n,p,Tn,Tp ;
    int nBest = 0 ;
    int pBest = 0 ;
    int deltaNBBest =  PB085_NB ;
    int D,D1 ;
    for(n=1,Tn=1,p=(int)Sqrt64(2*PB085_NB),Tp=(p*(p+1))/2,D1=0, D = Tp-PB085_NB; n<=p;n++,Tn += n , D += n * Tp ) {
        while(D > 0 ) {
            D1 = D ;
            D -= Tn * p ;
            Tp -= p-- ;
        }
        if(-D < deltaNBBest ) {
            deltaNBBest = - D ;
            nBest = n ;
            pBest = p ;
        }
        if (D1 < deltaNBBest ) {
            deltaNBBest = D1 ;
            nBest = n ;
            pBest = p+1 ;
        }
        
    }
    if(pbR->isVerbose) fprintf(stdout,"\t PB%s For %d,%d np=%d Nb rectangles=%d\n",pbR->ident,nBest,pBest,nBest*pBest,(nBest*(nBest+1)*pBest*(pBest+1))/4) ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nBest*pBest);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}


// en mettant le parallepipede a <= b <= c a plat deplie on voir rapidement
// que la plus coure distance au carre est (a+b)**2 + c**2
// donc a+b,c sont les petites valeurs d'un triangle pythagoricien
// donc on cherche a decomposer un triangle Pyth primaire (m,n) m>n premiers m**m - n**2 , 2mn
// on compte les cas : a+b = m**2 - n**2 , c =2mn et a<=b<=c et c<=M
// et les cas a+b =2mn c = m**2-n**2 et c <=M
// puis on mutliplie par k tant que l'on ne depasse pas M

#define PB086_MAX    5000
#define PB086_NBASK 1000000

int PB086(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int i,n,m,c,k ;
    int M = PB086_MAX ;
    int minM = 1 ;
    uint32_t *histoM = calloc(PB086_MAX+1,sizeof(histoM[0])) ;
    for(m=2;;m++) {
        // c >= 1/3 (a+b+c) as c>=b>=a
        // (a+b=c) = m**2 - n**2 + 2*m*n has a minima for n in [0,m] when n=0 => m**2
        // so minc <= m*m / 3
        int minC = (m*m)/3  ;
        for(n=(m&1) + 1;n<m;n += 2) {
            int apb,tmp,permut ;
            if(PGCD(m,n) != 1)  continue ;
            // first case a+b = m**2 - n**2 , c = 2*m*n
            // second case a+b = 2*m*n  c = m**2 - n**2
            c = 2 * m * n ;
            apb = m*m - n*n ;
            for(permut=0;permut<2;permut++) { // loop for permutation c <=> apb
                if (c<=M && apb <= 2*c) {
                    k = M / c ;
                    if( apb <= c ) {
                        for(i=1;i<=k;i++) histoM[i*c] += (i*apb)/2 ;
                    } else {
                        for(i=1;i<=k;i++) histoM[i*c] += i*c-(i*apb-1)/2 ;
                    }
                }
                tmp = apb ;
                apb = c ;
                c = tmp ;
            }
        }
        for(;minM+1<minC;) { // we know that histo under minc will remain unchanged
            // so we cand cumulate histogram
            minM++ ;
            histoM[minM] += histoM[minM-1] ;
            if(histoM[minM]>PB086_NBASK) {
                break ;
            }
        }
        if(histoM[minM]>PB086_NBASK) break ;
    }
    free(histoM) ;
    if(pbR->isVerbose)printf("\t PB%s Nb[%d]=%d => Nb[%d]=%d\n",pbR->ident,minM-1,histoM[minM-1],minM,histoM[minM]) ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",minM);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB087_MAXN  50000000
int PB087(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    CTX_PRIMETABLE *ctxP ;
    uint32_t n_sqr2 = (uint32_t)Sqrt64(50000000) ;
    uint32_t n_sqr4 = (uint32_t)Sqrt64(n_sqr2+1) ;
    uint32_t n_sqr3 = 800 ;
    while(n_sqr3*n_sqr3*n_sqr3 > PB087_MAXN) n_sqr3-- ;
    uint32_t  * pow2 , * pow3 ,*pow4 ;
    pow2 = malloc(n_sqr2*sizeof(pow2[0])) ;
    pow3 = malloc(n_sqr3*sizeof(pow3[0])) ;
    pow4 = malloc(n_sqr4*sizeof(pow4[0])) ;
    if((ctxP = Gen_tablePrime(n_sqr2)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    
    int i,i2,i3,i4 ;
    uint32_t nbPrime = GetNbPrime(ctxP) ;
    const T_prime * tbPrime = GetTbPrime(ctxP) ;
    
    for(i=i2=i3=i4=0;i<nbPrime;i++) {
        uint32_t p,p2 ;
        p = (uint32_t)tbPrime[i] ;
        pow2[i2++] = p2 = p*p ;
        if(p< n_sqr3) {
            pow3[i3++] = p*p2 ;
            if(p < n_sqr4) {
                pow4[i4++] = p2*p2 ;
            }
        }
    }
    n_sqr2 = i2 ;
    n_sqr3 = i3 ;
    n_sqr4 = i4 ;
    int nbLoop = 4;
    int nb = 0 ;
    int sizeLoop = (((PB087_MAXN/16+1) / nbLoop) +1) * 16 ;
    uint16_t * isDec = calloc(sizeLoop/16,sizeof(isDec[0])) ;
    int nl ;
    for(nl=0;nl<nbLoop;nl++) {
        int infLoop = sizeLoop * nl ;
        for(i4=0;i4<n_sqr4;i4++) {
            int32_t S4 = pow4[i4] - infLoop ;
            if(S4 >= sizeLoop) break ;
            for(i3=0;i3<n_sqr3;i3++) {
                int32_t S3 = S4 + pow3[i3] ;
                if(S3 >= sizeLoop) break ;
                for(i2=0;i2<n_sqr2;i2++) {
                    int32_t S2 = S3 + pow2[i2] ;
                    if(S2 < 0) continue ;
                    if( S2 >= sizeLoop) break ;
                    if((isDec[S2>>4] & ( 1 << ( S2 & 0xf ))) ==0) {
                        nb++; isDec[S2>>4] |= 1 << ( S2 & 0xf ) ;
                    }
                }
            }
        }
        if(nl < nbLoop-1) memset(isDec,0, (sizeLoop/16) * sizeof(isDec[0]));
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nb) ;
    free(isDec); free(pow2); free(pow3); free(pow4) ;
    Free_tablePrime(ctxP) ;
    return 1 ;
}


#define PB088_MAXK  12000

static int CmpKminN(const void *el1,const void *el2) {
    return ((uint32_t *)el1)[0] - ((uint32_t *)el2)[0] ;
}
int PB088(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    uint32_t *kminN = calloc(PB088_MAXK+1,sizeof(kminN[0])) ;
    int nv ;
    uint32_t V[32] /* values */ ,P[32] /* product */, S[32] /* sum */ ;
    int Pmax = 2 * PB088_MAXK ;
    int isNotAll2 = 1;
    for(nv=2;isNotAll2;nv++) {
        int iP , iPbad ;
        uint32_t k = 1 ;
        V[nv] = 1;
        P[nv] = 1 ;
        S[nv] = - nv ;
        for(iPbad = 0, iP=nv-1, V[iP]=1 ; iP<nv ; iP++) {
            uint32_t newV = V[iP] + 1;
            for(; iP>=0 ; iP--) {
                V[iP] = newV ;
                P[iP] = P[iP+1]*newV ;
                S[iP] = S[iP+1]+newV ;
                k = P[iP] - S[iP]  ;
                if(P[iP] > Pmax ||  k >  PB088_MAXK ) break ;
            }
            if(iP >= 0) {
                if(iP == 0 && V[iP] == 2) {
                    isNotAll2 = 0 ; break ;// END, only V==2 and too big
                }
                iP = iPbad++ ;
                continue ;
            } else {
                iPbad = 0 ; // find a new value
                if(kminN[k]) {
                    if(P[0] < kminN[k]) {
                        kminN[k] = P[0];
                    }
                } else {
                    kminN[k] = P[0] ;
                }
            }
            
        }
    }
    {   // compute the Sum
        int k ; uint64_t S=0;
        qsort(kminN+1,PB088_MAXK,sizeof(kminN[0]),CmpKminN) ;
        uint32_t ant = kminN[2] ;
        S += kminN[2] ;
        for(k=3;k<=PB088_MAXK;k++) {
            if(kminN[k] == ant) continue ;
            S += kminN[k] ;
            ant = kminN[k] ;
        }
        snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",S) ;
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#include "p089_data.h"

static int Value89(char c) {
    switch(c) {
        case 'I' : return 1 ;
        case 'V' : return 5 ;
        case 'X' : return 10 ;
        case 'L' : return 50 ;
        case 'C' : return 100 ;
        case 'D' : return 500 ;
        case 'M' : return 1000 ;
        default : return 0 ;
    }
}
int PB089(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    const char ** Roman = P089_GetData() ;
    int nb = 0 ;
    int i,n ;
    char * Milliers[] = { "" , "M", "MM" , "MMM", "MMMM" } ;
    char * Centaines[] = { "" , "C" , "CC", "CCC" , "CD" , "D" , "DC" , "DCC" , "DCCC" , "DM" } ;
    char * Dizaines[] = { "" , "X" ,"XX" , "XXX" , "XL", "L" , "LX", "LXX", "LXXX" , "XC" } ;
    char * Units[] = { "", "I", "II", "III" , "IV" , "V" , "VI" , "VII", "VIII", "IX" } ;
    for(n=0;n<p089_size;n++) {
        const char *inR = Roman[n] ;
         int len = strlen(inR) ;
        int totValue = 0 ;
        int antValue = 0 ;
        for(i=0;i<len;i++) {
            int value = Value89(inR[i]);
            if(antValue < value) {
                totValue += value - 2 * antValue ;
            } else {
                totValue += value ;
            }
            antValue = value ;
        }
/*        strcpy(reduct,Milliers[(totValue/1000) % 10]) ;
        strcat(reduct,Centaines[(totValue/100) % 10]) ;
        strcat(reduct,Dizaines[(totValue/10) % 10]) ;
        strcat(reduct,Units[totValue % 10]);
        printf("%s=%d=%s ",inR, totValue,reduct) ;
 */
        nb += len - strlen(Milliers[(totValue/1000) % 10])
            - strlen(Centaines[(totValue/100) % 10])
            - strlen(Dizaines[(totValue/10) % 10])
            - strlen(Units[totValue % 10]) ;
    }
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nb);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}


#define isPresent(v0,v1)    ( (( D1 & (1<<v0)) && (D2 & (1<<v1)) ) || (( D2 & (1<<v0)) && (D1 & (1<<v1)) ) )
#define isPresent6(v0)     ( (( D1 & (1<<v0)) && ( D2 & ( (1<<6) + (1<<9) ) ) ) || (( D2 & (1<<v0)) && (D1 & ( (1<<6) + (1<<9) ) ) ) )
int PB090(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int nb = 0 ;
    // as number of 6 bits between 10
    uint16_t diffPosition[210] ;
    // by complementary place 4 zero's in ten position
    {
        int i = 0 ;
        int i1,i2,i3,i4 ;
        for(i1=9;i1>2;i1--) {
            for(i2=i1-1;i2>1;i2--) {
                for(i3=i2-1;i3>0;i3--) {
                    for(i4=i3-1;i4>=0;i4--) {
                        uint16_t v = (1<<i1) + (1<<i2) + (1<<i3) + (1<<i4)  ;
                        diffPosition[i++] = 1023 ^ v ;
                    }
                }
            }
        }
    }
    int i1,i2 ;
    for(i1=0;i1<210;i1++) {
        uint16_t D1 = diffPosition[i1] ;
        for(i2=i1;i2<210;i2++) {
            uint16_t D2 = diffPosition[i2] ;
            if(!isPresent(0,1)) continue ;
            if(!isPresent(0,4)) continue ;
            if(!isPresent6(0)) continue ; // 09
            if(!isPresent6(1)) continue ; // 16
            if(!isPresent(2,5)) continue ;
            if(!isPresent6(3)) continue ; // 36
            if(!isPresent6(4)) continue ; // 64
            if(!isPresent(8,1)) continue ;
            nb++ ;
        }
    }
    
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nb);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}
#define PB091_MAXY  50

int PB091(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int nbO=0, nbp=0,nbq = 0 ;
    int x1,y1,x2,y2 ;
    for(x1=0;x1<=PB091_MAXY;x1++) {
        for(y1=0;y1<=PB091_MAXY;y1++){
            if(x1+y1 == 0) continue ;
            for(y2=0;y2<y1;y2++) {
                for(x2=0;x2<=PB091_MAXY;x2++) {
                    if(x2+y2 == 0) continue ;
                    if(x1*(x2-x1)+y1*(y2-y1) == 0) { nbp++ ;}
                    else if(x1*x2+y1*y2 == 0) {  nbO++; }
                    else if(x2*(x1-x2)+y2*(y1-y2)== 0 ) { nbq++; }
                    
                }
            }
            for(y2=y1,x2=0;x2<x1;x2++) {
                if(x2+y2==0) continue ;
                if(x1*(x2-x1)+y1*(y2-y1) == 0) {  nbp++ ;}
                else if(x1*x2+y1*y2 == 0) {  nbO++; }
                else if(x2*(x1-x2)+y2*(y1-y2)== 0 ) {  nbq++; }
                
            }
            
        }
    }
    if(pbR->isVerbose)fprintf(stdout,"\t PB%s Nb=%d (O:%d,P:%d,Q:%d)\n",pbR->ident,nbO+nbp+nbq, nbO,nbp,nbq) ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbO+nbp+nbq);
    
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB092_NBDIG     19
#define PB092_NBASK     7
#define PB092_T89       2

int PB092(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    uint8_t dig[PB092_NBDIG] ;
    int16_t sum[PB092_NBDIG] ;
    int maxValue = 81*PB092_NBDIG ;
    int nbT[PB092_T89+1] ;
    nbT[1] = 0 ;
    nbT[PB092_T89] = 0 ;
    int i,n,lgBack  ;
    uint8_t *terminal = calloc(maxValue+1,sizeof(terminal[0]));
    int16_t *backTrace = malloc((maxValue+1)*sizeof(backTrace[0])) ;
    terminal[1] = 1 ; nbT[1]++ ;
    terminal[89] = PB092_T89 ; nbT[PB092_T89]++ ;
    for(i=1;i<=maxValue;i++) {
        n = i; lgBack = 0 ;
        while(n>maxValue || terminal[n]==0 ) {
            if(n<=maxValue) backTrace[lgBack++] = n ;
            int nxt = 0 ;
            while(n>0) {
                nxt += (n % 10) * (n % 10) ;
                n /= 10 ;
            }
            n = nxt ;
        }
        int k ;
        for(k=0;k<lgBack;k++) {
            terminal[backTrace[k]] = terminal[n] ;
        }
        nbT[terminal[n]] += lgBack ;
    }
    // on parcours tous les nombres au dela de max value
    memset(sum,0,PB092_NBDIG*sizeof(sum[0])) ;
    memset(dig,0,PB092_NBDIG*sizeof(dig[0])) ;
    for(n=maxValue,i=PB092_NBDIG-1;n!=0;i--) {
        dig[i] = n % 10 ;
        sum[i] = dig[i]*dig[i] ;
        n /= 10 ;
    }
    while(++i<=PB092_NBDIG-1) { sum[i] += sum[i-1] ; }
    int is = PB092_NBDIG-1 ;
    while(is >=0) {
        if(dig[is] < 9) {
            sum[is] += 2*dig[is]+1 ;
            dig[is]++ ;
            while(is <PB092_NBDIG-1){
                is++ ;
                sum[is] = sum[is-1] ;
                dig[is] = 0 ;
            }
            nbT[terminal[sum[is]]]++ ;
        } else {
            is-- ;
        }
    }
    
    
    if(pbR->isVerbose)fprintf(stdout,"\t PB%s Term=1[%d] Term=89[%d]\n",pbR->ident,nbT[1],nbT[PB092_T89]) ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbT[PB092_T89]);
    
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

int PB092a(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int maxValue = 81*PB092_NBDIG ;
    uint64_t nbT[PB092_T89+1] ;
    nbT[1] = 0 ;
    nbT[PB092_T89] = 0 ;
    int i,k, n,lgBack  ;
//    uint8_t *terminal = calloc(maxValue+1,sizeof(terminal[0]));
    uint8_t terminal[81*PB092_NBDIG+1] ;
    memset(terminal,0,sizeof(terminal)) ;
    int16_t *backTrace = malloc((maxValue+1)*sizeof(backTrace[0])) ;
    terminal[1] = 1 ;
    terminal[89] = PB092_T89 ;
    for(i=1;i<=maxValue;i++) { // compute terminal pour possible sum
        n = i; lgBack = 0 ; // lgback longueur de la chaine
        while(n>maxValue || terminal[n]==0 ) {
            if(n<=maxValue) backTrace[lgBack++] = n ; // valeur en dessous
            int nxt ;
            for(nxt = 0 ; n > 0; n /= 10 ) {
                nxt += (n % 10) * (n % 10) ;
            }
            n = nxt ;
        }
        for(k=0;k<lgBack;k++) {  terminal[backTrace[k]] = terminal[n] ;  }
    }
    {
        uint64_t * histoSum[PB092_NBDIG] ;
        int i,nb,is ;
        for(nb=0;nb<PB092_NBDIG;nb++) histoSum[nb] = calloc((81+1)*(nb+1),sizeof(histoSum[nb][0])) ;
        for(i=0;i<10;i++) histoSum[0][i*i]++  ; // first digit
        for(nb=1;nb<PB092_NBDIG;nb++) {
            uint64_t sum =0;
            for(is=0;is<=81*nb;is++) { // add a digit
                for(i=0;i<10;i++) {
                    if(histoSum[nb-1][is]) histoSum[nb][is+i*i] += histoSum[nb-1][is] ;
                    sum += histoSum[nb-1][is] ;
                }
            }
            nbT[0] = nbT[1] = nbT[PB092_T89] = 0 ;
            for(is=0;is<=81*(nb+1);is++) {
                nbT[terminal[is]] += histoSum[nb][is] ;
            }
            if(pbR->isVerbose)fprintf(stdout,"\t PB%s %cTerm=89[%lld]\n",pbR->ident,(nb==PB092_NBASK-1) ? '*':' ',nbT[PB092_T89]) ;
            if(nb==PB092_NBASK-1 ) snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",nbT[PB092_T89]); ;
        }
        for(i=0;i<PB092_NBDIG;i++) free(histoSum[i]) ;
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}



typedef struct Fract_093 {
    int32_t N ;
    int32_t D ;
} Fract_093 ;
Fract_093 Oper(int nop, Fract_093 d0, Fract_093 d1) {
    Fract_093 fr ;
        switch(nop){
            case 0: fr.D=d0.D*d1.D; fr.N=d0.N*d1.D+d1.N*d0.D;  break ; // +
            case 1: fr.D=d0.D*d1.D ; fr.N=d0.N*d1.N ; break ; // *
            case 2: fr.D=d0.D*d1.D; fr.N=d0.N*d1.D-d1.N*d0.D; if(fr.N<0) fr.N = -fr.N ; break ; // -
            case 3: fr.D=d0.D*d1.N ; fr.N=d0.N*d1.D ; break ; // d0/d1
            case 4: fr.D=d1.D*d0.N ; fr.N=d1.N*d0.D ; break ; // d1/d0
        }
       if(fr.D > 1) {
            int32_t gd = PGCD(fr.N,fr.D) ;
            if(gd > 1) { fr.N /= gd ; fr.D /= gd ; }
        }
    return fr ;
}


Fract_093 FractDig(uint8_t dig) {
    Fract_093 fr ;
    fr.D = 1 ; fr.N = dig ;
    return fr ;
}

#define PB095_NBD       4
#define PB095_NBO       5
#define PB095_EXPMAX   100

// EXPMAX 1000 pour 5 digits ou  6 digits

void AddVal(uint8_t *isValFind, Fract_093 res) {
    if(res.D != 1 || res.N == 0 || res.N > PB095_EXPMAX ) return ;
    isValFind[res.N-1] = 1 ;
}
typedef struct niv_093 {
    Fract_093 vals[PB095_NBD] ;
    int16_t op1 ;
    int16_t op2 ;
    int16_t oper ;
} niv_093 ;

// version avec parcours recursif
int PB093a(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    uint8_t isValFind[PB095_EXPMAX] ;
    int maxCons =0 ;
    char maxABCD[PB095_NBD+1] ;
    maxABCD[PB095_NBD] = 0 ;
    niv_093 niv[PB095_NBD] ;
    int i , indDig ;
    for(i=0;i<PB095_NBD;i++) {
        niv[0].vals[i] = FractDig(i+1) ;
        niv[i].op1 = 0 ; niv[i].op2 = 1 ;
        niv[i].oper = 0 ;
    }
    indDig = PB095_NBD - 1 ;
    do {
        if(indDig == PB095_NBD - 1) {
            int curNiv = 0 ;
            memset(isValFind,0,PB095_EXPMAX) ;
            do {
              int lgNiv = PB095_NBD - curNiv ;
              if(niv[curNiv].oper >=PB095_NBO){
                  niv[curNiv].oper = 0 ;
                    if(niv[curNiv].op2>= lgNiv-1 ) {
                        if(niv[curNiv].op1>= lgNiv-2 ) {
                            niv[curNiv].op1 = 0 ; niv[curNiv].op2 = 1 ;
                            curNiv-- ;
                            continue ;
                        } else {
                            niv[curNiv].op1++ ;
                            niv[curNiv].op2 = niv[curNiv].op1 + 1 ;
                        }
                    } else {
                        niv[curNiv].op2++ ;
                    }
                }
                Fract_093 newVal = Oper(niv[curNiv].oper,niv[curNiv].vals[niv[curNiv].op1],niv[curNiv].vals[niv[curNiv].op2]);
                niv[curNiv].oper++ ;
                if(newVal.D != 0) { // no division par zero
                    if(lgNiv == 2) {
                        AddVal(isValFind,newVal);
                        continue ;
                    } else {
                        // on avance d'un cran
                        int i,j;
                        for(i=0;i<niv[curNiv].op1;i++) {
                            niv[curNiv+1].vals[i] = niv[curNiv].vals[i] ;
                        }
                        niv[curNiv+1].vals[i++] = newVal ;
                        for(j=niv[curNiv].op1+1;j<lgNiv;j++) {
                            if(j != niv[curNiv].op2 ) {
                                niv[curNiv+1].vals[i++] = niv[curNiv].vals[j] ;
                            }
                        }
                        curNiv++ ;
                    }
                }
            } while(curNiv >= 0) ;
            {
                int i ;
                for(i=0;i<PB095_EXPMAX && isValFind[i] ; i++) ;
                if(i>=maxCons){
                    maxCons = i ;
                    int k ;
                    for(k=0;k<PB095_NBD;k++) maxABCD[k] = niv[0].vals[k].N + '0' ;
                }
            }
        }
        if(niv[0].vals[indDig].N < 10 - PB095_NBD + indDig ){
            niv[0].vals[indDig].N++ ;
            for(;indDig<PB095_NBD-1;){
                indDig++ ;
                niv[0].vals[indDig].N = niv[0].vals[indDig-1].N+1 ;
            }
        } else {
            indDig-- ;
        }
    } while(indDig >=0) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    if(maxCons < PB095_EXPMAX) {
        snprintf(pbR->strRes, sizeof(pbR->strRes),"%s",maxABCD);
        if(pbR->isVerbose)fprintf(stdout,"\t PB%s Nb=%d (%s)\n",pbR->ident,maxCons,maxABCD) ;
        return 1 ;
    } else {
        if(pbR->isVerbose)fprintf(stdout,"\t PB%s ERROR MAX_PRESUMED=%d reached for (%s)\n",pbR->ident,maxCons,maxABCD) ;
        return 0 ;
        
    }
}

int PB093(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int a,b,c,d ;
    // 2 parenthesage possible ((a@b)@c)@d) et (a@b)@(c@d)
    // ou @ designe un operateur commutatif (+,x) ou un operateur non commutatif a un seul sens.
    // Pour le premeir choix a,b puis c, et 3 operations. Pour le second choix a,b et " operateurs
    uint8_t isValFind[PB095_EXPMAX] ;
    //   int tbValues[3000] ;
    int maxCons =0 ;
    int maxABCD = 0 ;
    uint8_t dig[4] ;
    for(a=1;a<7;a++) {
        dig[0] = a ;
        for(b=a+1;b<8;b++) {
            dig[1] = b ;
            for(c=b+1;c<9;c++) {
                dig[2] = c ;
                for(d=c+1;d<10;d++) {
                    dig[3]= d ;
                    memset(isValFind,0,PB095_EXPMAX) ;
                    do {
                        int op1 ;
                        if(dig[0]>dig[1]) continue ;
                        for(op1=0;op1<PB095_NBO;op1++) {
                            Fract_093 v1 = Oper(op1 ,FractDig(dig[0]),FractDig(dig[1]) );
                            if(v1.D==0) continue ;
                            int op2 ;
                            for(op2=0;op2<PB095_NBO;op2++) {
                                Fract_093 v2 = Oper(op2 ,FractDig(dig[2]),FractDig(dig[3]) );
                                if(v2.D != 0) {
                                    int op3 ;
                                    for(op3=0;op3<PB095_NBO;op3++) {
                                        Fract_093 v3 =  Oper(op3 ,v1,v2);
                                        AddVal(isValFind,v3);
                                    }
                                }
                                v2 = Oper(op2,v1,FractDig(dig[2])) ;
                                if(v2.D != 0) {
                                    int op3 ;
                                    for(op3=0;op3<PB095_NBO;op3++) {
                                        Fract_093 v3 =  Oper(op3 ,v2,FractDig(dig[3]));
                                        AddVal(isValFind,v3);
                                    }
                                }
                                v2 = Oper(op2,v1,FractDig(dig[3])) ;
                                if(v2.D != 0) {
                                    int op3 ;
                                    for(op3=0;op3<PB095_NBO;op3++) {
                                        Fract_093 v3 =  Oper(op3 ,v2,FractDig(dig[2]));
                                        AddVal(isValFind,v3);
                                    }
                                }
                            }
                        }
                    } while(NextPermutRg(dig,4,1)>=0) ;
                    HeapSortUint8(dig,4) ;
                    int i ;
                    for(i=0;i<PB095_EXPMAX && isValFind[i] ; i++) ;
                    if(i>=maxCons){
                        maxCons = i ;
                        maxABCD= a*1000+b*100+c*10+d ;
                    }
                }
            }
        }
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    if(maxCons < PB095_EXPMAX) {
        snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",maxABCD);
        if(pbR->isVerbose)fprintf(stdout,"\t PB%s Nb=%d (%d)\n",pbR->ident,maxCons,maxABCD) ;
        return 1 ;
    } else {
        if(pbR->isVerbose)fprintf(stdout,"\t PB%s ERROR MAX_PRESUMED=%d reached for (%d)\n",pbR->ident,maxCons,maxABCD) ;
        return 0 ;
        
    }
}

// Isocele triangle ,
// For parity reason in pythagore, the base is the even length
//  B) Length 2N+1,2N+1,2N
//      (Py) (2N+1)*(2N+1) = N*N + h*h (primary pytha triangle because N and 2N-1 prime)
//     (2N+1) = m*m+n*n et N=2*m*n (other case N=m*m-n*n impossible due to modulo 4)
//     => m*m+n*n-4m*n = 1 <=> (m-2n)*(m-2n)=3*n*n + 1 ;
//     so if d=m-2n n/d near 1/sqrt(3). Continuous fraction 1/sqrt(3) d*d=n*n+1 ;
//     So m = d+2n ; N=2*m*n ; 2N+1 = m*m+n*n ; h = m*m-n*n ; P=6*N+2
//     We check (Py)
//  A) length 2N-1,2N-1,2N
//     (Py) (2N-1)*(2N-1) = N*N + h*h (primary pytha triangle because N and 2N+1 prime)
//     (2N-1) = m*m+n*n et N=m*m-n*n (other case N=2*m*n impossible  due to modulo 3)
//     => m*m=3*n*n+1 so n/m near 1/sqrt(3). Continuous fraction 1/sqrt(3) d*d=n*n+1 ;
//     Avec m=d N=m*m-n*n 2N-1 = m*m+n*n ; h = 2*m*n P=6*N-2
//     Check (Py)

#define PB094_MAX   1000000000
int PB094(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    // continuous fraction for 1/sqrt(3) => 0 1 1 2 1 2 1 2 ...
    FractCont64 Fsqrt3 = {0,1,1,0} ;
    NextFract(&Fsqrt3,0);
    NextFract(&Fsqrt3,1);
    uint64_t sumP = 0;
    while(1) {
        NextFract(&Fsqrt3,1);
        int64_t d = Fsqrt3.D1 ;
        int64_t n = Fsqrt3.N1 ;
        assert(d*d - 3*n*n == 1 ) ;
        { // cas A (2N-1)
            int64_t m = d ;
            int64_t N = m*m-n*n ;
            int64_t h = 2*d*n ;
            int64_t P = 6*N-2;
            if(P >= PB094_MAX) break ;
            assert((2*N-1)*(2*N-1) - h*h - N*N == 0 ); // check Pythagore
            if(pbR->isVerbose)fprintf(stdout,"\t PB%s T(2x%lld,%lld) %lld^2 = %lld^2 + %lld^2\n"
                                      ,pbR->ident,2*N-1,2*N,2*N-1,N,h) ;
            sumP += P ;
        }
        { // cas B (2N+1)
            int64_t m = 2*n + d ;
            int64_t N = 2*m*n ;
            int64_t h= m*m - n*n ;
            int64_t P = 6*N+2;
            assert((2*N+1)*(2*N+1) - h*h - N*N == 0) ; // check Pythagore
            if(P<PB094_MAX) sumP += P ;
            if(pbR->isVerbose)fprintf(stdout,"\t PB%s T(2x%lld,%lld) %lld^2= %lld^2 + %lld^2\n"
                                      ,pbR->ident,2*N+1,2*N,2*N+1,N,h) ;
            
        }
        NextFract(&Fsqrt3,2);
    }
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",sumP);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB095_MAX   1000000
#define PB095_MAXLG 200
int PB095(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    CTX_PRIMETABLE *ctxP ;
    if((ctxP = Gen_tablePrime(PB095_MAX)) == NULL) {
        fprintf(stdout,"\t PB%s Fail to alloc prime table\n",pbR->ident);
        return 0 ;
    }
    int32_t *SumDiv = malloc(PB095_MAX*sizeof(SumDiv[0]));
    int32_t Back[PB095_MAXLG+1] ;
    int32_t BestChain[PB095_MAXLG+1] ;

    int i ;
    int nbP = GetNbPrime(ctxP);
    const T_prime *tbPrime = GetTbPrime(ctxP) ;
    int sqrtMax = Sqrt32(PB095_MAX) ;
    int np ;

    for(i=0;i<PB095_MAX;i++) {
        SumDiv[i]= 1 ;
    }
    // compute Sum of divisor with prime P < sqrt(PB095_MAXLG)
    for(np=0;np<nbP; np++) {
        uint32_t P = (uint32_t) tbPrime[np] ;
        if(P> sqrtMax) break ;
        uint32_t m_powP , m  ,powP , mulP , mGtP;
        for(mGtP=1,powP=P,mulP= (P+1);mGtP;powP *= P , mulP = mulP*P+1) {
            for(m=1,mGtP=0,m_powP = powP ; m_powP < PB095_MAX ; m++, m_powP+=powP ) {
                if(m==P) { m= 0; mGtP = 1 ; continue ; }
                SumDiv[m_powP] *= mulP;
            }
        }
    }
    
    // only m x P**1 can occurs
    for(;np < nbP ; np++) {
        uint32_t P = (uint32_t)tbPrime[np] ;
        uint32_t m_P ;
        for(m_P = P ; m_P < PB095_MAX ; m_P += P ) {
            SumDiv[m_P] *= P+1 ;
        }
    }
    // substract N, and remove too big by setting to zero
    for(i=0;i<PB095_MAX;i++) {
        if(SumDiv[i]-i<PB095_MAX ) {
            SumDiv[i] = SumDiv[i] - i ;
        } else {
            SumDiv[i] = 0 ;
        }
    }
    int lgMax = 0;
    int jmin = 0 ;
    // search loop
    for(i=0;i<PB095_MAX;i++) {
        if(SumDiv[i]==0) continue ;
        int j , nj , lg ;
        for(j=i,lg=0 ; SumDiv[j] > 0 ; j=nj) {
            Back[lg++] = j ;
            nj = SumDiv[j] ;
            SumDiv[j] = - lg ; // mark the current path by negative number
            if(lg >= PB095_MAXLG) {
                fprintf(stdout,"\t PB%s FATAL ERROR MAXLG=%d Reached\n",pbR->ident,PB095_MAXLG) ;
                return 0 ;
            }
        }
        if(SumDiv[j] < 0 ) { // loop detected
            int lgChain = lg + SumDiv[j] + 1 ; // remove the initial branch
            if(lgChain > lgMax) { // better loop ?
                lgMax = lgChain ;
                int k , kmin = 0  ;
                jmin = PB095_MAX ;
                for(k=lg - lgChain ;k<lg;k++) { // search min value
                    if(Back[k] < jmin) {
                        jmin = Back[k] ;
                        kmin = k ;
                    }
                }
                for(k=0;k<lgChain;k++) { // save loop for final result
                    BestChain[k] = (k+kmin < lg) ? Back[k+kmin] : Back[k+kmin - lgChain];
                }
                BestChain[lgChain] = jmin ;
            }

        }
        while(--lg >= 0) { // erase the current path
            SumDiv[Back[lg]] = 0 ;
        }
    }
    Free_tablePrime(ctxP);
    free(SumDiv) ;
    if(pbR->isVerbose) {
        int k ;
        fprintf(stdout,"\t PB%s Chain[%d]=%d ",pbR->ident,jmin,lgMax) ;
        for(k=0;k<=lgMax;k++) printf("%d%c",BestChain[k],(k==lgMax) ? '\n' : '>');
    }
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",jmin) ;
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}


#define PB097_M0D   10000000000LL
#define PB097_POW2  7830457

uint64_t mult097(uint64_t m1, uint64_t m2) {
    uint64_t hm1 = m1 >> 30 ;
    uint64_t lm1 = m1 & 0x3fffffff ;
    uint64_t hm2 = m2 >> 30 ;
    uint64_t lm2 = m2 & 0x3fffffff ;
    uint64_t ll = (lm1 *lm2 ) % PB097_M0D ;
    uint64_t lh = (((lm1 * hm2 + hm1 * lm2 ) % PB097_M0D) << 30) % PB097_M0D  ;
    uint64_t hh = ((((hm1 * hm2) << 30 ) % PB097_M0D) << 30) % PB097_M0D  ;
    return (ll +lh + hh) % PB097_M0D ;
}

int PB097(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    int exp = 0 ;
    int exp2 = 1 ;
    uint64_t pow2 =  2;
    uint64_t pow2Exp = 1 ;
    do {
        if(exp2 & PB097_POW2) {
            exp += exp2 ;
            pow2Exp = mult097(pow2Exp,pow2);
        }
        exp2 <<= 1 ;
        pow2 = mult097(pow2,pow2) ;
    } while(exp < PB097_POW2) ;
    uint64_t result = mult097(28433,pow2Exp) +1 ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",result);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#include "p098_data.h"
#define P098_MAXL   20

int anaCmp(const void *c1, const void *c2) {
    return ((char *)c1)[0] - ((char *)c2)[0] ;
}


typedef struct anaIndex {
    int16_t index ;
    int16_t len ;
    char *  ptStr ;
} anaIndex ;

int indexCmp(const void *i1, const void *i2) { // tri decroissant longueur puis alpha
    anaIndex * ptI1 = ((anaIndex *)i1);
    anaIndex * ptI2 = ((anaIndex *)i2);
    int diffLen = ptI2->len - ptI1->len ; // ordre inverse
    if(diffLen) return diffLen ;
    else return strcmp(ptI1->ptStr,ptI2->ptStr) ;
}

int PB098(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    unsigned char str2TOstr1[P098_MAXL] ;
    char sortedAna[p098_size*P098_MAXL] ;

    const char **  Anagrams = P098_GetData() ;
    int n ;
//    int indexSorted[p098_size] ;
    anaIndex sortedI[p098_size] ;
    for(n=0;n<p098_size;n++) {
        strcpy(sortedAna+n*P098_MAXL,Anagrams[n]) ;
        int len = strlen(Anagrams[n]) ;
        qsort(sortedAna+n*P098_MAXL,strlen(Anagrams[n]),1,anaCmp) ;
        sortedI[n].index = n ;
        sortedI[n].len = len ;
        sortedI[n].ptStr = sortedAna+n*P098_MAXL ;
        
        
    }
    qsort(sortedI,p098_size,sizeof(sortedI[0]),indexCmp) ;
    int best = 0 ;
    for(n=0;n<p098_size-1;n++) {
        if(strcmp(sortedI[n].ptStr,sortedI[n+1].ptStr) == 0) {
            const char *str1 = Anagrams[sortedI[n].index] ;
            const char *str2 = Anagrams[sortedI[n+1].index] ;
            // on va calculer la permutation des lettres.
            int len = sortedI[n].len ;
            int i,j, maxq = 1 ;
            for(i=0;i<len;i++) maxq *= 10 ; // si majorant inferieur au best c'est fini
            if(maxq <= best) break ;
            int nbL = 0 ;
            for(i=0;i<len;i++) {
                for(j=0;j<i;j++) {
                    if(str1[j] == str1[i]) { // same letter ?
                        str2TOstr1[i] = str2TOstr1[j];
                        break ;
                    }
                }
                if(i==0 || j==i) {
                    for(j=0;;j++) { // new letter
                        if(str2[j] == str1[i]) break ;
                    }
                    str2TOstr1[i] = j ;
                    nbL++ ; // diff letter count
                }
            }
            int isq2, isq1, iq2, iq1 ;
            uint8_t digits[10] ;
            char digitsToLetter[10] ;
            uint8_t anaDigits[10] ;
            isq2 = Sqrt32(maxq / 10) ; // loop over square with same digit number
            for(;(iq2=isq2*isq2) < maxq;isq2++) {
                memset(digitsToLetter,0, sizeof(digitsToLetter));
                int iq = iq2 ;
                int nbD = 0 ;
                for(i=0;i<len;i++) {
                    int d = iq % 10 ;
                    if(digitsToLetter[d] == 0) {
                        digitsToLetter[d] = str2[len-i-1] ;
                        nbD++ ;
                    } else if(str2[len-i-1] != digitsToLetter[d] ) {
                        break ;
                    }
                    digits[len-i-1] = d ; iq /= 10 ;
                }
                if(i<len || nbL != nbD || digits[str2TOstr1[0]]==0) continue ;
                for(iq1=0,i=0;i<len;i++) {
                    anaDigits[i] = digits[str2TOstr1[i]];
                    iq1 = 10*iq1+anaDigits[i] ;
                }
                isq1 = Sqrt32(iq1);
                if(iq1 == isq1*isq1) {
                    if(iq2 > best) best = iq2 ;
                    if(iq1 > best) best = iq1 ;
                    if(pbR->isVerbose)printf("\t PB%s %s(%d)=%s(%d)\n",pbR->ident, str1,iq1,str2,iq2) ;
                  }
            
            }
            /*
            printf(" %s=%s,",str1,str2);
            for(i=0;i<len;i++) printf("%d",str2TOstr1[i]);
            */

        }
    }

    
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",best);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}
#include "p099_data.h"

int PB099(PB_RESULT *pbR) {
    pbR->nbClock = clock() ;
    const P099_couple *cpl = P099_GetData() ;
    int nbest=0, n ;
    for(n=1;n<p099_size;n++) {
        double diff = log(cpl[nbest].val / (double) cpl[n].val) - (cpl[n].exp - cpl[nbest].exp) / (double) cpl[nbest].exp * log ( cpl[n].val) ;
        if(diff < 0) {
            nbest = n ;
        }
        
    }
    if(pbR->isVerbose)printf("\t PB%s Best value line %d %d**%d\n",pbR->ident,nbest+1, cpl[nbest].val,cpl[nbest].exp) ;
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%d",nbest+1);
    pbR->nbClock = clock() - pbR->nbClock ;
    return 1 ;
}

#define PB100_DEBUG 1
#define PB100_MIN_N 1000000000000
#define INIT_FROM_MIN_N 1

/* RESULT
 delta=-567209652218	; p=707106781186	; n=1000000000000	; d=1279794257938 ;
 delta=0	; p=756872327472	; n=1070379110496	; d=2584123765442 ;
 PB100(72.711029s) blue=756872327473 total=1070379110497 1070379110496x1070379110497=2x756872327472x756872327473
 */
int PB100(PB_RESULT *pbR) {
    //
    // p/n doit approximer 1/sqrt(2).
    // 2 *(p+1)**2 > n**2 > 2* p**2
    // d = 2 *(p+1)**2 - n**2
    // delta = p*(p+1) - (n*(n+1))/2
    // si delta==0 gagne
    
    int64_t n,p,p2,d, delta ;
    pbR->nbClock = clock() ;
    
    n = 1 ; p = 0 ;
    /*
     A decommenter pour partir d'une solution pour trouver la suivante
     delta=0	; p=2	; n=3	; d=9 ;
     delta=0	; p=14	; n=20	; d=50 ;
     delta=0	; p=84	; n=119	; d=289 ;
     delta=0	; p=492	; n=696	; d=1682 ;
     delta=0	; p=2870	; n=4059	; d=9801 ;
     delta=0	; p=16730	; n=23660	; d=57122 ;
     delta=0	; p=97512	; n=137903	; d=332929 ;
     delta=0	; p=568344	; n=803760	; d=1940450 ;
     delta=0	; p=3312554	; n=4684659	; d=11309769 ;
     delta=0	; p=19306982	; n=27304196	; d=65918162 ;
     delta=0	; p=112529340	; n=159140519	; d=384199201 ;
     delta=0	; p=655869060	; n=927538920	; d=2239277042 ;
     delta=0	; p=3822685022	; n=5406093003	; d=13051463049 ;
     delta=0	; p=22280241074	; n=31509019100	; d=76069501250 ;
     delta=0	; p=129858761424	; n=183648021599	; d=443365544449 ;
     delta=0	; p=756872327472	; n=1070379110496	; d=2584123765442 ;
     delta=0	; p=129858761424	; n=183648021599	; d=443365544449 ;
     
     delta=-56711258	;       p= 707106781	; n= 1000000000	;    d=2300791048 ;
     delta=-10168600468 ;    p= 7071067811    ;n= 10000000000   ; d=3804934688 ;
     delta=-71885299958 ;    p= 70710678118  ; n= 100000000000  ; d=97650756322 ;
     delta=-567209652218;    p= 707106781186 ; n= 1000000000000 ; d=127979425793 ;
     */
    // formule pour des petites valeurs d'initialisation
    d = 2*(p+1)*(p+1) - n * n ;
    delta = p * (p + 1) - (n * (n + 1))/2 ;
    
#if defined(INIT_FROM_MIN_N)
    n = PB100_MIN_N ;
    { // on cherche 2*p*p = n*n
        int64_t r = n ;
        int nd ;
        for(nd=0; (r > 10) && ((r % 10) == 0) ; nd++) {  r /= 10 ; }
        r =  r * r;
        p = 0 ;
        while(nd-- > 0 ) {
            int i ;
            p = 10 * p ;
            r = 100 * r ;
            for(i=0; r >= 4 * p + 4 * i + 2 ;i++) {
                r -= 4 * p + 4 * i + 2 ;
            }
            p += i ;
        } // n * n = 2 * p * p + r
        d = 4*p+2-r ; // 4*p+2-r = 4*p+2 + 2*p*p - n*n= 2*(p+1)(p+1) - n*n
        delta = (d-n)/2-p-1 ; // (d-n)/2-p-1 =p*(p+1)-(n*(n+1))/2
    }
#endif
    p2 = 2*p ;
    
#if PB100_DEBUG
    if(pbR->isVerbose)fprintf(stdout,"\t PB%s delta=%lld\t; p=%lld\t; n=%lld\t; d=%lld ;\n",pbR->ident,delta ,p2 >> 1,n,d) ;
#endif
    while(1) {
        d -= (n * 2) + 1 ;
        delta -= n + 1 ;
        n++ ;
        if(d < 0)  {
            delta += p2 + 2 ;
            d += (p2 * 2) + 6 ;
            p2 += 2 ;
            if(delta == 0) {
#if PB100_DEBUG
                if(pbR->isVerbose)fprintf(stdout,"\t PB%s delta=%lld\t; p=%lld\t; n=%lld\t; d=%lld ;\n",pbR->ident, delta ,p2 >> 1,n,d) ;
#endif
                if(n > PB100_MIN_N ) break ;
            }
        }
    }
    pbR->nbClock = clock() - pbR->nbClock ;
    if(pbR->isVerbose)fprintf(stdout,"\t PB%s blue=%lld total=%lld %lldx%lld=2x%lldx%lld \n"
                              ,pbR->ident
                              ,(p2>>1)+1, n+1, n,n+1,(p2>>1),(p2>>1)+1
                              );
    snprintf(pbR->strRes, sizeof(pbR->strRes),"%lld",(p2>>1)+1);
    return 1;
    
}

