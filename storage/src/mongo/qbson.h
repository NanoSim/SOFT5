#ifndef QBSON_H_DEF
#define QBSON_H_DEF

#include <QObject>
#include <QString>
#include <memory>
#include <mongoc.h>
#include <bson.h>
#include <softtypes.h>
#include "mongons.h"

SOFT_BEGIN_NAMESPACE

MONGO_BEGIN_NAMESPACE
class Client;
class Collection;
class GridFSFileOpt;
MONGO_END_NAMESPACE

BSON_BEGIN_NAMESPACE

class Bson : public QObject
{
   Q_OBJECT

public:
   explicit Bson (QObject * parent = nullptr);
   Bson (QString const& json, QObject * parent = nullptr);
   Bson (bson_t *bsn, QObject * parent = nullptr);
   Bson (Bson const & other);
   Bson (Bson && other);
   virtual ~Bson();

   bool appendBson (char const *key, Bson const &value);
   bool appendString (char const * key, char const * value);
   bool appendStringList (char const * key, QStringList const &value);
   bool appendInt8 (char const * key, qint8 value);
   bool appendInt32 (char const * key, qint32 value);
   bool appendInt64 (char const * key, qint64 value);
   bool appendDouble(char const * key, double const &value);
   bool appendFloat(char const * key, float const &value);
   bool appendBool (char const * key, bool value);
   bool appendBinary (char const *key, QByteArray const &value);

   bool append(char const *key, soft::StdIntArray const &value);
   bool append(char const *key, soft::StdDoubleArray const &value);
   bool append(char const *key, soft::StdDoubleArray2D const &value);
   bool append(char const *key, soft::StdDoubleArray3D const &value);
   bool get(char const *key, soft::StdIntArray &value);
   bool get(char const *key, soft::StdDoubleArray &value);
   bool get(char const *key, soft::StdDoubleArray2D &value);
   bool get(char const *key, soft::StdDoubleArray3D &value);
   
   bool getInt32(const char *key, qint32 &value);
   bool getInt64(const char *key, qint64 &value);
   bool getString(const char *key, QString &value);
   bool getDouble(const char *key, double &value);
   bool getFloat(const char *key, float &value);
   bool getBinary(const char *key, QByteArray &value);
   bool getStringList (char const * key, QStringList &value);
   qint32 countKeys() const;
   bool hasField(char const * key) const;
   QString asString() const;

protected:
  bson_t* data() const;

private:
  friend class SOFT_NAMESPACE::MONGO_NAMESPACE::GridFSFileOpt;
  friend class SOFT_NAMESPACE::MONGO_NAMESPACE::Client;
  friend class SOFT_NAMESPACE::MONGO_NAMESPACE::Collection;
  std::shared_ptr<bson_t> bson;
};

BSON_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QBSON_H_DEF
