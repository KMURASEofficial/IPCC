#ifndef ENCRYPT_H
#define ENCRYPT_H

#include "params.h"
#include "invpoly.h"

void get_neig(graph *g, const uint8_t (*edge)[2]);
void reduce_dim(invpoly *res, invpoly *p, int dimk);
void reduce_sort(invpoly *res, int dimk);
void reduce_neig(invpoly *res, const graph *g, int dimk, int *length);
void reduce_dup(invpoly *res, invpoly *p, int dimk, int *length);
int poly_mul(invpoly *p, invpoly *p1, invpoly *p2, int k1, int k2);
void poly_shuffle(invpoly *ct, int dimk, int *length);
#endif