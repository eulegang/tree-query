#include "tree_sitter/api.h"
#include "ts.h"

ts::cursor::cursor() { _cursor = ts_query_cursor_new(); }
ts::cursor::~cursor() { ts_query_cursor_delete(_cursor); }

void ts::cursor::exec(const ts::query &query, const ts::tree &tree) {
  ts_query_cursor_exec(_cursor, query._query, ts_tree_root_node(tree._tree));
}

ts::matches_iter ts::cursor::begin() const {
  matches_iter iter;
  iter._cursor = _cursor;
  iter._valid = ts_query_cursor_next_match(_cursor, &iter._match);
  return iter;
}

ts::matches_iter ts::cursor::end() const {
  matches_iter iter;
  iter._valid = false;
  iter._cursor = _cursor;
  return iter;
}

bool ts::matches_iter::operator==(const matches_iter &other) const {
  return other._valid == _valid && other._cursor == _cursor;
}

ts::match ts::matches_iter::operator*() const {
  ts::match m;
  m._match = _match;
  return m;
}

ts::matches_iter ts::matches_iter::operator++() {
  ts::matches_iter old{*this};

  _valid = ts_query_cursor_next_match(_cursor, &_match);

  return old;
}

ts::capture_iter ts::match::begin() const {
  return capture_iter(_match.captures, 0);
}

ts::capture_iter ts::match::end() const {
  return capture_iter(_match.captures, _match.capture_count);
}

ts::capture_iter ts::capture_iter::operator++() {
  ts::capture_iter old{*this};
  pos++;
  return old;
}

bool ts::capture_iter::operator==(const capture_iter &other) const {
  return pos == other.pos;
}

const ts::node ts::capture_iter::operator*() const { return base[pos].node; }
