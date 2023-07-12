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

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "invpoly.h"
#include "params.h"
#include "rng.h"

void poly_gendim1(invpoly *p, graph *g, int depend, uint8_t *vlist, uint32_t *value) {
    uint8_t n;
    randombytes(&n, 1);
    
    int lastindex;
    for ( int i=0; i<TERM; i++ )
        if ( p->d[i].coeffs == 0 ) {
            lastindex = i;
            break;
        }
    uint8_t vnum;
    uint32_t coeffs;
    randombytes(&vnum, 1);
    coeffs = randomint() % Q;

    if ( depend ) {
        while(1) {
            int re = -1;
            for ( int i=0; i<VERTEXES; i++ ) {
                if ( vnum == vlist[i] )
                    re = 1;
            }
            if ( re == -1 )
                break;
            randombytes(&vnum, 1);
        }

        vlist[4*(K-1)+0] = g->v[vnum].neig[0];
        vlist[4*(K-1)+1] = g->v[vnum].neig[1];
        vlist[4*(K-1)+2] = g->v[vnum].neig[2];
        vlist[4*(K-1)+3] = g->v[vnum].neig[3];
    }
    for ( int i=0; i<4; i++ ) {
        p->d[lastindex + i].v[0] = g->v[vnum].neig[i];
        p->d[lastindex + i].coeffs = coeffs;
    }
    value[0] += coeffs;
    value[0] %= Q;
}

void poly_geninvariant(invpoly *p, graph *g, int depend, uint8_t *vlist, uint32_t *value, int k, uint32_t *m, int maxk) {

    if ( k == 1 ) {
        uint8_t numv;
        randombytes(&numv, 1);
        if ( depend == DEPEND ) { 
            uint8_t s; 
            randombytes(&s, 1);
            numv =  (numv % 3) + 1; 
            s %= numv; 

            for ( int i=0; i<numv; i++ ) {
                if ( i == s )
                    poly_gendim1(p, g, DEPEND, vlist, value); 
                else
                    poly_gendim1(p, g, NOTDEPEND, vlist, value); 
            }
        }
        else {
            numv = numv % 4; 
            for ( int i=0; i<numv; i++ )
                poly_gendim1(p, g, NOTDEPEND, vlist, value);
        }
    }
    if ( k > 1 ) {
        uint32_t mp;
        mp = randomint() % Q;

        uint8_t vnum;
        while (1) {
            randombytes(&vnum, 1);
            if ( depend == DEPEND ) {
                for ( int i=0; i<VERTEXES; i++ ) { 
                    if ( vnum == vlist[i] ) 
                        continue;
                }
                for ( int i=0; i<4; i++ ) {
                    vlist[4*K-(4*k)+i] = g->v[vnum].neig[i]; 
                }
            }
            break;                          
        }
        uint32_t a[4] = { 0, };
        uint8_t s; 
        
        randombytes(&s, 1);
        s = s % 4; 
        for ( int i=0; i<4; i++ ) {
            value[0] = 0;
            if ( depend ) {
                if ( i == s )
                    poly_geninvariant(p, g, DEPEND, vlist, value, k-1, m, maxk);
                else 
                    poly_geninvariant(p, g, NOTDEPEND, vlist, value, k-1, m, maxk);
            }
            else 
                poly_geninvariant(p, g, NOTDEPEND, vlist, value, k-1, m, maxk);
            if (k == maxk)
                mp = m[0];

            a[i] = (value[0]+Q-mp) % Q;
            value[0] = mp;
            for ( int j=0; j<TERM; j++ ) {
                if ( p->d[j].coeffs == 0 ) {
                    p->d[j].coeffs = Q - a[i];
                    for ( int l=0; l<=k-1; l++ ) {
                        p->d[j].v[l] = g->v[vnum].neig[i]; 
                    }
                    break;
                }
                else if ( (p->d[j].v[k-1]) == 0 )
                    p->d[j].v[k-1] = g->v[vnum].neig[i]; 
            }
        }
    }
    else
        return;


}

void vlist_print(char *msg, uint8_t vlist[VERTEXES]) {
    printf(" %s ", msg);
    for ( int i=0; i<4*K; i++ ) {
        printf("%d ", vlist[i]);
    }
    printf("\n");
}

void poly_print(const char *msg, invpoly *p, int dimk, int length) {
    printf(" %s ", msg);
    printf("\n     [ dim %d ]\n", dimk);
    int terms;
    for ( int i=0; i<=length; i++ ) {
        printf(" %10d | ", p->d[i].coeffs);
        for ( int j=0; j<dimk; j++ ) {
            if (p->d[i].v[j])
                printf("%3d ", p->d[i].v[j]);
        }
        printf("\n");
    }
}

void poly_print_tex(const char *msg, invpoly *p, int dimk, int length) {
    printf(" %s ", msg);
    printf("\n     [ dim %d ]\n (", dimk);
    int terms;
    for ( int i=0; i<=length; i++ ) {
        printf(" %10d\\cdot (", p->d[i].coeffs);
        for ( int j=0; j<dimk; j++ ) {
            if (p->d[i].v[j])
                printf("\\cdot x_v_{%d} + ", p->d[i].v[j]);
        }
        printf(" ) + (");
    }
}
void poly_print_debug(const char *msg, invpoly *p, int dimk, int length) {
    printf(" %s ", msg);
    printf("\n     [ dim %d ]\n (", dimk);
    int terms;
    for ( int i=0; i<=length; i++ ) {
        printf(" %10d * (", p->d[i].coeffs);
        for ( int j=0; j<dimk; j++ ) {
            if (p->d[i].v[j]) {
                printf(" x_v_{%3d} * ", p->d[i].v[j]);
            }
        }
        printf(" )\n + ");
    }
    
}