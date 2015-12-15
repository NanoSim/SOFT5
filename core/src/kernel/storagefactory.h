#ifndef STORAGEFACTORY_H_DEF
#define STORAGEFACTORY_H_DEF

#include <functional>
#include <string>
#include "softns.h"

SOFT_BEGIN_NAMESPACE

class IStorageStrategy;
class StorageFactory
{   
public:
   StorageFactory();
   StorageFactory (StorageFactory const &) =delete;
   virtual ~StorageFactory();
   StorageFactory& operator= (StorageFactory const &) =delete;
  
   bool registerStrategy(const char *name, IStorageStrategy*(*createFunc)(const char*, const char*));
   IStorageStrategy* create(const char *name, const char *uri, const char *options) const;
   int driverCount() const;
   std::string driverName(int n) const;

private:
   class Private;
   Private *d;
};

SOFT_END_NAMESPACE

#endif // STORAGEFACTORY_H_DEF
