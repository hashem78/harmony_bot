#ifndef HARMONY_CONFIG
#define HARMONY_CONFIG

#include <fstream>
#include <optional>
#include <string>

#include "fmt/format.h"
#include "nlohmann/json.hpp"

namespace harmony {
  struct HarmonyConfig;
  using optional_config = std::optional<HarmonyConfig>;
  struct HarmonyConfig {
    std::string bot_token;
    static optional_config load_config();
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HarmonyConfig, bot_token)

   private:
    HarmonyConfig();
  };

}  // namespace harmony

#endif /* HARMONY_CONFIG */
