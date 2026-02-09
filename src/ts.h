#ifndef _TQ_TS_H
#define _TQ_TS_H

#include <filesystem>
#include <optional>
#include <string>
#include <tree_sitter/api.h>
namespace ts {

class tree final {
  TSTree *_tree;
  friend class parser;

  tree(TSTree *tree) : _tree{tree} {}

public:
  ~tree();
};

class parser final {
  TSParser *_parser;

public:
  parser(const TSLanguage *lang);
  ~parser();

  tree parse(std::string_view content);
};

struct symbol {
  enum class type {
    Regular,
    Anonymous,
    Supertype,
    Auxiliary,
  };
  const char *name;
  type ty;
  TSSymbol id;
};

class symbol_iterator final {
  int cur;
  const TSLanguage *_lang;

public:
  symbol_iterator(int cur, const TSLanguage *lang) : cur{cur}, _lang{lang} {}

  symbol_iterator *operator++() {
    cur++;
    return this;
  }

  symbol operator*() const {
    const char *name = ts_language_symbol_name(_lang, cur);
    const auto ty = ts_language_symbol_type(_lang, cur);

    return symbol{.name = name, .ty = (symbol::type)ty, .id = (TSSymbol)cur};
  }

  bool operator!=(const symbol_iterator other) const {
    return cur != other.cur;
  }
};

class symbols final {
  int cur;
  const TSLanguage *_lang;

  friend class lang;
  symbols(int cur, const TSLanguage *lang) : cur{cur}, _lang{lang} {}

public:
  symbol_iterator begin() { return symbol_iterator(0, _lang); }
  symbol_iterator end() { return symbol_iterator(cur, _lang); }
};

class lang final {
  std::string _name;
  const TSLanguage *_lang;
  void *_handle;

public:
  lang(std::filesystem::path path, std::string_view name);
  ~lang();

  std::optional<TSLanguageMetadata> metadata();
  symbols list_symbols();
};
} // namespace ts

std::ostream &operator<<(std::ostream &, const TSLanguageMetadata &);
#endif
