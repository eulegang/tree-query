#include "actions.h"
#include <iostream>

void action_list(state &state, Cli &cli) {
  (void)cli;
  for (const auto &type : state.reg.types()) {
    std::cout << type << std::endl;
  }
}
