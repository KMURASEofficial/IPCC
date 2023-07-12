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
        if ( !ct->d[i].coeffs ) break;
        for ( int j=0; j<K; j++ )
            if (ct->d[i].v[j]) 
                sign *= pdsarr[ct->d[i].v[j]];
        temp = ct->d[i].coeffs * sign;
        recovered = (uint32_t)(((uint64_t)recovered + (uint64_t)temp)%Q);
        recovered %= Q;
        
    }
    m[0] = recovered;
    int fail;
    fail = -1;
    return -fail;

}