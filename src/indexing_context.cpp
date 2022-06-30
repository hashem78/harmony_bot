#include "include/indexing_context.hpp"

namespace harmony {
  namespace fs = std::filesystem;
  namespace indexing {

    IndexingContext::IndexingContext(guild_id gid, channel_id cid, dpp::cluster &bot, bool include_bot_messages, dpp::snowflake start_after)
        : _bot(bot),
          _gid(gid),
          _cid(cid),
          _start_after(start_after),
          _include_bot_messages(include_bot_messages) {
      start_indexing();
    }

    void IndexingContext::create(guild_id gid, channel_id cid, dpp::cluster &bot, bool include_bot_messages, dpp::snowflake start_after) {
      indexing_contexts.add({gid, cid}, managed_indexing_context(new IndexingContext(gid, cid, bot, include_bot_messages, start_after)));
    }

    bool IndexingContext::is_indexer_running() const {
      return indexing_thread.get_stop_token().stop_requested();
    }

    void IndexingContext::start_indexing() {
      indexing_thread = std::jthread([this](std::stop_token stoken) {
        dpp::snowflake after = _start_after;

        // get the specific channel and guild objects
        // associated with the event that the context was started for
        auto guild = _bot.guild_get_sync(_gid);
        auto channel = _bot.channel_get_sync(_cid);

        fmt::print("Indexing context started in {} -> {}\n", guild.name, channel.name);

        auto path = fmt::format("{}/{}", guild.name, channel.name);
        if (not fs::exists(path)) {
          fs::create_directories(path);
        }
        int file_counter = 0;
        auto output = std::ofstream();

        // continue to iterate until a stop request comes through.
        // since the discord api specifies that only a maximum of 100 messages
        // can be retrieved with a signle api call, so this loops exits when
        // we can no longer retrieve messages or when a user calls /stop_indexing
        while (!stoken.stop_requested()) {
          auto messages = _bot.messages_get_sync(_cid, 0, 0, after, 100);

          if (messages.empty()) { // We've reached the most recent message
            stop_indexing();
          } else {
            output.open(fmt::format("{}/{}.json", path, file_counter++));

            if (not output.is_open()) {
              fmt::print("Failed to open {}/{}.json for writing, stopping index\n", path, file_counter - 1);
              _bot.message_create(
                  dpp::message()
                      .set_guild_id(_gid)
                      .set_channel_id(_cid)
                      .set_content("An unknown error occured."));
              stop_indexing();
            } else {
              auto messages_json_array = nlohmann::json::array();

              auto *last_message = &messages.begin()->second;

              for (auto &[message_id, message] : messages) {
                auto parsed_message_json = json::parse(message.build_json());

                if (message.author.is_bot() && _include_bot_messages) {
                  messages_json_array.push_back(parsed_message_json);
                  fmt::print("bot message: {}\n", message.content);
                } else if (!message.author.is_bot()) {
                  messages_json_array.push_back(parsed_message_json);
                  fmt::print("message: {}\n", message.content);
                }

                if (message.get_creation_time() >
                    last_message->get_creation_time()) {
                  last_message = &message;
                }
              }
              after = last_message->id;

              output << std::setw(4) << messages_json_array;
              fmt::print("Saved {}/{}.json\n", path, file_counter - 1);
              output.close();
              std::this_thread::sleep_for(std::chrono::seconds(1));
            }
          }
        }
      });
    }

    void IndexingContext::stop_indexing() {
      indexing_thread.request_stop();
      _bot.message_create(
          dpp::message()
              .set_guild_id(_gid)
              .set_channel_id(_cid)
              .set_content("Indexing completed."));
      fmt::print("Indexing context stopped\n");
    }

    IndexingContext::~IndexingContext() {
      fmt::print("Called indexing context destructor\n");
    }
  }  // namespace indexing
}  // namespace harmony