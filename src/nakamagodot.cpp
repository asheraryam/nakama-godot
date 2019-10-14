#include "nakamagodot.h"

using namespace std::placeholders;
using namespace godot;

Dictionary stringMapToDict(const NStringMap& map)
{
    Dictionary d;
    for (auto it = map.begin(); it != map.end(); it++)
    {
        d[String(it->first.c_str())] = String(it->second.c_str());
    }
    return d;
}

NStringMap varMap(Dictionary vars)
{
    NStringMap v;
    auto keys = vars.keys();
    for (int i = 0; i < keys.size(); i++)
    {
        if (keys[i].get_type() != Variant::Type::STRING) break;
        auto key = keys[i].operator String();
        if (vars[key].get_type() != Variant::Type::STRING) break;
            
        v[key.utf8().get_data()] = vars[key].operator String().utf8().get_data();
    }
    return v;
}

Dictionary groupToDict(NGroup group)
{
    Dictionary d;
    d["id"] = String(group.id.c_str());
    d["creatorId"] = String(group.creatorId.c_str());
    d["name"] = String(group.name.c_str());
    d["description"] = String(group.description.c_str());
    d["lang"] = String(group.lang.c_str());
    d["metadata"] = String(group.metadata.c_str());
    d["avatarUrl"] = String(group.avatarUrl.c_str());
    d["open"] = group.open;
    d["edgeCount"] = group.edgeCount;
    d["maxCount"] = group.maxCount;
    d["createTime"] = group.createTime;
    d["updateTime"] = group.updateTime;
    return d;
}

Dictionary userGroupToDict(NUserGroup userGroup)
{
    auto d = groupToDict(userGroup.group);
    d["state"] = (int)userGroup.state;
    return d;
}

Dictionary userToDict(NUser user)
{
    Dictionary d;
    d["id"] = String(user.id.c_str());
    d["username"] = String(user.username.c_str());
    d["displayName"] = String(user.displayName.c_str());
    d["avatarUrl"] = String(user.avatarUrl.c_str());
    d["lang"] = String(user.lang.c_str());
    d["location"] = String(user.location.c_str());
    d["timeZone"] = String(user.timeZone.c_str());
    d["metadata"] = String(user.metadata.c_str());
    d["facebookId"] = String(user.facebookId.c_str());
    d["googleId"] = String(user.googleId.c_str());
    d["gameCenterId"] = String(user.gameCenterId.c_str());
    d["steamId"] = String(user.steamId.c_str());
    d["online"] = user.online;
    d["edgeCount"] = user.edgeCount;
    d["createdAt"] = user.createdAt;
    d["updatedAt"] = user.updatedAt;
    return d;
}

Dictionary friendToDict(NFriend fr)
{
    auto d = userToDict(fr.user);
    d["state"] = (int)fr.state;
    return d;
}

Dictionary groupMemberToDict(NGroupUser groupUser)
{
    auto d = userToDict(groupUser.user);
    d["state"] = (int)groupUser.state;
    return d;
}

Dictionary userPresenceToDict(NUserPresence pres)
{
    Dictionary d;
    d["userId"] = String(pres.userId.c_str());
    d["username"] = String(pres.username.c_str());
    d["sessionId"] = String(pres.sessionId.c_str());
    d["status"] = String(pres.status.c_str());
    d["persistence"] = pres.persistence;
    return d;
}

Dictionary channelToDict(NChannelPtr channel)
{
    Dictionary d;
    d["id"] = String(channel->id.c_str());
    d["groupId"] = String(channel->groupId.c_str());
    d["roomName"] = String(channel->roomName.c_str());
    d["userIdOne"] = String(channel->userIdOne.c_str());
    d["userIdTwo"] = String(channel->userIdTwo.c_str());
    d["self"] = userPresenceToDict(channel->self);
    Array presences;
    for (auto& pres : channel->presences)
    {
        presences.append(userPresenceToDict(pres));
    }
    d["presences"] = presences;
    return d;
}

Dictionary messageToDict(NChannelMessage msg)
{
    // TODO: Add more fields. This is good enough for now though.
    Dictionary d;
    d["channelId"] = String(msg.channelId.c_str());
    d["messageId"] = String(msg.messageId.c_str());
    d["code"] = msg.code;
    d["senderId"] = String(msg.senderId.c_str());
    d["username"] = String(msg.username.c_str());
    d["content"] = String(msg.content.c_str());
    return d;
}

Dictionary sessionToDict(NSessionPtr session)
{
    Dictionary d;
    d["authToken"] = String(session->getAuthToken().c_str());
    d["userId"] = String(session->getUserId().c_str());
    d["username"] = String(session->getUsername().c_str());
    d["isCreated"] = session->isCreated();
    d["variables"] = stringMapToDict(session->getVariables());
    return d;
}

