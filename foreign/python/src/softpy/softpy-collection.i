/* -*- C -*-  (not really, but good for syntax highlighting) */

/*
 * C code
 */
%{
#include "softc-collection.h"
%}


/*
 * SWIG declarations
 */

softc_collection_s *softc_collection_create_new();
softc_collection_s *softc_collection_create(const char *id);
softc_collection_s *softc_collection_create_ext(const char *name,
						const char *version);
void softc_collection_free(softc_collection_s *self);

softc_collection_properties_s * const softc_collection_props(softc_collection_s *self);

softc_collection_dimensions_s * const softc_collection_dims(softc_collection_s *self);

softc_collection_allocatable_s * softc_collection_allocatables(softc_collection_s *self);

void softc_collection_register_entity(softc_collection_s *self,
				      const char *label,
				      softc_entity_t *entity);

size_t softc_collection_num_entities(softc_collection_s *self);

size_t softc_collection_num_relations(softc_collection_s *self);

softc_string_list_s *softc_collection_find_relations(softc_collection_s *self,
						     const char *subject,
						     const char *predicate);

void softc_collection_get_name(softc_collection_s *self,
			       softc_string_s name);

void softc_collection_set_name(softc_collection_s *self,
			       const char *name);

void softc_collection_set_version(softc_collection_s *self,
				  const char *version);

void softc_collection_get_version(softc_collection_s *self,
				  softc_string_s version);

void softc_collection_add_relation(softc_collection_s *self,
				   const char *subject,
				   const char *predicate,
				   const char *object);

void softc_collection_connect(softc_collection_s *self,
			      const char *subject,
			      const char *predicate,
			      const char *object);
