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

mapping::mapping() {
  // Default mapping currently just my local grammars
  exts.assign({
      {".cpp", "cpp"},
      {".hpp", "cpp"},
      {".h", "cpp"},
      {".h", "c"},
      {".c", "c"},
      {".sh", "bash"},
      {".zsh", "bash"}, // hack zsh doesn't exist on my system.
      {".js", "javascript"},
      {".ts", "typescript"},
      {".tsx", "typescript"},
      {".lua", "lua"},
      {".py", "python"},
  });
}

std::optional<std::string> mapping::resolve(std::filesystem::path &path) {
  const auto target = path.extension();
  for (const auto &[ext, type] : exts) {
    if (ext == target) {
      return type;
    }
  }

  return std::nullopt;
}

ext_pred mapping::rev(const std::string &type) {
  std::string res;

  for (const auto &[ext, ty] : exts) {
    if (type == ty) {
      res += ext;
    }
  }

  return res;
}

bool ext_pred::applies(const std::filesystem::path &path) const {
  auto ext = path.extension().string();
  if (ext.empty()) {
    return false;
  }

  if (_exts.ends_with(ext)) {
    return true;
  }

  ext += ".";

  return _exts.contains(ext);
}
