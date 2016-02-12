#include <gtest/gtest.h>
#include <softc/softc-vector.h>

class VectorTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}
};

TEST_F(VectorTest, vector1d_construct0)
{
  softc_vector1d v = softc_vector1d_create(1);
  ASSERT_TRUE(v != NULL);

  softc_vector1d_destroy(v);
}

TEST_F(VectorTest, vector2d_construct0)
{
  softc_vector2d v = softc_vector2d_create(2,2);
  ASSERT_TRUE(v != NULL);

  softc_vector2d_destroy(v);
}

TEST_F(VectorTest, vector2d_use)
{
  double **v = softc_vector2d_create(2,2);
  size_t *dims;
  size_t rank;
  
  ASSERT_TRUE(v != NULL);
  v[0][0] = 0.0;
  v[0][1] = 1.0;
  v[1][0] = 0.1;
  v[1][1] = 1.1;

  softc_vector2d_info(v, &dims, &rank);
  ASSERT_EQ(rank, 2);
  ASSERT_EQ(dims[0], 2);
  ASSERT_EQ(dims[1], 2);
  softc_vector2d_destroy(v);
}


TEST_F(VectorTest, vector3d_construct0)
{
  softc_vector3d v = softc_vector3d_create(2,2,2);
  //  ASSERT_TRUE(v != NULL);

  softc_vector3d_destroy(v);
}

TEST_F(VectorTest, vector4d_construct0)
{
  softc_vector4d v = softc_vector4d_create(2,2,2,2);
  //  ASSERT_TRUE(v != NULL);

  softc_vector4d_destroy(v);
}
