/*  -*- Mode: C -*- */

%define DOCSTRING
"Python bindings to SOFT5."
%enddef

%module(docstring=DOCSTRING) softpy

%feature("autodoc","0");
%feature("keyword");
%include "typemaps.i"


/**********************************************
 ** C code included in the wrapper
 **********************************************/
%{
 #include "softc.h"

  typedef struct {
    softc_entity_t *ref;
  } Entity;

  typedef struct {
    softc_collection_t *ref;
  } Collection;
 
%}



/**********************************************
 ** Entity
 **********************************************/
typedef struct {
  %immutable;
  softc_entity_t *ref;
  %mutable;
} Entity;

%extend Entity {
  Entity(const char *uri=NULL) {
    Entity *e = calloc(1, sizeof(Entity));
    e->ref = softc_entity_new(uri);
    return e;
  }
  char *__repr__() {
    static char buff[100];
    const char *p = ($self->ref) ? softc_entity_get_id($self) : "None";
    snprintf(buff, sizeof(buff), "Entity(%s)", p);
    return buff;
  }
  void store() {
    softc_entity_store($self->ref);
  }
  void load() {
    softc_entity_load($self->ref);
  }
}


/**********************************************
 ** Collection
 **********************************************/
typedef struct {
  %immutable;
  softc_collection_t *ref;
  %mutable;
} Collection;

%extend Collection {
  Collection(void) {
    Collection *c = calloc(1, sizeof(Collection));
    c->ref = softc_collection_new();
    return c;
  }
  ~Collection(void) {
    if ($self->ref) softc_collection_delete(self->ref);
  }
  //char *__repr__() {
  //  static char buff[100];
  //  const char *p = ($self->ref) ? softc_entity_get_id($self) : "None";
  //  snprintf(buff, sizeof(buff), "Entity(%s)", p);
  //  return buff;
  //}
}
