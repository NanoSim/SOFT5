#include "storage.h"
#include "soft.h"
#include "istoragestrategy.h"
#include "idatamodel.h"

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
    
  IStorageStrategy *strategy;
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
  IDataModel *dataModel = d->strategy->dataModel();
  dataModel->setId(e->id());
  dataModel->setMetaName(e->metaName());
  dataModel->setMetaNamespace(e->metaNamespace());
  dataModel->setMetaVersion(e->metaVersion());

  e->save(dataModel);
  d->strategy->store(dataModel);
}

void Storage :: load (IEntity *e)
{
  auto dataModel = d->strategy->dataModel();
  d->strategy->retrieve(dataModel);  
  e->load(dataModel);
}

IStorageStrategy *Storage :: strategy()
{
  if (d != nullptr && d->strategy != nullptr) {
    return d->strategy;
  }
  return nullptr;
}

SOFT_END_NAMESPACE
