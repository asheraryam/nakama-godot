#include "nakamagodot.h"

using namespace std::placeholders;
using namespace godot;

Dictionary errToDict(const NError& error)
{
    Dictionary d;
    d["code"] = (int)error.code;
    d["message"] = String(error.message.c_str());
    return d;
}

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

Dictionary groupMemberToDict(NGroupUser groupUser)
{
    auto d = userToDict(groupUser.user);
    d["state"] = (int)groupUser.state;
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

    // Signals
    register_signal<NakamaGodot>("authenticated");
    register_signal<NakamaGodot>("authentication_failed", "error", GODOT_VARIANT_TYPE_DICTIONARY);

    register_signal<NakamaGodot>("nakama_error", "error", GODOT_VARIANT_TYPE_DICTIONARY);

    register_signal<NakamaGodot>("chat_message_recieved", "channel_id", GODOT_VARIANT_TYPE_STRING, "message_id", GODOT_VARIANT_TYPE_STRING, "message_code", GODOT_VARIANT_TYPE_INT, "sender_id", GODOT_VARIANT_TYPE_STRING, "username", GODOT_VARIANT_TYPE_STRING, "content", GODOT_VARIANT_TYPE_STRING);

    register_signal<NakamaGodot>("chat_joined", "channel_id", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("chat_join_failed", "error", GODOT_VARIANT_TYPE_DICTIONARY);

    register_signal<NakamaGodot>("storage_read_complete", "objects", GODOT_VARIANT_TYPE_DICTIONARY, "error", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("storage_write_complete", "error", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("storage_remove_complete", "error", GODOT_VARIANT_TYPE_DICTIONARY);

    register_signal<NakamaGodot>("rpc_failed", "error", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("rpc_complete", "rpc_id", GODOT_VARIANT_TYPE_STRING, "response", GODOT_VARIANT_TYPE_STRING);

    register_signal<NakamaGodot>("group_listing_complete", GODOT_VARIANT_TYPE_ARRAY, "groups", GODOT_VARIANT_TYPE_STRING, "cursor");
    register_signal<NakamaGodot>("list_groups_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
    register_signal<NakamaGodot>("user_groups_listed", GODOT_VARIANT_TYPE_ARRAY, "groups", GODOT_VARIANT_TYPE_STRING, "cursor");
    register_signal<NakamaGodot>("list_user_groups_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
    register_signal<NakamaGodot>("group_members_listed", GODOT_VARIANT_TYPE_DICTIONARY, "members", GODOT_VARIANT_TYPE_STRING, "cursor");
    register_signal<NakamaGodot>("list_group_members_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
    register_signal<NakamaGodot>("group_created", GODOT_VARIANT_TYPE_DICTIONARY, "group");
    register_signal<NakamaGodot>("create_group_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
    register_signal<NakamaGodot>("group_opened", GODOT_VARIANT_TYPE_STRING, "groupId");
    register_signal<NakamaGodot>("open_group_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
    register_signal<NakamaGodot>("group_closed", GODOT_VARIANT_TYPE_STRING, "groupId");
    register_signal<NakamaGodot>("close_group_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
    register_signal<NakamaGodot>("group_created", GODOT_VARIANT_TYPE_DICTIONARY, "group");
    register_signal<NakamaGodot>("create_group_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
    register_signal<NakamaGodot>("group_removed", GODOT_VARIANT_TYPE_DICTIONARY, "group");
    register_signal<NakamaGodot>("remove_group_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
    register_signal<NakamaGodot>("joined_group", GODOT_VARIANT_TYPE_STRING, "groupId");
    register_signal<NakamaGodot>("join_group_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
    register_signal<NakamaGodot>("left_group", GODOT_VARIANT_TYPE_STRING, "groupId");
    register_signal<NakamaGodot>("leave_group_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
    register_signal<NakamaGodot>("accepted_members", GODOT_VARIANT_TYPE_STRING, "groupId", GODOT_VARIANT_TYPE_ARRAY, "userIds");
    register_signal<NakamaGodot>("accept_members_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
    register_signal<NakamaGodot>("promoted_members", GODOT_VARIANT_TYPE_STRING, "groupId", GODOT_VARIANT_TYPE_ARRAY, "userIds");
    register_signal<NakamaGodot>("promote_members_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
    register_signal<NakamaGodot>("kicked_members", GODOT_VARIANT_TYPE_STRING, "groupId", GODOT_VARIANT_TYPE_ARRAY, "userIds");
    register_signal<NakamaGodot>("kick_members_failed", GODOT_VARIANT_TYPE_INT, "code", GODOT_VARIANT_TYPE_STRING, "message");
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
    Godot::print("Destructing");
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
        Dictionary d;
        d["code"] = (int)error.code;
        d["message"] = String(error.message.c_str());
        emit_signal("autentication_failed", d);
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
    Godot::print(godot::String("Session token: ") + session->getAuthToken().c_str());
    Godot::print(godot::String("Authenticated succesfully. User ID: ") + session->getUserId().c_str());
    emit_signal("authenticated");
}

void NakamaGodot::connect_realtime_client() {
    if (!session) return;
    if (session->isExpired()) return;
    if (rtClient) return;

    int port = 7350;
    bool createStatus = true;
    rtClient = client->createRtClient(port);

    _NRtListener.setConnectCallback([]() {
                Godot::print("Realtime client connected");
            });
    rtClient->setListener(&_NRtListener);
    rtClient->connect(session, createStatus);
}

bool NakamaGodot::is_realtime_client_connected() {
    return (rtClient && rtClient->isConnected());
}

bool NakamaGodot::is_session_expired() {
    return !(session && !session->isExpired());
}

void NakamaGodot::join_chat_room(String roomName, int type, bool persist, bool hidden) {
    _NRtListener.setChannelMessageCallback([this](const NChannelMessage& message) {
                Godot::print("Recieved message on channel {0}", message.channelId.c_str());
                Godot::print("Message content: {0}", message.content.c_str());

                emit_signal("chat_message_recieved", message.channelId.c_str(), message.messageId.c_str(), message.code, message.senderId.c_str(), message.username.c_str(), message.content.c_str());
            });

    auto sucessJoinCallback = [this](NChannelPtr channel) {
        Godot::print("Joined chat: {0}", channel->id.c_str());
        emit_signal("chat_joinec", channel->id.c_str());
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
        emit_signal("nakama_error", Dictionary(), rtErrToDict(error));
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
        emit_signal("storage_write_complete", Dictionary()); 
    };
    auto err_callback = [this](const NError& error)
    {
        emit_signal("storage_write_complete", errToDict(error));
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
        emit_signal("storage_read_complete", Dictionary(), errToDict(error));
    };
    auto success_callback = [this](const NStorageObjects& objects) 
    {
        Dictionary d;
        for (auto& object : objects)
        {
            d[String(object.key.c_str())] = String(object.value.c_str());
        }
        emit_signal("storage_read_complete", d, Dictionary());
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
        emit_signal("storage_remove_complete", errToDict(error));
    };
    auto success_callback = [this]()
    { 
        emit_signal("storage_remove_complete", Dictionary()); 
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
    auto errCallback = [this](const NError& error)
    {
        emit_signal("rpc_failed", errToDict(error));
    };
    auto successCallback = [this](const NRpc& rpc)
    {
        emit_signal("rpc_complete", String(rpc.id.c_str()), String(rpc.payload.c_str()));
    };
    client->rpc(
        session, 
        rpcId.utf8().get_data(), 
        payload.to_json().utf8().get_data(), 
        successCallback, 
        errCallback
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

void NakamaGodot::emit_error_signal(String signal, const NError& error)
{
    emit_signal(signal, (int)error.code, String(error.message.c_str()));
}