Dictionary presenceEventToDict(const NChannelPresenceEvent& event)
{
    Dictionary d;
    d["channelId"] = String(event.channelId.c_str());
    d["groupId"] = String(event.groupId.c_str());
    d["roomName"] = String(event.roomName.c_str());
    d["userIdOne"] = String(event.userIdOne.c_str());
    d["userIdTwo"] = String(event.userIdTwo.c_str());
    Array left;
    for (auto& l : event.leaves)
    {
        left.append(userPresenceToDict(l));
    }
    d["left"] = left;
    Array joins;
    for (auto & j : event.joins)
    {
        joins.append(userPresenceToDict(j));
    }
    d["joins"] = joins;
    return d;
}

Dictionary deviceToDict(const NAccountDevice& device)
{
    Dictionary d;
    d["id"] = String(device.id.c_str());
    d["vars"] = stringMapToDict(device.vars);
    return d;
}

Dictionary accountToDict(const NAccount& account)
{
    Dictionary d;
    d["user"] = userToDict(account.user);
    d["email"] = String(account.email.c_str());
    d["customId"] = String(account.custom_id.c_str());
    d["wallet"] = String(account.wallet.c_str());
    Array devices;
    for (auto& device : account.devices)
    {
        devices.append(deviceToDict(device));
    }
    d["devices"] = devices;
    return d;
}

Dictionary notificationToDict(const NNotification notification)
{
    Dictionary d;
    d["id"] = String(notification.id.c_str());
    d["code"] = int(notification.code);
    d["content"] = String(notification.content.c_str());
    d["subject"] = String(notification.subject.c_str());
    d["senderId"] = String(notification.senderId.c_str());
    d["createTime"] = int(notification.createTime);
    d["persistent"] = notification.persistent;
    return d;
}

