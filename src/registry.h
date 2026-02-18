#ifndef _TQ_REGISTRY_H
#define _TQ_REGISTRY_H

#include "ts.h"
#include <filesystem>
#include <map>
#include <string>
#include <tree_sitter/api.h>
#include <vector>

class mapping {
  std::map<std::string, std::string> exts;

public:
  mapping();

  std::optional<std::string> resolve(std::filesystem::path &);
};

class registry {
  std::vector<std::filesystem::path> paths;

public:
  registry();

  std::vector<std::string> types();

  ts::lang load(std::string_view);
};

#endif
