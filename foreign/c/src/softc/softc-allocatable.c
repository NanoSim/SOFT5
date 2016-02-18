#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include "softc-block.h"
#include "softc-block-private.h"
#include "softc-allocatable.h"

#define MAX_RANK 4

struct _softc_allocatable_s
{
  softc_block_s *block;
  void          *data;
};

static softc_allocatable_s* alloc_rank1(softc_allocatable_s* alloc)
{
  alloc->data = (void*)softc_block_data(alloc->block);
  return alloc;
}

static softc_allocatable_s* alloc_rank2(softc_allocatable_s* alloc)
{
  const size_t *dims = softc_block_dims(alloc->block);
  const size_t nj = dims[1];
  const size_t ni = dims[0];
  size_t j;
  double **vec = malloc(sizeof(*vec) * nj);  
  for (j = 0; j < nj; ++j) {
    vec[j] = (double*)alloc->block->data + (j*ni);
  }
  alloc->data = (void*)vec;
  return alloc;
}

static softc_allocatable_s* alloc_rank3(softc_allocatable_s* alloc)
{
  const size_t nk = alloc->block->dims[2];
  const size_t nj = alloc->block->dims[1];
  const size_t ni = alloc->block->dims[0];
  size_t j, k;
  double ***vec = malloc(sizeof(*vec) * nk);
  for (k = 0; k < nk; ++k) {
    vec[k] = malloc(sizeof(**vec) * nj);
    for (j = 0; j < nj; ++j) {
      vec[k][j] = (double*)alloc->block->data + (j*ni) + (k*ni*nj);
    }
  }
  alloc->data = (void*)vec;
  return alloc;
}

static softc_allocatable_s* alloc_rank4(softc_allocatable_s* alloc)
{
  const size_t ni = alloc->block->dims[0];
  const size_t nj = alloc->block->dims[1];
  const size_t nk = alloc->block->dims[2];
  const size_t nl = alloc->block->dims[3];

  size_t j, k, l;
  double ****vec = malloc(sizeof(*vec) * nl);
  for (l = 0; l < nl; ++l) {
    vec[l] = malloc(sizeof(***vec) * nk);
    for (k = 0; k < nk; ++k) {
      vec[l][k] = malloc(sizeof(**vec) * nj);
      for (j = 0; j < nj; ++j) {
	vec[l][k][j] = (double*)alloc->block->data + (j*ni) + (k*ni*nj) + (l*ni*nj*nk);
      }
    }
  }
  alloc->data = (void*)vec;
  return alloc;
}

static void free_rank1(softc_allocatable_s* alloc)
{
  (void)(alloc);
}

static void free_rank2(softc_allocatable_s* alloc)
{
  double **vec = alloc->data;
  free(vec);
}

static void free_rank3(softc_allocatable_s* alloc)
{
  const size_t nk = alloc->block->dims[2];
  size_t k;
  double ***vec = alloc->data;
  for (k = 0; k < nk; ++k) {
    free(vec[k]);
  }
  free(vec);
}

static void free_rank4(softc_allocatable_s* alloc)
{
  const size_t nk = alloc->block->dims[2];
  const size_t nl = alloc->block->dims[3];
  size_t k, l;
  double ***vec = alloc->data;
  for (l = 0; l < nl; ++l) {
    for (k = 0; k < nk; ++k) {
      free(vec[l][k]);
    }
    free(vec[l]);
  }
  free(vec);
}

typedef softc_allocatable_s*(*alloc_fptr)(softc_allocatable_s*);
typedef void(*free_fptr)(softc_allocatable_s*);

static alloc_fptr alloc_fptrs[] = {
  alloc_rank1,
  alloc_rank2,
  alloc_rank3,
  alloc_rank4
};
static free_fptr free_fptrs[] = {
  free_rank1,
  free_rank2,
  free_rank3,
  free_rank4
};

softc_allocatable_s *softc_allocatable_create(size_t rank, const size_t dims[])
{
  assert(rank > 0 && rank <= MAX_RANK);

  softc_allocatable_s *allocatable = malloc(sizeof *allocatable);
  allocatable->block = softc_block_create_type(rank, dims, sizeof(double*));
  return alloc_fptrs[rank-1](allocatable);
}

softc_allocatable_s *softc_allocatable_createv(const size_t rank, ...)
{
  assert(rank > 0 && rank <= MAX_RANK);

  size_t i;
  va_list vl;
  va_start(vl, rank);
  size_t dims[MAX_RANK];
  for (i = 0; i < rank; i++) {
    dims[i] = va_arg(vl, const size_t);
  }
  va_end(vl);
  return softc_allocatable_create(rank, dims);
}

void softc_allocatable_free(softc_allocatable_s *self)
{
  free_fptrs[self->block->rank-1](self);
  softc_block_free(self->block);
  free(self);
}

void *softc_allocatable_data(softc_allocatable_s *self)
{
  return self->data;
}

void softc_allocatable_dimensions(const softc_allocatable_s *self, size_t *rank, size_t **dims)
{
  *rank = self->block->rank;
  *dims = self->block->dims;
}

void softc_allocatable_reshape(softc_allocatable_s *self, size_t rank, const size_t dims[])
{
  free_fptrs[self->block->rank-1](self);
  softc_block_reshape(self->block, rank, dims);
  alloc_fptrs[self->block->rank-1](self);
}
