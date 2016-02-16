#ifndef SOFTC_STORAGE_H
#define SOFTC_STORAGE_H

#include "softc-macros.h"
#include "softc-storage-strategy.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_storage_t softc_storage_t;

softc_storage_t * softc_storage_create (const char *driver, const char *uri, const char *options);
void              softc_storage_free (softc_storage_t *);
void              softc_storage_load (softc_storage_t *, void *entity);
void              softc_storage_save (softc_storage_t *, void *entity);
softc_storage_strategy_t * softc_storage_get_storage_strategy(softc_storage_t *);
void softc_storage_free_storage_strategy(softc_storage_strategy_t *strategy);

/*void        softc_storage_load (softc_entity_t *);*/
/*void        softc_storage_save (softc_entity_t const *);*/

SOFTC_END_DECLS

#endif 
