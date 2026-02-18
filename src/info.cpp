#include <iostream>

#include "actions.h"
#include "cli.h"

void action_info(state &state, Cli &cli) {
  ts::lang lang = state.reg.load(cli.arg);

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
