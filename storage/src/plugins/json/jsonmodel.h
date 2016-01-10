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

  bool appendVariant    (const char *, StdVariant const &) override;
  bool appendString     (const char*, const std::string &) override;
  bool appendInt8       (const char *, int8_t) override;
  bool appendUInt8      (const char *, uint8_t) override;
  bool appendInt16      (const char *, int16_t) override;
  bool appendUInt16     (const char *, uint16_t) override;
  bool appendInt32      (const char *, int32_t) override;
  bool appendUInt32     (const char *, uint32_t) override;
  bool appendInt64      (const char *, int64_t) override;
  bool appendUInt64     (const char *, uint64_t) override;
  bool appendFloat      (const char *, float) override;
  bool appendDouble     (const char *, double) override;
  bool appendBool       (const char *, bool) override;
  bool appendInt32Array (const char *, const std::vector<int32_t> &) override;
  bool appendDoubleArray(const char *, const std::vector<double> &) override;
  bool appendDoubleArray2D (const char *, const std::vector<std::vector<double> >&) override;
  bool appendDoubleArray3D (const char *, const std::vector<std::vector<std::vector<double> > >&) override;

  bool appendByteArray  (const char *, const std::vector<unsigned char> &) override;
  bool appendStringArray(const char *, const std::vector<std::string> &) override;
  bool appendArray      (const char *, const IDataModel *) override;
  bool appendModel      (const char *, const IDataModel *) override;

  bool getVariant       (const char *, StdVariant &) const override;
  bool getString        (const char *, std::string &str) const override;
  bool getInt8          (const char *, int8_t &) const override;
  bool getUInt8         (const char *, uint8_t &) const override;
  bool getInt16         (const char *, int16_t &) const override;
  bool getUInt16        (const char *, uint16_t &) const override;
  bool getInt32         (const char *, int32_t &) const override;
  bool getUInt32        (const char *, uint32_t &) const override;
  bool getInt64         (const char *, int64_t &) const override;
  bool getUInt64        (const char *, uint64_t &) const override;
  bool getFloat         (const char *, float &) const override;
  bool getDouble        (const char *, double &) const override;
  bool getBool          (const char *, bool &) const override;
  bool getInt32Array    (const char *, std::vector<int32_t> &) const override;
  bool getDoubleArray   (const char *, std::vector<double> &) const override;
  bool getDoubleArray2D (const char *, std::vector<std::vector<double> > &) const override;
  bool getDoubleArray3D (const char *, std::vector<std::vector<std::vector<double> > > &) const override;

  bool getByteArray     (const char *, std::vector<unsigned char> &) const override;
  bool getStringArray(const char *, std::vector<std::string> &) const override;
  bool getArray         (const char *, IDataModel *) const override;
  bool getModel         (const char *, IDataModel *) const override;

  /*
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
  */
private:
  void setJson(QJsonObject const &);
  const QJsonObject *json() const;

  class Private;
  Private *d;
};

SOFT_END_NAMESPACE

#endif // JSONMODEL_H_DEF
