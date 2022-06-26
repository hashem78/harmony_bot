#include "include/harmony_commands.hpp"
namespace harmony {
  namespace commands {
    namespace slash {
      commands_array commands{
          dpp::slashcommand("start_indexing_channel", "Start Indexing a channel", 0)
              .add_option(dpp::command_option(dpp::command_option_type::co_channel, "channel", "Channel", true)
                              .add_channel_type(dpp::channel_type::CHANNEL_TEXT)),

          dpp::slashcommand("stop_indexing_channel", "Stop Indexing a channel", 0)
              .add_option(dpp::command_option(dpp::command_option_type::co_channel, "channel", "Channel", true)
                              .add_channel_type(dpp::channel_type::CHANNEL_TEXT))};
    };

    void init_harmony(dpp::cluster &bot) {
      slash::handlers["start_indexing_channel"] =
          [&bot](const dpp::slashcommand_t &event) {
            auto channel_id =
                get<dpp::snowflake>(event.get_parameter("channel"));
            auto guild_id = event.command.guild_id;
            indexing::IndexingContext::create(guild_id, channel_id, bot);
            event.reply("Indexing Started");
          };

      slash::handlers["stop_indexing_channel"] =
          [&bot](const dpp::slashcommand_t &event) {
            auto channel_id =
                get<dpp::snowflake>(event.get_parameter("channel"));

            auto guild_id = event.command.guild_id;
            auto context_optional = indexing::IndexingContext::find_context_for(
                guild_id, channel_id);
            if (!context_optional.has_value()) {
              event.reply("There is no indexer running here");
              return;
            }
            event.reply("Indexing stopped");
            context_optional.value()->stop_indexing();
          };

      for (auto &command : slash::commands)
        command.set_application_id(bot.me.id);
    }

    void handle_slash_command(const std::string &command_name, const dpp::slashcommand_t &event) {
      slash::handlers[command_name](event);
    }
  };  // namespace commands

};  // namespace harmony
