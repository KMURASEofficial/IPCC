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

#include "encrypt.h"
#include "rng.h"
#include "invpoly.h"
#include <stdlib.h>
#include <stdint.h>

void get_neig(graph *g, const uint8_t (*edge)[2]) {
	
	uint8_t index[VERTEXES] = { 0, };

	for (int i = 0; i < VERTEXES; i++) {
		g->v[i].neig[0] = i;
		index[i]++;
	}
	
	for (int i = 0; i < EDGES; i++) {
		uint8_t a = edge[i][0] % VERTEXES;
		uint8_t b = edge[i][1] % VERTEXES;

		g->v[a].neig[index[a]] = b; 
		index[a]++;
		g->v[b].neig[index[b]] = a; 
		index[b]++;
	}
}

void reduce_sort(invpoly *res, int dimk) {

	int temp;
	for (int i=0; i<TERM; i++) {
		if ( !res->d[i].coeffs ) break;
		for (int j=0; j<dimk; j++) {
			for (int k=0; k<dimk-1; k++) {   
				if (res->d[i].v[k] > res->d[i].v[k+1] || res->d[i].v[k]==0) { 
					temp = res->d[i].v[k];
					res->d[i].v[k] = res->d[i].v[k+1];
					res->d[i].v[k+1] = temp;
				}
			}
		}
	}
}

void reduce_dim(invpoly *res, invpoly *p, int dimk) {

	int j, check;
	uint8_t offset;
	for ( int i=0; i<TERM; i++ ) {
		if ( !p->d[i].coeffs ) break; 
		res->d[i].coeffs=p->d[i].coeffs;
		res->d[i].v[0]=p->d[i].v[0];
		offset=1;
		for ( int j=1; j<dimk-1; j++ ) {
			for ( int k=j; k<dimk; k++ ) {
				if ( p->d[i].v[j] != p->d[i].v[k] ) {
					check=0;
					for ( int l=0; l<dimk; l++ ) {
						if ( res->d[i].v[l]==p->d[i].v[j] ) check=1;
					}
					if (check==0) res->d[i].v[offset++]=p->d[i].v[j];
					if (offset==dimk) break;
					check=0;
					for ( int l=0; l<dimk; l++ ) {
						if ( res->d[i].v[l]==p->d[i].v[k] ) check=1;
					}
					if (check==0) res->d[i].v[offset++]=p->d[i].v[k];
					if (offset==dimk) break;
				}
			}
		}
	}
}

int isneigd2(uint8_t v1, uint8_t v2, const graph *g) {

	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			if ( !(g->v[v1].neig[i] ^ g->v[v2].neig[j]) ) { 
				return 1;
			}
		}
	}
	return 0;
}

void reduce_neig(invpoly *res, const graph *g, int dimk, int *length) {

	for (int i=0; i<TERM; i++) {
		if ( !res->d[i].coeffs ) {
			*length=i-1; 
			break;
		}
		for (int j=0; j<dimk-1; j++) {
			if ( res->d[i].v[j]==0 ) break; 
			for (int k=j+1; k<dimk; k++) {
				if ( res->d[i].v[k]==0 ) break;
				if ( isneigd2(res->d[i].v[j], res->d[i].v[k], g)==1 ) {
					res->d[i].coeffs=0; 
					break;
				}
			}
			if (res->d[i].coeffs==0) break;
		}
	}
}

void reduce_dup(invpoly *res, invpoly *p, int dimk, int *length) {

	int dup=0;
	int dupcnt=0;
	for (int i=0; i<=*length-1; i++) { 
		if (p->d[i].coeffs==0) continue;
		for (int j=i+1; j<=*length; j++) { 
			if (p->d[j].coeffs==0) continue;
			dup=1;
			for (int k=0; k<dimk; k++) {
				if (p->d[i].v[k] != p->d[j].v[k]) {
					dup=0; 
					break;
				}
			}
			if (dup==1) {
				p->d[j].coeffs=0;
				dupcnt++;
			}
		}
	}

	int c=0;
	for (int i=0; i<=*length; i++) {
		if ( p->d[i].coeffs ) {
			res->d[c].coeffs = p->d[i].coeffs; 
			for (int j=0; j<dimk; j++)
				res->d[c].v[j]=p->d[i].v[j]; 
			c++;
		}
	}
	*length=c-1;
}

void poly_shuffle(invpoly *ct, int dimk, int *length) {

	uint32_t index;
	uint8_t temp;
	uint32_t coef_temp;
	for (int i=0; i<*length; i++) {
		index = randomint();
		index = (index % (*length+1)) % ((*length+1)-i) +i;
		for (int j=0; j<dimk; j++) {
			temp = ct->d[i].v[j];
			ct->d[i].v[j] = ct->d[index].v[j];
			ct->d[index].v[j] = temp;
		}
		coef_temp = ct->d[i].coeffs;
		ct->d[i].coeffs = ct->d[index].coeffs;
		ct->d[index].coeffs = coef_temp;
		
	}
}

int poly_mul(invpoly *p, invpoly *p1, invpoly *p2, int k1, int k2) {

	uint32_t c=0;
	for (int i=0; i<TERM; i++) {
		if ( !p1->d[i].coeffs ) break;
		for (int j=0; j<TERM; j++) {
			if ( !p2->d[j].coeffs ) break;
			p->d[c].coeffs = (uint32_t) ((uint64_t) p1->d[i].coeffs * (uint64_t) p2->d[j].coeffs % Q); // type casting
			for (int k=0; k<k1; k++)
				p->d[c].v[k] = p1->d[i].v[k];
			for (int k=0; k<k2; k++)
				p->d[c].v[k+k1] = p2->d[j].v[k];
			c++;
		}
	}
	return 1;
}
