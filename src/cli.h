#ifndef _TQ_CLI_H
#define _TQ_CLI_H

#include <exception>
#include <initializer_list>
#include <string>
#include <string_view>

struct Cli {
  enum class Action {
    List,
    Info,
    Tree,
  };

  class Exception : public std::exception {
    std::string msg;

  public:
    Exception(std::string_view msg) : msg{msg} {}

    const char *what() const noexcept override { return msg.c_str(); }
  };

  Action action;
  bool help;
  bool version;
  bool verbose;
  std::string arg;
  std::string type;

  Cli(int argc, char *argv[]);
  Cli(std::initializer_list<std::string_view>);
};

#endif
