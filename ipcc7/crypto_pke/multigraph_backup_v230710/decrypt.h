#ifndef DECRYPTION_H
#define DECRYPTION_H

#include "params.h"
#include "invpoly.h"

void get_pdsarr(uint8_t pdsarr[VERTEXES], const uint8_t *pds);
int decrypt(uint32_t *m, invpoly *ct, uint8_t pdsarr[VERTEXES]);
#endif