#ifndef JSONMODEL_H_DEF
#define JSONMODEL_H_DEF

#include "softns.h"
#include "idatamodel.h"

class QJsonObject;
SOFT_BEGIN_NAMESPACE

class JSONModel : public IDataModel
{
  friend class JSONStrategy;
public:
  JSONModel();
  virtual ~JSONModel();
  IDataModel *createModel() override;
  void appendVariant(const char *, StdVariant const & value) override;
  void appendInt32(const char *key, int value) override;
  void appendString(const char *key, const std::string &str) override;
  void appendDouble(const char *key, double value) override; 
  void appendBool(const char *key, bool value) override; 
  void appendArray(const char *key, IDataModel *model) override; 
  void appendModel(const char *key, IDataModel *model) override; 
  void appendDoubleArray(const char *key, const double *value, size_t length) override; 
  void appendByteArray(const char* key, const unsigned char *data, size_t size) override;
  bool getVariant(const char *, StdVariant &) const override;
  bool getInt32(const char *, int *) const override;
  bool getDouble(const char *, double *) const override;
  bool getBool(const char *, bool *) const override;
  bool getDoubleArray(const char *, double **, size_t *len) const override;
  bool getString(const char *key, std::string &str) const override;
 
private:
  void setJson(QJsonObject const &);
  const QJsonObject *json() const;

  class Private;
  Private *d;
};

SOFT_END_NAMESPACE

#endif // JSONMODEL_H_DEF
