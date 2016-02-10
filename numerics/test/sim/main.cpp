#include <gtest/gtest.h>
#include "args.h"

int t_argc;
char **t_argv;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  t_argc = argc;
  t_argv = argv;
  return RUN_ALL_TESTS();
}
