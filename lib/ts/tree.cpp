#include <tree_sitter/api.h>

#include "ts.h"

ts::tree::~tree() {
  if (_tree) {
    ts_tree_delete(_tree);
  }
}

ts::tree::operator std::string() {
  TSNode node = ts_tree_root_node(_tree);

  char *mem = ts_node_string(node);
  std::string repr{mem};
  free(mem);

  return repr;
}
