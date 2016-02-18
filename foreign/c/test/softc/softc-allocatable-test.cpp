#include <gtest/gtest.h>
#include <softc/softc-allocatable.h>

class AllocatableTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}
};

TEST_F(AllocatableTest, rank1)
{
  auto ref = softc_allocatable_createv(1, 3);
  double *vec = (double*)softc_allocatable_data(ref);
  vec[0] = 0.0;
  vec[1] = 1.0;
  vec[2] = 1.0;
  softc_allocatable_free(ref);
}

TEST_F(AllocatableTest, rank2)
{
  auto ref = softc_allocatable_createv(2, 3, 2);
  double **vec = (double**)softc_allocatable_data(ref);
  vec[0][0] = 0.0;
  vec[1][0] = 1.0;
  softc_allocatable_free(ref);
}

TEST_F(AllocatableTest, rank3)
{
  auto ref = softc_allocatable_createv(3, 3, 2, 3);
  double ***vec = (double***)softc_allocatable_data(ref);
  vec[0][0][0] = 0.0;
  vec[1][0][1] = 1.0;
  softc_allocatable_free(ref);
}

TEST_F(AllocatableTest, rank4)
{
  auto ref = softc_allocatable_createv(4, 3, 2, 3, 3);
  double ****vec = (double****)softc_allocatable_data(ref);
  vec[0][0][0][0] = 0.0;
  vec[1][0][1][0] = 1.0;
  softc_allocatable_free(ref);
}

TEST_F(AllocatableTest, dimensions)
{
  size_t *dims;
  size_t rank;
  auto ref = softc_allocatable_createv(3, 300, 200, 89);
  softc_allocatable_dimensions(ref, &rank, &dims);
  ASSERT_EQ(dims[0], 300);
  ASSERT_EQ(dims[1], 200);
  ASSERT_EQ(dims[2], 89);
  ASSERT_EQ(rank, 3);
  softc_allocatable_free(ref);
}

TEST_F(AllocatableTest, createAndReshape)
{
  const size_t dims[2] = {3,4};
  const size_t dims2[1] = {12};
  const size_t rank = 2;
  auto alloc = softc_allocatable_create(rank, dims);
  double** vec = (double**)softc_allocatable_data(alloc);
  const double num = 1.1;

  vec[1][1] = num;
  
  softc_allocatable_reshape(alloc, 1, dims2);
  double *vec1 = (double*)softc_allocatable_data(alloc);

  ASSERT_EQ(vec1[4], num);
  softc_allocatable_free(alloc);
}

#if 0




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

static bool set_vector(double ***vec)
{
  if (*vec != NULL) return false;
  *vec = (double**)softc_allocatable_allocatev(2, 30, 20);
}


static bool set_vector1(double **vec)
{
  if (*vec != NULL) return false;
  *vec = (double*)softc_allocatable_allocatev(1, 30);
}

static bool set_vector2(double ***vec)
{
  if (*vec != NULL) return false;
  *vec = (double**)softc_allocatable_allocatev(2, 3, 2);
}

TEST_F(AllocatableTest, copy)
{
  double **vec = NULL;
  bool isok = set_vector(&vec);
  ASSERT_TRUE(isok);
  ASSERT_EQ(softc_allocatable_rank(vec), 2);
  softc_allocatable_free(vec);
}

size_t numElements(const size_t *dims, size_t rank)
{
  size_t n = 1;
  for (size_t i = 0; i < rank; ++i) {
    n *= dims[i];
  }
  return n;
}

TEST_F(AllocatableTest, toStdVector1D)
{
  double *vec = NULL;
  bool isok = set_vector1(&vec);
  ASSERT_TRUE(isok);
  ASSERT_EQ(softc_allocatable_rank(vec), 1);
  
  size_t *dims;
  size_t rank;
  softc_allocatable_dimensions(vec, &dims, &rank);
  size_t n = numElements(dims, rank);

  for (size_t i = 0; i < n; ++i) {
    vec[i] = 2.13 * i;
  }

  //std::vector<double> stdvec (vec, vec + n);
  for (size_t i = 0; i < n; ++i) {
    //ASSERT_EQ(stdvec[i], vec[i]);
  }

  softc_allocatable_free(vec);
  free(dims);
}

TEST_F(AllocatableTest, toStdVector2D)
{
  double **vec = NULL;
  bool isok = set_vector2(&vec);
  ASSERT_TRUE(isok);
  ASSERT_EQ(softc_allocatable_rank(vec), 2);
  
  size_t *dims;
  size_t rank;
  softc_allocatable_dimensions(vec, &dims, &rank);
  for (size_t j = 0; j < dims[1]; ++j) {
    for (size_t i = 0; i < dims[0]; ++i) {
      vec[j][i] = 2.3 * i + 2.1 * j;
    }
  }
  
  std::vector< std::vector<double> > stdvec(dims[1]);
  for (size_t j = 0; j < dims[1]; ++j) {
    stdvec[j].assign(vec[j], vec[j] + dims[0]);
  }
  
  free(dims);			      
  softc_allocatable_free(vec);
}
#endif
