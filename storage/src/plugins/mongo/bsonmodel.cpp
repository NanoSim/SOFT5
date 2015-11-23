#include <bson.h>
#include <QString>
#include <QByteArray>
#include "bsonmodel.h"

SOFT_BEGIN_NAMESPACE

class BsonModel :: Private
{
   friend class BsonModel;

   Private()
      : bson (bson_new ())
   {}

   ~Private()
   {
      bson_destroy(bson);
   }
   bson_t *bson;
};

BsonModel :: BsonModel()
   : IDataModel()
   , d(new BsonModel::Private)
      
{}

BsonModel :: ~BsonModel()
{
   delete d;
}

IDataModel* BsonModel :: createModel()
{
   return new BsonModel;
}

void BsonModel :: appendInt32(const char *key, int value) 
{
   BSON_APPEND_INT32(d->bson, key, value);
}

void BsonModel :: appendDouble(const char *key, double value) 
{
   BSON_APPEND_DOUBLE(d->bson, key, value);
}

void BsonModel :: appendBool(const char *key, bool value) 
{
   BSON_APPEND_BOOL(d->bson, key, value);
}

void BsonModel :: appendArray(const char *key, IDataModel *model) 
{
   auto m = dynamic_cast<BsonModel*>(model);
   bson_append_array (d->bson, key, strlen(key), m->d->bson);
}

void BsonModel :: appendModel(const char *key, IDataModel *model) 
{
   auto m = dynamic_cast<BsonModel*>(model);
   bson_append_document (d->bson, key, strlen(key), m->d->bson);
}

void BsonModel :: appendDoubleArray(const char *key, size_t length, const double *value)
{
   auto ptr = &value[0];
   auto b = bson_new();
   for (size_t i = 0; i < length; ++i) {
      BSON_APPEND_DOUBLE(b, i==0?"0":"_id", *(ptr++));
   }
   bson_append_array(d->bson, key, strlen(key), b);
   bson_destroy(b);   
}

void BsonModel :: appendByteArray(const char* key, const unsigned char *data, size_t size)
{
  BSON_APPEND_BINARY(d->bson, key, BSON_SUBTYPE_BINARY, data, size);
}

void BsonModel :: appendString(const char *key, const std::string &str)
{
  QByteArray utfStr = QString::fromStdString(str).toUtf8();
  bson_append_utf8 (d->bson, key, strlen(key), utfStr.constData(), utfStr.length());
}

const _bson_t *BsonModel :: bson() const
{
   return (_bson_t const*) d->bson;
}

SOFT_END_NAMESPACE
