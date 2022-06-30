#include "include/indexer_map.hpp"

namespace harmony {
  namespace indexing {
    IndexerMap indexing_contexts;
    const optional_context IndexerMap::get(const ikey &&key) {
      std::scoped_lock lock(mutex);
      if (indexing_map.contains(key)) {
        const auto context = indexing_map.at(key);

        if (!context->is_indexer_running()) {
          return context;
        }
      }
      return std::nullopt;
    }
    const optional_context IndexerMap::operator[](const ikey &&key) {
      return get(std::move(key));
    }
    void IndexerMap::erase(const ikey &&key) {
      indexing_map.erase(std::move(key));
    }
    void IndexerMap::add(const ikey &&key, managed_indexing_context managed_context) {
      std::scoped_lock lock(mutex);
      indexing_map[key] = managed_context;
    }
  };  // namespace indexing
};    // namespace harmony