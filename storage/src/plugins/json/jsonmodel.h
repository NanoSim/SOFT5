#ifndef JSONMODEL_H_DEF
#define JSONMODEL_H_DEF

#include "softns.h"
#include "idatamodel.h"

class QJsonObject;
SOFT_BEGIN_NAMESPACE

class JSONModel : public IDataModel
{
public:
  JSONModel();
  virtual ~JSONModel();
  IDataModel *createModel() override;
  void appendInt32(const char *key, int value) override;
  void appendString(const char *key, const std::string &str) override;
  void appendDouble(const char *key, double value) override; 
  void appendBool(const char *key, bool value) override; 
  void appendArray(const char *key, IDataModel *model) override; 
  void appendModel(const char *key, IDataModel *model) override; 
  void appendDoubleArray(const char *key, size_t length, const double *value) override; 
  void appendByteArray(const char* key, const unsigned char *data, size_t size) override;

  const QJsonObject *json() const;
private:
  class Private;
  Private *d;
};

SOFT_END_NAMESPACE

#endif // JSONMODEL_H_DEF
