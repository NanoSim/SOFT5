#include <gtest/gtest.h>
#include <softc/softc-block.h>

class BlockTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}
};

TEST_F(BlockTest, construct0)
{
  const size_t dims[3] = {3,4,3};
  softc_block *block = softc_block_alloc(3, dims);
  ASSERT_TRUE(block != NULL);

  softc_block_free(block);
}

TEST_F(BlockTest, construct1)
{
  const size_t dims[3] = {3,4,3};
  softc_block *block = softc_block_alloc(3, dims);
  ASSERT_TRUE(block != NULL);


  softc_block_free_zero(&block);
  ASSERT_TRUE(block == NULL);
}

TEST_F(BlockTest, rankAndDims)
{
  const size_t rank = 3;
  const size_t dims[rank] = {3,4,3};
  size_t var_rank;
  size_t *var_dims;
  double *var_data;

  softc_block *block = softc_block_alloc(rank, dims);
  ASSERT_TRUE(block != NULL);
  ASSERT_EQ(softc_block_rank(block), rank);

  softc_block_dims(block, &var_dims, &var_rank);
  ASSERT_EQ(var_rank, rank);
  ASSERT_EQ(var_dims[0], dims[0]);
  ASSERT_EQ(var_dims[1], dims[1]);
  ASSERT_EQ(var_dims[2], dims[2]);

  softc_block_free(block);
}

TEST_F(BlockTest, data)
{
  const size_t dims[2] = {1000,2000};
  softc_block *block = softc_block_alloc(2, dims);
  double *ref;
  ref = (double*)softc_block_data(block);

  ASSERT_TRUE(ref != NULL);
  softc_block_free(block);
}
