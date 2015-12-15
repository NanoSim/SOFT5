#ifndef SOFTC_STORAGE_PRIVATE_H_DEF
#define SOFTC_STORAGE_PRIVATE_H_DEF

#include "softc-storage.h"
#include "softc-macros.h"

SOFTC_BEGIN_DECLS

softc_storage_t *softc_storage_private_create(const char *driver, const char *uri, const char *options);
void softc_storage_private_free(softc_storage_t *storage);


SOFTC_END_DECLS
#endif
