#ifndef INDEXER_MAP
#define INDEXER_MAP
#include <map>
#include <memory>
#include <mutex>

#include "indexing_context.hpp"
#include "utils.hpp"
namespace harmony {
  namespace indexing {

    /**
     * @brief A thread safe storage class for instances of
     *  harmony::indexing::IndexingContext this is a wrapper
     *  around an std::map that holds IndexingContexts managed
     *  by std::shared_ptrs
     */
    class IndexerMap {
      using ikey = std::pair<guild_id, channel_id>;
      using imap = std::map<ikey, managed_indexing_context>;

      imap indexing_map;
      std::mutex mutex;

     public:
      /**
       * @brief Retireve an instance of harmony::indexing::IndexingContext
       *
       * @return const optional_context that contains a managed_indexing_context
       * iff indexing_map contains ikey
       */
      const optional_context get(const ikey &&);
      /**
       * @brief Overload that forwards ikey to get()
       *
       * @return const optional_context that contains a managed_indexing_context
       * iff indexing_map contains ikey
       */
      const optional_context operator[](const ikey &&);
      /**
       * @brief Removes an IndexingContext that is no longer needed.
       *
       */
      void erase(const ikey &&);
      /**
       * @brief Adds an IndexingContext to be stored in idexing_map
       *
       */
      void add(const ikey &&, managed_indexing_context);
    };
    /**
     * @brief A managed global instance of IndexerMap that acts as a store
     * for managed_indexing contexts
     *
     */
    extern IndexerMap indexing_contexts;
  }  // namespace indexing
}  // namespace harmony

#endif /* INDEXER_MAP */
