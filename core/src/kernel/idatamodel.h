#ifndef IDATAMODEL_H_DEF
#define IDATAMODEL_H_DEF

#include <string>
#include "softns.h"
#include "softtypes.h"

SOFT_BEGIN_NAMESPACE

class IDataModel
{
public:
   IDataModel();
   virtual ~IDataModel() = 0;

   virtual IDataModel* createModel() = 0;
   virtual void appendVariant(const char *, StdVariant const & value) = 0;
   virtual void appendInt32(const char *, int) = 0;
   virtual void appendDouble(const char *, double) = 0;
   virtual void appendBool(const char*, bool value) = 0;
   virtual void appendDoubleArray(const char *, const double *value, size_t length) = 0;
   virtual void appendArray(const char*, IDataModel *array) = 0;
   virtual void appendModel(const char*, IDataModel *model) = 0;
   virtual void appendByteArray(const char*, const unsigned char *, size_t) = 0;
   virtual void appendString(const char*, const std::string &str) = 0;

   virtual bool getVariant(const char *, StdVariant &) const = 0;
   virtual bool getInt32(const char *, int *) const = 0;
   virtual bool getDouble(const char *, double *) const = 0;
   virtual bool getBool(const char *, bool *) const = 0;
   virtual bool getDoubleArray(const char *, double **, size_t *len) const = 0;
   virtual bool getString(const char *key, std::string &str) const = 0;
};

SOFT_END_NAMESPACE

#endif // IDATAMODEL_H_DEF
