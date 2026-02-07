#include "cli.h"
#include <initializer_list>

struct Processor {
  enum class State {
    Init,
    Action,
    Arg,
    Flags,
    Ignore,
  };

  Cli *cli;
  State state;

  Processor(Cli *cli) : cli{cli}, state{State::Init} {}

  void accept(std::string_view arg) {
    switch (state) {
    case State::Init:
      state = State::Action;
      break;

    case State::Action:
      if (arg == "list") {
        cli->action = Cli::Action::List;
        state = State::Flags;
      } else if (arg == "info") {
        cli->action = Cli::Action::Info;
        state = State::Arg;
      } else if (arg == "-h" || arg == "--help") {
        cli->help = true;
        state = State::Ignore;
      } else if (arg == "-V" || arg == "--version") {
        cli->version = true;
        state = State::Ignore;
      }
      break;

    case State::Arg:
      cli->arg = arg;
      state = State::Flags;

      break;

    case State::Flags:
      if (arg == "--verbose" || arg == "-v") {
        cli->verbose = true;
      }

      break;

    case State::Ignore:
      break;
    }
  }

  void finalize() {
    if (cli->action == Cli::Action::Info && cli->arg.empty()) {
      throw Cli::Exception("need language argument");
    }
  }
};

Cli::Cli(int argc, char *argv[])
    : action{Action::List}, help{false}, version{false}, verbose{false} {
  Processor proc(this);

  for (int i{}; i < argc; i++) {
    proc.accept(argv[i]);
  }

  proc.finalize();
}

Cli::Cli(std::initializer_list<std::string_view> args)
    : action{Action::List}, help{false}, version{false}, verbose{false} {
  Processor proc(this);

  for (const auto arg : args) {
    proc.accept(arg);
  }

  proc.finalize();
}
