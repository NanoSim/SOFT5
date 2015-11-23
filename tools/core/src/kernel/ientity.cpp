#include <QUuid>
#include <QString>

#include "ientity.h"
SOFT_BEGIN_NAMESPACE

const char *IEntity::staticMetaType = "Undefined:0.0";

class IEntity::Private
{
   friend class IEntity;
   Private(const QString id)
      : uuid(id)
   {}

   Private()
      : uuid(QUuid::createUuid())
   {}

   QUuid uuid;
};

IEntity :: IEntity() 
   :d (new IEntity::Private())
{}

IEntity :: IEntity(const char *id) 
   : d(new IEntity::Private(QString::fromLocal8Bit(id)))
{}

IEntity :: ~IEntity()
{
   delete d;
}

std::string IEntity :: id() const 
{
   return d->uuid.toString().toStdString();
}

SOFT_END_NAMESPACE
