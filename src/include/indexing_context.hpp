#ifndef INDEXING_CONTEXT
#define INDEXING_CONTEXT
#include <atomic>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <thread>

#include "dpp/dpp.h"
#include "fmt/format.h"
#include "nlohmann/json.hpp"

namespace harmony {
  namespace indexing {
    class IndexingContext;
    using guild_id = dpp::snowflake;
    using channel_id = dpp::snowflake;
    using indexer_map_key = std::pair<guild_id, channel_id>;
    using managed_indexing_context = std::shared_ptr<IndexingContext>;
    using indexer_map = std::map<indexer_map_key, managed_indexing_context>;
    using optional_context = std::optional<managed_indexing_context>;
    extern indexer_map indexing_contexts;
    extern optional_context find_context_for(guild_id gid, channel_id cid);

    class IndexingContext {
      const guild_id _gid;
      const channel_id _cid;
      dpp::cluster &_bot;
      const dpp::snowflake _start_after;
      std::jthread execution_thread;

      const bool _include_bot_messages;

      IndexingContext(guild_id gid, channel_id cid, dpp::cluster &bot, bool allow_bot_messages = false, dpp::snowflake = 1420070400000);

     public:
      static void create(guild_id gid, channel_id cid, dpp::cluster &bot, bool allow_bot_messages = false, dpp::snowflake = 1420070400000);

      bool is_indexer_running() const;
      void start_indexing();
      void stop_indexing();

      ~IndexingContext();
    };
  };  // namespace indexing
};    // namespace harmony

#endif /* INDEXING_CONTEXT */
