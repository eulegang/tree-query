#include "actions.h"
#include <iostream>

void action_list(registry &reg, Cli &cli) {
  (void)cli;
  for (const auto &type : reg.types()) {
    std::cout << type << std::endl;
  }
}
