@{
    ENTITY = soft.model.name.toUpperCase();
    entity = soft.model.name.toLowerCase();
    Entity = entity.charAt(0).toUpperCase() + entity.slice(1);
    undefined;
}
#include <stdexcept>
#include "@{entity}.h"

SOFT_BEGIN_NAMESPACE
#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

class @{Entity} :: Private
{
  friend class @{Entity};
  
};

@{Entity} :: @{Entity} ()
  : IEntity()
  , d(new @{Entity}::Private())
{}

@{Entity} :: @{Entity}(std::string const &id)
  : IEntity(id)
  , d(new @{Entity}::Private())
{}

@{Entity} :: @{Entity}(const IEntity *ptr)
  : IEntity(ptr)
  , d(new @{Entity}::Private())
{}

@{Entity} :: @{Entity}(IDataModel const *dm)
  : IEntity()
  , d(new @{Entity}::Private())
{
  load(dm);
}

@{Entity} :: ~@{Entity} ()
{
  delete d;
}

IEntity* @{Entity} :: create (std::string const &uuid)
{
  NOT_IMPLEMENTED	
  return nullptr;	
}

void @{Entity} :: save (IDataModel *dataModel) const
{
  NOT_IMPLEMENTED	
}

void @{Entity} :: load (IDataModel const *dataModel)
{
  NOT_IMPLEMENTED	
}

std::vector<std::string> @{Entity} :: dimensions() const
{
  NOT_IMPLEMENTED	
  return std::vector<std::string>();
}

SOFT_END_NAMESPACE