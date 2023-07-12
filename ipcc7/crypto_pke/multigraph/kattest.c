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

#include "params.h"
#include "keygen.h"
#include "encrypt.h"
#include "invpoly.h"
#include "decrypt.h"
#include "rng.h"
#include <time.h>


int len(invpoly *ct);
void kat_create(invpoly *ct);

int crypto_encrypt_keypair(uint8_t (*pk)[2], uint8_t *sk) {

    keygen(pk, sk);
	return 1;
}

int crypto_decrypt(uint32_t *m, invpoly *ct, const uint8_t *sk) {

	uint8_t pdsarr[VERTEXES] = { 0, };
	uint32_t recovered = 0;

	get_pdsarr(pdsarr, sk);
	decrypt(&recovered, ct, pdsarr);
	*m = recovered;
	return 1;
}

int crypto_encrypt(invpoly *ct, const uint32_t *m, const uint8_t (*pk)[2]) {

	int fail = -1;

	graph g; memset(&g, 0, sizeof(g));
	get_neig(&g, pk); 
	
	uint8_t vlist[VERTEXES] = { 0, };
	uint32_t value = 0, m1, m2;
	int length; 

	invpoly pr, p; memset(&pr, 0, sizeof(pr)); memset(&p, 0, sizeof(p));
	randomint();randomint();randomint();

#if MULTIGRAPH == 1
	invpoly p1, p2; memset(&p1, 0, sizeof(p1)); memset(&p2, 0, sizeof(p2));
	m1 = 10;  m2 = (*m)/m1;

	poly_geninvariant(&p1, &g, DEPEND, vlist, &value, K1, &m1, K1); memset(&vlist, 0, sizeof(vlist));
	poly_geninvariant(&p2, &g, DEPEND, vlist, &value, K2, &m2, K2);
#else
	m1 = *m;
#endif

#if MULTIGRAPH == 1
#ifdef REDUCE 	// milti,reduce
	poly_mul(&p, &p1, &p2, K1, K2);   
	reduce_dim(&pr, &p, K); 
	reduce_sort(&pr, K);  
	reduce_neig(&pr, &g, K, &length); 
	reduce_dup(ct, &pr, K, &length); 
	poly_shuffle(ct, K, &length); 
#else 			// multi, not reduce
	poly_mul(ct, &p1, &p2, K1, K2);   
#endif

#else
#ifdef REDUCE	 // single, reduce
	poly_geninvariant(&p, &g, DEPEND, vlist, &value, K, &m1, K);
	reduce_dim(&pr, &p, K);
	reduce_sort(&pr, K);
	reduce_neig(&pr, &g, K, &length); 
	reduce_dup(ct, &pr, K, &length);  
	poly_shuffle(ct, K, &length);
#else 			// single, not reduce
	poly_geninvariant(ct, &g, DEPEND, vlist, &value, K, &m1, K);
#endif
#endif

	return -fail;
}

// main function
int main() {

    uint8_t sk[NUMPDS], pk[6*NUMPDS][2]; 
	uint32_t recovered[1], m = MESSAGE;
	invpoly ct; 
	memset(&sk, 0, sizeof(sk)); 
	memset(&pk, 0, sizeof(pk)); 
	memset(&ct, 0, sizeof(ct));


    crypto_encrypt_keypair(pk, sk);
	
	crypto_encrypt(&ct, &m, pk);

	crypto_decrypt(recovered, &ct, sk);

	kat_create(&ct);

    return 0;
}

#include <stdio.h>
#define convert_bigendian_tobytes(x, y) \
	x[0] = (char)(y>>24)&0xff; \
	x[1] = (char)(y>>16)&0xff; \
	x[2] = (char)(y>>8)&0xff; \
	x[3] = (char)y&0xff;

void kat_create(invpoly *ct) {

	int i, j;
	char coeffs[32/8];
	FILE* fp = fopen("pqckat_n.rsp","w"); 

	for (i=0; i<TERM; i++) {
		if ( !ct->d[i].coeffs ) break;
		memcpy(coeffs, &ct->d[i].coeffs, 4);

		for (j=0; j<4; j++)
			fputc(coeffs[j], fp); 

		for (j=0; j<K; j++) {
			if ( !ct->d[i].v[j] )
				break;
				
			fputc(ct->d[i].v[j], fp); 
		}
	}
	fclose(fp);
}

int len(invpoly *ct) {

	for (int len=0; len<TERM; len++)
		if (!ct->d[len].coeffs)
			return len-1;
	return -1;
}

// EOF