#pragma once

#include "zenoh.hxx"
#include "NatNetTypes.h"
#include "NatNetClient.h"
#include <string>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <wyrm/types.hpp> 

using DescriptionTable = std::unordered_map<int32_t, WyrmDescription>;

struct WyrmContext {
    zenoh::Session*                                             session;
    std::mutex                                                  buffer_mutex;
    std::mutex                                                  descriptions_mutex;
    std::condition_variable                                     buffer_cv;
    std::unordered_map<int32_t, WyrmDescription>                descriptions;
    std::queue<WyrmFrame>                                       frame_buffer;
};

struct WyrmConfig {
    uint16_t    server_command_port;
    uint16_t    server_data_port;
    std::string server_address;
    std::string local_address;
    std::string multicast_address;
    ConnectionType connection_type;

    sNatNetClientConnectParams ToNatNet() const {
        sNatNetClientConnectParams p;
        p.serverCommandPort = server_command_port;
        p.serverDataPort    = server_data_port;
        p.serverAddress     = server_address.c_str();
        p.localAddress      = local_address.c_str();
        p.multicastAddress  = multicast_address.c_str();
        p.connectionType    = connection_type;
        return p;
    }
};

void BuildDescriptionTable(sDataDescriptions* desc, WyrmContext& ctx);