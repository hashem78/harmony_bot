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
    using indexer = std::function<void()>;
    using indexers = std::vector<indexer>;
    using indexer_map_key = std::pair<guild_id, channel_id>;
    using optional_context = std::optional<std::shared_ptr<IndexingContext>>;
    using indexer_map = std::map<indexer_map_key, optional_context>;

    class IndexingContext {
      std::atomic_bool should_end;
      const guild_id _gid;
      const channel_id _cid;
      dpp::cluster &_bot;
      static indexer_map indexing_contexts;

      IndexingContext(guild_id gid, channel_id cid, dpp::cluster &bot);

     public:
      static optional_context find_context_for(guild_id gid, channel_id cid);
      static optional_context create(guild_id gid, channel_id cid, dpp::cluster &bot);

      bool is_indexer_running() const;
      void start_indexing();
      void stop_indexing();
      ~IndexingContext();
    };
  };  // namespace indexing
};    // namespace harmony

#endif /* INDEXING_CONTEXT */
