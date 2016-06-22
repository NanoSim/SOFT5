#include <list>
#include <map>
#include "idatamodel.h"
#include "collection.h"
#include "tripletstore.h"

SOFT_BEGIN_NAMESPACE

#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

// TODO:
// * How do we check for correctness after a load?
// * How do we specify the Collection schema? We have a name/version field, but
//   is this suficient? Probably not!
// * Dimension maps using triples should be implemented and tested
// * The private data class d is not freed properly, memory leak posibilities

class Collection :: Private
{
  friend class Collection;
  std::string name;
  std::string version;
  std::map<std::string, IEntity*> entityMap;

  TripletStore tripletStore;
};


Collection :: Collection()
  : IEntity()
  , d (new Collection::Private())
{}

Collection :: Collection(std::string const &id)
  : IEntity(id)
  , d(new Collection::Private())
{
}

Collection :: Collection(const IEntity *ptr)
  : IEntity(ptr)
  , d (new Collection::Private())
{
}

// Instanciate a collection from a data model
Collection :: Collection(IDataModel const *dm)
  : IEntity()
  , d(new Collection::Private())
{
  load(dm);
}

Collection :: ~Collection()
{
  delete d;
}

IEntity *Collection :: create (std::string const &uuid)
{
  if (uuid.empty()) {
    return dynamic_cast<IEntity*>(new Collection());
  }
  Collection *collection = new Collection(uuid);
  return dynamic_cast<IEntity*>(collection);
}

std::string Collection :: name() const
{
  return d->name;
}

void Collection :: setName(std::string const &name)
{
  d->name = name;
}

std::string Collection :: version() const
{
  return d->version;
}

void Collection :: setVersion(std::string const &version)
{
  d->version = version;
}

void Collection :: registerEntity(std::string const &label, IEntity const *entity)
{
  this->addEntity(label,
          entity->metaName(),
          entity->metaVersion(),
          entity->metaNamespace(),
          entity->id());
}

// TODO: Remove one of these duplicates?
void Collection :: addEntity(std::string const &label,
         std::string const &name,
         std::string const &version,
         std::string const &ns,
         std::string const &uuid)
{
  addRelation(label, "is-a", "Entity");
  addRelation(label, "name", name);
  addRelation(label, "version", version);
  addRelation(label, "namespace", ns);
  addRelation(label, "id", uuid);
}

// Attaches the entity to this collection so that it is loaded and stored
// with the collection.
void Collection :: attachEntity(std::string const &label, IEntity *entity) {
  registerEntity(label, entity);
  d->entityMap.insert({label, entity});
}


int Collection :: numEntities() const
{
  return d->tripletStore.findTriplets("Entity", "^is-a").size();
}


void Collection :: addDim(std::string const &label,
              std::string const &description)
{
  NOT_IMPLEMENTED
}

void Collection :: addRelation(std::string const &subject,
                   std::string const &predicate,
                   std::string const &object)
{
  d->tripletStore.addTriplet(subject, predicate, object);
}

// TODO: Remove this duplicate here, replaced by addRelation
void Collection :: connect (std::string const &subject,
                std::string const &predicate,
                std::string const &object)
{
  addRelation(subject, predicate, object);
}

int Collection :: numRelations() const
{
  NOT_IMPLEMENTED
}

void Collection :: load (IDataModel const *dataModel)
{
  setId(dataModel->id());

  dataModel->getString("__name__", d->name);
  dataModel->getString("__version__", d->version);

  std::string csv;
  dataModel->getString("triplets", csv);
  d->tripletStore.fromCSV(csv);

  // Also attach entities that we have data models for
  auto labels = d->tripletStore.findTriplets("Entity", "^is-a");
  for (auto l: labels) {
    IDataModel *dm = dataModel->getModel(l.c_str());
    if (dm) {
      // Pass the datamodel on to attached entities
      auto ie = d->entityMap.find(l);
      if (ie != d->entityMap.end()) {
        IEntity *e = ie->second;
        ie->second->load(dm);
      }
    }
  }
}

void Collection :: save (IDataModel *dataModel) const
{
  dataModel->setId(id());

  dataModel->appendString("__name__", d->name);
  dataModel->appendString("__version__", d->version);

  dataModel->appendString("triplets", d->tripletStore.toCSV());

  // Also perform a save on all attached entities.
  for(auto &e: d->entityMap) {
    // Creates an empty clone of the same data model type
    // TODO: Who owns this data model now? Needs to be freed at some point?
    auto dm = dataModel->createModel();
    e.second->save(dm);
    dataModel->appendModel(e.first.c_str(), dm);
  }
}

IEntity const *Collection :: findInstance(std::string const &label) const
{
  const auto it = d->entityMap.find(label);
  if (it != d->entityMap.cend()) {
    return (*it).second;
  }
  return nullptr;
}

std::list<RelationTriplet> Collection :: findRelations(std::string const &subject) const
{
  NOT_IMPLEMENTED
}


std::vector<std::string> Collection :: dimensions() const {
  NOT_IMPLEMENTED
}


SOFT_END_NAMESPACE
