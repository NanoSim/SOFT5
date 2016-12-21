#include "softc-collection-private.h"
#include <cassert>
#include "softc-entity.h"
#include <collection.h>
#include <memory>
#include <map>

struct softc_private_s {
  std::unique_ptr<soft::Collection> collection;
  std::map<std::string, softc_entity_t*> entityMap;
};

void *softc_collection_private_create(const char *id)
{
  softc_private_s *d = new softc_private_s();
  assert( d != nullptr);
  d->collection = std::unique_ptr<soft::Collection>(new soft::Collection(id));
  return (void*)d;
}

void softc_collection_private_register_entity(void *ref, const char *label, softc_entity_t *entity)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);

  d->entityMap.insert({std::string(label), entity});
  d->collection->addEntity(label, 
			   softc_entity_get_meta_name(entity),
			   softc_entity_get_meta_version(entity),
			   softc_entity_get_meta_namespace(entity),
			   softc_entity_get_id(entity));
}

void softc_collection_private_add_relation(void *ref
					   , const char *subject
					   , const char *predicate
					   , const char *object)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  d->collection->addRelation(subject, predicate, object);
}

void softc_collection_private_connect(void *ref, const char *subject, const char *predicate, const char *object)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  d->collection->connect(subject, predicate, object);
}

void softc_collection_private_get_name(void *ref, softc_string_s name)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  auto n = d->collection->name();
  softc_string_assign(name, n.c_str());
}				

void softc_collection_private_get_version(void *ref, softc_string_s version)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  auto n = d->collection->version();
  softc_string_assign(version, n.c_str());
}
	   
void softc_collection_private_set_name(void *ref, const char *name)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  d->collection->setName(name);
}

void softc_collection_private_set_version(void *ref, const char *version)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  d->collection->setVersion(version);
}

int softc_collection_private_num_relations(void *ref)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  return d->collection->numRelations();
}

int softc_collection_private_num_entities(void *ref)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  return d->collection->numEntities();
}

softc_string_s *softc_collection_private_find_relations(void *ref, const char *subject, const char *predicate)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  auto rs = d->collection->findRelations(subject, predicate);
  auto n = rs.size();
  softc_string_s *stringList = softc_string_createlist0(n);
  for(int i = 0; i < n; ++i) {
    auto v = rs.front();
    softc_string_assign(stringList[i], v.c_str());
    rs.pop_front();
  }
  return stringList;
}
