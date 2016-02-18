#ifndef SOFTC_ALLOCATABLE_H_DEF
#define SOFTC_ALLOCATABLE_H_DEF

#include <stddef.h>
#include "softc-macros.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_allocatable_s softc_allocatable_s;

softc_allocatable_s *softc_allocatable_create(size_t rank, const size_t dims[]);
softc_allocatable_s *softc_allocatable_createv(size_t rank, ...);
void                 softc_allocatable_free(softc_allocatable_s *);
void                *softc_allocatable_data(softc_allocatable_s *);
void                 softc_allocatable_dimensions(const softc_allocatable_s *, size_t *rank, size_t **dims);
void                 softc_allocatable_reshape(softc_allocatable_s *, size_t rank, const size_t dims[]);
SOFTC_END_DECLS

#endif /* SOFTC_ALLOCATABLE_H_DEF */
