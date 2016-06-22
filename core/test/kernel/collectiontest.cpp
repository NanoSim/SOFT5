#include <memory>
#include <gtest/gtest.h>
#include <soft.h>
#include <collection.h>
#include <istoragestrategy.h>
#include <idatamodel.h>
#include <json/jsonmodel.h>
#include "collection_test_entity.h"
#include <softc/softc-entity.h>

//static const char * driver  = "mongo";
static const char * driver = "json";
static const char * uri    = "output.json";
static const char * options= "";

//static const char * uri     = "mongodb://localhost";
//static const char * options = "db=soft;coll=testa";

class CollectionTest : public ::testing::Test {
protected:
  CollectionTest() {
  }

  virtual ~CollectionTest(){
  }

  static void SetUpTestCase() {
    printf("SetUpTestCase");
    storageStrategy_ = soft::createStrategy(driver, uri, options);
    printf("%s\n", storageStrategy_->metaType());
  }

  static void TearDownTestCase() {
    delete storageStrategy_;
    storageStrategy_ = nullptr;
  }

  virtual void SetUp(){
  }

  virtual void TearDown(){
  }

  static soft::IStorageStrategy *storageStrategy_;
};

soft::IStorageStrategy *CollectionTest::storageStrategy_ = nullptr;

TEST_F(CollectionTest, Constructor1) {
  soft::Collection *coll = new soft::Collection();
  ASSERT_TRUE(NULL != coll);
}

TEST_F(CollectionTest, Constructor2) {
  const char *uuid = "6b3827f8-1386-481d-a6cb-396e07575666";
  soft::Collection *coll = new soft::Collection(uuid);
  ASSERT_TRUE(NULL != coll);
  delete coll;
}

TEST_F(CollectionTest, Constructor3) {
  soft::Collection *orig = new soft::Collection();
  soft::Collection *coll = new soft::Collection(orig);
  ASSERT_TRUE(NULL != coll);
  delete coll;
  delete orig;
}

TEST_F(CollectionTest, StaticCreate) {
  const char *uuid = "88bf2784-5939-42e9-8b95-2626d21b1c07";
  soft::Collection *coll = (soft::Collection*)soft::Collection::create(uuid);
  ASSERT_TRUE(NULL != coll);
  delete coll;
}


TEST_F(CollectionTest, metaType) {
  /*
  const char *uuid = "bb1bb66e-fa49-40a2-8796-6e2ba6e3c50f";

  soft::IEntity *e = soft::Collection::create(uuid);
  ASSERT_STREQ("http://emmc.eu/TR/metadata-entity#Collection:1.0-SNAPSHOT-4", e->metaType());
  delete e;
  */
}

TEST_F(CollectionTest, save1) {
  soft::IEntity *e = soft::Collection::create();
  ASSERT_TRUE(e != NULL);

  ASSERT_TRUE(storageStrategy_ != NULL);
  ASSERT_STREQ("http://sintef.no/soft/TR/storage-strategy#json:0.1-SNAPSHOT-1", storageStrategy_->metaType());

  // soft::IDataModel const* model = CollectionTest::storageStrategy_->dataModel();
  //  ASSERT_TRUE(model != NULL);
  //  auto model = storageStrategy_->dataModel();
  //  ASSERT_TRUE(model != NULL);

  //  model->appendInt32("test", 42);
  //  storageStrategy_->store(model);

  delete e;
}

TEST_F(CollectionTest, load1) {
  soft::IEntity *e = soft::Collection::create();
  ASSERT_TRUE(e != NULL);

  //  auto model = storageStrategy_->dataModel();
  //  ASSERT_TRUE(model != NULL);
  delete e;
}

TEST_F(CollectionTest, registerEntity) {
  soft::IEntity *e = soft::Collection::create();
  ASSERT_TRUE(e != NULL);

  soft::Collection collection;
  collection.registerEntity("label1", e);
  ASSERT_EQ(1, collection.numEntities());
}

TEST_F(CollectionTest, collection) {
  soft::Collection collection;
  collection.addRelation("label1", "inherits", "label2");
  collection.addRelation("label2", "inherits", "label3");
}

TEST_F(CollectionTest, registerAndFetch) {
  soft::Collection subCollection;
  soft::Collection collection;
  collection.registerEntity("subCollection", &subCollection);
  collection.connect("subCollection", "is", "subCollection");

  soft::IEntity const *e = collection.findInstance("subCollection");
  ASSERT_TRUE(e != NULL);
  ASSERT_STREQ(e->id().c_str(), subCollection.id().c_str());
}

