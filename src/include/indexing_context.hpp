#ifndef INDEXING_CONTEXT
#define INDEXING_CONTEXT
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

#include "dpp/dpp.h"
#include "fmt/format.h"
#include "nlohmann/json.hpp"
#include "utils.hpp"
#include "indexer_map.hpp"
namespace harmony {
  namespace indexing {

    class IndexingContext {
      const guild_id _gid;
      const channel_id _cid;
      dpp::cluster &_bot;
      const dpp::snowflake _start_after;
      std::jthread indexing_thread;
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
