#include <gtest/gtest.h>

#include "registry.h"

TEST(Mapping, cpp_file_should_map_to_cpp_grammer) {
  mapping map;
  std::filesystem::path filename{"main.cpp"};

  EXPECT_EQ(map.resolve(filename), "cpp");
}

TEST(Mapping, unregistered_lang_should_not_be_recognized) {
  mapping map;
  std::filesystem::path filename{"main.pl"};

  EXPECT_EQ(map.resolve(filename), std::nullopt);
}

TEST(RevMapping, should_match_fileext) {
  mapping map;
  {
    auto rev = map.rev("cpp");
    std::filesystem::path header{"main.h"};
    std::filesystem::path impl{"main.cpp"};
    std::filesystem::path neg_impl{"main.c"};

    EXPECT_TRUE(rev.applies(header));
    EXPECT_TRUE(rev.applies(impl));
    EXPECT_FALSE(rev.applies(neg_impl));
  }
  {
    auto rev = map.rev("c");
    std::filesystem::path header{"main.h"};
    std::filesystem::path impl{"main.c"};
    std::filesystem::path neg_impl{"main.cpp"};

    EXPECT_TRUE(rev.applies(header));
    EXPECT_TRUE(rev.applies(impl));
    EXPECT_FALSE(rev.applies(neg_impl));
  }
}
