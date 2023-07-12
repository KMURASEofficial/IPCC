
#include "params.h"
#include "keygen.h"
#include "encrypt.h"
#include "invpoly.h"
#include "decrypt.h"
#include "rng.h"
#include <time.h>

int len(invpoly *ct);

int crypto_encrypt_keypair(uint8_t (*pk)[2], uint8_t *sk) {
    keygen(pk, sk);
	return 1;
}

// Decrypt part.
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
	get_neig(&g, pk); // [256][4]
	
	uint8_t vlist[VERTEXES] = { 0, };
	uint32_t value = 0, m1, m2;
	int length; // term의 length (MAX TERM은 14비트 정도)

	invpoly pr, p; memset(&pr, 0, sizeof(pr)); memset(&p, 0, sizeof(p));

	randomint();
	randomint();
	randomint();

#if MULTIGRAPH == 1
	invpoly p1, p2; memset(&p1, 0, sizeof(p1)); memset(&p2, 0, sizeof(p2));
	m1 = 10;  m2 = (*m)/m1;

	poly_geninvariant(&p1, &g, DEPEND, vlist, &value, K1, &m1, K1); memset(&vlist, 0, sizeof(vlist));
	poly_geninvariant(&p2, &g, DEPEND, vlist, &value, K2, &m2, K2);
#else
	m1 = *m;
#endif

#if MULTIGRAPH==1
#ifdef REDUCE // milti,reduce
	poly_mul(&p, &p1, &p2, K1, K2);   // p = p1*p2
	reduce_dim(&pr, &p, K); 
	reduce_sort(&pr, K);  // 이부분 막 섞어보기 23.5.22
	reduce_neig(&pr, &g, K, &length); 
	reduce_dup(ct, &pr, K, &length); 
	poly_shuffle(ct, K, &length); 
	// printf(" Encryption part == \n");
	// poly_print("ct = ", ct, K, length);
	// printf("multi(K=%d,%d), reduce : length of ciphertext is %d\n", K1, K2, len(ct));
#else // multi, not reduce
	poly_mul(ct, &p1, &p2, K1, K2);   // p = p1*p2
	// printf("multi(K=%d,%d), not reduce : length of ciphertext is %d\n", K1, K2, len(ct));
#endif

#else
#ifdef REDUCE // single, reduce
	poly_geninvariant(&p, &g, DEPEND, vlist, &value, K, &m1, K);
	reduce_dim(&pr, &p, K);
	reduce_sort(&pr, K);
	reduce_neig(&pr, &g, K, &length); 
	reduce_dup(ct, &pr, K, &length);  // ct = pr = pdsnd (이부분 7차이상 single graph 오류)
	poly_shuffle(ct, K, &length);
	printf(" Encryption part == \n");
	// poly_print("ct = ", ct, K, length);
	printf("single(K=%d), reduce : length of ciphertext is %d\n", K, len(ct));
#else // single, not reduce
	poly_geninvariant(ct, &g, DEPEND, vlist, &value, K, &m1, K);
	printf("single(K=%d), not reduce : length of ciphertext is %d\n", K, len(ct));
#endif
#endif

// #if DEBUG == 1
// 	poly_print("ct = ", ct, K, length);
// #elif DEBUG == 2
// 	poly_print_tex("ct = ", ct, K, length);
// #elif DEBUG == 3
// 	poly_print_debug("ct = ", ct, K, length);
// #endif

	return -fail;
}

// main function
int main() {

    uint8_t sk[NUMPDS], pk[6*NUMPDS][2]; // pds sets, graph edges
	uint32_t recovered[1], m = MESSAGE;
	invpoly ct; memset(&sk, 0, sizeof(sk)); memset(&pk, 0, sizeof(pk)); memset(&ct, 0, sizeof(ct));


    crypto_encrypt_keypair(pk, sk);
	
	crypto_encrypt(&ct, &m, pk);
	crypto_decrypt(recovered, &ct, sk);

#if DEBUG == 1
	// poly_print("ct = ", &ct, K, 100);
#elif DEBUG == 2
	poly_print_tex("ct = ", &ct, K, 100);
#elif DEBUG == 3
	poly_print_debug("ct = ", &ct, K, 100);
#endif
#ifdef DEBUG
	printf("\nrecovered = %d\n", *recovered);
#endif
    return 0;
}

int len(invpoly *ct) {
	for (int len=0; len<TERM; len++)
		if (!ct->d[len].coeffs)
			return len-1;
	return -1;
}

// EOF