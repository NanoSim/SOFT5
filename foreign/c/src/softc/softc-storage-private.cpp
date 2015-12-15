#include <cstdlib>
#include "storage.h"
#include "softc-storage-private.h"

struct _softc_storage_t
{
  void *ref;
};

softc_storage_t *softc_storage_private_create(const char *driver, const char *uri, const char *options)
{
  softc_storage_t *storage;
  storage = (softc_storage_t *)malloc(sizeof *storage);
  storage->ref = (void*) new soft::Storage(driver, uri, options);
}

void softc_storage_private_free(softc_storage_t *storage)
{
  soft::Storage *ptr = (soft::Storage *)storage->ref;
  delete ptr;
  storage->ref = nullptr;
  free (storage);
}
