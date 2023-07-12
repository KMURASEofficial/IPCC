#ifndef KEYGEN_H
#define KEYGEN_H

#include "params.h"
#include "invpoly.h"



/*   Bob   */

// void keygen(uint8_t(*edge)[2], uint8_t *pds, uint8_t numVertex, uint8_t start_ver);
void keygen(uint8_t (*pk)[2], uint8_t *pds);

// void shuffle(type(*arr), type arr_len);
void shuffle(uint8_t *arr, int arr_len);

void add_graphedge(uint8_t (*edge)[2], int i, uint8_t v1, uint8_t v2);

// void set_pds(type(*PDS), type G1PDS[G1numPDS], type G2PDS[G2numPDS]);
void setpds(uint8_t (*pds), uint8_t g1pds[NUMPDS]);

#endif