void NakamaGodot::_register_methods() 
{
    // Properties
    register_property<NakamaGodot, String>("server_key", &NakamaGodot::serverKey, "defaultkey");
    register_property<NakamaGodot, String>("host", &NakamaGodot::host, "127.0.0.1");
    register_property<NakamaGodot, int>("port", &NakamaGodot::port, 7350);
    register_property<NakamaGodot, bool>("ssl", &NakamaGodot::ssl, false);
    register_property<NakamaGodot, bool>("realtime", &NakamaGodot::realtime, true);

    // Methods
    register_method("_process", &NakamaGodot::_process);
    register_method("_ready", &NakamaGodot::_ready);
    register_method("authenticate_email", &NakamaGodot::authenticate_email);
    register_method("authenticate_device", &NakamaGodot::authenticate_device);
    register_method("authenticate_facebook", &NakamaGodot::authenticate_facebook);
    register_method("authenticate_google", &NakamaGodot::authenticate_google);
    register_method("authenticate_game_center", &NakamaGodot::authenticate_game_center);
    register_method("authenticate_steam", &NakamaGodot::authenticate_steam);
    register_method("authenticate_custom", &NakamaGodot::authenticate_custom);

    register_method("join_chat", &NakamaGodot::join_chat);
    register_method("leave_chat", &NakamaGodot::leave_chat);
    register_method("write_chat_message", &NakamaGodot::write_chat_message);
    register_method("list_chat_messages", &NakamaGodot::list_chat_messages);

    register_method("is_realtime_client_connected", &NakamaGodot::is_realtime_client_connected);
    register_method("is_session_expired", &NakamaGodot::is_session_expired);

    register_method("store_object", &NakamaGodot::store_object);
    register_method("store_objects", &NakamaGodot::store_objects);
    register_method("fetch_objects", &NakamaGodot::fetch_object);
    register_method("fetch_objects", &NakamaGodot::fetch_objects);
    register_method("remove_object", &NakamaGodot::remove_object);
    register_method("remove_objects", &NakamaGodot::remove_objects);

    register_method("send_rpc", &NakamaGodot::send_rpc);

    register_method("list_groups", &NakamaGodot::list_groups);
    register_method("list_user_groups", &NakamaGodot::list_user_groups);
    register_method("list_group_members", &NakamaGodot::list_group_members);
    register_method("create_group", &NakamaGodot::create_group);
    register_method("open_group", &NakamaGodot::open_group);
    register_method("close_group", &NakamaGodot::close_group);
    register_method("remove_group", &NakamaGodot::remove_group);
    register_method("join_group", &NakamaGodot::join_group);
    register_method("leave_group", &NakamaGodot::leave_group);
    register_method("accept_members", &NakamaGodot::accept_members);
    register_method("promote_members", &NakamaGodot::promote_members);
    register_method("kick_members", &NakamaGodot::kick_members);

    register_method("add_friends", &NakamaGodot::add_friends);
    register_method("list_friends", &NakamaGodot::list_friends);
    register_method("remove_friends", &NakamaGodot::remove_friends);
    register_method("block_friends", &NakamaGodot::block_friends);

    register_method("fetch_account", &NakamaGodot::fetch_account);
    register_method("fetch_users", &NakamaGodot::fetch_users);
    register_method("update_account", &NakamaGodot::update_account);

    register_method("list_notifications", &NakamaGodot::list_notifications);
    register_method("delete_notifications", &NakamaGodot::delete_notifications);

    // Signals
    register_signal<NakamaGodot>("realtime_client_connected");

    register_signal<NakamaGodot>("authenticated", "session", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("authentication_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);

    register_signal<NakamaGodot>("channel_presence_event", "event", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("chat_message_sent", "channelId", GODOT_VARIANT_TYPE_STRING, "messageId", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("write_chat_message_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("chat_message_recieved", "message", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("chat_joined", "channel", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("join_chat_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("left_chat", "channelId", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("leave_chat_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("chat_messages_listed", "messages", GODOT_VARIANT_TYPE_ARRAY, "next", GODOT_VARIANT_TYPE_STRING, "prev", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("list_chat_messages_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);

    register_signal<NakamaGodot>("storage_read_complete", "objects", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("fetch_object_list_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("storage_write_complete");
    register_signal<NakamaGodot>("store_object_list_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("storage_remove_complete");
    register_signal<NakamaGodot>("remove_object_list_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);

    register_signal<NakamaGodot>("rpc_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("rpc_complete", "rpc_id", GODOT_VARIANT_TYPE_STRING, "response", GODOT_VARIANT_TYPE_STRING);

    register_signal<NakamaGodot>("group_listing_complete", "groups", GODOT_VARIANT_TYPE_ARRAY, "cursor", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("list_groups_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("user_groups_listed", "groups", GODOT_VARIANT_TYPE_ARRAY, "cursor", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("list_user_groups_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("group_members_listed", "members", GODOT_VARIANT_TYPE_DICTIONARY, "cursor", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("list_group_members_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("group_created", "group", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("create_group_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("group_opened", "groupId", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("open_group_failed", "code", GODOT_VARIANT_TYPE_INT, "message",GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("group_closed", "groupId", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("close_group_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("group_created", "group", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("create_group_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("group_removed", "group", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("remove_group_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("joined_group", "groupId", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("join_group_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("left_group", "groupId", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("leave_group_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("accepted_members", "groupId", GODOT_VARIANT_TYPE_STRING, "userIds", GODOT_VARIANT_TYPE_ARRAY);
    register_signal<NakamaGodot>("accept_members_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("promoted_members", "groupId", GODOT_VARIANT_TYPE_STRING, "userIds", GODOT_VARIANT_TYPE_ARRAY);
    register_signal<NakamaGodot>("promote_members_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("kicked_members", "groupId", GODOT_VARIANT_TYPE_STRING, "userIds", GODOT_VARIANT_TYPE_ARRAY);
    register_signal<NakamaGodot>("kick_members_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);

    register_signal<NakamaGodot>("friends_added");
    register_signal<NakamaGodot>("add_friends_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("friends_listed", "friends", GODOT_VARIANT_TYPE_ARRAY, "cursor", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("list_friends_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("friends_removed");
    register_signal<NakamaGodot>("remove_friends_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("friends_blocked");
    register_signal<NakamaGodot>("block_friends_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);

    register_signal<NakamaGodot>("account_fetched", "account", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("fetch_account_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("users_fetched", "users", GODOT_VARIANT_TYPE_ARRAY);
    register_signal<NakamaGodot>("fetch_users_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("acount_updated");
    register_signal<NakamaGodot>("update_account_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);

    register_signal<NakamaGodot>("notifications_listed", "notifications", GODOT_VARIANT_TYPE_ARRAY, "cursor", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("list_notifications_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("notifications_deleted", "deletedIds", GODOT_VARIANT_TYPE_ARRAY);
    register_signal<NakamaGodot>("delete_notifications_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("notifications_received", "notifications", GODOT_VARIANT_TYPE_ARRAY, "cursor", GODOT_VARIANT_TYPE_STRING);
}

void NakamaGodot::_init() 
{
    NLogger::init(std::make_shared<NGodotLogSink>(), NLogLevel::Debug);
}

void NakamaGodot::_ready()
{
    create_client();
}

void NakamaGodot::_process(float delta) 
{
    if (!client) return;
    time_since_last_tick += delta;

    if (time_since_last_tick > 0.05) 
    {
        client->tick();
        if (rtClient)
            rtClient->tick();
        time_since_last_tick = 0;
    }
}

NakamaGodot::NakamaGodot() { }

NakamaGodot::~NakamaGodot() { }

void NakamaGodot::create_client() 
{
    parameters.serverKey = serverKey.utf8().get_data();
    parameters.host = host.utf8().get_data();
    parameters.port = port;
    parameters.ssl = ssl;
    client = createDefaultClient(parameters);
}

int NakamaGodot::authenticate_email(String email, String password, String username, bool create, Dictionary vars) 
{
    if (!client) return 3; // ERR_UNCONFIGURED

    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("authentication_failed", error);
    };
    client->authenticateEmail(
        std::string(email.utf8().get_data()), 
        std::string(password.utf8().get_data()), 
        username.utf8().get_data(), 
        create, 
        varMap(vars), 
        std::bind(&NakamaGodot::authenticated, this, _1), 
        err_callback
    );

    return 0; // OK
}

int NakamaGodot::authenticate_device(String deviceId, String username, bool create, Dictionary vars)
{
    if (!client) return 3; // ERR_UNCONFIGURED

    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("authentication_failed", error);
    };
    opt::optional_lite::optional<std::string> userOpt = opt::nullopt;
    if (username != "") userOpt = username.utf8().get_data();
    client->authenticateDevice(
        deviceId.utf8().get_data(),
        userOpt,
        create,
        varMap(vars),
        std::bind(&NakamaGodot::authenticated, this, _1),
        err_callback
    );

    return 0; // OK
}

int NakamaGodot::authenticate_facebook(String oauthToken, String username, bool create, bool importFriends, Dictionary vars)
{
    if (!client) return 3; // ERR_UNCONFIGURED

    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("authentication_failed", error);
    };
    
    client->authenticateFacebook(
        oauthToken.utf8().get_data(),
        username.utf8().get_data(),
        create,
        importFriends,
        varMap(vars),
        std::bind(&NakamaGodot::authenticated, this, _1),
        err_callback
    );

    return 0; // OK
}

int NakamaGodot::authenticate_google(String oauthToken, String username, bool create, Dictionary vars)
{
    if (!client) return 3; // ERR_UNCONFIGURED

    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("authentication_failed", error);
    };
    
    client->authenticateGoogle(
        oauthToken.utf8().get_data(),
        username.utf8().get_data(),
        create,
        varMap(vars),
        std::bind(&NakamaGodot::authenticated, this, _1),
        err_callback
    );

    return 0; // OK
}

int NakamaGodot::authenticate_game_center(
    String playerId, 
    String bundleId, 
    int timestampSeconds, 
    String salt, 
    String signature, 
    String publicKeyUrl, 
    String username, 
    bool create, 
    Dictionary vars
)
{
    if (!client) return 3; // ERR_UNCONFIGURED

    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("authentication_failed", error);
    };

    client->authenticateGameCenter(
        playerId.utf8().get_data(),
        bundleId.utf8().get_data(),
        NTimestamp(timestampSeconds),
        salt.utf8().get_data(),
        signature.utf8().get_data(),
        publicKeyUrl.utf8().get_data(),
        username.utf8().get_data(),
        create,
        varMap(vars),
        std::bind(&NakamaGodot::authenticated, this, _1),
        err_callback
    );

    return 0; // OK
}

int NakamaGodot::authenticate_steam(String token, String username, bool create, Dictionary vars)
{
    if (!client) return 3; // ERR_UNCONFIGURED

    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("authentication_failed", error);
    };
    client->authenticateSteam(
        token.utf8().get_data(),
        username.utf8().get_data(),
        create,
        varMap(vars),
        std::bind(&NakamaGodot::authenticated, this, _1),
        err_callback
    );

    return 0; // OK
}

int NakamaGodot::authenticate_custom(String id, String username, bool create, Dictionary vars)
{
    if (!client) return 3; // ERR_UNCONFIGURED

    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("authentication_failed", error);
    };
    opt::optional_lite::optional<std::string> userOpt = opt::nullopt;
    if (username != "") userOpt = username.utf8().get_data();
    client->authenticateDevice(
        id.utf8().get_data(),
        userOpt,
        create,
        varMap(vars),
        std::bind(&NakamaGodot::authenticated, this, _1),
        err_callback
    );

    return 0; // OK
}

void NakamaGodot::authenticated(NSessionPtr session) 
{
    NakamaGodot::session = session;
    rtListener.setNotificationsCallback([this](const NNotificationList& list){
        Array notifications;
        for (auto& notification : list.notifications)
        {
            notifications.append(notificationToDict(notification));
        }
        emit_signal("notifications_received", notifications, String(list.cacheableCursor.c_str()));
    });
    if (realtime)
        connect_realtime_client();
    emit_signal("authenticated", sessionToDict(session));
}

int NakamaGodot::connect_realtime_client() 
{
    if (!session) return 2; // ERR_UNCONFIGURED
    if (session->isExpired()) return 4; // ERR_UNAUTHORIZED
    if (rtClient) return 32; //ERR_ALREADY_EXISTS

    int port = 7350;
    bool createStatus = true;
    rtClient = client->createRtClient(port);

    rtListener.setConnectCallback([this]() 
    {
        emit_signal("realtime_client_connected");
    });
    rtListener.setChannelPresenceCallback([this](const NChannelPresenceEvent& event)
    {
        emit_signal("channel_presence_event", presenceEventToDict(event));
    });

    rtListener.setChannelMessageCallback([this](const NChannelMessage& message) 
    {
        emit_signal("chat_message_recieved", messageToDict(message));
    });

    rtClient->setListener(&rtListener);
    rtClient->connect(session, createStatus);

    return 0; // OK
}

bool NakamaGodot::is_realtime_client_connected() 
{
    return (rtClient && rtClient->isConnected());
}

bool NakamaGodot::is_session_expired() 
{
    return !(session && !session->isExpired());
}

void NakamaGodot::join_chat(String roomName, int type, bool persist, bool hidden) 
{
    auto sucessJoinCallback = [this](NChannelPtr channel) 
    {
        emit_signal("chat_joined", channelToDict(channel));
    };

    auto errorJoinCallback = [this](const NRtError& error) 
    {
        emit_rt_error_signal("join_chat_failed", error);
    };

    rtClient->joinChat(
        roomName.utf8().get_data(),
        (NChannelType)type,
        persist,
        hidden,
        sucessJoinCallback,
        errorJoinCallback
    );
}

void NakamaGodot::leave_chat(String channelId)
{
    auto err_callback = [this](const NRtError& error)
    {
        emit_rt_error_signal("leave_chat_failed", error);
    };
    auto success_callback = [this, channelId](){
        emit_signal("left_chat", channelId);
    };

    rtClient->leaveChat(
        channelId.utf8().get_data(),
        success_callback, 
        err_callback
    );
}

void NakamaGodot::write_chat_message(String channelId, String content) 
{
    auto err_callback = [this](const NRtError& error)
    {
        emit_rt_error_signal("write_chat_message_failed", error);
    };
    auto success_callback = [this](const NChannelMessageAck& ack)
    {
        emit_signal("chat_message_sent", String(ack.channelId.c_str()), String(ack.messageId.c_str()));
    };
    rtClient->writeChatMessage(
        channelId.utf8().get_data(), 
        content.utf8().get_data(), 
        success_callback, 
        err_callback
    );
}

void NakamaGodot::list_chat_messages(String channelId, int limit, bool forward, String cursor)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("list_chat_messages_failed", error);
    };

    auto success_callback = [this](NChannelMessageListPtr list)
    {
        Array messages;
        for (auto& listItem : list->messages)
        {
            messages.append(messageToDict(listItem));
        }        
        emit_signal("chat_messages_listed", messages, String(list->nextCursor.c_str()), String(list->prevCursor.c_str()));
    };
    opt::optional_lite::optional<int> limitOpt = opt::nullopt;
    if (limit >= 0) limitOpt = limit;
    client->listChannelMessages(
        session,
        channelId.utf8().get_data(),
        limitOpt,
        cursor.utf8().get_data(),
        forward,
        success_callback,
        err_callback
    );
}

void NakamaGodot::store_object(String collection, String key, Dictionary value) 
{
    std::vector<NStorageObjectWrite> objects;
    NStorageObjectWrite write;
    write.collection = collection.utf8().get_data();
    write.key = key.utf8().get_data();
    write.value = value.to_json().utf8().get_data();
    objects.push_back(write);
    store_object_list(objects);
}

void NakamaGodot::store_objects(String collection, Dictionary kvps) 
{
    std::vector<NStorageObjectWrite> objects;
    auto keys = kvps.keys();
    for (int i = 0; i < keys.size(); i++)
    {
        NStorageObjectWrite write;
        write.collection = collection.utf8().get_data();
        auto key = keys[i].operator String();
        auto value = Dictionary(kvps[keys[i]]);
        write.key = key.utf8().get_data();
        write.value = value.to_json().utf8().get_data();
        objects.push_back(write);    
    }
    store_object_list(objects);
}

void NakamaGodot::store_object_list(std::vector<NStorageObjectWrite> objects)
{
    auto success_callback = [this](const auto& _) 
    {
        emit_signal("storage_write_complete"); 
    };
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("store_object_list_failed", error);
    };
    client->writeStorageObjects(
        session, objects, 
        success_callback,  
        err_callback
    );
}

void NakamaGodot::fetch_object(String collection, String key) 
{
    std::vector<NReadStorageObjectId> objectIds;
    NReadStorageObjectId id;
    id.collection = collection.utf8().get_data();
    id.key = key.utf8().get_data();
    id.userId = session->getUserId();
    objectIds.push_back(id);
    fetch_object_list(objectIds);
}

void NakamaGodot::fetch_objects(String collection, Array keys)
{
    std::vector<NReadStorageObjectId> objectIds;
    for (int i = 0; i < keys.size(); i++)
    {
        auto key = keys[i].operator String();
        NReadStorageObjectId id;
        id.collection = collection.utf8().get_data();
        id.key = key.utf8().get_data();
        objectIds.push_back(id);
    }
    fetch_object_list(objectIds);
}

void NakamaGodot::fetch_object_list(std::vector<NReadStorageObjectId> ids)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("fetch_object_list_failed", error);
    };
    auto success_callback = [this](const NStorageObjects& objects) 
    {
        Dictionary d;
        for (auto& object : objects)
        {
            d[String(object.key.c_str())] = String(object.value.c_str());
        }
        emit_signal("storage_read_complete", d);
    };
    client->readStorageObjects(
        session, ids, 
        success_callback,
        err_callback
    );
}

void NakamaGodot::remove_object(String collection, String key)
{
    std::vector<NDeleteStorageObjectId> objectIds;
    NDeleteStorageObjectId id;
    id.collection = collection.utf8().get_data();
    id.key = key.utf8().get_data();
    objectIds.push_back(id);
    remove_object_list(objectIds);
}

void NakamaGodot::remove_objects(String collection, Array keys)
{
    std::vector<NDeleteStorageObjectId> objectIds;
    for (int i = 0; i < keys.size(); i++)
    {
        auto key = keys[i].operator String();
        NDeleteStorageObjectId id;
        id.collection = collection.utf8().get_data();
        id.key = key.utf8().get_data();
        objectIds.push_back(id);
    }
    remove_object_list(objectIds);
}

void NakamaGodot::remove_object_list(std::vector<NDeleteStorageObjectId> ids)
{
    auto err_callback = [this](const NError& error)
    { 
        emit_error_signal("remove_object_list_failed", error);
    };
    auto success_callback = [this]()
    { 
        emit_signal("storage_remove_complete"); 
    };
    client->deleteStorageObjects(
        session, 
        ids,
        success_callback,  
        err_callback
    );
}

void NakamaGodot::send_rpc(String rpcId, Dictionary payload)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("rpc_failed", error);
    };
    auto success_callback = [this](const NRpc& rpc)
    {
        emit_signal("rpc_complete", String(rpc.id.c_str()), String(rpc.payload.c_str()));
    };
    client->rpc(
        session, 
        rpcId.utf8().get_data(), 
        payload.to_json().utf8().get_data(), 
        success_callback, 
        err_callback
    );
}

void NakamaGodot::list_groups(String filter, int limit, String cursor)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("list_groups_failed", error);
    };
    auto success_callback = [this](NGroupListPtr list)
    {
        Array arr;
        for (auto& group : list->groups)
        {
            arr.append(groupToDict(group));
        }
        emit_signal("group_listing_complete", arr, String(list->cursor.c_str()));
    };
    client->listGroups(
        session,
        filter.utf8().get_data(),
        limit,
        cursor.utf8().get_data(),
        success_callback,
        err_callback
    );
}

void NakamaGodot::list_user_groups(String userId, int limit, int state, String cursor)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("list_user_groups_failed", error);
    };
    auto success_callback = [this](NUserGroupListPtr list)
    {
        Array arr;
        for (auto& userGroup : list->userGroups)
        {
            arr.append(userGroupToDict(userGroup));
        }
        emit_signal("user_groups_listed", arr, String(list->cursor.c_str()));
    };
    opt::optional<NFriend::State> stateOpt = {};
    opt::optional<int32_t> limitOpt = {};
    if (state >= 0 && state <= 3) { stateOpt = NFriend::State(state); }
    if (limit > 0) { limitOpt = limit; }
    client->listUserGroups(
        session,
        userId.utf8().get_data(),
        limitOpt, 
        stateOpt,
        cursor.utf8().get_data(),
        success_callback,
        err_callback
    );
}

void NakamaGodot::list_group_members(String groupId, int limit, int state, String cursor)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("list_group_members_failed", error);
    };
    auto success_callback = [this](NGroupUserListPtr list)
    {
        Array arr;
        for (auto& member : list->groupUsers)
        {
            arr.append(groupMemberToDict(member));
        }
        emit_signal("group_members_listed", arr, String(list->cursor.c_str()));
    };
    opt::optional<NFriend::State> stateOpt = {};
    opt::optional<int32_t> limitOpt = {};
    if (state >= 0 && state <= 3) { stateOpt = NFriend::State(state); }
    if (limit > 0) { limitOpt = limit; }
    client->listGroupUsers(
        session,
        groupId.utf8().get_data(),
        limitOpt,
        stateOpt,
        cursor.utf8().get_data(),
        success_callback,
        err_callback
    );
}

void NakamaGodot::create_group(String name, String lang, bool open, String desc, String avatarUrl, int maxCount)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("create_group_failed", error);
    };
    auto success_callback = [this](const NGroup& group)
    {
        emit_signal("group_created", groupToDict(group));
    };
    opt::optional<int32_t> maxCountOpt = {};
    if (maxCount > 0) { maxCountOpt = maxCount; }
    client->createGroup(
        session,
        name.utf8().get_data(),
        desc.utf8().get_data(),
        avatarUrl.utf8().get_data(),
        lang.utf8().get_data(),
        open,
        maxCountOpt,
        success_callback,
        err_callback
    );
}

void NakamaGodot::update_group(String groupId, String name, String desc, String lang, String avatarUrl)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("update_group_failed", error);
    };
    auto success_callback = [this, groupId]()
    {
        emit_signal("group_updated", groupId);
    };
    opt::optional_lite::optional<std::string> nameOpt = opt::nullopt;
    opt::optional_lite::optional<std::string> descOpt = opt::nullopt;
    opt::optional_lite::optional<std::string> avatarOpt = opt::nullopt;
    opt::optional_lite::optional<std::string> langOpt = opt::nullopt;
    if (name != "") { nameOpt = name.utf8().get_data(); }
    if (desc != "") { descOpt = desc.utf8().get_data(); }
    if (avatarUrl != "") { avatarOpt = avatarUrl.utf8().get_data(); }
    if (lang != "") { langOpt = lang.utf8().get_data(); }
    client->updateGroup(
        session,
        groupId.utf8().get_data(),
        nameOpt,
        descOpt,
        avatarOpt,
        langOpt,
        opt::nullopt,
        success_callback,
        err_callback
    );
}

void NakamaGodot::open_group(String groupId)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("open_group_failed", error);
    };
    auto success_callback = [this, groupId]()
    {
        emit_signal("group_opened", groupId);
    };
    client->updateGroup(
        session,
        groupId.utf8().get_data(),
        opt::nullopt,
        opt::nullopt,
        opt::nullopt,
        opt::nullopt,
        true,
        success_callback,
        err_callback
    );
}

void NakamaGodot::close_group(String groupId)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("close_group_failed", error);
    };
    auto success_callback = [this, groupId]()
    {
        emit_signal("group_closed", groupId);
    };
    client->updateGroup(
        session,
        groupId.utf8().get_data(),
        opt::nullopt,
        opt::nullopt,
        opt::nullopt,
        opt::nullopt,
        false,
        success_callback,
        err_callback
    );
}

void NakamaGodot::remove_group(String groupId)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("remove_group_failed", error);
    };
    auto success_callback = [this, groupId]()
    {
        emit_signal("group_removed", groupId);
    };
    client->deleteGroup(
        session,
        groupId.utf8().get_data(),
        success_callback,
        err_callback
    );
}

void NakamaGodot::join_group(String groupId)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("join_group_failed", error);
    };
    auto success_callback = [this, groupId]()
    {
        emit_signal("joined_group", groupId);
    };
    client->joinGroup(
        session, 
        groupId.utf8().get_data(), 
        success_callback,
        err_callback
    );
}

void NakamaGodot::leave_group(String groupId)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("leave_group_failed", error);
    };
    auto success_callback = [this, groupId]()
    {
        emit_signal("left_group", groupId);
    };
    client->leaveGroup(
        session, 
        groupId.utf8().get_data(), 
        success_callback, 
        err_callback
    );
}

void NakamaGodot::accept_members(String groupId, Array userIds)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("accept_members_failed", error);
    };
    auto success_callback = [this, groupId, userIds]()
    {
        emit_signal("accepted_members", groupId, userIds);
    };
    std::vector<std::string> buff;
    for (int i = 0; i < userIds.size(); i++)
    {
        auto id = userIds[i].operator String();
        buff.push_back(id.utf8().get_data());
    }
    client->addGroupUsers(
        session,
        groupId.utf8().get_data(),
        buff,
        success_callback,
        err_callback
    );
}

