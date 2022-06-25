#include "include/harmony_config.hpp"
namespace harmony {
  HarmonyConfig::HarmonyConfig() {}
  optional_config HarmonyConfig::load_config() {
    if (auto config_file = std::ifstream("config.json")) {
      nlohmann::json json_config_file;
      config_file >> json_config_file;
      auto config = HarmonyConfig();
      config.from_json(json_config_file, config);
      return config;
    }

    return std::nullopt;
  }
}  // namespace harmony