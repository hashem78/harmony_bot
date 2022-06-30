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
#include "indexer_map.hpp"
#include "nlohmann/json.hpp"
#include "utils.hpp"
namespace harmony {
  namespace indexing {
    /**
     * @brief Abstraction for calling dpp::get_messages_sync on a separate thread.
     *
     */
    class IndexingContext {
      /**
       * @brief Guild Id where the context is to be run.
       *
       */
      const guild_id _gid;
      /**
       * @brief Channel Id inside the guild where the context is to run.
       *
       */
      const channel_id _cid;
      /**
       * @brief The timestamp of the first message to start indexing from
       * as a dpp::snowflake.
       *
       */
      const dpp::snowflake _start_after;
      /**
       * @brief Whether or not to include bot messages in the saved responses
       *
       */
      const bool _include_bot_messages;

      dpp::cluster &_bot;
      /**
       * @brief The thread that dpp::get_messages_sync will run on
       *
       */
      std::jthread indexing_thread;
      /**
       * @brief Construct a new Indexing Context object
       *
       * @param gid Guild Id where the context is to be run.
       * @param cid Channel Id inside the guild where the context is to run.

       * @param allow_bot_messages Whether or not to include bot messages in the saved responses.
       */
      IndexingContext(guild_id gid, channel_id cid, dpp::cluster &bot,
                      bool allow_bot_messages = false, dpp::snowflake = 1420070400000);

     public:
      /**
       * @brief Construct a new Indexing Context object
       *
       * @param gid Guild Id where the context is to be run.
       * @param cid Channel Id inside the guild where the context is to run.
       * @param bot A refrence to the dpp::cluster instance created when a bot joins a new guild.
       * @param allow_bot_messages Whether or not to include bot messages in the saved responses.
       */
      static void create(guild_id gid, channel_id cid, dpp::cluster &bot,
                         bool allow_bot_messages = false, dpp::snowflake = 1420070400000);
      
      /**
       * @brief Check whether the launched thread is still fetching data and indexing it.
       * 
       * @return true iff all messages are fetched or the user summoned /stop_indexing.
       * @return false if the thread is still fetching messages.
       */
      bool is_indexer_running() const;
      /**
       * @brief Starts the indexing thread.
       * 
       */
      void start_indexing();
      /**
       * @brief Stops the indexing thread.
       * 
       */
      void stop_indexing();

      ~IndexingContext();
    };
  };  // namespace indexing
};    // namespace harmony

#endif /* INDEXING_CONTEXT */
