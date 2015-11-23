#include "storage.h"
#include "soft.h"
#include "storagestrategy.h"


SOFT_BEGIN_NAMESPACE

class Storage::Private
{
  friend class Storage;
  Private(char const *driver, char const *uri, const char *options)
    : strategy (createStrategy(driver, uri, options))
  {}
  
  ~Private()
  {
    delete strategy;
    strategy = nullptr;
  }
    
  StorageStrategy *strategy;
};

Storage :: Storage(char const* driver, char const *uri, const char *options)
  : d (new Storage::Private(driver, uri, options))
{}

Storage :: ~Storage()
{
  delete d;
}

void Storage :: save (IEntity const *e)
{
  auto dataModel = d->strategy->dataModel();
  e->save(dataModel);
  d->strategy->store(dataModel);
}

SOFT_END_NAMESPACE
