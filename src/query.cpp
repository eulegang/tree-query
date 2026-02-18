#include "actions.h"
#include <filesystem>
#include <iostream>
#include <mfile.h>

void action_query(state &state, Cli &cli) {
  mfile file{cli.arg};
  ts::lang lang = state.reg.load(cli.type);
  auto pred = state.map.rev(cli.type);
  ts::parser parser{lang};
  ts::query query{lang, file};

  for (const auto &ent : std::filesystem::recursive_directory_iterator(
           std::filesystem::current_path())) {

    if (!ent.is_regular_file())
      continue;

    if (!pred.applies(ent.path()))
      continue;

    mfile source{ent.path()};
    std::cerr << "parsing " << ent.path() << std::endl;

    ts::tree tree = parser.parse(source);
    ts::cursor cursor;

    cursor.exec(query, tree);

    for (const auto &match : cursor) {
      int s = -1, e = -1;

      for (const auto &node : match) {
        if (s == -1) {
          s = node.start_byte();
        } else {
          s = std::min(s, (int)node.start_byte());
        }

        if (e == -1) {
          e = node.end_byte();
        } else {
          e = std::min(e, (int)node.end_byte());
        }
      }

      if (s != -1 && e != -1) {
        std::string_view view = source;
        auto content = view.substr(s, e - s);

        std::cout << ent.path() << std::endl;
        std::cout << content << std::endl;
      }
    }
  }
}
