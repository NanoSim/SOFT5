#ifndef JSONMODEL_H_DEF
#define JSONMODEL_H_DEF

#include <functional>
#include "softns.h"
#include "idatamodel.h"

class QJsonObject;
SOFT_BEGIN_NAMESPACE

class JSONModel : public IDataModel
{
  friend class HDF5Strategy;
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
  bool getDoubleArray2D (const char *, std::vector<std::vector<double> > &) const override;;
  bool getDoubleArray3D (const char *, std::vector<std::vector<std::vector<double> > > &) const override;
  bool getByteArray     (const char *, std::vector<unsigned char> &) const override;
  bool getStringArray(const char *, std::vector<std::string> &) const override;
  bool getArray         (const char *, IDataModel *) const override;
  bool getModel         (const char *, IDataModel *) const override;

private:
  void setJson(QJsonObject const &);
  const QJsonObject *json() const;

  void registerGetStringFn(std::function<bool(const char *,std::string&)> &fn);
  void registerGetInt8Fn(std::function<bool(const char *,int8_t&)> &fn);
  void registerGetUInt8Fn(std::function<bool(const char *,uint8_t&)> &fn);
  void registerGetInt16Fn(std::function<bool(const char *, int16_t &)> &fn);
  void registerGetUInt16Fn(std::function<bool(const char *, uint16_t &)> &fn);
  void registerGetInt32Fn(std::function<bool(const char *, int32_t &)> &fn);
  void registerGetUInt32Fn(std::function<bool(const char *, uint32_t &)> &fn);
  void registerGetInt64Fn(std::function<bool(const char *, int64_t &)> &fn);
  void registerGetUInt64Fn(std::function<bool(const char *, uint64_t &)> &fn);
  void registerGetFloatFn(std::function<bool(const char *, float &)> &fn);
  void registerGetDoubleFn(std::function<bool(const char *, double &)> &fn);
  void registerGetBoolFn(std::function<bool(const char *, bool &)> &fn);
  void registerGetInt32ArrayFn(std::function<bool(const char *, std::vector<int32_t> &)> &fn);
  void registerGetDoubleArrayFn(std::function<bool(const char *, std::vector<double> &)> &fn);
  void registerGetDoubleArray2DFn(std::function<bool(const char *, std::vector<std::vector<double> > &)> &fn);
  void registerGetDoubleArray3DFn(std::function<bool(const char *, std::vector<std::vector<std::vector<double> > > &)> &fn);
  void registerGetByteArrayFn(std::function<bool(const char *, std::vector<unsigned char>&)> &fn);
  void registerGetStringArrayFn(std::function<bool(const char *, std::vector<std::string> &)> &fn);
  
  void *data;
  class Private;
  Private *d;
};

SOFT_END_NAMESPACE

#endif // JSONMODEL_H_DEF
