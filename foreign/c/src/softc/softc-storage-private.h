#ifndef SOFTC_STORAGE_PRIVATE_H_DEF
#define SOFTC_STORAGE_PRIVATE_H_DEF

struct _softc_storage_t
{
  char *driver;
  char *uri;
};

softc_storage_t *softc_storage_private_create(const char *driver, const char *uri)
{
  return 0;
}


void softc_storage_private_free(softc_storage_t *storage)
{
}

#endif
