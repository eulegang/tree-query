#include <dlfcn.h>
#include <format>
#include <tree_sitter/api.h>

#include "ts.h"

using LangHandle = const TSLanguage *(*)();

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

ts::lang::~lang() { dlclose(_handle); }