void NakamaGodot::promote_members(String groupId, Array userIds)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("promote_members_failed", error);
    };
    auto success_callback = [this, groupId, userIds]()
    {
        emit_signal("promoted_members", groupId, userIds);
    };
    std::vector<std::string> buff;
    for (int i = 0; i < userIds.size(); i++)
    {
        auto id = userIds[i].operator String();
        buff.push_back(id.utf8().get_data());
    }
    client->promoteGroupUsers(
        session,
        groupId.utf8().get_data(),
        buff,
        success_callback,
        err_callback
    );
}

void NakamaGodot::kick_members(String groupId, Array userIds)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("kick_members_failed", error);
    };
    auto success_callback = [this, groupId, userIds]()
    {
        emit_signal("kicked_members", groupId, userIds);
    };
    std::vector<std::string> buff;
    for (int i = 0; i < userIds.size(); i++)
    {
        auto id = userIds[i].operator String();
        buff.push_back(id.utf8().get_data());
    }
    client->kickGroupUsers(
        session,
        groupId.utf8().get_data(),
        buff,
        success_callback,
        err_callback
    );
}

void NakamaGodot::add_friends(Array userIds, Array usernames)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("add_friends_failed", error);
    };
    auto success_callback = [this]()
    {
        emit_signal("friends_added");
    };
    std::vector<std::string> idBuff;
    std::vector<std::string> usernameBuff;
    for (int i = 0; i < userIds.size(); i++)
    {
        auto userId = userIds[i].operator String();
        idBuff.push_back(userId.utf8().get_data());
    }
    for (int i = 0; i < usernames.size(); i++)
    {
        auto username = usernames[i].operator String();
        usernameBuff.push_back(username.utf8().get_data());
    }
    client->addFriends(
        session,
        idBuff,
        usernameBuff,
        success_callback,
        err_callback
    );
}

