#include "cli.h"
#include "registry.h"

struct state {
  registry reg;
  mapping map;
};

void action_list(state &state, Cli &cli);
void action_info(state &state, Cli &cli);
void action_tree(state &state, Cli &cli);
