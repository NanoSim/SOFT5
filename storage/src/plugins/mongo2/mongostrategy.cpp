#include <QtCore>

#include "bsondatamodel.h"
#include "mongostrategy.h"

SOFT_BEGIN_NAMESPACE

const char *MongoStrategy::staticMetaType = "http://sintef.no/soft/TR/storage-strategy#mongo:0.2-SNAPSHOT-1";

const QString defaultDb = "default";
const QString defaultColl = "default";

class MongoStrategy :: Private
{
public:
  
  Private()
    : client(nullptr)
    , collection(nullptr)
  {}
  
  Private(QString const &uri, QString const &options)
  {
    auto optionMap = ([options]()
      -> QMap<QString,QString>
      {
	QMap<QString,QString> ret;
	auto opt = options.split(";");
	foreach (auto o, opt) {
	  auto pair = o.split("=");
	  Q_ASSERT(pair.count() == 2);
	  ret.insert(pair[0], pair[1]);
	}
	return ret;
      })();
        
    client = mongoc_client_new (qPrintable(uri));
    Q_CHECK_PTR(client);
    auto db = (optionMap.contains("db") ? optionMap["db"] : defaultDb);
    auto coll = (optionMap.contains("coll") ? optionMap["coll"] : defaultColl);
    collection = mongoc_client_get_collection(client, qPrintable(db), qPrintable(coll));
    Q_CHECK_PTR(collection);    
  }
  
  ~Private()
  {
    if (nullptr != client) mongoc_client_destroy (client);
    if (nullptr != collection) mongoc_collection_destroy (collection);
  }
  
  mongoc_client_t *client;
  mongoc_collection_t *collection;
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
  bson_error_t error;
  auto bsonModel = dynamic_cast<BsonDataModel const*>(model);
  auto propObj = bsonModel->propertyObject;
  auto dimsObj = bsonModel->dimsObject;
  bson::Bson metaObj;

  metaObj.append("name", model->metaName());
  metaObj.append("version", model->metaVersion());
  metaObj.append("namespace", model->metaNamespace());
  metaObj.append("uuid", model->id());
  metaObj.append("properties", propObj);
  metaObj.append("dimensions", dimsObj);

  mongoc_collection_insert(d->collection, MONGOC_INSERT_NONE, metaObj.bsonPtr().get(), 0, &error);
  if (error.code) throw std::runtime_error(error.message);
}


void MongoStrategy :: retrieve (IDataModel *model) const
{
  auto bsonModel = dynamic_cast<BsonDataModel*>(model);
  auto propObj = bsonModel->propertyObject;
  auto dimsObj = bsonModel->dimsObject;
  
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
