#ifndef PARAMS_H
#define PARAMS_H


#define SGL 0
#define DBL 1
#define REPEAT_KEY 1
#define REPEAT 1
#define REPEAT_DEC 1

#define MULTIGRAPH DBL     // 0 is single graph, 1 is multi graph
#define K1 4
#define K2 3
#define K  (K1+K2)
#define REDUCE
#define NE 3
#define TERM (1<<(2*K+1))*2*(NE/3)  //* 2 // 2배 여유롭게

// #define MULTIGRAPH SGL     // 0 is single graph, 1 is multi graph
// #define K  7
// #define REDUCE
// #define NE 3
// #define K1 4
// #define K2 3

#define REDUCE_DUP         // O(n^2)
#define MESSAGE 4410
#define POLY_SHUFFLE
#define DEBUG 3            // 1(table forms) or 2(polynomial forms) or 3(for tex)
#define Q (2147483648)   // message space
#define UI 0



#define VERTEXES 256
#define DEPEND 1
#define NOTDEPEND 0
#define NEIGHBORHOODS 4
#define NUMPDS VERTEXES/4
#define EDGES NUMPDS*6
#define NUMSTARTVER 0


/*=================    auto SETTING   =================*/

// #define DATASIZE	16

// #define MAXnumVertex (G1numVertex>G2numVertex) ? G1numVertex:G2numVertex
// #define MAXnumVertex G1numVertex

//* Graph 1
// #define Graph1Start	1
// #define G1numPDS	G1numVertex / 4
// #define G1BOX		(G1numVertex / DATASIZE) + 1

// //* Graph 2
// #define Graph2Start	1 + G1numVertex
// #define G2numPDS	G2numVertex / 4
// #define G2BOX		(G2numVertex / DATASIZE) + 1


// //* Static Array
// #define maxVERTEX	300
// #define maxNumCs	2
// #define maxDIM		2
// #define maxPDS		maxVERTEX / 4
// #define maxBOX		(maxVERTEX / DATASIZE) + 1


//* Chipher
// #define CIPHER_VERTEX	G1numVertex + G2numVertex
// #define CIPHER_BOX		(CIPHER_VERTEX / DATASIZE) + 1
// #define CIPHER_DIM		G1fDim + G2fDim
// #define CIPHER_PDS		G1numPDS + G2numPDS
// #define CIPHER_TERMS	MUL_TERMS + (3 * maxTERMS)

/* F1) {f(G1, m1) + 1} * {f(G2, m2) + 1} - 1
 *     = f(G1, m1)f(G2, m2) + f(G1, m1) + f(G2, m2)
 * F2) {f(G1, m1) + 1} * {f(G2, m2) + 1} + f(G1, m3) - 1
 *     = f(G1, m1)f(G2, m2) + f(G1, m1) + f(G1, m3) + f(G2, m2)
 * F3) {f(G1, m1) + 1} * f(G2, m2) + f(G1, m3) + 1 - 1
 *     = f(G1, m1)f(G2, m2) + f(G1, m3) + f(G2, m2)
 * F4) f(G1, m1)f(G2, m2) + f(G1, m3) + f(G2, m4)
*/

//* FINAL_MESSAGE
// #if (IPCC_f == 1)
// 	#define PRINT_selectF()	printf("   (MESSAGE1 + 1) * (MESSAGE2 + 1) - 1\n")
// #elif (IPCC_f == 2)
// 	#define PRINT_selectF()	printf("   (MESSAGE1 + 1) * (MESSAGE2 + 1) + MESSAGE3 - 1\n")
// #elif (IPCC_f == 3)
// 	#define PRINT_selectF()	printf("   (MESSAGE1 + 1) * MESSAGE2 + MESSAGE3\n")
// #elif (IPCC_f == 4)
// 	#define PRINT_selectF()	printf("   MESSAGE1 * MESSAGE2 + MESSAGE3 + MESSAGE\n4")
// #elif ((IPCC_f > 4) || (IPCC_f < 1))
// 	#define PRINT_selectF()	printf("   selection ERROR : This option is NOT supported\n\n");
// #endif


// for DigitValue (bit digit with laxical order)
// #define Box(X) (X - 1) / DATASIZE
// #define Val(X) 1 << ((X - 1) % DATASIZE)


// #define TRUE	1
// #define FALSE	0









#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <stdint.h>
// #include <crtdbg.h>
#include <math.h>

//*  Define
typedef unsigned short	type;
typedef unsigned char	u8;
typedef unsigned short	u32;

#define IPCC_PUBLICKEYBYTES CIPHER_VERTEX // pk
#define IPCC_PUBLICKEYBYTES_ROW 6*maxPDS // pk_row
#define IPCC_PUBLICKEYBYTES_COL 2 // pk_col
#define IPCC_SECRETKEYBYTES CIPHER_PDS // sk
#define IPCC_CIPHERTEXTBYTES CIPHER_TERMS * CIPHER_BOX // ct
#define IPCC_CIPHERTEXTBYTES_ROW CIPHER_TERMS // ct_row
#define IPCC_CIPHERTEXTBYTES_COL CIPHER_DIM + 2 // ct_col

#endif