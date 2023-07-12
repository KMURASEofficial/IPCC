#ifndef KEYGEN_H
#define KEYGEN_H

#include "params.h"
#include "invpoly.h"


void keygen(uint8_t (*pk)[2], uint8_t *pds);
void shuffle(uint8_t *arr, int arr_len);
void add_graphedge(uint8_t (*edge)[2], int i, uint8_t v1, uint8_t v2);
void setpds(uint8_t (*pds), uint8_t g1pds[NUMPDS]);

#endif