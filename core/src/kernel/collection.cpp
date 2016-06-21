#include <list>
#include <map>
#include "idatamodel.h"
#include "collection.h"

SOFT_BEGIN_NAMESPACE

struct DimMap_
{
  DimMap_(std::string const l,
      std::string const e,
      std::string const c)
    : label(l)
    , entityDim(e)
    , collectionDim(c)
  {}
  std::string const label;
  std::string const entityDim;
  std::string const collectionDim;
};

struct Dim_
{
  Dim_(std::string const &l,
      std::string const &d)
    : label(l)
    , description(d)
  {}

  std::string const label;
  std::string const description;
};

class RelationTriplet :: Private
{
  friend class RelationTriplet;
  Private(std::string const &s,std::string const &p, std::string const &o)
    : subject(s)
    , predicate(p)
    , object(o)
  {}

  std::string const subject;
  std::string const predicate;
  std::string const object;
};

RelationTriplet :: RelationTriplet (std::string const &s,
                    std::string const &p,
                    std::string const &o)
  : d (new RelationTriplet :: Private (s, p, o))
{}

std::string RelationTriplet :: subject() const
{
  return d->subject;
}

std::string RelationTriplet :: predicate() const
{
  return d->predicate;
}

std::string RelationTriplet :: object() const
{
  return d->object;
}

/*
class RelationTriplet_
{
public:
  RelationTriplet_(std::string const &s,
          std::string const &p,
          std::string const &o)
    : subject(s)
    , predicate(p)
    , object(o)
  {}


private:
  friend class Collection;
  std::string const subject;
  std::string const predicate;
  std::string const object;
};
*/
struct EntityRef_
{
  EntityRef_(std::string const &l,
         std::string const &n,
         std::string const &v,
         std::string const &ns,
         std::string const &id)
    : label(l)
    , name(n)
    , version(v)
    , ns(ns)
    , uuid(id)
  {}

  std::string const label;
  std::string const name;
  std::string const version;
  std::string const ns;
  std::string const uuid;
};

class Collection :: Private
{
  friend class Collection;
  std::string name;
  std::string version;
  std::list<EntityRef> entityList;
  std::list<Dim> dimList;
  std::list<RelationTriplet>relationList;
  std::list<DimMap>dimMapList;
  std::map<std::string, const IEntity*> entityMap;
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

  // TODO: Discuss and find out, do we want exceptions or error codes?
  if (!checkCorrectness())
    throw std::runtime_error("Failed to construct Collection from entity");
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
  d->entityMap.insert({label, entity});
  this->addEntity(label,
          entity->metaName(),
          entity->metaVersion(),
          entity->metaNamespace(),
          entity->id());
}

void Collection :: addEntity(std::string const &label,
         std::string const &name,
         std::string const &version,
         std::string const &ns,
         std::string const &uuid)
{
  EntityRef ref(label, name, version, ns, uuid);
  d->entityList.push_back(ref);
}

int Collection :: numEntities() const
{
  return d->entityList.size();
}


void Collection :: addDim(std::string const &label,
              std::string const &description)
{
  Dim dim (label, description);
  d->dimList.push_back(dim);
}

int Collection :: numDims() const
{
  return d->dimList.size();
}

void Collection :: addRelation(std::string const &subject,
                   std::string const &predicate,
                   std::string const &object)
{
  RelationTriplet triplet(subject, predicate, object);
  d->relationList.push_back(triplet);
}

void Collection :: connect (std::string const &subject,
                std::string const &predicate,
                std::string const &object)
{
  addRelation(subject, predicate, object);
}

int Collection :: numRelations() const
{
  return d->relationList.size();
}

void Collection :: addDimMap(std::string const &label,
                 std::string const &entityDim,
                 std::string const &collectionDim)
{
  DimMap dimMap(label, entityDim, collectionDim);
  d->dimMapList.push_back(dimMap);
}

int Collection :: numDimMaps() const
{
  return d->dimMapList.size();
}

void Collection :: save (IDataModel *dataModel) const
{
  dataModel->appendString("__name__", d->name);
  dataModel->appendString("__version__", d->version);
}

IEntity const *Collection :: findInstance(std::string const &label) const
{
  return d->entityMap.at(label);
}

std::list<RelationTriplet> Collection :: findRelations(std::string const &subject) const
{
  if (subject.empty()) {
    return this->relationList();
  }

  std::list<RelationTriplet> retval;
  for (auto r : this->relationList()) {
    if (r.subject() == subject) {
      retval.push_back(r);
    }
  }
  return retval;
}

void Collection :: load (IDataModel const *dataModel)
{
  // TODO: Reflect name/version in load
  // dataModel->getStringArray("");
}

bool Collection :: checkCorrectness()
{
  // TODO: Implement this
  return true;
}

std::list<DimMap> Collection :: dimMapList() const
{
  return d->dimMapList;
}

std::vector<std::string> Collection :: dimensions() const
{
  // TODO: Implement this function
  std::vector<std::string> retval;
  return retval;
}

std::list<RelationTriplet> Collection :: relationList() const
{
  return d->relationList;
}

std::list<Dim> Collection :: dimList() const
{
  return d->dimList;
}

std::list<EntityRef> Collection :: entityList() const
{
  return d->entityList;
}


SOFT_END_NAMESPACE
