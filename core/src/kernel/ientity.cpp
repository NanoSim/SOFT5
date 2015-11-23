#include <QUuid>
#include <QString>

#include "ientity.h"
SOFT_BEGIN_NAMESPACE

class IEntity::Private
{
   friend class IEntity;
  Private(std::string const &id)
    : uuid(id.c_str())
  {}
  
  Private()
    : uuid(QUuid::createUuid())
  {}
  
  QUuid uuid;
};

IEntity :: IEntity() 
   :d (new IEntity::Private())
{}

IEntity :: IEntity(std::string const &id) 
   : d(new IEntity::Private(id))
{}

IEntity :: IEntity(IEntity const *other) 
  : d(new IEntity::Private(other->id()))
{}

IEntity :: ~IEntity()
{
   delete d;
}

IEntity* create (const std::string &)
{
  return NULL;
}

std::string IEntity :: id() const 
{
   return d->uuid.toString().toStdString();
}

SOFT_END_NAMESPACE
