#include "actions.h"
#include "cli.h"
#include "mfile.h"
#include "registry.h"
#include <filesystem>
#include <iostream>

void action_tree(state &state, Cli &cli) {
  mfile file(cli.arg);

  std::filesystem::path path = {cli.arg};

  std::string grammar;
  if (cli.type != "") {
    grammar = cli.type;
  } else if (const auto ty = state.map.resolve(path); ty) {
    grammar = *ty;
  } else {
    std::cerr << "grammar not specificed" << std::endl;
    exit(1);
  };

  ts::lang lang = state.reg.load(grammar);
  ts::parser parser{lang};
  ts::tree tree = parser.parse(file);
  std::cout << (std::string)tree << std::endl;
}
