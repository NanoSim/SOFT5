#include <functional>
#include <QMap>
#include <QString>
#include <QDebug>

#include "storagefactory.h"

SOFT_BEGIN_NAMESPACE

class StorageFactory::Private
{
  friend class StorageFactory;
   typedef StorageStrategy*(*createFunc)(const char*, const char*);
  QMap<QString, createFunc> map;
};

StorageFactory :: StorageFactory()
   : d (new StorageFactory::Private())
{}

StorageFactory :: ~StorageFactory()
{
   delete d;
}

bool StorageFactory :: registerStrategy(const char *name, StorageStrategy*(*createFunc)(const char*, const char*))
{
   d->map[name] = createFunc;
   return true;
}

StorageStrategy* StorageFactory :: create(const char *name, const char *uri, const char *options) const
{
  qDebug() << "StorageFactory :: create" << name << uri << options;
  if (!d->map.contains(name)) return nullptr;
  
  StorageStrategy*(*createFunc)(const char*, const char*) = d->map.value(name);
  return (*createFunc)(uri, options);
}

int StorageFactory :: driverCount() const
{
  return d->map.count();
}

std::string StorageFactory :: driverName(int n) const
{
  if (n >= 0 && d->map.count() > n) {
    auto it = d->map.constBegin();
    auto name = (it+n).key();
    return name.toStdString();
  }
  return std::string();
}


SOFT_END_NAMESPACE
