#include "actions.h"
#include "cli.h"
#include "mfile.h"
#include "registry.h"
#include <filesystem>
#include <iostream>

void action_tree(state &state, Cli &cli) {
  mfile file(cli.arg);

  std::filesystem::path path = {cli.arg};
  if (const auto ty = state.map.resolve(path); ty) {
    std::cout << *ty << std::endl;
    ts::lang lang = state.reg.load(cli.type);
    ts::parser parser{lang};
    ts::tree tree = parser.parse(file);
    std::cout << (std::string)tree << std::endl;
  };
}
