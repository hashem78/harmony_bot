#ifndef UTILS
#define UTILS

#include <memory>
#include <optional>

#include "dpp/dpp.h"

namespace harmony {
  using guild_id = dpp::snowflake;
  using channel_id = dpp::snowflake;
  namespace indexing {
    class IndexingContext;
    using managed_indexing_context = std::shared_ptr<IndexingContext>;
    using optional_context = std::optional<managed_indexing_context>;

  }  // namespace indexing
}  // namespace harmony

#endif /* UTILS */
