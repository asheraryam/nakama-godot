#include "nakamagodot.h"

using namespace std::placeholders;
using namespace godot;

Dictionary rtErrToDict(const NRtError& error) {
    Dictionary d;
    d["code"] = (int)error.code;
    d["message"] = String(error.message.c_str());
    return d;
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
    Dictionary vd;
    auto vars = session->getVariables();
    for (auto it = vars.begin(); it != vars.end(); it++)
    {
        vd[String(it->first.c_str())] = String(it->second.c_str());
    }
    d["variables"] = vd;
    return d;
}

void NakamaGodot::_register_methods() {
    // Methods
    register_method("_process", &NakamaGodot::_process);
    register_method("create_client_default", &NakamaGodot::create_client_default);
    register_method("create_client", &NakamaGodot::create_client);
    register_method("authenticate_email", &NakamaGodot::authenticate_email);
    register_method("connect_realtime_client", &NakamaGodot::connect_realtime_client);
    register_method("join_chat_room", &NakamaGodot::join_chat_room);
    register_method("write_chat_message", &NakamaGodot::write_chat_message);

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

    // Signals
    register_signal<NakamaGodot>("authenticated", "session", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("authentication_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);

    register_signal<NakamaGodot>("write_chat_message_failed", "code", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("chat_message_recieved", "message", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("chat_joined", "channel", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("chat_join_failed", "error", GODOT_VARIANT_TYPE_DICTIONARY);

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
}

void NakamaGodot::_init() {
    NLogger::init(std::make_shared<NGodotLogSink>(), NLogLevel::Debug);
}

void NakamaGodot::_process(float delta) {
    if (!client) return;
    time_since_last_tick += delta;

    if (time_since_last_tick > 0.05) {
        client->tick();
        if (rtClient)
            rtClient->tick();
        time_since_last_tick = 0;
    }
}

NakamaGodot::NakamaGodot() {
}

NakamaGodot::~NakamaGodot() {
    
}

void NakamaGodot::create_client_default() {
    create_client("defaultkey", "127.0.0.1", 7349);
}

void NakamaGodot::create_client(String server_key, String server_host, int port) {
    parameters.serverKey = server_key.utf8().get_data();
    parameters.host = server_host.utf8().get_data();
    parameters.port = port;
    client = createDefaultClient(parameters);
}

void NakamaGodot::authenticate_email(String email, String password, String username) {

    if (!client) return;

    auto err_callback = [this](const NError& error)
    {
        emit_error_signal("authentication_failed", error);
    };

    client->authenticateEmail(
        std::string(email.utf8().get_data()), 
        std::string(password.utf8().get_data()), 
        username.utf8().get_data(), 
        true, 
        {}, 
        std::bind(&NakamaGodot::authenticated, this, _1), 
        err_callback);
}

void NakamaGodot::authenticated(NSessionPtr session) {
    NakamaGodot::session = session;
    emit_signal("authenticated", sessionToDict(session));
}

int NakamaGodot::connect_realtime_client() {
    if (!session) return 2; // ERR_UNCONFIGURED
    if (session->isExpired()) return 4; // ERR_UNAUTHORIZED
    if (rtClient) return 32; //ERR_ALREADY_EXISTS

    int port = 7350;
    bool createStatus = true;
    rtClient = client->createRtClient(port);

    rtListener.setConnectCallback([]() {
                Godot::print("Realtime client connected");
            });
    rtClient->setListener(&rtListener);
    rtClient->connect(session, createStatus);
    
    return 0; // OK
}

bool NakamaGodot::is_realtime_client_connected() {
    return (rtClient && rtClient->isConnected());
}

bool NakamaGodot::is_session_expired() {
    return !(session && !session->isExpired());
}

void NakamaGodot::join_chat_room(String roomName, int type, bool persist, bool hidden) {
    rtListener.setChannelMessageCallback([this](const NChannelMessage& message) {
                Godot::print("Recieved message on channel {0}", message.channelId.c_str());
                Godot::print("Message content: {0}", message.content.c_str());

                emit_signal("chat_message_recieved", messageToDict(message));
            });

    auto sucessJoinCallback = [this](NChannelPtr channel) {
        emit_signal("chat_joined", channelToDict(channel));
    };

    auto errorJoinCallback = [this](const NRtError& error) {
        emit_signal("chat_join_failed", rtErrToDict(error));
    };

    rtClient->joinChat(
            roomName.utf8().get_data(),
            (NChannelType)type,
            persist,
            hidden,
            sucessJoinCallback,
            errorJoinCallback);
}

void NakamaGodot::write_chat_message(String channelId, String content) {
    auto err_callback = [this](const NRtError& error)
    {
        emit_rt_error_signal("write_chat_message_failed", error);
    };
    rtClient->writeChatMessage(
        channelId.utf8().get_data(), 
        content.utf8().get_data(), 
        nullptr, 
        err_callback);
}

void NakamaGodot::store_object(String collection, String key, Dictionary value) {
    std::vector<NStorageObjectWrite> objects;
    NStorageObjectWrite write;
    write.collection = collection.utf8().get_data();
    write.key = key.utf8().get_data();
    write.value = value.to_json().utf8().get_data();
    objects.push_back(write);
    store_object_list(objects);
}

void NakamaGodot::store_objects(String collection, Dictionary kvps) {
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

void NakamaGodot::emit_error_signal(String signal, const NError& error)
{
    emit_signal(signal, (int)error.code, String(error.message.c_str()));
}

void NakamaGodot::emit_rt_error_signal(String signal, const NRtError& error)
{
    emit_signal(signal, (int)error.code, String(error.message.c_str()));
}