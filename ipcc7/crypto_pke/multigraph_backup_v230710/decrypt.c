#include "decrypt.h"
#include "rng.h"
#include "invpoly.h"
#include <stdlib.h>
#include <stdint.h>

void get_pdsarr(uint8_t pdsarr[VERTEXES], const uint8_t *pds) {

    for ( int i=0; i<NUMPDS; i++ ) {
        pdsarr[pds[i]]++;
    }
}

int decrypt(uint32_t *m, invpoly *ct, uint8_t pdsarr[VERTEXES]) {
    uint32_t recovered, temp;
    recovered = 0;
    int sign;

    for ( int i=0; i<TERM; i++ ) {
        sign=1;
        if ( !ct->d[i].coeffs ) break; // coeffs가 없으면 끝내기
        for ( int j=0; j<K; j++ )
            if (ct->d[i].v[j]) // 0이 아니면 검색(4.11 추가)
                sign *= pdsarr[ct->d[i].v[j]];
        temp = ct->d[i].coeffs * sign;
        recovered = (uint32_t)(((uint64_t)recovered + (uint64_t)temp)%Q);
        // printf("recovered = %d\n", recovered);
        recovered %= Q;
        
    }
    m[0] = recovered;
    int fail;
    fail = -1;
    return -fail;

}