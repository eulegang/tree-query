#include <dlfcn.h>
#include <format>
#include <iostream>
#include <tree_sitter/api.h>

#include "ts.h"

using LangHandle = const TSLanguage *(*)();

ts::tree::~tree() {
  if (_tree) {
    ts_tree_delete(_tree);
  }
}

ts::parser::parser(const TSLanguage *lang) {
  _parser = ts_parser_new();

  if (!_parser)
    throw std::bad_alloc();

  ts_parser_set_language(_parser, lang);
}

ts::parser::~parser() { ts_parser_delete(_parser); }

ts::lang::lang(std::filesystem::path path, std::string_view name)
    : _name{name} {
  _handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
  if (_handle == NULL) {
    throw std::runtime_error(dlerror());
  }

  auto sym = std::format("tree_sitter_{}", name);
  LangHandle lang = (LangHandle)dlsym(_handle, sym.c_str());
  _lang = lang();
}

ts::lang::~lang() {
  ts_language_delete(_lang);
  dlclose(_handle);
}

ts::symbols ts::lang::list_symbols() {
  int len = ts_language_symbol_count(_lang);

  return ts::symbols(len, _lang);
}

std::optional<TSLanguageMetadata> ts::lang::metadata() {
  auto md = ts_language_metadata(_lang);
  if (md) {
    return *md;
  } else {
    return std::nullopt;
  }
}

std::ostream &operator<<(std::ostream &os, const TSLanguageMetadata &metadata) {
  return os << (int)metadata.major_version << "." << (int)metadata.minor_version
            << "." << (int)metadata.patch_version;
}
