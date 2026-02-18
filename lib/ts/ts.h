#ifndef _TQ_TS_H
#define _TQ_TS_H

#include <filesystem>
#include <optional>
#include <string>
#include <tree_sitter/api.h>

namespace ts {

struct point {
  uint32_t row;
  uint32_t col;

  point(TSPoint p) : row{p.row}, col{p.column} {}
};

class tree final {
  TSTree *_tree;
  friend class parser;
  friend class cursor;

  tree(TSTree *tree) : _tree{tree} {}

public:
  ~tree();

  operator std::string();
};

class node final {
  TSNode _node;
  node(TSNode node) : _node{node} {}

  friend class capture_iter;

public:
  const char *node_type() const;

  uint32_t start_byte() const;
  point start_point() const;
  uint32_t end_byte() const;
  point end_point() const;
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

  friend class parser;
  friend class query;

public:
  lang(std::filesystem::path path, std::string_view name);
  ~lang();

  std::optional<TSLanguageMetadata> metadata();
  symbols list_symbols();
};

class parser final {
  TSParser *_parser;

public:
  parser(lang &);
  ~parser();

  tree parse(std::string_view content);
};

class QueryException : public std::exception {
  std::string _message;
  uint32_t _offset;
  TSQueryError _error;

public:
  QueryException(uint32_t offset, TSQueryError error);
  const char *what() const noexcept override { return _message.c_str(); }
};

class query final {
  TSQuery *_query;

  friend class cursor;

public:
  query(const lang &, std::string_view);

  query(const query &) = delete;
  query &operator=(const query &) = delete;

  query(query &&);
  query &operator=(query &&);
  operator bool();

  ~query();
};

class capture_iter final {
  const TSQueryCapture *base;
  uint16_t pos;
  capture_iter(const TSQueryCapture *base, uint16_t pos)
      : base{base}, pos{pos} {}

  friend class match;

public:
  capture_iter operator++();
  bool operator==(const capture_iter &) const;
  const node operator*() const;
};

class match final {
  TSQueryMatch _match;

  friend class matches_iter;

public:
  capture_iter begin() const;
  capture_iter end() const;
};

class matches_iter final {
  bool _valid;
  TSQueryMatch _match;
  TSQueryCursor *_cursor;
  friend class cursor;

public:
  matches_iter operator++();
  match operator*() const;

  bool operator==(const matches_iter &) const;
};

class cursor final {
  TSQueryCursor *_cursor;

public:
  cursor();
  ~cursor();

  cursor(const cursor &) = delete;
  cursor &operator=(const cursor &) = delete;
  cursor(cursor &&) = delete;
  cursor &operator=(cursor &&) = delete;

  void exec(const query &, const tree &);

  matches_iter begin() const;
  matches_iter end() const;
};

} // namespace ts

std::ostream &operator<<(std::ostream &, const TSLanguageMetadata &);
#endif
