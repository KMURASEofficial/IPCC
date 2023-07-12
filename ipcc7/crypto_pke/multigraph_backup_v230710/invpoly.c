/*
(k=1)
mp <- Z_Q
value <- c1 + c2 .. (이웃 제외)
a2 <- value - mp mod Q
value <- mp

(1<k<K)
mp <- Z_Q
a3 <- value - mp mod Q
value <- mp

(k=K)
a4 <- value-m
end

(일반화)
mp <- Z_Q
if k=K mp = m
a = value - mp mod Q
value <- mp

*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "invpoly.h"
#include "params.h"
#include "rng.h"

// #define convert_littleendian_u32(a,b,c,d) ((uint32_t)a << 24) || ((uint32_t)b << 16) || ((uint32_t)c << 8) || ((uint32_t)d)

// 1차 만들기
void poly_gendim1(invpoly *p, graph *g, int depend, uint8_t *vlist, uint32_t *value) {
    // v <- g;                             // v 만들기
    // p->d1[n].v.coeffs <- Zp             // c <- Zp
    // sum += p->d1[n].v.coeffs            // sum 반환하기 위해서 호출될 때마다 더해주기.
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
    // coeffs = randomint() % Q;
    coeffs = randomint() % Q;

    if ( depend ) {
        while(1) {
            int re = -1;
            for ( int i=0; i<VERTEXES; i++ ) {
                if ( vnum == vlist[i] ) // 이웃도 vlist에 담아놔야함
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
    // lastindex+=4;
    value[0] += coeffs;
    value[0] %= Q;


}

void poly_geninvariant(invpoly *p, graph *g, int depend, uint8_t *vlist, uint32_t *value, int k, uint32_t *m, int maxk) {

    if ( k == 1 ) {
        uint8_t numv;
        randombytes(&numv, 1);
        if ( depend == DEPEND ) { // k = 1 일 때도 vnum 만큼 뽑는 동안 한 번만 차수 1 증가를 보장하면 됨
            uint8_t s; // 밑의 s랑 동일
            randombytes(&s, 1);
            numv =  (numv % 3) + 1; // 항의 개수를 결정 (1개이상)
            s %= numv; // Depend항을 결정 s <- {0, 1, 2, 3}

            for ( int i=0; i<numv; i++ ) {
                if ( i == s )
                    poly_gendim1(p, g, DEPEND, vlist, value); // 제외정점 항을 고려하여 선택
                else
                    poly_gendim1(p, g, NOTDEPEND, vlist, value); // 제외정점 항을 고려하지 않고 선택
            }
        }
        else {
            numv = numv % 4; //항의 개수를 결정 (0개이상)  numv <- {0, 1, 2, 3}
            for ( int i=0; i<numv; i++ )
                poly_gendim1(p, g, NOTDEPEND, vlist, value);
        }

    }
    // if ( k == K ) {
    //     depend = DEPEND;
    // }
    if ( k > 1 ) {
        uint32_t mp;
        mp = randomint() % Q;

        uint8_t vnum;
        while (1) {
            randombytes(&vnum, 1);
            if ( depend == DEPEND ) {
                for ( int i=0; i<VERTEXES; i++ ) { // v의 모든 정점을 찾아보면서
                    if ( vnum == vlist[i] ) // 이전에 뽑았던 것이면 다시
                        continue;
                }
                for ( int i=0; i<4; i++ ) {
                    vlist[4*K-(4*k)+i] = g->v[vnum].neig[i]; // vlist에 DEPEND추가
                }
            }
            break;                          // 아니면 통과, // NOTDEPEND이 면 그냥 통과
        }
        uint32_t a[4] = { 0, };
        uint8_t s; // s 는 k가 올라갈 때마다 어디를 DEPEND로 할건지 결정
        // s = 0;
        randombytes(&s, 1);
        s = s % 4; // Depend 항 결정 {0, 1, 2, 3}
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
            if (k == maxk) { 
                mp = m[0];
            }
            a[i] = (value[0]+Q-mp) % Q;
            // printf(" a = %d - %d = %d\n", mp, value[0], a[i]); //
            value[0] = mp;
            for ( int j=0; j<TERM; j++ ) {
                if ( p->d[j].coeffs == 0 ) {
                    p->d[j].coeffs = Q - a[i];
                    for ( int l=0; l<=k-1; l++ ) {
                        p->d[j].v[l] = g->v[vnum].neig[i]; // 정점 차수 증가 (항 a * x_v)
                    }
                    break;
                }
                else if ( (p->d[j].v[k-1]) == 0 )
                    p->d[j].v[k-1] = g->v[vnum].neig[i]; // 나머지 정점 차수 증가
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
    
    // for ( int i=0; i<=length; i++ ) {
    //     printf(" %10d | ", p->d[i].coeffs);
    //     for ( int j=0; j<dimk; j++ ) {
    //         if (p->d[i].v[j])
    //             printf("%3d ", p->d[i].v[j]);
    //     }
    //     printf("\n");
    // }
}