//
//  euler.c
//  EulerProject
//  Created by Jeannot on 26/01/2018.
//  Copyright © 2018 Jeannot. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "euler_utils.h"
#include "PB001_050.h"
#include "PB051_100.h"
#include "PB101_150.h"
#include "PB_gmp.h"
#include "PB_other.h"





typedef struct TotalRun {
    clock_t TotalClock ;
    int isVerbose ;
    int nbPBOK ;
    int nbPBerror ;
} TotalRun ;


void Execute(TotalRun *ttr, PB_CALL *pbCall) {
    PB_RESULT pbR ;
    memset(&pbR,0,sizeof(pbR)) ;
    pbR.isVerbose = ttr->isVerbose ;
    if( pbCall->pbSolve != NULL) {
        pbR.ident = pbCall->ident ;
        pbR.nbClock = 0 ;
        if(pbCall->pbSolve(&pbR)) {
            ttr->TotalClock += pbR.nbClock ;
            if(strcmp(pbCall->Solution,pbR.strRes)==0) {
                ttr->nbPBOK++ ;
                fprintf(stdout,"OK\tPB%s(%.06fs) Sol=%s '%s'\n",pbCall->ident,(float)pbR.nbClock / CLOCKS_PER_SEC,pbR.strRes,(pbCall->name) ? pbCall->name : "") ;
            } else {
                ttr->nbPBerror++ ;
                fprintf(stdout,"ERROR\tPB%s(%.06fs) Find=%s != Exp=%s\n",pbCall->ident,(float)pbR.nbClock / CLOCKS_PER_SEC,pbR.strRes,pbCall->Solution) ;
            }
        } else {
            ttr->TotalClock += pbR.nbClock ;
            ttr->nbPBerror++ ;
            fprintf(stdout," BAD EXECUTION PB%s(%.06fs)\n",pbCall->ident,(float)pbR.nbClock / CLOCKS_PER_SEC) ;
        }
        
    } else {
        fprintf(stdout,"SKIPPED\tPB%s\n",pbCall->ident) ;
    }

    
}


#define PROTO_CALL(NUM,SOL,NAME)   {#NUM, PB##NUM , #SOL, #NAME}

