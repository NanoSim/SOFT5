#include <functional>
#include <QMap>
#include <QString>
#include <QTextStream>
#include <QDebug>

#include "storagefactory.h"

SOFT_BEGIN_NAMESPACE

class StorageFactory::Private
{
  friend class StorageFactory;
  using MapType = QMap<QString, StorageFactory::CreateFunc>;
  MapType map;
};

StorageFactory :: StorageFactory()
   : d (new StorageFactory::Private())
{}

StorageFactory :: ~StorageFactory()
{
   delete d;
}

bool StorageFactory :: registerStrategy(const char *name, StorageFactory::CreateFunc createFunc)
{
   d->map[name] = createFunc;
   return true;
}

IStorageStrategy* StorageFactory :: create(const char *name, const char *uri, const char *options) const
{
  auto it = d->map.find(name);
  if(it == d->map.end()) {
    QTextStream(stderr) << "StorageFactory cannot create storage strategy: " << name << " " << uri << endl;
    return nullptr;
  }
  
  CreateFunc createFunc = *it;
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
