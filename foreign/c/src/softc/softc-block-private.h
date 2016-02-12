#ifndef SOFTC_BLOCK_PRIVATE_H_DEF
#define SOFTC_BLOCK_PRIVATE_H_DEF

struct _softc_block
{
  size_t *dims;
  size_t rank;
  double *data;
};

#endif /* SOFTC_BLOCK_PRIVATE_H_DEF */
