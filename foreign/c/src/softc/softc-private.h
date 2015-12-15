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
EXTERNAL int softc_private_registered_storage_driver_count();
EXTERNAL char **softc_private_get_storage_drivers();

#endif /* SOFTC_PRIVATE_H_DEF */
