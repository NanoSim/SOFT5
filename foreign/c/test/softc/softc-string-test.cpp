#include <gtest/gtest.h>
#include <softc/softc-string.h>
#include <softc/softc-string-list.h>


TEST(SoftC_StringTest, dummy)
{
  ASSERT_FALSE(false);
}

TEST(SoftC_StringTest, create)
{
  softc_string_s text = softc_string_create("The quick brown fox jumps over the lazy dog");
  const char *buff = from_softc_string(text);
  ASSERT_STREQ(buff, "The quick brown fox jumps over the lazy dog");
  softc_string_destroy(text);
}

TEST(SoftC_StringListTest, create) 
{
  auto lst = softc_string_list_create();
  ASSERT_TRUE(lst != nullptr);
  ASSERT_EQ(softc_string_list_count(lst), 0);
  softc_string_list_free(lst);
}

TEST(SoftC_StringListTest, append) 
{
  auto lst = softc_string_list_create();
  auto str = softc_string_create("Hello");
  ASSERT_TRUE(lst != nullptr);
  ASSERT_TRUE(str != nullptr);

  softc_string_list_append(lst, str);
  ASSERT_EQ(softc_string_list_count(lst), 1);  
}
