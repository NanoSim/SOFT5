#include <QtCore>
#include "qh5.h"
#include "jsonmodel.h"
#include "hdf5strategy.h"

SOFT_BEGIN_NAMESPACE

const char *HDF5Strategy::staticMetaType = "http://sintef.no/soft/TR/storage-strategy#hdf5:0.1-SNAPSHOT-1";

const char *HDF5Strategy::metaType() const
{
  return HDF5Strategy::staticMetaType;
}

class HDF5Strategy::Private
{
  friend class HDF5Strategy;
  Private (QString const &uri, QString const &options)
    : uri(uri)
    , options (options)
  {}
  
  QString uri;
  QString options;
};

HDF5Strategy :: HDF5Strategy()
  : IStorageStrategy()
  , d (new HDF5Strategy::Private(QString(), QString()))
{}

HDF5Strategy :: HDF5Strategy(const char *uri, const char *options)
  : IStorageStrategy()
  , d (new HDF5Strategy::Private(uri, options))
{}

HDF5Strategy :: ~HDF5Strategy()
{
  delete d;
}

IDataModel* HDF5Strategy :: dataModel() const
{
  return (IDataModel*) new JSONModel();
}

void HDF5Strategy :: store (IDataModel const *model)
{
  JSONModel const * jsonModel = dynamic_cast<JSONModel const*>(model);
  auto jsonObj = jsonModel->json();
  QJsonDocument doc(*jsonObj);

  soft::hdf5::QH5 h5;
  h5.create(d->uri);
  h5.createGroup("properties");

  if (doc.isObject()) {
    auto jsonObject = doc.object();
    for (auto key: jsonObject.keys()) {
      auto value = jsonObject.value(key);
      h5.write(QString("properties/%1").arg(key), value.toVariant());
    }    
  }

  h5.close();
}

void HDF5Strategy :: retrieve (IDataModel *) const
{
}

IStorageStrategy* HDF5Strategy :: create(char const *uri, char const *opts)
{
  auto s = new HDF5Strategy(uri, opts);
  return s;
}

SOFT_END_NAMESPACE
