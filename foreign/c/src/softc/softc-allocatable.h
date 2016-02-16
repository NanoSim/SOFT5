#ifndef SOFT_ALLOCATABLE_H_DEF
#define SOFT_ALLOCATABLE_H_DEF

#include <stddef.h>
#include "softc-macros.h"

SOFTC_BEGIN_DECLS

void  *softc_allocatable_allocate(const size_t *dimensions, const size_t rank);
void  *softc_allocatable_allocatev(const size_t ndims, ...);
void  *softc_allocatable_reshape (const void *source, const size_t *dimensions, const size_t rank);
void  *softc_allocatable_reshapev (const void *source, const size_t rank, ...);
void  *softc_allocatable_shallow_copy (const void *source);
void   softc_allocatable_info (const void *source);
size_t softc_allocatable_rank (const void *source);
void   softc_allocatable_dimensions (const void *source, size_t **dims, size_t *rank);
void   softc_allocatable_free(void *source);

SOFTC_END_DECLS

#endif /* SOFT_ALLOCATABLE_H_DEF */
