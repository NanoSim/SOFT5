#include <gtest/gtest.h>
#include <softc/softc-allocatable.h>

class AllocatableTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}
};

TEST_F(AllocatableTest, rank)
{
  double **vec = (double **)softc_allocatable_allocatev(2, 3, 2);
  ASSERT_EQ(softc_allocatable_rank(vec), 2);
  softc_allocatable_free(vec);
}

TEST_F(AllocatableTest, dimensions)
{
  size_t *dims;
  size_t rank;
  double ***vec = (double ***)softc_allocatable_allocatev(3, 300, 200, 89);
  softc_allocatable_dimensions(vec, &dims, &rank);
  ASSERT_EQ(dims[0], 300);
  ASSERT_EQ(dims[1], 200);
  ASSERT_EQ(dims[2], 89);
  ASSERT_EQ(rank, 3);
  free(dims);
  softc_allocatable_free(vec);
}

TEST_F(AllocatableTest, allocateAndReshape)
{
  const size_t dims[2] = {3,4};
  const size_t rank = 2;
  double **vec = (double**) softc_allocatable_allocate(dims, rank);
  double **copy = (double**) softc_allocatable_reshape(vec, dims, rank);

  vec[2][2] = 3.2;
  ASSERT_EQ(vec[2][2], copy[2][2]);

  softc_allocatable_free(vec);
  softc_allocatable_free(copy);
}

TEST_F(AllocatableTest, shallowCopy)
{
  double testA = 3.0;
  double testB = 12.0;
  double testC = 32.0;
  double ***vec = (double***) softc_allocatable_allocatev(3, 300, 200, 89);
  double ***copy = (double***) softc_allocatable_shallow_copy(vec);

  vec[0][0][0] = testA;
  vec[10][10][10] = testB;
  vec[1][10][100] = testC;

  ASSERT_EQ(vec[0][0][0], copy[0][0][0]);
  ASSERT_EQ(vec[10][10][10], copy[10][10][10]);
  ASSERT_EQ(vec[1][10][100], copy[1][10][100]);

  softc_allocatable_free(vec);
  softc_allocatable_free(copy);
}

TEST_F(AllocatableTest, reshape)
{
  size_t dims[2] = {3,2};
  double **vec1 = (double**) softc_allocatable_allocate(dims, 2);
  double **vec2 = (double**) softc_allocatable_reshapev(vec1, 2, 2, 3);

  vec1[1][1] = 5.0;
  ASSERT_EQ(vec1[1][1], vec2[2][0]);

  softc_allocatable_free(vec1);
  softc_allocatable_free(vec2);
}
