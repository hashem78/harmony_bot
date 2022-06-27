#include "include/indexing_context.hpp"

namespace harmony {
  namespace fs = std::filesystem;
  namespace indexing {
    indexer_map IndexingContext::indexing_contexts;
    IndexingContext::IndexingContext(guild_id gid, channel_id cid,
                                     dpp::cluster &bot, dpp::snowflake start_after)
        : _bot(bot), _gid(gid), _cid(cid), _start_after(start_after) {
      
      should_end = false;
      start_indexing();
    }

    optional_context IndexingContext::find_context_for(guild_id gid,
                                                       channel_id cid) {
      if (indexing_contexts.contains({gid, cid}))
        return indexing_contexts[{gid, cid}];

      return std::nullopt;
    }

    optional_context IndexingContext::create(guild_id gid, channel_id cid,
                                             dpp::cluster &bot, dpp::snowflake start_after) {
      indexing_contexts[{gid, cid}] =
          std::shared_ptr<IndexingContext>(new IndexingContext(gid, cid, bot, start_after));
      return indexing_contexts[{gid, cid}];
    }

    bool IndexingContext::is_indexer_running() const { return should_end; }

    void IndexingContext::start_indexing() {
      indexer indexing_handler = [this]() {
        dpp::snowflake after = _start_after;
        auto guild = _bot.guild_get_sync(_gid);
        auto channel = _bot.channel_get_sync(_cid);

        fmt::print("Indexing context started in {} -> {}", guild.name,
                   channel.name);

        auto path = fmt::format("{}/{}", guild.name, channel.name);
        if (not fs::exists(path)) {
          fs::create_directories(path);
        }
        int file_counter = 0;
        auto output = std::ofstream();

        while (!should_end) {
          auto messages = _bot.messages_get_sync(_cid, 0, 0, after, 100);

          if (messages.empty()) {
            stop_indexing();

            break;
          }
          output.open(fmt::format("{}/{}.json", path, file_counter++));

          if (not output.is_open()) {
            fmt::print("Failed to open {}/{}.json for writing, stopping index\n",
                       path, file_counter - 1);
            stop_indexing();
            break;
          }

          auto messages_json_array = nlohmann::json::array();

          auto *last_message = &messages.begin()->second;

          for (auto &[message_id, message] : messages) {
            if (not message.author.is_bot()) {
              auto parsed_message_json = json::parse(message.build_json());

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
      };
      std::thread(indexing_handler).detach();
    }

    void IndexingContext::stop_indexing() {
      should_end = true;
      indexing_contexts.erase({_gid, _cid});
      fmt::print("Indexing context stopped\n");
    }

    IndexingContext::~IndexingContext() {}
  }  // namespace indexing
}  // namespace harmony