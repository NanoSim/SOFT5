#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "softc-allocatable.h"

#define MAX_RANK 3
#define UNUSED(arg) (void)arg;

typedef struct {
  size_t rank;
  size_t *dimensions;
  double *data;
} block_s;

typedef struct {
  size_t rank;
  size_t *dimensions;
  int32_t *data;
} blocki32_s;

void *alloc_block_rank0 (const block_s*);
void *alloc_block_rank1 (const block_s*);
void *alloc_block_rank2 (const block_s*);
void *alloc_block_rank3 (const block_s*);
void free_block_rank0   (void *);
void free_block_rank1   (void *);
void free_block_rank2   (void *);
void free_block_rank3   (void *);
void free_reference_counted_data(double **data);

typedef void*(*alloc_block_fn)(const block_s*);
typedef void(*free_block_fn)(void*);

static alloc_block_fn alloc_block_fptrs[] = {
  alloc_block_rank0,
  alloc_block_rank1,
  alloc_block_rank2,
  alloc_block_rank3
};

static free_block_fn free_block_fptrs[] = {
  free_block_rank0,
  free_block_rank1,
  free_block_rank2,
  free_block_rank3
};

void *alloc_block_rank0 (const block_s* block)
{
  fprintf(stderr, "rank 0 is undefined\n");
  UNUSED(block);
  return NULL;
}

void *alloc_block_rank1 (const block_s* block)
{
  const uintptr_t n = (uintptr_t)&block[0];
  void * ptr        = malloc(sizeof(uintptr_t) + sizeof(double));  
  double *vec       = (ptr + sizeof (uintptr_t));

  memcpy (ptr, &n, sizeof(uintptr_t));
  vec = block->data;
  return (void*)vec;
}

void *alloc_block_rank2 (const block_s* block)
{
  const size_t ni   = block->dimensions[0];
  const size_t nj   = block->dimensions[1];
  const uintptr_t n = (uintptr_t)&block[0];
  size_t j          = 0;
  void *ptr         = malloc(sizeof(uintptr_t) + sizeof(double*)*nj);  
  double **vec      = (ptr + sizeof (uintptr_t));

  memcpy (ptr, &n, sizeof(uintptr_t));
  
  for (j = 0; j < nj; ++j) {
    vec[j] = block->data + (j * ni);
  }

  return (void*)vec;
}

void *alloc_block_rank3 (const block_s* block)
{
  const size_t ni   = block->dimensions[0];
  const size_t nj   = block->dimensions[1];
  const size_t nk   = block->dimensions[2];
  const uintptr_t n = (uintptr_t)&block[0];
  size_t j          = 0;
  size_t k          = 0;
  void *ptr         = malloc(sizeof(uintptr_t) + sizeof(double**)*nk);  
  double ***vec     = (ptr + sizeof (uintptr_t));

  memcpy (ptr, &n, sizeof(uintptr_t));

  for (k = 0; k < nk; ++k) {
    vec[k] = malloc(sizeof(double*)*nj);
    for (j = 0; j < nj; ++j) {
      vec[k][j] = block->data + (j*ni) + (k*nj*ni);
    }
  }
  
  return (void*)vec;
}

void free_block_rank0   (void *ref)
{
  UNUSED(ref);
}

void free_block_rank1   (void *ref)
{
  block_s *alloc = (block_s *)(*(uintptr_t*)(ref - sizeof(uintptr_t)));
  free_reference_counted_data(&alloc->data);
  void *ptr = ref - sizeof(uintptr_t); 
  free (ptr);
}

void free_block_rank2   (void *ref)
{
  block_s *alloc = (block_s *)(*(uintptr_t*)(ref - sizeof(uintptr_t)));
  void *ptr            = ref - sizeof(uintptr_t);
  
  free_reference_counted_data(&alloc->data);
  free (ptr); 
}

void free_block_rank3   (void *ref)
{
  block_s *alloc = (block_s *)(*(uintptr_t*)(ref - sizeof(uintptr_t)));
  size_t nk            = alloc->dimensions[2];
  size_t k;
  void *ptr            = ref - sizeof(uintptr_t);
  double ***vec        = (double***)ref;
  
  free_reference_counted_data(&alloc->data);
  for (k = 0; k < nk; ++k) {
    free(vec[k]);
  }
  free (ptr); 
}

/*
 * Decrement the reference counter one size_t ahead of the
 * vector-memory. When counter reaches 0, free the entire block
 */
void free_reference_counted_data(double **data)
{
  void *ref = (void*)*data;
  size_t *ref_count = (size_t*)(ref - sizeof(size_t));
  if (*ref_count > 0) {
    *ref_count -= 1;
  } else {
    free ((void*)ref_count);
    *data = NULL;
  }  
}

void *allocate_dims(block_s *alloc)
{
  void *ptr = 0;
  size_t dims = 1;
  size_t i;
  const size_t ref_counter = 0;

  assert(alloc->rank > 0 && alloc->rank <= MAX_RANK);
  
  for (i = 0; i < alloc->rank; ++i) {
    dims *= alloc->dimensions[i];
  }

  ptr = malloc(sizeof(size_t) + sizeof(double)*dims);
  memcpy(ptr, &ref_counter, sizeof(size_t));
  alloc->data = (ptr + sizeof(size_t));
  return alloc_block_fptrs[alloc->rank](alloc);
}

