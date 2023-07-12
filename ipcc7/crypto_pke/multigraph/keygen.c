//  Copyright © 2023 by Yongbhin Kim
// 
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include "keygen.h"
#include "rng.h"
#include "invpoly.h"
#include <time.h>
#include <stdlib.h>

void keygen(uint8_t (*pk)[2], uint8_t *pds) {

	// Generate for Secret Key PDS
	uint8_t tmp_index[VERTEXES] = { 0, };
	shuffle(tmp_index, VERTEXES);

	uint8_t A[NUMPDS] = { 0, };
	uint8_t B[NUMPDS] = { 0, };
	uint8_t C[NUMPDS] = { 0, };
	uint8_t D[NUMPDS] = { 0, };

	for (int i = 0; i < NUMPDS; i++) {
		A[i] = tmp_index[4*i+0];
		B[i] = tmp_index[4*i+1];
		C[i] = tmp_index[4*i+2];
		D[i] = tmp_index[4*i+3];
	}
	for (int i = 0; i < NUMPDS; i++) 
		pds[i] = A[i];

	uint8_t tmp_arr[NUMPDS] = { 0, };
	
	shuffle(tmp_arr, NUMPDS);
	for (int i = 0; i < NUMPDS; i++) add_graphedge(pk, i, A[i], B[tmp_arr[i]]);				

	shuffle(tmp_arr, NUMPDS);
	for (int i = 0; i < NUMPDS; i++) add_graphedge(pk,	 NUMPDS + i, A[i], C[tmp_arr[i]]);	

	shuffle(tmp_arr, NUMPDS);
	for (int i = 0; i < NUMPDS; i++) add_graphedge(pk, 2 * NUMPDS + i, A[i], D[tmp_arr[i]]);	

	shuffle(tmp_arr, NUMPDS);
	for (int i = 0; i < NUMPDS; i++) add_graphedge(pk, 3 * NUMPDS + i, B[i], C[tmp_arr[i]]);	

	shuffle(tmp_arr, NUMPDS);
	for (int i = 0; i < NUMPDS; i++) add_graphedge(pk, 4 * NUMPDS + i, B[i], D[tmp_arr[i]]);	

	shuffle(tmp_arr, NUMPDS);
	for (int i = 0; i < NUMPDS; i++) add_graphedge(pk, 5 * NUMPDS + i, C[i], D[tmp_arr[i]]);	
}

void shuffle(uint8_t *arr, int length) {
	for (int i = 0; i < length; i++)
		arr[i] = i; 

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

