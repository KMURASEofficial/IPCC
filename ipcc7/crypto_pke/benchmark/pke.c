
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

	// Check speed
	double s,e;
	s=(double)clock();

	for (int i=0; i<REPEAT_DEC; i++) {
		recovered = 0; memset(&pdsarr, 0, sizeof(pdsarr));
		*m=0;
		get_pdsarr(pdsarr, sk);
		decrypt(&recovered, ct, pdsarr);
		*m = recovered;
	}

	e=(double)clock();
	printf("(Decrypt Speed : %d+%d,  %10.3lf)\n", K1,K2, (((e-s)/CLOCKS_PER_SEC) * 1000) / REPEAT_DEC);
	// printf("(%d, %10llu, %10.2lf)\n", K, (((e-s)/CLOCKS_PER_SEC) * 1000) / REPEAT_KEY);
	return 1;
}

int crypto_encrypt(invpoly *ct, const uint32_t *m, const uint8_t (*pk)[2]) {

	int fail = -1;
	uint64_t ct_bytes = 0;

	// Check speed
	double s,e;
	s=(double)clock();
	// Check CIPH length
	uint64_t l=0;
	uint32_t value, m1, m2;
	graph g;
	uint8_t vlist[VERTEXES];
	int length;
	invpoly pr, p;
	invpoly p1, p2;
	l=0;



	for (int r=0; r<REPEAT; r++) {

		memset(ct, 0, sizeof(invpoly)); 
		memset(&g, 0, sizeof(g));      // memset(vlist, 0, sizeof(vlist)); 
		value=0; m1=0; m2=0; length=0; // term의 length (MAX TERM은 14비트 정도)
		memset(&pr, 0, sizeof(pr)); memset(&p, 0, sizeof(p)); memset(&vlist, 0, sizeof(vlist));
		get_neig(&g, pk);              // [256][4]


#if MULTIGRAPH == DBL   // [multi]
		m1 = 10;  m2 = (*m)/m1;
		memset(&p1, 0, sizeof(p1)); memset(&p2, 0, sizeof(p2));
		poly_geninvariant(&p1, &g, DEPEND, vlist, &value, K1, &m1, K1);
		memset(&vlist, 0, sizeof(vlist));
		poly_geninvariant(&p2, &g, DEPEND, vlist, &value, K2, &m2, K2);
#else				    // [single]
		m1 = *m;
		poly_geninvariant(&p, &g, DEPEND, vlist, &value, K, &m1, K);
#endif


#if MULTIGRAPH == DBL
#ifdef REDUCE 			// [milti ,reduce]
		poly_mul(&p, &p1, &p2, K1, K2); 
		reduce_dim(&pr, &p, K); 
		reduce_sort(&pr, K); 
		reduce_neig(&pr, &g, K, &length); 
		reduce_dup(ct, &pr, K, &length); 
		poly_shuffle(ct, K, &length); 
		poly_print_tex("ct = ", ct, K, length);
#else 					// [multi, not reduce]
		poly_mul(ct, &p1, &p2, K1, K2); // 이부분 오류
#endif

#else
#ifdef REDUCE 			// [single, reduce]
		reduce_dim(&pr, &p, K); 		  
		reduce_sort(&pr, K);
		reduce_neig(&pr, &g, K, &length); 
		reduce_dup(ct, &pr, K, &length);
		poly_shuffle(ct, K, &length);
#else 					// [single, not reduce]
		poly_geninvariant(ct, &g, DEPEND, vlist, &value, K, &m1, K);
#endif
#endif
		l+=len(ct);
		// printf(".");
	}
	e=(double)clock();

	ct_bytes = (l/REPEAT * (4+K));

#if MULTIGRAPH == SGL
	printf("(Encryption : %d, %10llu(%llu-byte), %10.2lf)\n", K, l/REPEAT, ct_bytes, (((e-s)/CLOCKS_PER_SEC) * 1000) / REPEAT);
	// printf("(Encryption : %d, %10.2lf)\n", K, (((e-s)/CLOCKS_PER_SEC) * 1000) / REPEAT);
#else
	printf("(Encryption speed : %d+%d, %10llu(%llu-byte), %10.2lf)\n", K1,K2, l/REPEAT, ct_bytes, (((e-s)/CLOCKS_PER_SEC) * 1000) / REPEAT);
#endif


	return -fail;
}

// main function
int main() {

    uint8_t sk[NUMPDS], pk[6*NUMPDS][2]; // pds sets, graph edges
	uint32_t recovered[1], m = MESSAGE;
	invpoly ct; memset(&sk, 0, sizeof(sk)); memset(&pk, 0, sizeof(pk)); memset(&ct, 0, sizeof(ct));

	for (int i=0; i<clock()/100; i++)
		randomint();

    crypto_encrypt_keypair(pk, sk);
	
	crypto_encrypt(&ct, &m, pk);
	crypto_decrypt(recovered, &ct, sk);

#if DEBUG == 1
	// poly_print("ct = ", &ct, K, 100);
#elif DEBUG == 2
	poly_print_tex("ct = ", &ct, K, 100);
#elif DEBUG == 3
	// poly_print_debug("ct = ", &ct, K, TERM);
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