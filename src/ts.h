#ifndef _TQ_TS_H
#define _TQ_TS_H

#include <filesystem>
#include <string>
#include <tree_sitter/api.h>
namespace ts {
class parser final {
  TSParser *_parser;

public:
  parser(const TSLanguage *lang);
  ~parser();
};

class lang final {
  std::string _name;
  const TSLanguage *_lang;
  void *_handle;

public:
  lang(std::filesystem::path path, std::string_view name);
  ~lang();
};
} // namespace ts

#endif
