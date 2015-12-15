#ifndef SOFTC_ENTITY_H
#define SOFTC_ENTITY_H

#include "softc-macros.h"
#include "softc-storage.h"
#include "softc-datamodel.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_entity_t softc_entity_t;
typedef struct softc_entity_vtable_
{
  void (*store)(softc_entity_t *self, softc_datamodel_t *data_model);
  void (*load) (softc_entity_t *self, softc_datamodel_t *data_model);
} softc_entity_vtable;

void          softc_entity_store  (void *self, softc_datamodel_t *data_model);
void          softc_entity_load   (void *self, softc_datamodel_t *data_model);
const char *  softc_entity_get_id (void *self);
void *        softc_entity_new (const char *uri);

SOFTC_END_DECLS

#endif 
