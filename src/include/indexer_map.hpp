#ifndef INDEXER_MAP
#define INDEXER_MAP
#include <map>
#include <memory>
#include <mutex>

#include "indexing_context.hpp"
#include "utils.hpp"
namespace harmony {
  namespace indexing {

    class IndexerMap {
      using ikey = std::pair<guild_id, channel_id>;
      using imap = std::map<ikey, managed_indexing_context>;
      imap indexing_map;
      std::mutex mutex;

     public:
      const optional_context get(const ikey &&);
      const optional_context operator[](const ikey &&);
      void erase(const ikey &&);
      void add(const ikey &&, managed_indexing_context);
    };
    extern IndexerMap indexing_contexts;
  }  // namespace indexing
}  // namespace harmony

#endif /* INDEXER_MAP */
