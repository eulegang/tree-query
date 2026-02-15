#include "cli.h"
#include "mfile.h"
#include "registry.h"
#include <iostream>

void action_tree(registry &reg, Cli &cli) {
  mfile file(cli.arg);

  ts::lang lang = reg.load(cli.type);
  ts::parser parser{lang};
  ts::tree tree = parser.parse(file);

  std::cout << (std::string)tree << std::endl;
}
