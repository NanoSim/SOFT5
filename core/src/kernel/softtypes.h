#ifndef IDATAMODEL_H_DEF
#define IDATAMODEL_H_DEF

#include <cstdint>
#include <cstdbool>
#include "softns.h"
#include "variant.h"

SOFT_BEGIN_NAMESPACE
using StdString  = std::string;
using StdInt     = int32_t;
using StdUInt    = uint32_t;
using StdInt8    = int8_t;
using StdUInt8   = uint8_t;
using StdInt64   = int64_t;
using StdUInt64  = uint64_t;
using StdFloat   = float;
using StdDouble  = double;
using StdBool    = bool;
using StdBlob    = std::vector<unsigned char>;
using StdStringList = std::vector<StdString>;
using StdIntArray = std::vector<StdInt>;
using StdDoubleArray = std::vector<StdDouble>;

using StdVariant = Variant<StdString,
			   StdInt,
			   StdUInt,
			   StdInt8,
			   StdUInt8,
			   StdInt64,
			   StdUInt64,
			   StdFloat,
			   StdDouble,
			   StdBool,
			   StdBlob,
			   StdStringList,
			   StdIntArray,
			   StdDoubleArray>;
SOFT_END_NAMESPACE

#endif
