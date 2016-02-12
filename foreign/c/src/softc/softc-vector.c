#include <stdint.h>
#include "softc-block.h"
#include "softc-block-private.h"
#include "softc-vector.h"

softc_block* vector_to_block(void *ptr)
{
  void *v = (void*)ptr - (sizeof(uintptr_t));
  uintptr_t addr = *(uintptr_t*)v;
  softc_block *block = (softc_block*)addr;
  return block;
}

void block_to_vector1d(const softc_block *block, softc_vector1d *dest)
{
  size_t dims[1] = {0};
  size_t rank;
  softc_block_dims(block, (size_t**)&dims, &rank);
  assert(rank == 1 && dims[0] > 0);
  *dest = (softc_vector1d)softc_block_data(block);
}

void softc_vector1d_info(softc_vector1d vector1d, size_t **dims, size_t *rank)
{
  softc_block *block = vector_to_block(&vector1d[0]);
  softc_block_dims(block, dims, rank);
}

void softc_vector2d_info(softc_vector2d vector2d, size_t **dims, size_t *rank)
{
  softc_block *block = vector_to_block(&vector2d[0][0]);
  softc_block_dims(block, dims, rank);
}

void block_to_vector2d(const softc_block *block, softc_vector2d *dest)
{
  size_t *dims;
  size_t rank;
  softc_block_dims(block, &dims, &rank);
  assert(rank == 2 && dims[0] > 0 && dims[1] > 0);

  const size_t NI=dims[0];
  const size_t NJ=dims[1]; 
  size_t j;
  
  *dest = malloc(sizeof(double*) * NJ);
  for (j = 0; j < NJ; ++j) {
    (*dest)[j] = (double*)softc_block_data(block) + (j * NI);
  }
  free(dims);
}

softc_vector1d softc_vector1d_create(const size_t n)
{
  softc_vector1d vec;
  const size_t dims[1] = {n};
  softc_block* array = softc_block_alloc(1, dims);
  block_to_vector1d(array, &vec);
  return vec;
}

softc_vector2d softc_vector2d_create(const size_t ni, const size_t nj)
{
  softc_vector2d vec;
  const size_t dims[2] = {ni,nj};
  softc_block* array = softc_block_alloc(2, dims);
  block_to_vector2d(array, &vec);
  return vec;
}

softc_vector3d softc_vector3d_create(const size_t ni, const size_t nj, const size_t nk )
{
  return NULL;
}

softc_vector4d softc_vector4d_create(const size_t ni, const size_t nj, const size_t nk, const size_t nl)
{
  return NULL;
}

void softc_vector1d_destroy(softc_vector1d vec)
{
}

void softc_vector2d_destroy(softc_vector2d vec)
{
}

void softc_vector3d_destroy(softc_vector3d vec)
{
}

void softc_vector4d_destroy(softc_vector4d vec)
{
}

