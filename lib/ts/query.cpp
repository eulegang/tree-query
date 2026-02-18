#include "ts.h"
#include <format>

ts::QueryException::QueryException(uint32_t offset, TSQueryError error)
    : _offset{offset}, _error{error} {

  std::string ty;
  switch (error) {
  case TSQueryErrorNone:
    ty = "none";
    break;
  case TSQueryErrorSyntax:
    ty = "syntax";
    break;
  case TSQueryErrorNodeType:
    ty = "node_type";
    break;
  case TSQueryErrorField:
    ty = "field";
    break;
  case TSQueryErrorCapture:
    ty = "capture";
    break;
  case TSQueryErrorStructure:
    ty = "structure";
    break;
  case TSQueryErrorLanguage:
    ty = "lang";
    break;
  }

  _message = std::format("invalid query @{} {}", offset, ty);
}

ts::query::query(const ts::lang &l, std::string_view source) {
  uint32_t offset;
  TSQueryError error = TSQueryErrorNone;

  _query = ts_query_new(l._lang, source.data(), source.size(), &offset, &error);

  if (error != TSQueryErrorNone) {
    throw QueryException(offset, error);
  }
}

ts::query::query(query &&q) {
  _query = q._query;
  q._query = nullptr;
}

ts::query &ts::query::operator=(query &&q) {
  this->_query = q._query;
  q._query = nullptr;
  return *this;
}

ts::query::operator bool() { return _query != nullptr; }

ts::query::~query() {
  if (_query) {
    ts_query_delete(_query);
    _query = nullptr;
  }
}
