#ifndef SOFTC_ENTITY_H
#define SOFTC_ENTITY_H

#include "softc-macros.h"
#include "softc-storage.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_entity_t softc_entity_t;
typedef struct softc_entity_vtable_
{
  void (*store)(softc_entity_t *self);
  void (*load) (softc_entity_t *self);
} softc_entity_vtable;

extern void          softc_entity_store  (void *self);
extern void          softc_entity_load   (void *self);
extern const char *  softc_entity_get_id (void *self);
extern void *        softc_entity_new (const char *uri);

SOFTC_END_DECLS

#endif 
