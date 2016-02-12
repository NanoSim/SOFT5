
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "softc-block.h"
#include "softc-block-private.h"

/*!
 * Allocate a memory block with a given \param rank and \param dims.
 */
softc_block* softc_block_alloc(size_t rank, size_t const dims[])
{
  softc_block *ret = malloc(sizeof *ret);
  uintptr_t addr  = (uintptr_t)&ret[0];
  size_t size     = 1;
  size_t i        = 0;
  
  for (; i < rank; ++i) {
    size *= dims[i];
  }
  
  *ret = (softc_block){
    malloc(sizeof *(ret->dims) * rank),                    /*dims*/
    rank,                                                  /*rank*/
    malloc(sizeof(uintptr_t) + size * sizeof *(ret->data)) /*data*/
  };
 
  memcpy(ret->dims, dims, (sizeof *(ret->dims) * rank));
  memcpy(ret->data, &addr, sizeof(uintptr_t));
  ret->data = (void*)ret->data + sizeof(uintptr_t);	 

  return ret;
}

/*!
 * Frees softc_block memory
 */
void softc_block_free(softc_block* block)
{
  void *data_ptr;
  
  free(block->dims);
  block->dims = NULL;
  data_ptr = (void*)(block)->data - sizeof(uintptr_t);
  free(data_ptr);
  block->data = NULL;
  free(block);
}

/*!
 * Frees softc_block memory and sets pointer to NULL
 */
void softc_block_free_zero(softc_block** block)
{
  void *data_ptr;
  
  free((*block)->dims);
  data_ptr = (void*)(*block)->data - sizeof(uintptr_t);
  free(data_ptr);
  free(*block);
  *block = NULL;
}

/*!
 * Returns the rank of a given \parameter block
 */
size_t softc_block_rank(const softc_block *block)
{
  return block->rank;
}

/*!
 * Fetch the dimensions and rank of a given \parameter block Note that
 * the \parameter dims needs to be freed after calling this function
 */
void softc_block_dims(const softc_block *block, size_t **dims, size_t *rank)
{
  (*dims) = malloc(block->rank);
  memcpy ((*dims), block->dims, sizeof(size_t)*block->rank);
  *rank = block->rank;
}

/*!
 * Returns the data element from a \parameter block
 */
const double* softc_block_data(const softc_block *block)
{
  return (const double*) block->data;
}
