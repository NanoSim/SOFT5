#include "softc-storage.h"
#include "softc-storage-private.h"

softc_storage_t * softc_storage_create (const char *driver, const char *uri, const char *options)
{
  softc_storage_t *ret;
  ret = softc_storage_private_create(driver, uri, options);
  return ret;
}

void softc_storage_free (softc_storage_t *storage)
{
  softc_storage_private_free(storage);
}

/*
void softc_storage_load (softc_entity_t *entity)
{
  softc_storage_private_load (entity);
}

void softc_storage_save (softc_entity_t const *entity)
{
  softc_storage_private_save (entity);
}
*/
