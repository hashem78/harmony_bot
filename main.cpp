
#include <dpp/dpp.h>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <fstream>
#include <string_view>
#include <string>
#include <ranges>
#include <vector>
#include <functional>
#include <map>
#include <thread>
#include <unordered_map>
#include <array>
#include <atomic>
#include <thread>
#include <chrono>
#include <memory>
#include <optional>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;


namespace harmony
{
    struct HarmonyConfig {
        std::string bot_token;
        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HarmonyConfig, bot_token)
    };
    HarmonyConfig get_config() {
        if (auto config_file = std::ifstream("config.json")) {
            json json_config_file;
            config_file >> json_config_file;
            HarmonyConfig config;
            config.from_json(json_config_file, config);
            return config;
        }
        else {
            fmt::print("Failed to open config.json");
            exit(1);
        }

    }
    namespace indexing
    {
        class IndexingContext;
        using guild_id = dpp::snowflake;
        using channel_id = dpp::snowflake;
        using indexer = std::function<void()>;
        using indexers = std::vector<indexer>;
        using indexer_map_key = std::pair<guild_id, channel_id>;
        using optional_context = std::optional<std::shared_ptr<IndexingContext>>;
        using indexer_map = std::map<indexer_map_key, optional_context>;

        indexer_map indexing_contexts;

        optional_context find_context_for(guild_id gid, channel_id cid)
        {
            
            if (indexing_contexts.contains({gid, cid}))
            {

                return indexing_contexts[{gid, cid}];
            }
            return std::nullopt;
        }

        class IndexingContext
        {
            std::atomic_bool should_end;
            guild_id _gid;
            channel_id _cid;
            dpp::cluster &_bot;

            IndexingContext(guild_id gid, channel_id cid, dpp::cluster &bot) : _bot(bot)
            {
                _gid = gid;
                _cid = cid;
                should_end = false;
                start_indexing();
            }

        public:
            static optional_context create(guild_id gid, channel_id cid, dpp::cluster &bot)
            {
                indexing_contexts[{gid, cid}] = shared_ptr<IndexingContext>(new IndexingContext(gid, cid, bot));
                return indexing_contexts[{gid, cid}];
            }

            bool is_indexer_running() const
            {
                return should_end;
            }
            void stop_indexing()
            {
                should_end = true;
                indexing_contexts.erase({_gid, _cid});
                fmt::print("Indexing context stopped");
            }
            void start_indexing()
            {
                fmt::print("Indexing context started");

                indexer indexing_handler = [this]()
                {
                    dpp::snowflake after = 1420070400000;
                    while (!should_end)
                    {
                        auto messages = _bot.messages_get_sync(_cid, 0, 0, after, 100);
                        fmt::print("messages length: {}\n", messages.size());
                        if (messages.empty())
                        {
                            stop_indexing();
                            break;
                        }
                        dpp::message *last_message = &messages.begin()->second;

                        for (auto &[message_id, message] : messages)
                        {
                            if (not message.author.is_bot())
                            {
                                fmt::print("message: {}\n", message.content);
                            }

                            if (message.get_creation_time() > last_message->get_creation_time())
                            {
                                last_message = &message;
                            }
                        }
                        after = last_message->id;
                    }
                };
                std::thread(indexing_handler).detach();
            }
            ~IndexingContext()
            {
                should_end = true;
            }
        };

    };
    namespace commands
    {
        namespace slash
        {
            using handler = std::function<void(const dpp::slashcommand_t &)>;
            using handler_map = std::unordered_map<std::string, handler>;
            static handler_map handlers;
            using commands_array = std::array<dpp::slashcommand, 2>;
            static commands_array commands{
                dpp::slashcommand("start_indexing_channel", "Start Indexing a channel", 0)
                    .add_option(dpp::command_option(dpp::command_option_type::co_channel, "channel", "Channel", true)
                                    .add_channel_type(dpp::channel_type::CHANNEL_TEXT)),
                dpp::slashcommand("stop_indexing_channel", "Stop Indexing a channel", 0)
                    .add_option(dpp::command_option(dpp::command_option_type::co_channel, "channel", "Channel", true)
                                    .add_channel_type(dpp::channel_type::CHANNEL_TEXT))};
        };
        void init_harmony(dpp::cluster &bot)
        {
            slash::handlers["start_indexing_channel"] = [&bot](const dpp::slashcommand_t &event)
            {
                auto channel_id = get<dpp::snowflake>(event.get_parameter("channel"));
                auto channel = bot.channel_get_sync(channel_id);
                auto guild_id = event.command.guild_id;
                indexing::IndexingContext::create(guild_id, channel_id, bot);

                event.reply(fmt::format("Starting indexer for {}...", channel.name));
            };
            slash::handlers["stop_indexing_channel"] = [&bot](const dpp::slashcommand_t &event)
            {
                auto channel_id = get<dpp::snowflake>(event.get_parameter("channel"));
                auto channel = bot.channel_get_sync(channel_id);
                auto guild_id = event.command.guild_id;
                auto context_optional = indexing::find_context_for(guild_id, channel_id);
                if (context_optional.has_value())
                {
                    context_optional.value()->stop_indexing();
                }
                else
                {
                    event.reply(fmt::format("There is no indexer running for {}", channel.name));
                    return;
                }
                event.reply(fmt::format("Stopped indexing{}", channel.name));
            };
            for (auto &command : slash::commands)
                command.set_application_id(bot.me.id);
        }
        void handle_slash_command(const string &command_name, const dpp::slashcommand_t &event)
        {
            slash::handlers[command_name](event);
        }
    };

};


int main()
{
    // Read the configuration file from disk
    auto config = harmony::get_config();
    
    auto bot = dpp::cluster(config.bot_token, dpp::i_default_intents | dpp::i_message_content);

    bot.on_log(dpp::utility::cout_logger());

    harmony::commands::init_harmony(bot);

    bot.on_slashcommand([&bot](const dpp::slashcommand_t &event)
                        {
		auto command_name = event.command.get_command_name();
		harmony::commands::handle_slash_command(command_name, event); });

    bot.on_ready([&bot](const dpp::ready_t &event)
                 {
		if (dpp::run_once<struct register_bot_commands>()) {
			for (const auto& command : harmony::commands::slash::commands)
				bot.global_command_create(command);
		} });

    // Start the bot
    bot.start(false);

    return 0;
}
