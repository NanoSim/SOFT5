#ifndef SOFTC_BYTEARRAY_PRIVATE_H_CPP
#define SOFTC_BYTEARRAY_PRIVATE_H_CPP

#include <QByteArray>

struct _softc_bytearray_s
{
  _softc_bytearray_s(){}
  _softc_bytearray_s(const unsigned char *data, size_t len)
    : bytearray((const char *)data, len)
  {}

  explicit _softc_bytearray_s(_softc_bytearray_s &&other) noexcept
    : bytearray(std::move(other.bytearray))
  {}

  _softc_bytearray_s& operator=(_softc_bytearray_s &&other)
  {
    bytearray = std::move(other.bytearray);
    return *this;
  }

  QByteArray bytearray;
};

#endif // SOFTC_BYTEARRAY_PRIVATE_H_CPP
