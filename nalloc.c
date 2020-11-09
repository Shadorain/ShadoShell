#include "shadosh.h"

#define alignto(m, n)   ((m + n - 1) & ~(n - 1))
#define BLOCKSIZE ((size_t) 4096)

static struct Block {
	size_t used, size;
	char *mem;
	Block *n;
} *fl, *ul;

static void getblock(size_t n) {
	Block *r, *p;
	for (r = fl, p = NULL; r != NULL; p = r, r = r->n)
		if (n <= r->size)
			break;	/* look for a block which fits the request */
	if (r != NULL) {	/* if one is found, take it off the free list */	
		if (p != NULL)
			p->n = r->n;
		else
			fl = r->n;
	} else {		/* else allocate a new block */
		r = malloc(sizeof(Block));
		r->mem = malloc(r->size = alignto(n, BLOCKSIZE));
	}
	r->used = 0;
	r->n = ul;
	ul = r;
}

extern void *nalloc(size_t n) {
	size_t base;
	Block *ulp;
	n = alignto(n, sizeof(align_t));
	ulp = ul;
	if (ulp != NULL && n + (base = ulp->used) < ulp->size) {
		ulp->used = base + n;
		return &ulp->mem[base];
	} else {
		getblock(n);
		assert(ul->used == 0);
		(ulp = ul)->used = n;
		return &ulp->mem[0];
	}
}

#define MAXMEM 500000
extern void nfree() {
	size_t count;
	Block *r;
	if (ul == NULL)
		return;
	for (r = ul; r->n != NULL; r = r->n)
		;	/* get to end of used list */
	r->n = fl;	/* tack free list onto it */
	fl = ul;	/* and make it the free list */
	ul = NULL;	/* finally, zero out the used list */
	for (r = fl, count = r->size; r->n != NULL; r = r->n, count += r->size) {
		if (count >= MAXMEM) {
			Block *tmp = r;
			r = r->n;
			tmp->n = NULL;		/* terminate the free list */
			while (r != NULL) {	/* free memory off the tail of the free list */
				tmp = r->n;
				free(r->mem);
				free(r);
				r = tmp;
			}
		return;
		}
	}
}

extern Block *newblock() {
	Block *old = ul;
	ul = NULL;
	return old;
}

extern void restoreblock(Block *old) {
	nfree();
	ul = old;
}