static PB_CALL ALL_calls[] = {
    

    PROTO_CALL(001,233168,'Multiples of 3 and 5')
    ,PROTO_CALL(002,4613732,'Even Fibonacci numbers')
    ,PROTO_CALL(003,6857,'Largest prime factor')
    ,PROTO_CALL(004,906609,'Largest palindrome product')
    ,PROTO_CALL(005,232792560,'Smallest multipl')
    ,PROTO_CALL(006,25164150,'Smallest multiple')
    ,PROTO_CALL(007,104743,'Smallest multiple')
    ,PROTO_CALL(008,23514624000,'Largest product in a series')
    ,PROTO_CALL(009,31875000,'Special Pythagorean triplet')
    ,PROTO_CALL(010,142913828922,'Summation of primes')
    ,PROTO_CALL(011,70600674,'Largest product in a grid')
    ,PROTO_CALL(012,76576500,'Highly divisible triangular number')
    ,PROTO_CALL(013,5537376230,'Large sum')
    ,PROTO_CALL(014,837799,'Longest Collatz sequence')
    ,PROTO_CALL(015,137846528820,'Lattice paths')
    ,PROTO_CALL(016,1366,'Power digit sum')
    ,PROTO_CALL(016_gmp,1366,'Power digit sum')
 //         ,PROTO_CALL(017,XXX,'Number letter counts')
    ,PROTO_CALL(018,1074,'Maximum path sum I')
    //        ,PROTO_CALL(19,XXXX,'Counting Sundays')
    ,PROTO_CALL(020,648,'Factorial digit sum')
    ,PROTO_CALL(021,31626,'Amicable numbers')
    //        ,PROTO_CALL(022,XXX,'Names scores')
    ,PROTO_CALL(023,4179871,'Non-abundant sums')
    ,PROTO_CALL(024,2783915460,'Lexicographic permutations')
    ,PROTO_CALL(025,4782,'1000-digit Fibonacci number')
    ,PROTO_CALL(026,983,'Reciprocal cycles')
    ,PROTO_CALL(027,-59231,'Quadratic primes')
    ,PROTO_CALL(028,669171001,'Number spiral diagonals')
    ,PROTO_CALL(029,9183,'Distinct powers')
    ,PROTO_CALL(030,443839,'Digit fifth powers')
    ,PROTO_CALL(031,73682,'Coin sums')
    ,PROTO_CALL(032,45228,'Pandigital products')
    ,PROTO_CALL(033,100,'Digit cancelling fractions')
    ,PROTO_CALL(034,40730,'Digit factorials')
    ,PROTO_CALL(035,55,'Circular primes')
    ,PROTO_CALL(036,872187,'Double-base palindromes')
    ,PROTO_CALL(037,748317,'Truncatable primes')
    ,PROTO_CALL(037r,748317,'Truncatable primes')
    ,PROTO_CALL(038,932718654,'Truncatable primes')
    ,PROTO_CALL(039,840,'Integer right triangles')
    ,PROTO_CALL(040,210,'Champernowne\'s constant')
    ,PROTO_CALL(041,7652413,'Pandigital prime')
    ,PROTO_CALL(042,162,'Coded triangle numbers')
    ,PROTO_CALL(043,16695334890,'Sub-string divisibility')
    ,PROTO_CALL(044,5482660,'Pentagon numbers')
    ,PROTO_CALL(045,1533776805,'Triangular, pentagonal, and hexagonal')
    ,PROTO_CALL(046,5777,'Goldbach\'s other conjecture')
    ,PROTO_CALL(047,134043,'Distinct primes factors')
    ,PROTO_CALL(048,9110846700,'Self powers')
    ,PROTO_CALL(049,296962999629,'Prime permutations')
    ,PROTO_CALL(050,997651,'Consecutive prime sum')
    ,PROTO_CALL(051,121313,'Prime digit replacements')
    ,PROTO_CALL(052,142857,'Permuted multiples')
    ,PROTO_CALL(052a,142857,'Permuted multiples')
    ,PROTO_CALL(053,4075,'Combinatoric selections')
    ,PROTO_CALL(055,249,'Lychrel numbers')
    ,PROTO_CALL(056_gmp,972,'Powerful digit sum')
    ,PROTO_CALL(057,153,'Square root convergents')
    ,PROTO_CALL(058,26241,'Spiral primes')
    ,PROTO_CALL(059,107359,'XOR decryption')
    ,PROTO_CALL(060,26033,'Prime pair sets')
    ,PROTO_CALL(061,28684,'Cyclical figurate numbers')
    ,PROTO_CALL(062,127035954683,'Cubic permutations')
    ,PROTO_CALL(063,49,'Powerful digit counts')
    ,PROTO_CALL(064,1322,'Odd period square roots')
    ,PROTO_CALL(065,272,'Convergents of e')
    ,PROTO_CALL(066,661,'Diophantine equation')
    ,PROTO_CALL(067,7273,'Maximum path sum II')
    ,PROTO_CALL(068,6531031914842725,'Magic 5-gon ring')
//    ,PROTO_CALL(069a,510510,'Totient maximum')
    ,PROTO_CALL(069b,510510,'Totient maximum')
//    ,PROTO_CALL(070,8319823,'Totient permutation')
    ,PROTO_CALL(070a,8319823,'Totient permutation')
    ,PROTO_CALL(071,428570,'Ordered fractions')
    ,PROTO_CALL(072,303963552391,'Counting fractions')
    ,PROTO_CALL(073,7295372,'Counting fractions in a range')
//    ,PROTO_CALL(074,402,'Digit factorial chains')
    ,PROTO_CALL(074a,402,'Digit factorial chains')
    ,PROTO_CALL(075,161667,'Singular integer right triangles')
    ,PROTO_CALL(076,190569291,'Counting summations')
    ,PROTO_CALL(076a,190569291,'Counting summations')
    ,PROTO_CALL(077,71,'Prime summations')
    ,PROTO_CALL(078,55374,'Counting summations')
    ,PROTO_CALL(080_gmp,40886,'Square root digital expansion')
    ,PROTO_CALL(081,427337,'Path sum: two ways')
    ,PROTO_CALL(082,260324,'Path sum: three ways')
    ,PROTO_CALL(083,425185,'Path sum: four ways')
    ,PROTO_CALL(085,2772,'Counting rectangles')
    ,PROTO_CALL(086,1818,'Cuboid route')
    ,PROTO_CALL(087,1097343,'Prime power triples')
    ,PROTO_CALL(088,7587457,'Product-sum numbers')
    ,PROTO_CALL(090,1217,'Cube digit pairs')
    ,PROTO_CALL(091,14234,'Right triangles with integer coordinates')
//    ,PROTO_CALL(092,8581146,'Square digit chains')
    ,PROTO_CALL(092a,8581146,'Square digit chains')
    ,PROTO_CALL(093,1258,'Arithmetic expressions')
// version recursive parametrable en nombre de digits
//    ,PROTO_CALL(093a,1258,'Arithmetic expressions'),
    ,PROTO_CALL(094,518408346,'Almost equilateral triangles')
    ,PROTO_CALL(095,14316,'Amicable chains')

    ,PROTO_CALL(579,3805524,'Lattice points in lattice cubes')

// a revoir beaucoup trop lent
//    ,{100,  PB100,756872327473,'XX')
//    ,PROTO_CALL(1000,179424673,'Test 10 000 000 prime numbers')
// version moins rapide pour grande valeurs
//    ,PROTO_CALL(597_gmpa,50018178282,'Torpids')
// version rapide mais calculs complets (pour debug)
//    ,PROTO_CALL(597_gmp,50018178282',Torpids')
// version la plus rapide avec calcul FACT en u_int64_t et optimisation last loop
//    ,PROTO_CALL(597_gmpx,50018178282,'Torpids')
// version beaucoup plus rapide en O(n**2)
    ,PROTO_CALL(597_gmpy,50018178282,'Torpids')
    ,PROTO_CALL(622,3010983666182123972,'Riffle Shuffles')
    ,PROTO_CALL(101,37076114526,'Optimum polynomial')
  

    ,{ NULL,NULL,"","" }
};


