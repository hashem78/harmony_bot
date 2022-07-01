<p align="center">
  <img src="https://i.imgur.com/zdir6xS.png" />
</p>

# Harmony

## What is Harmony?
> Harmony is a discord bot.
## Why Harmony?
> Harmony intends to make guild archival, indexing and message searching open to the entire web.
## How is Harmony useful?
> Imagine being a programming related server, say for example discussing python related topics, people ask questions and helpful community members try their best to give relvant answers, currently those answers remain lockedup in that specific server, only members of that server are able to see those interactions, they're not visible to entirety of the internet this is unfortunately a shortcomming of the single-page applications these days, which discord is certainly one.
## How is Harmony the solution?
> Privliged guild members are able to start Harmony on a specifc channel, harmony will then essentialy collect all the messages of the channel and present them as a nice webpage, that can be exposed to the internet.


## How to use Harmony?
1. Add Harmony to your sever

    > Click [here](https://discord.com/api/oauth2/authorize?client_id=989231908858785794&permissions=27648&scope=applications.commands%20bot) to add Harmony to your sever/guild.
2. Issue /index <channel_name> in chat, and voila!
3. To stop the bot at any point issue /stop_indexing <channel_name>

## Supported Commands

/index *<channel_name> ?<start_time> ?<include_bot_messages>

_starts indexing a channel._

* channel_name: the channel where the bot is expected to run on.
* start_time: syntax -> AyBmCd where ABC are integers, represents how far back the bot has to start indexing from.
* include_bot_messages: whether or not bot interactions are to be included in the indexed messages.

/stop_indexing *<channel_name>

_stops indexing a channel._

NOTE:

\* : parameter is required <br> ? : parameter is optional


---

## Building Harmony from source
* Harmony is written in C++.
* Harmony needs the following dependencies to be present.
    * zlib
    * libopenssl
* Harmony supports building on Windows(MSVC19) and on Linux(g++11).
* Harmony uses cmake as its build system.
* External libraries used (packaged already)
    * [dpp](https://github.com/brainboxdotcc/DPP)
    * [re2](https://github.com/google/re2)
    * [nlohmannjson](https://github.com/nlohmann/json)


### Build Steps:

1. git clone https://github.com/hashem78/harmony_bot
2. cd harmony_bot
3. cmake .







