#include "ts.h"

const char *ts::node::node_type() const { return ts_node_type(_node); }

uint32_t ts::node::start_byte() const { return ts_node_start_byte(_node); }
uint32_t ts::node::end_byte() const { return ts_node_end_byte(_node); }

ts::point ts::node::start_point() const { return ts_node_start_point(_node); }
ts::point ts::node::end_point() const { return ts_node_end_point(_node); }
