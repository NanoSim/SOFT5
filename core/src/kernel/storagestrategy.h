#ifndef STORAGESTRATEGY_H_DEF
#define STORAGESTRATEGY_H_DEF

#include "softns.h"
#include "idatamodel.h"

SOFT_BEGIN_NAMESPACE

class StorageStrategy
{
public:
  StorageStrategy();
  StorageStrategy(char const *uri, const char *options = nullptr);
  StorageStrategy (StorageStrategy const &) =delete;
  StorageStrategy &operator=(StorageStrategy const &) =delete;
  virtual ~StorageStrategy() = 0;

  virtual const char *metaType() const               = 0;
  virtual IDataModel *dataModel () const             = 0;
  virtual void        store     (IDataModel const *) = 0;
};

SOFT_END_NAMESPACE

#endif // STORAGESTRATEGY_H_DEF
