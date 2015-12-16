#ifndef IDATAMODEL_H_DEF
#define IDATAMODEL_H_DEF

#include <string>
#include <vector>
#include <cstdint>
#include "softns.h"
#include "softtypes.h"

SOFT_BEGIN_NAMESPACE

class IDataModel
{
public:
  IDataModel();
  virtual ~IDataModel() = 0;

  virtual IDataModel* createModel() = 0;
  virtual bool appendVariant    (const char *, StdVariant const &) = 0;
  virtual bool appendString     (const char*, const std::string &) = 0;
  virtual bool appendInt8       (const char *, int8_t) = 0;
  virtual bool appendUInt8      (const char *, uint8_t) = 0;
  virtual bool appendInt16      (const char *, int16_t) = 0;
  virtual bool appendUInt16     (const char *, uint16_t) = 0;
  virtual bool appendInt32      (const char *, int32_t) = 0;
  virtual bool appendUInt32     (const char *, uint32_t) = 0;
  virtual bool appendInt64      (const char *, int64_t) = 0;
  virtual bool appendUInt64     (const char *, uint64_t) = 0;
  virtual bool appendFloat      (const char *, float) = 0;
  virtual bool appendDouble     (const char *, double) = 0;
  virtual bool appendBool       (const char *, bool) = 0;
  virtual bool appendInt32Array (const char *, const std::vector<int32_t> &) = 0;
  virtual bool appendDoubleArray(const char *, const std::vector<double> &) = 0;
  virtual bool appendByteArray  (const char *, const std::vector<unsigned char> &) = 0;
  virtual bool appendStringArray(const char *, const std::vector<std::string> &) = 0;
  virtual bool appendArray      (const char *, const IDataModel *) = 0;
  virtual bool appendModel      (const char *, const IDataModel *) = 0;

  virtual bool getVariant       (const char *, StdVariant &) const = 0;
  virtual bool getString        (const char *, std::string &str) const = 0;
  virtual bool getInt8          (const char *, int8_t &) const = 0;
  virtual bool getUInt8         (const char *, uint8_t &) const = 0;
  virtual bool getInt16         (const char *, int16_t &) const = 0;
  virtual bool getUInt16        (const char *, uint16_t &) const = 0;
  virtual bool getInt32         (const char *, int32_t &) const = 0;
  virtual bool getUInt32        (const char *, uint32_t &) const = 0;
  virtual bool getInt64         (const char *, int64_t &) const = 0;
  virtual bool getUInt64        (const char *, uint64_t &) const = 0;
  virtual bool getFloat         (const char *, float &) const = 0;
  virtual bool getDouble        (const char *, double &) const = 0;
  virtual bool getBool          (const char *, bool &) const = 0;
  virtual bool getInt32Array    (const char *, std::vector<int32_t> &) const = 0;
  virtual bool getDoubleArray   (const char *, std::vector<double> &) const = 0;
  virtual bool getByteArray     (const char *, std::vector<unsigned char> &) const = 0;
  virtual bool appendStringArray(const char *, std::vector<std::string> &) const = 0;
  virtual bool getArray         (const char *, IDataModel *) const = 0;
  virtual bool getModel         (const char *, IDataModel *) const = 0;
};

SOFT_END_NAMESPACE

#endif // IDATAMODEL_H_DEF
