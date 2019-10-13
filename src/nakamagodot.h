#ifndef NAKAMAGODOT_H
#define NAKAMAGODOT_H

#include <Godot.hpp>
#include <Node.hpp>

#include "nakama-cpp/Nakama.h"
#include "NGodotLogSink.h"

using namespace Nakama;

namespace godot {
    class NakamaGodot : public Node {
        GODOT_CLASS(NakamaGodot, Node);

        float time_since_last_tick;

        protected:
            DefaultClientParameters parameters;
            NClientPtr client;
            NSessionPtr session;
            NRtClientPtr rtClient;
            NRtDefaultClientListener rtListener;

        private:
            void authenticated(NSessionPtr);
            void store_object_list(std::vector<NStorageObjectWrite>);
            void remove_object_list(std::vector<NDeleteStorageObjectId>);
            void fetch_object_list(std::vector<NReadStorageObjectId>);
            void emit_error_signal(String signal, const NError& error);
            void emit_rt_error_signal(String signal, const NRtError& error);

        public:
            static void _register_methods();

            NakamaGodot();
            ~NakamaGodot();

            void _init(); // our initializer called by Godot
            void _process(float delta);

            // Clients
            void create_client_default();
            void create_client(String server_key, String server_host, int port);

            // Auth
            int authenticate_email(String email, String password, String username = "", bool create = false, Dictionary vars = Dictionary());
            int authenticate_device(String deviceId, String username = "", bool create = false, Dictionary vars = Dictionary());
            int authenticate_facebook(String oauthToken, String username, bool create = false, bool importFriends = false, Dictionary vars = Dictionary());
            int authenticate_google(String oauthToken, String username, bool create = false, Dictionary vars = Dictionary());
            int authenticate_game_center(
                String playerId, 
                String bundleId, 
                int timestampSeconds, 
                String salt, 
                String signature, 
                String publicKeyUrl, 
                String username, 
                bool create = false, 
                Dictionary vars = Dictionary()
            );
            int authenticate_steam(String token, String username = "", bool create = false, Dictionary vars = Dictionary());
            int authenticate_custom(String id, String username = "", bool create = false, Dictionary vars = Dictionary());
            int connect_realtime_client();
            bool is_realtime_client_connected();
            bool is_session_expired();

            // Chat
            void join_chat(String roomName, int type = 0, bool persist=false, bool hidden=true);
            void leave_chat(String channelId);
            void write_chat_message(String channelId, String content);
            void list_chat_messages(String channelId, int limit = -1, bool forward = true, String cursor = "");

            // Storage
            void store_object(String collection, String key, Dictionary value);
            void store_objects(String collection, Dictionary kvps);
            void fetch_object(String collection, String key);
            void fetch_objects(String collection, Array keys);
            void remove_object(String collection, String key);
            void remove_objects(String collection, Array keys);

            // RPC
            void send_rpc(String rpcId, Dictionary payload);

            // Groups/Clans
            void list_groups(String filter = "", int limit = 20, String cursor = "");
            void list_user_groups(String userId, int limit = 20, int state = -1, String cursor = "");
            void list_group_members(String groupId, int limit = 20, int state = -1, String cursor = "");
            void create_group(String name, String lang, bool open = true, String desc = "", String avatarUrl = "", int maxCount = 0);
            void update_group(String groupId, String name = "", String desc = "", String lang = "", String avatarUrl = "");
            void open_group(String groupId);
            void close_group(String groupId);
            void remove_group(String groupId);
            void join_group(String groupId);
            void leave_group(String groupId);
            void accept_members(String groupId, Array userIds);
            void promote_members(String groupId, Array userIds);
            void kick_members(String groupId, Array userIds);

            // Friends
            void add_friends(Array userIds = Array(), Array usernames = Array());
            void list_friends(int limit = 0, int state = -1, String cursor = "");
            void remove_friends(Array userIds = Array(), Array usernames = Array());
            void block_friends(Array userIds = Array(), Array usernames = Array());

            // Accounts
            void fetch_account();
            void fetch_users(Array ids = Array(), Array usernames = Array(), Array facebookIds = Array());
            void update_account(String username, String displayName, String avatarUrl, String lang, String location, String timezone);
    };
}

#endif
