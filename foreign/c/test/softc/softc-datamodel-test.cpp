#include <gtest/gtest.h>
#include <softc/softc.h>

class DatamodelTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}    
};


typedef struct {
  const char *uuid;
  softc_storage_t *storage;
  softc_storage_strategy_t *strategy;
  softc_datamodel_t *datamodel;
} Model;


Model *create_model(const char *uuid)
{
  Model *m = (Model *)calloc(1, sizeof(Model));
  m->storage = softc_storage_create("hdf5", "datamodel-test.h5", "");
  m->strategy = softc_storage_get_storage_strategy(m->storage);
  m->datamodel = softc_storage_strategy_get_datamodel(m->strategy);
  softc_datamodel_set_id(m->datamodel, uuid);
  softc_datamodel_set_meta_name(m->datamodel, "softc-datamodel-test");
  softc_datamodel_set_meta_version(m->datamodel, "0.1");
  softc_datamodel_set_meta_namespace(m->datamodel, "http://sintef.no/SOFT");
  return m;
}

void free_model(Model *m)
{
  softc_storage_strategy_free_datamodel(m->datamodel);
  softc_storage_free_storage_strategy(m->strategy);
  softc_storage_free(m->storage);
}




TEST_F(DatamodelTest, store_and_load)
{
  int i, j, k;
  double v;
  bool stat;
  Model *m;
  const char *uuid;
  
  char const *string = "This is a test...";
  char const *strlist[] = {"first", "second", "third"};
  double dbl = 3.14;
  double dbl_array[] = {0.0, 1.0, 2.0, 3.0};
  double **dbl_array2;
  double ***dbl_array3;

  
  FILE *fp = fopen("datamodel-test.log", "w");
  
  /* dbl_array2 */
  fprintf(fp, "\ndbl_array2\n");
  double data2[5][3];
  double *ptr2[5];
  for (i=0, v=0.0; i<5; i++) {
    for (j=0; j<3; j++) {
      data2[i][j] = v;
      v += 1.0;
    }
    ptr2[i] = &data2[i][0];
  }
  dbl_array2 = ptr2;

  for (i=0; i<5; i++)
    for (j=0; j<3; j++)
      fprintf(fp, "%d,%d -> %-.1f (+%ld)\n", i, j, dbl_array2[i][j],
	     (char *)(&dbl_array2[i][j]) - (char *)data2);
  fflush(fp);

  
  /* dbl_array3 */
  fprintf(fp, "\ndbl_array3\n");
  double data3[2][3][5];
  double *ptr3ij[2][3];
  double **ptr3i[2];
  for (i=0, v=0.0; i<2; i++) {
    for (j=0; j<3; j++) {
      for (k=0; k<5; k++) {
	data3[i][j][k] = v;
	v += 1.0;
      }
      ptr3ij[i][j] = &data3[i][j][0];
    }
    ptr3i[i] = &ptr3ij[i][0];
  }
  dbl_array3 = ptr3i;

  for (i=0; i<2; i++)
    for (j=0; j<3; j++)
      for (k=0; k<5; k++)
        fprintf(fp, "%d,%d,%d -> %-.1f (+%ld)\n", i, j, k, dbl_array3[i][j][k], 
	       (char *)(&dbl_array3[i][j][k]) - (char *)data3);
  fclose(fp);


  /* store */
  uuid = softc_uuidgen();                                                                                           ASSERT_TRUE(uuid);
  m = create_model(uuid);                                                                                           ASSERT_TRUE(m);
  stat = softc_datamodel_append_string(m->datamodel, "string", string);                                             ASSERT_TRUE(stat);
  stat = softc_datamodel_append_string_list(m->datamodel, "strlist", strlist, 3);                                   ASSERT_TRUE(stat);
  stat = softc_datamodel_append_double(m->datamodel, "dbl", dbl);                                                   ASSERT_TRUE(stat);
  stat = softc_datamodel_append_array_double(m->datamodel, "dbl_array", dbl_array, 4);                              ASSERT_TRUE(stat);
  stat = softc_datamodel_append_array_double_2d(m->datamodel, "dbl_array2", (const double **)dbl_array2, 5, 3);     ASSERT_TRUE(stat);
  //stat = softc_datamodel_append_array_double_3d(m->datamodel, "dbl_array3", (const double ***)dbl_array3, 2, 3, 5); ASSERT_TRUE(stat);
  softc_storage_strategy_store(m->strategy, m->datamodel);
  free_model(m);
  

  /* load */
  size_t ni, nj, nk;
  char *new_string;
  char **new_strlist;
  double new_dbl;
  double *new_dbl_array;
  double **new_dbl_array2;
  double ***new_dbl_array3;
  
  m = create_model(uuid);
  softc_storage_strategy_start_retrieve(m->strategy, m->datamodel);

  stat = softc_datamodel_get_string(m->datamodel, "string", &new_string);
  ASSERT_TRUE(stat);
  ASSERT_EQ(strcmp(string, new_string), 0);

  stat = softc_datamodel_get_string_list(m->datamodel, "strlist", &new_strlist, &ni);
  ASSERT_TRUE(stat);
  ASSERT_EQ(3, ni);
  for (i=0; i<3; i++) ASSERT_EQ(strcmp(strlist[i], new_strlist[i]), 0);

  stat = softc_datamodel_get_double(m->datamodel, "dbl", &new_dbl);
  ASSERT_TRUE(stat);
  ASSERT_EQ(dbl, new_dbl);

  stat = softc_datamodel_get_array_double(m->datamodel, "dbl_array", &new_dbl_array, &ni);
  ASSERT_TRUE(stat);
  ASSERT_EQ(4, ni);
  for (i=0; i<3; i++) ASSERT_EQ(dbl_array[i], new_dbl_array[i]);

  stat = softc_datamodel_get_array_double_2d(m->datamodel, "dbl_array2", &new_dbl_array2, &ni, &nj);
  ASSERT_TRUE(stat);
  ASSERT_EQ(5, ni);
  ASSERT_EQ(3, nj);
  for (i=0; i<5; i++)
    for (j=0; i<3; j++)
      ASSERT_EQ(dbl_array2[i][j], new_dbl_array2[i][j]);

  //stat = softc_datamodel_get_array_double_3d(m->datamodel, "dbl_array3", &new_dbl_array3, &ni, &nj, &nk);
  //ASSERT_TRUE(stat);
  //ASSERT_EQ(2, ni);
  //ASSERT_EQ(3, nj);
  //ASSERT_EQ(5, nj);
  //for (i=0; i<2; i++)
  //  for (j=0; i<3; j++)
  //    for (k=0; k<5; k++)
  //	ASSERT_EQ(dbl_array3[i][j][k], new_dbl_array3[i][j][k]);

  softc_storage_strategy_end_retrieve(m->strategy, m->datamodel);
  free_model(m);
}
