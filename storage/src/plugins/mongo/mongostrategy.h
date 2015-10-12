#ifndef MONGOSTRATEGY_H_DEF
#define MONGOSTRATEGY_H_DEF

#include "softns.h"
#include "idatamodel.h"
#include "storagestrategy.h"

SOFT_BEGIN_NAMESPACE

class MongoStrategy : public StorageStrategy
{
public:
  MongoStrategy();
  MongoStrategy(const char *uri, const char *options);
  MongoStrategy (MongoStrategy const &) =delete;
  MongoStrategy& operator= (MongoStrategy const &) =delete;  
  virtual ~MongoStrategy();

  IDataModel* dataModel()         override;
  void store (IDataModel const *) override;

  static StorageStrategy* create(char const *uri, char const *opts);
private:
   class Private;
   Private *d;
};

SOFT_END_NAMESPACE

#endif // MONGOSTRATEGY_H_DEF