static PB_CALL CUR_calls[] = {
 //    { 051,121313,'Prime digit replacements'),
 //   {100,756872327473,'XX'),
//    PROTO_CALL(103d,20313839404245,'Special subset sums: optimum')
    PROTO_CALL(107,259679,'Minimal network')
//        ,PROTO_CALL(103g,20313839404245,'Special subset sums: optimum')
//        ,PROTO_CALL(103f,20313839404245,'Special subset sums: optimum')
 //       ,PROTO_CALL(103e,20313839404245,'Special subset sums: optimum')
//    ,PROTO_CALL(103c,20313839404245,'Special subset sums: optimum')
//    ,PROTO_CALL(103b,20313839404245,'Special subset sums: optimum')
//    ,PROTO_CALL(103a,20313839404245,'Special subset sums: optimum')
//    ,PROTO_CALL(103,20313839404245,'Special subset sums: optimum')
    
//  ,PROTO_CALL(105,73702,'Special subset sums: testing')
//    ,PROTO_CALL(106,21384,'Special subset sums: meta-testing')

    ,{NULL,NULL,"",""}
} ;


int main(int argc, const char * argv[]) {
    PB_CALL *ptCall ;
    TotalRun ttr ;
    

    ttr.isVerbose = 1 ;
    ttr.nbPBerror  = ttr.nbPBOK = 0 ;
    ttr.TotalClock = 0 ;
    clock_t debut = clock() ;
 //   char * isALL = "100" ;
    char * isALL = NULL ;
    char  * pbMax = (isALL == NULL) ? "ZZZZ" : isALL ;
    for(ptCall = (isALL==NULL) ? CUR_calls : ALL_calls ; ptCall->ident != NULL && strcmp(ptCall->ident,pbMax) < 0 ; ptCall++) {
        Execute(&ttr,ptCall);
    }
    debut = clock() - debut ;
    fprintf(stdout,"### Execution of %d PB en %.03fs [%.03fs] (%d en erreur)\n",ttr.nbPBOK+ttr.nbPBerror,(float) ttr.TotalClock / CLOCKS_PER_SEC,(float) debut / CLOCKS_PER_SEC,ttr.nbPBerror) ;
    return 0;
}
