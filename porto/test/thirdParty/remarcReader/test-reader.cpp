#include <gtest/gtest.h>
#include <remarcReader.h>

TEST(RemarcReaderTest, OpenAndRead)
{
  IO::RemarcReader reader("remarc.in");
  reader.read();
}
