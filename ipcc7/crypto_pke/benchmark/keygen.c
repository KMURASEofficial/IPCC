
#include "keygen.h"
#include "rng.h"
#include "invpoly.h"
#include <time.h>
#include <stdlib.h>

void keygen(uint8_t (*pk)[2], uint8_t *pds) {
	// Check speed
	double s,e;
	s=(double)clock();

	// Generate for Secret Key PDS
	uint8_t tmp_index[VERTEXES] = { 0, };
	uint8_t A[NUMPDS] = { 0, };
	uint8_t B[NUMPDS] = { 0, };
	uint8_t C[NUMPDS] = { 0, };
	uint8_t D[NUMPDS] = { 0, };
	uint8_t tmp_arr[NUMPDS] = { 0, };

	for (int i=0; i<REPEAT_KEY; i++) {

		memset(&tmp_index, 0, sizeof(tmp_index));
		memset(&A, 0, sizeof(A));
		memset(&B, 0, sizeof(B));
		memset(&C, 0, sizeof(C));
		memset(&D, 0, sizeof(D));
		memset(pds, 0, sizeof(NUMPDS));
		memset(pk, 0, sizeof(6*NUMPDS*2));

		shuffle(tmp_index, VERTEXES);



		for (int i = 0; i < NUMPDS; i++) {
			A[i] = tmp_index[4*i+0];
			B[i] = tmp_index[4*i+1];
			C[i] = tmp_index[4*i+2];
			D[i] = tmp_index[4*i+3];
		}
		for (int i = 0; i < NUMPDS; i++) 
			pds[i] = A[i];

		//* Generate for Public Key Graph : Neighbor Relationships
		memset(&tmp_arr, 0, sizeof(tmp_arr));
		
		shuffle(tmp_arr, NUMPDS);
		for (int i = 0; i < NUMPDS; i++) add_graphedge(pk, i, A[i], B[tmp_arr[i]]);				// 1) A-B

		shuffle(tmp_arr, NUMPDS);
		for (int i = 0; i < NUMPDS; i++) add_graphedge(pk,	 NUMPDS + i, A[i], C[tmp_arr[i]]);	// 2) A-C

		shuffle(tmp_arr, NUMPDS);
		for (int i = 0; i < NUMPDS; i++) add_graphedge(pk, 2 * NUMPDS + i, A[i], D[tmp_arr[i]]);	// 3) A-D

		shuffle(tmp_arr, NUMPDS);
		for (int i = 0; i < NUMPDS; i++) add_graphedge(pk, 3 * NUMPDS + i, B[i], C[tmp_arr[i]]);	// 4) B-C

		shuffle(tmp_arr, NUMPDS);
		for (int i = 0; i < NUMPDS; i++) add_graphedge(pk, 4 * NUMPDS + i, B[i], D[tmp_arr[i]]);	// 5) B-D

		shuffle(tmp_arr, NUMPDS);
		for (int i = 0; i < NUMPDS; i++) add_graphedge(pk, 5 * NUMPDS + i, C[i], D[tmp_arr[i]]);	// 6) C-D

	}

	e=(double)clock();

#if MULTIGRAPH == DBL
	printf("(GenKey Speed : %d+%d,  %10.2lf)\n", K1,K2, (((e-s)/CLOCKS_PER_SEC) * 1000) / REPEAT_KEY);
#else

#endif
	// printf("(%d, %10llu, %10.2lf)\n", K, (((e-s)/CLOCKS_PER_SEC) * 1000) / REPEAT_KEY);
}

void shuffle(uint8_t *arr, int length) {
	for (int i = 0; i < length; i++)
		arr[i] = i;  // arr = [0~255]

	uint8_t index, temp;

	for (int i=0; i<length-1; i++) {
		randombytes(&index, 1);
		index = (index % VERTEXES) % (length-i) +i;
		temp = arr[i];
		arr[i] = arr[index];
		arr[index] = temp;
	}
}

void add_graphedge(uint8_t (*edge)[2], int i, uint8_t v1, uint8_t v2) {
	if (v1 < v2) {
		edge[i][0] = v1;
		edge[i][1] = v2;
	}
	else {
		edge[i][0] = v2;
		edge[i][1] = v1;
	}
}