void NakamaGodot::list_friends(int limit, int state, String cursor)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("list_friends_failed", error);
    };
    auto success_callback = [this](NFriendListPtr friends)
    {
        Array arr;
        for (auto& fr : friends->friends)
        {
            arr.append(friendToDict(fr));
        }
        emit_signal("friends_listed", arr, friends->cursor.c_str());
    };
    opt::optional_lite::optional<int32_t> limitOpt = {};
    opt::optional_lite::optional<NFriend::State> stateOpt;
    if (limit > 0) limitOpt = limit;
    if (state >= 0 && state <= 3) stateOpt = NFriend::State(state);
    client->listFriends(
        session,
        limitOpt,
        stateOpt,
        cursor.utf8().get_data(),
        success_callback,
        err_callback
    );
}

void NakamaGodot::remove_friends(Array userIds, Array usernames)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("remove_friends_failed", error);
    };
    auto success_callback = [this]()
    {
        emit_signal("friends_removed");
    };
    std::vector<std::string> idBuff;
    std::vector<std::string> usernameBuff;
    for (int i = 0; i < userIds.size(); i++)
    {
        auto userId = userIds[i].operator String();
        idBuff.push_back(userId.utf8().get_data());
    }
    for (int i = 0; i < usernames.size(); i++)
    {
        auto username = usernames[i].operator String();
        usernameBuff.push_back(username.utf8().get_data());
    }
    client->deleteFriends(
        session,
        idBuff,
        usernameBuff,
        success_callback,
        err_callback
    );   
}

