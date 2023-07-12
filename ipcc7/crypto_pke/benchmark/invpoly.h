#ifndef INVPOLY_H
#define INVPOLY_H

#include <stdint.h>
#include "params.h"

typedef struct {
    uint8_t v[K];           // K-dim vertexes
    uint32_t coeffs;
} poly_d;

typedef struct  {           // 최대 항의 개수에 결부됨
    poly_d d[TERM];
} invpoly;

typedef struct {
    uint8_t neig[NEIGHBORHOODS];        // 이웃 정보 저장
} neighborhood;

typedef struct {
    neighborhood v[VERTEXES];    // structure of public key graph
} graph;

void poly_gendim1(invpoly *p, graph *g, int depend, uint8_t *vlist, uint32_t *value);
void poly_geninvariant(invpoly *p, graph *g, int depend, uint8_t *vlist, uint32_t *value, int k, uint32_t *m, int maxk);

void vlist_print(char *msg, uint8_t vlist[VERTEXES]);
void poly_print(const char *msg, invpoly *p, int dimk, int length);
void poly_print_tex(const char *msg, invpoly *p, int dimk, int length);
void poly_print_debug(const char *msg, invpoly *p, int dimk, int length);

#endif