#ifndef STORAGESTRATEGY_H_DEF
#define STORAGESTRATEGY_H_DEF

#include "softns.h"
#include "idatamodel.h"

SOFT_BEGIN_NAMESPACE

class IStorageStrategy
{
public:
  IStorageStrategy();
  IStorageStrategy(char const *uri, const char *options = nullptr);
  IStorageStrategy (IStorageStrategy const &) =delete;
  IStorageStrategy &operator=(IStorageStrategy const &) =delete;
  virtual ~IStorageStrategy() = 0;

  virtual const char *metaType() const               = 0;
  virtual IDataModel *dataModel () const             = 0;
  virtual void        store     (IDataModel const *) = 0;
  virtual void        retrieve  (IDataModel *) const = 0;
  virtual void        startRetrieve (IDataModel *m) const;
  virtual void        endRetrieve (IDataModel*) const;
};

SOFT_END_NAMESPACE

#endif // STORAGESTRATEGY_H_DEF
