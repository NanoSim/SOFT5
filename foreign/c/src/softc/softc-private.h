#ifndef SOFTC_PRIVATE_H_DEF
#define SOFTC_PRIVATE_H_DEF

#ifdef __cplusplus
#  define EXTERNAL extern "C" 
#else
#  define EXTERNAL
#endif

struct _softc_t
{
  void *app;
};

EXTERNAL struct _softc_t *softc_private_init(int argc, char *argv[]);
#endif /* SOFTC_PRIVATE_H_DEF */
