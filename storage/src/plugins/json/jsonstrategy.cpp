#include <QtCore>
#include "jsonstrategy.h"
#include "jsonmodel.h"

SOFT_BEGIN_NAMESPACE

const char *JSONStrategy::staticMetaType = "http://sintef.no/soft/TR/storage-strategy#json:0.1-SNAPSHOT-1";

const char *JSONStrategy::metaType() const
{
  return JSONStrategy::staticMetaType;
}

class JSONStrategy::Private
{
  friend class JSONStrategy;
  Private (QString const &uri, QString const &options)
    : uri (uri)
    , options (options)
  {}

  QString uri;
  QString options;
};

JSONStrategy :: JSONStrategy()
  : StorageStrategy()
  , d (new JSONStrategy::Private(QString(),QString()))
{}

JSONStrategy :: JSONStrategy(const char *uri, const char *options)
  : StorageStrategy()
  , d (new JSONStrategy::Private(uri, options))
{
}

JSONStrategy :: ~JSONStrategy()
{
  delete d;
}

IDataModel *JSONStrategy :: dataModel() const
{
  return (IDataModel *)new JSONModel();
}

void JSONStrategy :: store (IDataModel const *model)
{
  JSONModel const * jsonModel = dynamic_cast<JSONModel const*>(model);
  auto jsonObj = jsonModel->json();
  QJsonDocument doc(*jsonObj);
  QFile file(d->uri);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream(stderr) << "Cannot store json data " << file.errorString() << endl;
    return;
  }
  file.write(doc.toJson());
}

StorageStrategy* JSONStrategy :: create(char const *uri, char const *opts)
{
  auto s = new JSONStrategy(uri, opts);
  return s;
}

SOFT_END_NAMESPACE
