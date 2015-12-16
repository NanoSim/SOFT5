#ifndef SOFTC_ENTITY_H
#define SOFTC_ENTITY_H

#include "softc-macros.h"
#include "softc-storage.h"
#include "softc-datamodel.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_entity_t softc_entity_t;
typedef struct softc_entity_vtable_
{
  void (*store)(const softc_entity_t *self, softc_datamodel_t *data_model);
  void (*load) (softc_entity_t *self, const softc_datamodel_t *data_model);
  const char **(*get_dimensions)(const softc_entity_t *self, size_t *size);
  int (*get_dimension_size)(const softc_entity_t *self, const char *label);
 
  const char * (*get_meta_type)();
  const char * (*get_meta_name)();
  const char * (*get_meta_namespace)();
  const char * (*get_meta_version)();  
} softc_entity_vtable;


/* Helper macro - define this in the ctor of the dervied entity types.*/
#define SOFTC_ENTITY_VTABLE_NAME(name) \
  name ## _vtable_

#define SOFTC_ENTITY_VTABLE(name) \
  static const softc_entity_vtable SOFTC_ENTITY_VTABLE_NAME(name)[] = { \
    {store, load, get_dimensions, get_dimension_size, get_meta_type, get_meta_name, get_meta_namespace, get_meta_version}}

    

const char *  softc_entity_get_id (const void *self);
void *        softc_entity_new (const char *uri);
const char *  softc_entity_get_meta_type(const void *self);
const char *  softc_entity_get_meta_name(const void *self);
const char *  softc_entity_get_meta_namespace(const void *self);
const char *  softc_entity_get_meta_version(const void *self);  
const char ** softc_entity_get_dimensions (const void *self, size_t *size);
int           softc_entity_get_dimension_size (const void *self, const char *label);
void          softc_entity_store  (const void *self, softc_datamodel_t *data_model);
void          softc_entity_load   (void *self, const softc_datamodel_t *data_model);

SOFTC_END_DECLS

#endif 
