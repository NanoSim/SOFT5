#include "bsondatamodel.h"
#include "mongostrategy.h"

SOFT_BEGIN_NAMESPACE

const char *MongoStrategy::staticMetaType = "http://sintef.no/soft/TR/storage-strategy#mongo:0.2-SNAPSHOT-1";

class MongoStrategy::Private
{
  Private()
  {}

  Private(QString const &, QString const &)
  {}

  friend class MongoStrategy;
};

const char *MongoStrategy::metaType() const
{
  return MongoStrategy::staticMetaType;
}

MongoStrategy :: MongoStrategy()
   : IStorageStrategy ()
   , d(new MongoStrategy::Private())
{}

MongoStrategy :: MongoStrategy(const char *uri, const char *options)
   : IStorageStrategy(uri, options)
   , d (new MongoStrategy::Private(uri, options))
{
}

MongoStrategy :: ~MongoStrategy()
{}

IDataModel* MongoStrategy :: dataModel() const
{
  return (IDataModel*)new BsonDataModel();
}

void MongoStrategy :: store (IDataModel const *model)
{
  auto bsonModel = dynamic_cast<BsonDataModel const*>(model);
  auto propObj = bsonModel->propertyObject;
  auto dimsObj = bsonModel->dimsObject;
   
}

void MongoStrategy :: retrieve (IDataModel *) const
{
}

void MongoStrategy :: startRetrieve (IDataModel *model) const
{
  auto bsonModel = dynamic_cast<BsonDataModel*>(model);
}

void MongoStrategy :: endRetrieve (IDataModel *model) const
{
  auto bsonModel = dynamic_cast<BsonDataModel*>(model);
}

IStorageStrategy* MongoStrategy :: create(char const *uri, char const *opts)
{
  auto strategy = new MongoStrategy(uri, opts);
  return strategy;
}

SOFT_END_NAMESPACE
