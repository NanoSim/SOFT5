#include <gtest/gtest.h>
#include <cstdlib>
#include <softc/softc-utils.h>

class SoftC_StorageTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}    
};

typedef char* soft_string;
typedef double* soft_double_array;

TEST_F(SoftC_StorageTest, construct)
{
  ASSERT_EQ(1, 1);

}
