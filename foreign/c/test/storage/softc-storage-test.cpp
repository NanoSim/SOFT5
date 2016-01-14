#include <gtest/gtest.h>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <softc/softc.h>
#include <vector>
#include <softc/softc-utils.h>
#include <softc/softc-datamodel.h>
#include <softc/softc-storage.h>


class SoftC_StorageTest : public ::testing::Test {
protected:
  SoftC_StorageTest()
    : hello_string("Hello softc")
    , intvec({0,1,2,3,4,5,6,7,8,9})
    , doublevec({0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9})
    , doublevec2d({{0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9},
	  {1.01,1.02,1.03,1.04,1.05,1.06,1.07,1.08,1.09},
	    {2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9},
	      {3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9}})
    , doublevec3d({
		  {{0.1, 0.2, 0.3},
		      {0.1, 0.2, 0.3},
			{0.1, 0.2, 0.3}},
		    {{0.1, 0.2, 0.3},
		      {0.1, 0.2, 0.3},
			{0.1, 0.2, 0.3}},
		    {{0.1, 0.2, 0.3},
		      {0.1, 0.2, 0.3},
			{0.1, 0.2, 0.3}}})
      
  {}

  ~SoftC_StorageTest()
  {}
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}

  const char *hello_string;
  const std::vector<int32_t> intvec;
  const std::vector<double> doublevec;
  const std::vector<std::vector<double> > doublevec2d;
  const std::vector<std::vector<std::vector<double> > > doublevec3d;

public:
  static char *uuid_string;
};

char *SoftC_StorageTest::uuid_string = softc_uuidgen();

typedef char* soft_string;
typedef double* soft_double_array;

template <typename T>
void arrayToPtr(T **dest, std::vector<T> const &source)
{
  const int size = source.size();
  (*dest) = new T[size];
  std::copy(source.cbegin(),source.cend(), *dest);
}

template <typename T>
void ptrToArray(std::vector<T> &dest, const T *src, size_t length)
{
  dest.resize(length);
  dest.assign(src, src+length);
}

template <typename T>
void ptrToArray(std::vector<std::vector<T>> &dest, const T **src, size_t i, size_t j)
{
  dest.resize(j);
  for (size_t idx = 0; idx < j; ++idx) {
    dest[idx].resize(i);
    dest[idx].assign(src[idx], src[idx]+i);
  }
}

void arrayToPtr(double ***dest, std::vector<std::vector<double>> const &source)
{
  *dest = new double*[source.size()];
  auto ptr = *dest;
  int idx = 0;
  for(auto &s: source) {
    arrayToPtr(&(*ptr++), s);
  }
}

void arrayToPtr(double ****dest, std::vector<std::vector<std::vector<double>>> const &source)
{
  *dest = new double**[source.size()];
  auto ptr = *dest;
  int idx = 0;
  for(auto &s: source) {
    arrayToPtr(&(*ptr++), s);
  }
}

TEST_F(SoftC_StorageTest, writeData)
{
  auto storage  = softc_storage_create("hdf5", "test.h5", "");
  auto strategy = softc_storage_get_storage_strategy(storage);
  auto model    = softc_storage_strategy_get_datamodel(strategy);
  double **v2d;
  double ***v3d;

  arrayToPtr(&v3d, doublevec3d);
  arrayToPtr(&v2d, doublevec2d);

  softc_datamodel_set_id (model, uuid_string);
  softc_datamodel_set_meta_name(model, "meta");
  softc_datamodel_set_meta_version(model, "meta");
  softc_datamodel_set_meta_namespace(model, "meta");

  softc_datamodel_append_string(model, "message", hello_string);
  softc_datamodel_append_array_int32(model, "int-array", intvec.data(), intvec.size());
  softc_datamodel_append_array_double(model, "double-array", doublevec.data(), doublevec.size());
  softc_datamodel_append_array_double_2d(model, "double-array-2d", (const double**)v2d, doublevec2d.size(), doublevec2d[0].size());
  softc_datamodel_append_array_double_3d(model, "double-array-3d", (const double***)v3d, doublevec3d.size(), doublevec3d[0].size(), doublevec3d[0][0].size());
  softc_storage_strategy_store(strategy, model);  
}

TEST_F(SoftC_StorageTest, readData)
{
  char *message;
  int32_t *intarray;
  size_t intarray_size;

  double *doublearray;  
  size_t doublearray_size;
    
  auto storage  = softc_storage_create("hdf5", "test.h5", "");
  auto strategy = softc_storage_get_storage_strategy(storage);
  auto model    = softc_storage_strategy_get_datamodel(strategy);

  softc_datamodel_set_id (model, uuid_string);
  softc_datamodel_set_meta_name(model, "meta");
  softc_datamodel_set_meta_version(model, "meta");

  softc_storage_strategy_start_retrieve(strategy, model);
  softc_datamodel_get_string(model, "message", &message);
  softc_datamodel_get_array_int32(model, "int-array", &intarray, &intarray_size);
  softc_datamodel_get_array_double(model, "double-array", &doublearray, &doublearray_size);

  softc_storage_strategy_end_retrieve(strategy, model);

  ASSERT_STREQ(message, hello_string);
  ASSERT_EQ(intarray_size, intvec.size());

  std::vector<int32_t> arr(intarray, intarray + intarray_size);
  ASSERT_EQ(arr, intvec);
  
}
