#include "dpp/dpp.h"
#include "include/harmony_commands.hpp"
#include "include/harmony_config.hpp"

int main() {
  // Read the configuration file from disk

  if (auto config = harmony::HarmonyConfig::load_config()) {
    auto bot = dpp::cluster(config->bot_token, dpp::i_default_intents | dpp::i_message_content);

    bot.on_log(dpp::utility::cout_logger());

    harmony::commands::init_harmony(bot);

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
      auto command_name = event.command.get_command_name();
      harmony::commands::handle_slash_command(command_name, event);
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
      if (dpp::run_once<struct register_bot_commands>()) {
        for (const auto& command : harmony::commands::slash::commands)
          bot.global_command_create(command);
      }
    });

    // Start the bot
    bot.start(false);
  } else {
    fmt::print("Failed to open config.json");
  }

  return 0;
}
