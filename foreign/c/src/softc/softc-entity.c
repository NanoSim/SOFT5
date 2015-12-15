#include "softc-entity.h"

struct _softc_entity_t
{
  const struct softc_entity_vtable_ *vtable_;
  char *id;
};

void softc_entity_store (void *ptr, softc_datamodel_t *data_model)
{
  softc_entity_t *self = (softc_entity_t *)ptr;
  self->vtable_->store (self, data_model);
}

void softc_entity_load (void *ptr, softc_datamodel_t *data_model)
{
  softc_entity_t *self = (softc_entity_t *)ptr;
  self->vtable_->load (self, data_model);
}

const char *
softc_entity_get_id (void *ptr)
{
  softc_entity_t *self = (softc_entity_t*)ptr;
  return self->id;
}

void *
softc_entity_new (const char *uri)
{
  SOFT_UNUSED(uri)
  return NULL;
}