void NakamaGodot::block_friends(Array userIds, Array usernames)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("block_friends_failed", error);
    };
    auto success_callback = [this]()
    {
        emit_signal("friends_blocked");
    };
    std::vector<std::string> idBuff;
    std::vector<std::string> usernameBuff;
    for (int i = 0; i < userIds.size(); i++)
    {
        auto userId = userIds[i].operator String();
        idBuff.push_back(userId.utf8().get_data());
    }
    for (int i = 0; i < usernames.size(); i++)
    {
        auto username = usernames[i].operator String();
        usernameBuff.push_back(username.utf8().get_data());
    }
    client->blockFriends(
        session,
        idBuff,
        usernameBuff,
        success_callback,
        err_callback
    );   
}

void NakamaGodot::fetch_account()
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("fetch_account_failed", error);
    };
    auto success_callback = [this](const NAccount& account)
    {
        emit_signal("account_fetched", accountToDict(account));
    };
    client->getAccount(
        session,
        success_callback,
        err_callback
    );
}

void NakamaGodot::fetch_users(Array ids, Array usernames, Array facebookIds)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("fetch_users_failed", error);
    };
    auto success_callback = [this](const NUsers& users)
    {
        Array arr;
        for (auto& user : users.users)
        {
            arr.append(userToDict(user));
        }
        emit_signal("users_fetched", arr);
    };
    std::vector<std::string> idVec;
    for (int i = 0; i < ids.size(); i++)
    {
        if (ids[i].get_type() == Variant::Type::STRING)
            idVec.push_back(ids[i].operator String().utf8().get_data());
    }
    std::vector<std::string> usernameVec;
    for (int i = 0; i < usernames.size(); i++)
    {
        if (usernames[i].get_type() == Variant::Type::STRING)
            usernameVec.push_back(usernames[i].operator String().utf8().get_data());
    }
    std::vector<std::string> facebookIdVec;
    for (int i = 0; i < facebookIds.size(); i++)
    {
        if (facebookIds[i].get_type() == Variant::Type::STRING)
            facebookIdVec.push_back(facebookIds[i].operator String().utf8().get_data());
    }
    client->getUsers(
        session,
        idVec,
        usernameVec,
        facebookIdVec,
        success_callback,
        err_callback
    );
}

