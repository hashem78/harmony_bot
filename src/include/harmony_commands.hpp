#ifndef HARMONY_COMMANDS
#define HARMONY_COMMANDS
#include <array>
#include <map>
#include <string>

#include "dpp/dpp.h"
#include "include/indexing_context.hpp"
namespace harmony {
  namespace commands {
    namespace slash {
      using handler = std::function<void(const dpp::slashcommand_t &)>;
      using handler_map = std::unordered_map<std::string, handler>;
      static handler_map handlers;
      using commands_array = std::array<dpp::slashcommand, 2>;
      extern commands_array commands;
    };  // namespace slash
    void init_harmony(dpp::cluster &bot);
    void handle_slash_command(const std::string &command_name,
                              const dpp::slashcommand_t &event);
  };  // namespace commands

};  // namespace harmony

#endif /* HARMONY_COMMANDS */
