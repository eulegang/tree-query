#include <cstdlib>
#include <filesystem>
#include <ranges>

#include "registry.h"

std::optional<std::filesystem::path>
resolve(std::vector<std::filesystem::path> &paths, std::string_view name) {
  for (const auto &path : paths) {
    for (const auto &ent : std::filesystem::directory_iterator(path)) {
      if (!ent.is_regular_file()) {
        continue;
      }

      if (ent.path().extension() != ".so") {
        continue;
      }

      if (ent.path().stem() == name) {
        return ent.path();
      }
    }
  }

  return std::nullopt;
}

registry::registry() : paths{} {
  std::string_view env{getenv("TQ_PATH")};
  constexpr std::string_view delim{":"};
  for (const auto &sub : env | std::views::split(delim)) {
    std::string_view part(sub.data(), sub.size());
    paths.push_back(std::filesystem::path(part));
  }
}

std::vector<std::string> registry::types() {

  std::vector<std::string> res;
  for (const auto &path : paths) {
    for (const auto &ent : std::filesystem::directory_iterator(path)) {
      if (!ent.is_regular_file()) {
        continue;
      }

      if (ent.path().extension() != ".so") {
        continue;
      }

      res.push_back(ent.path().stem());
    }
  }

  return res;
}

ts::lang registry::load(std::string_view name) {
  auto path = resolve(paths, name);

  if (!path)
    throw std::runtime_error("unable to resolve language");

  return ts::lang(*path, name);
}