void NakamaGodot::update_account(String username, String displayName, String avatarUrl, String lang, String location, String timezone)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("update_account_failed", error);
    };
    auto success_callback = [this]()
    {
        emit_signal("account_updated");
    };
    client->updateAccount(
        session,
        username.utf8().get_data(),
        displayName.utf8().get_data(),
        avatarUrl.utf8().get_data(),
        lang.utf8().get_data(),
        location.utf8().get_data(),
        timezone.utf8().get_data(),
        success_callback,
        err_callback
    );
}

void NakamaGodot::list_notifications(String cursor, int limit)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("list_notifications_failed", error);
    };
    auto success_callback = [this](NNotificationListPtr list)
    {
        Array notifications;
        for (auto& notification : list->notifications)
        {
            notifications.append(notificationToDict(notification));
        }
        emit_signal("notifications_listed", notifications, String(list->cacheableCursor.c_str()));
    };
    opt::optional_lite::optional<int32_t> limitOpt = opt::nullopt;
    if (limit > 0) limitOpt = limit;
    client->listNotifications(
        session,
        limitOpt,
        cursor.utf8().get_data(),
        success_callback,
        err_callback
    );
}

void NakamaGodot::delete_notifications(Array notificationIds)
{
    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("delete_notifications_failed", error);
    };
    auto success_callback = [this, notificationIds]()
    {
        emit_signal("notifications_deleted", notificationIds);
    };
    std::vector<std::string> ids;
    for (int i = 0; i < notificationIds.size(); i++)
    {
        if (notificationIds[i].get_type() == Variant::Type::STRING)
            ids.push_back(notificationIds[i].operator String().utf8().get_data());
    }
    client->deleteNotifications(
        session,
        ids,
        success_callback,
        err_callback
    );
}

void NakamaGodot::emit_error_signal(String signal, const NError& error)
{
    emit_signal(signal, (int)error.code, String(error.message.c_str()));
}

void NakamaGodot::emit_rt_error_signal(String signal, const NRtError& error)
{
    emit_signal(signal, (int)error.code, String(error.message.c_str()));
}