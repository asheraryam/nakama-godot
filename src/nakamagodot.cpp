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

    // Signals
    register_signal<NakamaGodot>("authenticated");
    register_signal<NakamaGodot>("authentication_failed", "error", GODOT_VARIANT_TYPE_DICTIONARY);

    register_signal<NakamaGodot>("nakama_error", "error", GODOT_VARIANT_TYPE_DICTIONARY);

    register_signal<NakamaGodot>("chat_joined", "channel_id", GODOT_VARIANT_TYPE_STRING);
    register_signal<NakamaGodot>("chat_join_failed", "error", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("storage_read_complete", "objects", GODOT_VARIANT_TYPE_DICTIONARY, "error", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("storage_write_complete", "error", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<NakamaGodot>("storage_remove_complete", "error", GODOT_VARIANT_TYPE_DICTIONARY);

    register_signal<NakamaGodot>("chat_message_recieved", "channel_id", GODOT_VARIANT_TYPE_STRING, "message_id", GODOT_VARIANT_TYPE_STRING, "message_code", GODOT_VARIANT_TYPE_INT, "sender_id", GODOT_VARIANT_TYPE_STRING, "username", GODOT_VARIANT_TYPE_STRING, "content", GODOT_VARIANT_TYPE_STRING);
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
