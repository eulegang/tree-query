#include <iostream>

#include "actions.h"
#include "cli.h"
#include "registry.h"

int main(int argc, char *argv[]) {
  try {

    Cli cli(argc, argv);
    registry reg;
    mapping map;

    if (cli.help) {
      std::cerr << "tq [info | list]" << std::endl;
      return 0;
    }

    if (cli.version) {
      std::cerr << "pre-beta" << std::endl;
    }

    state state{
        .reg = reg,
        .map = map,
    };

    switch (cli.action) {
    case Cli::Action::List:
      action_list(state, cli);
      break;
    case Cli::Action::Info:
      action_info(state, cli);
      break;
    case Cli::Action::Tree:
      action_tree(state, cli);
      break;
    }
  } catch (const Cli::Exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
