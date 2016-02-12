#ifndef SOFTC_VECTOR2D_H_DEF
#define SOFTC_VECTOR2D_H_DEF

#include "softc-macros.h"

SOFTC_BEGIN_DECLS

typedef double   * softc_vector1d;
typedef double  ** softc_vector2d;
typedef double *** softc_vector3d;
typedef double**** softc_vector4d;

softc_vector1d     softc_vector1d_create(const size_t n);
softc_vector2d     softc_vector2d_create(const size_t ni, const size_t nj);
softc_vector3d     softc_vector3d_create(const size_t ni, const size_t nj, const size_t nk );
softc_vector4d     softc_vector4d_create(const size_t ni, const size_t nj, const size_t nk, const size_t nl);
void             softc_vector1d_destroy(softc_vector1d);
void             softc_vector2d_destroy(softc_vector2d);
void             softc_vector3d_destroy(softc_vector3d);
void             softc_vector4d_destroy(softc_vector4d);

void             softc_vector1d_info(softc_vector1d, size_t **dims, size_t *rank);
void             softc_vector2d_info(softc_vector2d, size_t **dims, size_t *rank);

SOFTC_END_DECLS

#endif /*SOFTC_VECTOR2D_H_DEF*/
