#ifndef _TQ_REGISTRY_H
#define _TQ_REGISTRY_H

#include "ts.h"
#include <filesystem>
#include <string>
#include <tree_sitter/api.h>
#include <vector>

struct registry {
  std::vector<std::filesystem::path> paths;
  registry();

  std::vector<std::string> types();

  ts::lang load(std::string_view);
};

#endif
