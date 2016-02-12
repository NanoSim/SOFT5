#ifndef SOFTC_BLOCK_H_DEF
#define SOFTC_BLOCK_H_DEF

#include "softc-macros.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_block softc_block;

softc_block*  softc_block_alloc(size_t rank, size_t const dims[]);
void          softc_block_free(softc_block* block);
void          softc_block_free_zero(softc_block** block);
size_t        softc_block_rank(const softc_block *block);
void          softc_block_dims(const softc_block *block, size_t **dims, size_t *rank);
const double* softc_block_data(const softc_block *block);

SOFTC_END_DECLS

#endif /* SOFTC_BLOCK_H_DEF */