TEST_F (CollectionTest, DISABLED_findConnections1) {
  // TODO: Presently disabled, pending API change for connections and
  // relations. See test below.

  soft::Collection a;
  soft::Collection b;
  soft::Collection collection;
  collection.registerEntity("sub", &a);
  collection.registerEntity("obj", &b);
  collection.connect("sub", "has-a", "obj");
  auto p = collection.findRelations("sub");

  ASSERT_EQ(1, p.size());
  ASSERT_STREQ(p.front().object().c_str(), "obj");
  ASSERT_STREQ(p.front().predicate().c_str(), "has-a");
}

TEST_F (CollectionTest, DISABLED_findConnections2) {
  soft::Collection a;
  soft::Collection b;
  soft::Collection collection;
  collection.registerEntity("sub", &a);
  collection.registerEntity("obj", &b);
  collection.connect("sub", "has-a", "obj");
  collection.connect("obj", "owned-by", "sub");

  /*
  TODO: We must resolve this API change
  subj ----parent-of----> obj
  obj ----child-of----> subj

  collection.connect("obj1", "subj");
  collection.connect("obj2", "subj");
  collection.findConnections("subj")

  auto p = collection.findRelations("sub");
  auto p = collection.findRelations("connected-to");

  auto p = collection.findConnections("subj");
  auto p = collection.findReverseConnections("obj"); // Name can be revised
  */
  auto p = collection.findRelations("sub");

  ASSERT_EQ(1, p.size());
  ASSERT_STREQ(p.front().object().c_str(), "obj");
  ASSERT_STREQ(p.front().predicate().c_str(), "has-a");
}

TEST(Collection, instanciateFromDataModel) {

  soft::Collection recepie;
  soft::Collection baking_log;

  soft::Collection mums_cookies;

  // Confirm that we have been able to populate the collection.
  mums_cookies.setName("Mums best cookies!");
  mums_cookies.setVersion("1-with-some-improvements");
  ASSERT_EQ("Mums best cookies!", mums_cookies.name());
  ASSERT_EQ("1-with-some-improvements", mums_cookies.version());

  mums_cookies.registerEntity("cookie-recepie", &recepie);
  mums_cookies.registerEntity("cooking-competition-baking-log", &baking_log);
  ASSERT_EQ(2, mums_cookies.numEntities()) << "mums_cookies numEntities";

  mums_cookies.addRelation("cookie-recepie", "loged-in", "cooking-competition-baking-log");
  // TODO: Relations not yet implemented
  // ASSERT_EQ(1, mums_cookies.numRelations());

  // TODO: Can this fail?
  soft::JSONModel dm;
  mums_cookies.save((soft::IDataModel *)(&dm));

  // ... mum shelves her cookie activities and pursue other activities
  // while a generation passes. Until one day ...

  soft::Collection grandmas_cookies;
  grandmas_cookies.load(&dm);

  // Confirm that what we retrieve from the collection through the
  // data model is exactly what we sent in.
  ASSERT_EQ("Mums best cookies!", grandmas_cookies.name());
  ASSERT_EQ("1-with-some-improvements", grandmas_cookies.version());

  // TODO: Relations not yet implemented
  // ASSERT_EQ(1, grandmas_cookies.numRelations());

  ASSERT_EQ(2, grandmas_cookies.numEntities()) << "grandmas_cookies numEntities";


  // TODO: Check that I got a label here
}

/*
TEST(CollectionTest, instanciateFromDataModel2) {

  // FIXTURE:

  // Where is the entity code generation? See entities/ CMakeLists.txt

  collection_test_entity_s *e = collection_test_entity_create();

  // Note: We need to cast the entity to softc_entity_t. It seems to be safe!
  std::string uuid(softc_entity_get_id((softc_entity_t *)e));
  std::string name(softc_entity_get_meta_name((softc_entity_t *)e));

  soft::JSONModel jm();

  soft::Collection c(jm);
  std::string label{ "tubes" };

  c.addEntity(label, name, version, ns, uuid);

  c.store(jm);


  // TEST:
  soft::Collection d(jm);

  ASSERT_TRUE(c.hasEntity(label));
  std::string new_uuid = c.getEntityId(label);
  IDataModel *entity_idm = c.getEntityDataModel(label); // IDataModel or data_model_t ?
  data_model_t entity_dm = { entity_idm }; // TODO: Also might need a future conversion to/from IDataModel?

  tube_entity_s *e2 = tube_entity_create0(new_uuid.c_str());
  tube_entity_load(e2, &entity_dm);

  ASSERT_TRUE(nullptr != e2);
}
*/
