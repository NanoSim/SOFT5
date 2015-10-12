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
   void appendDouble(const char *key, double value) override; 
   void appendBool(const char *key, bool value) override; 
   void appendArray(const char *key, IDataModel *model) override; 
   void appendModel(const char *key, IDataModel *model) override; 
   void appendDoubleArray(const char *key, size_t length, const double *value) override; 

   const _bson_t *bson() const;
private:
   class Private;
   Private *d;
};

SOFT_END_NAMESPACE

#endif // BSONMODEL_H_DEF
