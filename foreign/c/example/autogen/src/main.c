#include <stdio.h>
#include <softc/softc-entity.h>
#include <softc/softc-allocatable.h>
#include <fcalc.h>
#include "simple.h"

typedef struct 
{
  softc_allocatable_s *vec;
  softc_allocatable_s *matrix;
} test_allocatable_s;

int main()
{
  simple_s *simple = simple_create(10,20);
  double **mtx = simple_get_matrix(simple);
  test_allocatable_s *alloc = (test_allocatable_s*)simple_allocatables(simple);
  size_t i, j, ndims;
  for (j = 0; j < 19; ++j) {
    for (i = 0; i < 10; ++i) {
      mtx[j][i] = 42.0+(double)i + (double)j/100.0; 
    }
  }
  double *vec = simple_get_vec(simple);
  vec[0] = 0.0;
  vec[1] = 1.0;
  vec[2] = 2.0;

  double *mtx2 = softc_allocatable_raw_data(alloc->matrix);
  printf("mtx: %f, %f\n", mtx2[0], mtx2[1]);
  
  const char **dims = softc_entity_get_dimensions((softc_entity_t *)simple, &ndims);
  for (i = 0; i < ndims; ++i) {
    int n = softc_entity_get_dimension_size((softc_entity_t *)simple, dims[i]);
    printf("%s - %d\n", dims[i], n);
  }

  calc_fn(simple);
  
  simple_free(simple);  
  return 0;
}
