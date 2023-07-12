#ifndef PARAMS_H
#define PARAMS_H

#define REDUCE
#define MULTIGRAPH 1
#define K1 3
#define K2 4
#define K  (K1+K2)


#define MESSAGE 4410
#define REDUCE_DUP
#define POLY_SHUFFLE
#define DEBUG 1      
#define Q 2147483648 
#define UI 0



#define VERTEXES 256
#define DEPEND 1
#define NOTDEPEND 0
#define TERM (1<<(2*K+1))*2
#define NEIGHBORHOODS 4
#define NUMPDS VERTEXES/4
#define EDGES NUMPDS*6
#define NUMSTARTVER 0





#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <stdint.h>
#include <math.h>

#define IPCC_PUBLICKEYBYTES CIPHER_VERTEX // pk
#define IPCC_PUBLICKEYBYTES_ROW 6*maxPDS // pk_row
#define IPCC_PUBLICKEYBYTES_COL 2 // pk_col
#define IPCC_SECRETKEYBYTES CIPHER_PDS // sk
#define IPCC_CIPHERTEXTBYTES CIPHER_TERMS * CIPHER_BOX // ct
#define IPCC_CIPHERTEXTBYTES_ROW CIPHER_TERMS // ct_row
#define IPCC_CIPHERTEXTBYTES_COL CIPHER_DIM + 2 // ct_col

#endif