#include <tree_sitter/api.h>

#include "ts.h"

ts::tree::~tree() {
  if (_tree) {
    ts_tree_delete(_tree);
  }
}