/*
 * Allocate a multidimensional (row-major order) array represented
 * internally as a rank one array for iso_c_binding interoperability.
 * \sa allocatev
 */
void *softc_allocatable_allocate(const size_t *dimensions, const size_t rank)
{
  block_s *a = malloc(sizeof *a);
  a->rank = rank;
  a->dimensions = malloc(sizeof (*a->dimensions) * rank);
  memcpy(a->dimensions, dimensions, sizeof (*a->dimensions) * rank);
  return allocate_dims(a);
}

/*
 * Allocate a multidimensional (row-major order) array represented
 * internally as a rank one array for iso_c_binding interoperability.
 * \sa allocate
 */
void *softc_allocatable_allocatev(const size_t ndims, ...)
{
  size_t i;
  va_list vl;
  block_s *a;
  size_t d;
  a = malloc(sizeof *a);
  va_start(vl, ndims);
  a->rank = ndims;
  a->dimensions = malloc(sizeof (*a->dimensions)*ndims);
  for (i = 0; i < ndims; i++) {
    d = va_arg(vl, const size_t);
    a->dimensions[i] = d;
  }

  va_end(vl);
  return allocate_dims(a);
}

/*
 * Reshapes a block to a new rank/dimension. The total number of array items
 * must match the source. The original data is reference counted.
 */
void *softc_allocatable_reshape(const void *source, const size_t *dims, const size_t rank)
{
  block_s *copy = malloc(sizeof *copy);
  size_t *ref_counter;
  const block_s *alloc = (const block_s *)(*(uintptr_t*)(source - sizeof(uintptr_t)));
  copy->rank = rank;
  copy->dimensions = malloc(sizeof (*copy->dimensions)*rank);
  memcpy(copy->dimensions, dims, sizeof (*copy->dimensions)*rank);

  ref_counter = (size_t*)((void*)alloc->data - sizeof(size_t));
  *ref_counter += 1;

  copy->data = alloc->data;
  return alloc_block_fptrs[copy->rank](copy);
}


/*
 * Reshapes a block to a new rank/dimension. The total number of array items
 * must match the source. The original data is reference counted.
 */
void *softc_allocatable_reshapev(const void *source, const size_t rank, ...)
{
  size_t i;
  va_list vl;
  block_s *copy = malloc(sizeof *copy);
  size_t *ref_counter;
  const block_s *alloc = (const block_s *)(*(uintptr_t*)(source - sizeof(uintptr_t)));
    
  va_start(vl, rank);
  copy->rank = rank;
  copy->dimensions = malloc(sizeof (*copy->dimensions)*rank);
  for (i = 0; i < rank; i++) {   
    copy->dimensions[i] = va_arg(vl, const size_t);
  }
  va_end(vl);

  ref_counter = (size_t*)((void*)alloc->data - sizeof(size_t));
  *ref_counter += 1;

  copy->data = alloc->data;
  return alloc_block_fptrs[copy->rank](copy);
}

/*
 * Create a shallow copy reference of an allocatable. The original
 * data is reference counted.
 */
void* softc_allocatable_shallow_copy(const void *ref)
{
  const block_s *alloc = (const block_s *)(*(uintptr_t*)(ref - sizeof(uintptr_t)));
  
  block_s *copy = malloc(sizeof *copy);
  size_t *ref_counter;
  copy->rank = alloc->rank;
  copy->dimensions = malloc(sizeof (*copy->dimensions) * copy->rank);
  memcpy(copy->dimensions, alloc->dimensions, sizeof (*copy->dimensions) * copy->rank);

  ref_counter = (size_t*)((void*)alloc->data - sizeof(size_t));
  *ref_counter += 1;
  copy->data = alloc->data;
  return alloc_block_fptrs[copy->rank](copy);
}

/*
 * Free the memory of an allocatable
 */
void softc_allocatable_free(void* ref)
{
  block_s *a = (block_s *)(*(uintptr_t*)(ref - sizeof(uintptr_t)));
  free_block_fptrs[a->rank](ref);
  if (a->dimensions != NULL) free(a->dimensions);  
  free (a);
}

/*
 * Display properties of the allocatable reference.
 */
void  softc_allocatable_info (const void *ref)
{
  size_t r;
  const block_s *a = (const block_s *)(*(uintptr_t*)(ref - sizeof(uintptr_t)));
  printf("rank: %zu, ", a->rank);
  printf("dims: [");
  for (r = 0; r < a->rank; ++r) {
    if (r > 0) (printf(", "));
    printf("%zu", a->dimensions[r]);
  }
  printf("]\n");
}

size_t softc_allocatable_rank (const void *source)
{
  const block_s *a = (const block_s *)(*(uintptr_t*)(source - sizeof(uintptr_t)));
  return a->rank;
}

void softc_allocatable_dimensions (const void *source, size_t **dims, size_t *rank)
{
  const block_s *a = (const block_s *)(*(uintptr_t*)(source - sizeof(uintptr_t)));
  (*dims) = malloc(a->rank);
  memcpy ((*dims), a->dimensions, sizeof(size_t)*a->rank);
  *rank = a->rank;
}
