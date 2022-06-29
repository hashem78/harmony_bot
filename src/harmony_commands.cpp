#include "include/harmony_commands.hpp"
namespace harmony {
  const auto DISCORD_EPOCH = dpp::snowflake(1420070400000);
  namespace commands {
    namespace slash {
      commands_array commands{
          dpp::slashcommand("index", "Start Indexing a channel", 0)
              .add_option(dpp::command_option(dpp::command_option_type::co_channel, "channel", "Channel", true)
                              .add_channel_type(dpp::channel_type::CHANNEL_TEXT))
              .add_option(dpp::command_option(dpp::co_string, "from", "Start indexing from this point", false))
              .add_option(dpp::command_option(dpp::co_boolean, "include_bot_messages", "Include bot messages(defaults to false)", false)),

          dpp::slashcommand("stop_indexing", "Stop Indexing a channel", 0)
              .add_option(dpp::command_option(dpp::command_option_type::co_channel, "channel", "Channel", true)
                              .add_channel_type(dpp::channel_type::CHANNEL_TEXT)),
      };
    };  // namespace slash
    const re2::RE2 time_range_exp(R"((\d+[ymd])?(\d+[ymd])?(\d+[ymd])?)");

    optional_time_jump_map parse_jump_time(const std::string &input) {
      const auto valid_endings = std::string("ymd");
      const auto time_range_exp = re2::RE2(R"((\s*\d+\s*[ymdYMD])\s*)");

      auto parsing_result = std::map<char, size_t>{{'y', 0}, {'m', 0}, {'d', 0}};
      auto to_be_matched = re2::StringPiece{input};
      auto token = std::string{};
      auto sucess = bool{false};

      // Iterate over matching tokens.
      while (RE2::Consume(&to_be_matched, time_range_exp, &token)) {
        // get the token's ending and normalize it
        auto token_ending = std::tolower(token.back());

        // check the end of the token for the identifier
        if (valid_endings.find(token_ending) != std::string::npos) {
          // take the number preceeding the ending
          auto parsed_number_str = token.substr(0, token.size() - 1);

          // Avoid the possiblity of having to use std::stoi with a large string
          if (parsed_number_str.length() <= 2) {
            parsing_result[token_ending] = std::stoi(parsed_number_str);
            sucess = true;
          }
        }
      }

      if (sucess)
        return parsing_result;

      return std::nullopt;
    }

    void init_harmony(dpp::cluster &bot) {
      slash::handlers["index"] =
          [&bot](const dpp::slashcommand_t &event) {
            auto channel_id = std::get<dpp::snowflake>(event.get_parameter("channel"));
            auto guild_id = event.command.guild_id;
            auto include_bot_messages{false};

            if (auto should_include_bot_messages = std::get_if<bool>(&event.get_parameter("include_bot_messages"))) {
              include_bot_messages = *should_include_bot_messages;
            }
            // Check if there is an indexer already running
            if (harmony::indexing::find_context_for(guild_id, channel_id) != std::nullopt) {
              event.reply("There is an Indexer already running in this channel");

            } else if (auto from_string = std::get_if<std::string>(&event.get_parameter("from"))) {
              fmt::print("from: {}\n", *from_string);

              if (auto jump_time = parse_jump_time(*from_string)) {
                // This will never throw because parse_jump_time initializes the values to 0
                size_t y = jump_time->at('y');
                size_t m = jump_time->at('m');
                size_t d = jump_time->at('d');

                // Get the timestamp based on the id of the command
                // https://discord.com/developers/docs/reference#snowflakes-snowflake-id-format-structure-left-to-right
                auto command_time = (event.command.id >> 22) + DISCORD_EPOCH;
                // Convert everything to std::chrono::milliseconds
                auto command_time_ms = std::chrono::milliseconds(command_time);
                auto years_ms = std::chrono::milliseconds(std::chrono::years(y));
                auto months_ms = std::chrono::milliseconds(std::chrono::months(m));
                auto days_ms = std::chrono::milliseconds(std::chrono::days(d));

                // Findout from when the user wants the indexing to start from
                auto jump_start_time = command_time_ms - (years_ms + months_ms + days_ms);

                // Convert the time point to a discord snowflake
                // https://discord.com/developers/docs/reference#snowflake-ids-in-pagination-generating-a-snowflake-id-from-a-timestamp-example
                auto jump_start_time_flake = dpp::snowflake();

                // Make sure we aren't jumping before discord was even a thing.
                if (jump_start_time.count() < DISCORD_EPOCH) {
                  jump_start_time_flake = DISCORD_EPOCH;
                  jump_start_time = std::chrono::milliseconds(DISCORD_EPOCH);
                } else {
                  jump_start_time_flake = dpp::snowflake((jump_start_time.count() - DISCORD_EPOCH) << 22);
                }

                indexing::IndexingContext::create(guild_id, channel_id, bot, include_bot_messages, jump_start_time_flake);

                event.reply(fmt::format("Indexing Started from <t:{}:F>", jump_start_time.count() / 1000));

              } else {
                fmt::print("Failed to parse command");
                event.reply("Failed to parse command");
              }
            } else {
              indexing::IndexingContext::create(guild_id, channel_id, bot, include_bot_messages);
              event.reply("Indexing Started ");
            }
          };

      slash::handlers["stop_indexing"] =
          [&bot](const dpp::slashcommand_t &event) {
            auto channel_id = get<dpp::snowflake>(event.get_parameter("channel"));
            auto guild_id = event.command.guild_id;

            if (auto context = indexing::find_context_for(guild_id, channel_id)) {
              context.value()->stop_indexing();
              event.reply("Indexing stopped");

              // Remove the context from the indexing map
              indexing::indexing_contexts.erase({guild_id, channel_id});
            } else {
              event.reply("There is no indexer running here");
            }
          };

      for (auto &command : slash::commands)
        command.set_application_id(bot.me.id);
    }

    void handle_slash_command(const std::string &command_name, const dpp::slashcommand_t &event) {
      slash::handlers[command_name](event);
    }
  };  // namespace commands

};  // namespace harmony
