#ifndef SOFTC_STORAGE_H
#define SOFTC_STORAGE_H

#include "softc-macros.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_storage_t softc_storage_t;

softc_storage_t * softc_storage_create (const char *driver, const char *uri);
void        softc_storage_free (softc_storage_t *);
/*void        softc_storage_load (softc_entity_t *);*/
/*void        softc_storage_save (softc_entity_t const *);*/

SOFTC_END_DECLS

#endif 
