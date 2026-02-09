#include "tree_sitter/api.h"
#include "ts.h"

ts::parser::parser(lang &lang) {
  _parser = ts_parser_new();

  if (!_parser)
    throw std::bad_alloc();

  ts_parser_set_language(_parser, lang._lang);
}

ts::parser::~parser() { ts_parser_delete(_parser); }

ts::tree ts::parser::parse(std::string_view content) {
  TSTree *inner =
      ts_parser_parse_string(_parser, NULL, content.data(), content.length());

  return tree(inner);
}
