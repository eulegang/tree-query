#include "registry.h"
#include "tree_sitter/api.h"

#include <gtest/gtest.h>

TEST(registry, resolve_cpp) {
  registry reg;

  ts::lang l = reg.load("c");

  auto metadata = l.metadata();
  // using personal version for now
  TSLanguageMetadata md = {
      .major_version = 0,
      .minor_version = 24,
      .patch_version = 1,

  };

  ASSERT_TRUE(metadata);
  EXPECT_EQ(metadata->major_version, md.major_version);
  EXPECT_EQ(metadata->minor_version, md.minor_version);
  EXPECT_EQ(metadata->patch_version, md.patch_version);
}
