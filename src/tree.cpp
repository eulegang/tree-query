#include "cli.h"
#include "map_file.h"
#include "registry.h"
#include <iostream>

void action_tree(registry &reg, Cli &cli) {
  map_file file(cli.arg);

  ts::lang lang = reg.load(cli.type);
  ts::parser parser{lang};
  ts::tree tree = parser.parse(file);

  std::cout << (std::string)tree << std::endl;
}
