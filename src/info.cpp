#include <iostream>

#include "cli.h"
#include "registry.h"

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
