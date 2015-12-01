#ifndef BSONMODEL_H_DEF
#define BSONMODEL_H_DEF

#include <cstddef>
#include "softns.h"
#include "idatamodel.h"

struct _bson_t;
SOFT_BEGIN_NAMESPACE

class BsonModel : public IDataModel
{
public:
   BsonModel();
   virtual ~BsonModel();
   IDataModel* createModel() override;
   void appendInt32(const char *key, int value) override;
   void appendString(const char *key, const std::string &str) override;
   void appendDouble(const char *key, double value) override; 
   void appendBool(const char *key, bool value) override; 
   void appendArray(const char *key, IDataModel *model) override; 
   void appendModel(const char *key, IDataModel *model) override; 
   void appendDoubleArray(const char *key, size_t length, const double *value) override; 
  void appendByteArray(const char* key, const unsigned char *data, size_t size) override;
  bool getInt32(const char *, int *) const override;
  bool getDouble(const char *, double *) const override;
  bool getBool(const char *, bool *) const override;
  bool getDoubleArray(const char *, double **, size_t *len) const override;
  bool getString(const char *key, std::string &str) const override;
  
   const _bson_t *bson() const;
private:
   class Private;
   Private *d;
};

SOFT_END_NAMESPACE

#endif // BSONMODEL_H_DEF
