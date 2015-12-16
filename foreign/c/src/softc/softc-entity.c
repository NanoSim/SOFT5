#include "softc-entity.h"

struct _softc_entity_t
{
  const struct softc_entity_vtable_ *vtable_;
  char *id;  
};

void softc_entity_store (const void *ptr, softc_datamodel_t *data_model)
{
  const softc_entity_t *self = (const softc_entity_t *)ptr;
  self->vtable_->store (self, data_model);
}

void softc_entity_load (void *ptr, const softc_datamodel_t *data_model)
{
  softc_entity_t *self = (softc_entity_t *)ptr;
  self->vtable_->load (self, data_model);
}

const char *
softc_entity_get_id (const void *ptr)
{
  const softc_entity_t *self = (const softc_entity_t*)ptr;
  return self->id;
}

void *
softc_entity_new (const char *uri)
{
  SOFT_UNUSED(uri)
  return NULL;
}

const char * softc_entity_get_meta_type(const void *ptr)
{
  const softc_entity_t *self = (const softc_entity_t*)ptr;
  return self->vtable_->get_meta_type();
}

const char * softc_entity_get_meta_name(const void *ptr)
{
  const softc_entity_t *self = (const softc_entity_t*)ptr;
  return self->vtable_->get_meta_name();
}

const char * softc_entity_get_meta_namespace(const void *ptr)
{
  const softc_entity_t *self = (const softc_entity_t*)ptr;
  return self->vtable_->get_meta_namespace();
}

const char * softc_entity_get_meta_version(const void *ptr)
{
  const softc_entity_t *self = (const softc_entity_t*)ptr;
  return self->vtable_->get_meta_version();
}

const char ** softc_entity_get_dimensions (const void *ptr, size_t *size)
{
  const softc_entity_t *self = (const softc_entity_t*)ptr;
  return self->vtable_->get_dimensions(self, size);
}

int softc_entity_get_dimension_size (const void *ptr, const char *label)
{
  const softc_entity_t *self = (const softc_entity_t*)ptr;
  return self->vtable_->get_dimension_size(self, label);
}
