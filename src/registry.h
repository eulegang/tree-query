#ifndef _TQ_REGISTRY_H
#define _TQ_REGISTRY_H

#include "ts.h"
#include <filesystem>
#include <string>
#include <tree_sitter/api.h>
#include <vector>

class ext_pred final {
  std::string _exts;

  friend class mapping;
  ext_pred(std::string exts) : _exts{exts} {}

public:
  bool applies(std::filesystem::path &) const;
};

class mapping {
  std::vector<std::tuple<std::string, std::string>> exts;

public:
  mapping();

  std::optional<std::string> resolve(std::filesystem::path &);
  ext_pred rev(const std::string &ty);
};

class registry {
  std::vector<std::filesystem::path> paths;

public:
  registry();

  std::vector<std::string> types();

  ts::lang load(std::string_view);
};

#endif
