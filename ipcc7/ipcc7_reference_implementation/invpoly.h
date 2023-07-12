#ifndef INVPOLY_H
#define INVPOLY_H

#include <stdint.h>
#include "params.h"

typedef struct {
    uint8_t v[K];          
    uint32_t coeffs;
} poly_d;

typedef struct  {          
    poly_d d[TERM];
} invpoly;

typedef struct {
    uint8_t neig[NEIGHBORHOODS];      
} neighborhood;

typedef struct {
    neighborhood v[VERTEXES];    
} graph;

void poly_gendim1(invpoly *p, graph *g, int depend, uint8_t *vlist, uint32_t *value);
void poly_geninvariant(invpoly *p, graph *g, int depend, uint8_t *vlist, uint32_t *value, int k, uint32_t *m, int maxk);

void vlist_print(char *msg, uint8_t vlist[VERTEXES]);
void poly_print(const char *msg, invpoly *p, int dimk, int length);
void poly_print_tex(const char *msg, invpoly *p, int dimk, int length);
void poly_print_debug(const char *msg, invpoly *p, int dimk, int length);

#endif