#include <iostream>

#include "cli.h"
#include "map_file.h"
#include "registry.h"
#include "ts.h"

void action_list(registry &reg, Cli &cli);
void action_info(registry &reg, Cli &cli);
void action_tree(registry &reg, Cli &cli);

int main(int argc, char *argv[]) {
  try {

    Cli cli(argc, argv);
    registry reg;

    if (cli.help) {
      std::cerr << "tq [info | list]" << std::endl;
      return 0;
    }

    if (cli.version) {
      std::cerr << "pre-beta" << std::endl;
    }

    switch (cli.action) {
    case Cli::Action::List:
      action_list(reg, cli);
      break;
    case Cli::Action::Info:
      action_info(reg, cli);
      break;
    case Cli::Action::Tree:
      action_tree(reg, cli);
      break;
    }
  } catch (Cli::Exception e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

void action_list(registry &reg, Cli &cli) {
  for (const auto &type : reg.types()) {
    std::cout << type << std::endl;
  }
}
void action_info(registry &reg, Cli &cli) {
  ts::lang lang = reg.load(cli.arg);

  auto md = lang.metadata();

  if (md) {
    std::cout << "version: " << *md << std::endl;
  } else {
    std::cout << "version: -" << std::endl;
  }

  if (cli.verbose) {
    for (const auto &sym : lang.list_symbols()) {
      if (sym.ty == ts::symbol::type::Regular) {
        std::cout << "  " << sym.name << std::endl;
      }
    }
  }
}

void action_tree(registry &reg, Cli &cli) {
  map_file file(cli.arg);

  ts::lang lang = reg.load(cli.type);
  ts::parser parser{lang};
  ts::tree tree = parser.parse(file);

  // std::cout << view << std::endl;
}
