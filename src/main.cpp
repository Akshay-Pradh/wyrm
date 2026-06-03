#include <iostream>
#include <sstream>
#include <fmt/format.h>
#include <fmt/color.h>
#include "NatNetTypes.h"
#include "NatNetClient.h"
#include "zenoh.hxx"
#include "include/types.hpp"
#include "include/io.hpp"
#include "include/constants.hpp"
#include "include/serialization.hpp"

void ConnectToServer(NatNetClient& client, const WyrmConfig& cfg) {
    switch (client.Connect(cfg.ToNatNet())) {
        case ErrorCode_OK:               break;
        case ErrorCode_Network:          throw std::runtime_error("Unable to Connect to Server.");
        case ErrorCode_InvalidArgument:  throw std::runtime_error("Invalid connection params.");
        case ErrorCode_InvalidOperation: throw std::runtime_error("Invalid operation.");
        default:                         throw std::runtime_error("Unknown NatNet error.");
    }

    sServerDescription server_desc;
    client.GetServerDescription(&server_desc);

    if (!server_desc.bConnectionInfoValid)
        throw std::runtime_error("Invalid connection params. Server predates NatNet 3.0");
    if (!server_desc.HostPresent)
        throw std::runtime_error("Unable to Connect to Server. Host not present.");

    if (server_desc.ConnectionMulticast &&
        cfg.connection_type != ConnectionType_Multicast)
            throw std::runtime_error("Invalid connection params. Connection Types do not match.");

    if (!server_desc.ConnectionMulticast &&
        cfg.connection_type != ConnectionType_Unicast)
            throw std::runtime_error("Invalid connection params. Connection Types do not match.");

    std::ostringstream ip_oss;
    ip_oss << static_cast<int>(server_desc.HostComputerAddress[0]) << '.'
           << static_cast<int>(server_desc.HostComputerAddress[1]) << '.'
           << static_cast<int>(server_desc.HostComputerAddress[2]) << '.'
           << static_cast<int>(server_desc.HostComputerAddress[3]);

    if (ip_oss.str() != cfg.server_address)
        throw std::runtime_error("Unable to Connect to Server. Host IP Addresses do not Match.");

    std::ostringstream mc_oss;
    mc_oss << static_cast<int>(server_desc.ConnectionMulticastAddress[0]) << '.'
           << static_cast<int>(server_desc.ConnectionMulticastAddress[1]) << '.'
           << static_cast<int>(server_desc.ConnectionMulticastAddress[2]) << '.'
           << static_cast<int>(server_desc.ConnectionMulticastAddress[3]);

    if (mc_oss.str() != cfg.multicast_address)
        throw std::runtime_error("Unable to Connect to Server. Multicast Addresses do not match.");

    if (server_desc.ConnectionDataPort != cfg.server_data_port)
        throw std::runtime_error("Invalid connection params. Data ports do not match.");
}

int main(int argc, char* argv[]) {
    try {
        IOHandler io(argc, argv);

        // Setup and connect to NatNet server
        WyrmConfig wyrm_cfg = ParseMotiveConfig();
        NatNetClient client = {};
        ConnectToServer(client, wyrm_cfg);

        io.LogMessage(fmt::format("[Success] Connected to Server at {}!", wyrm_cfg.server_address));
        io.LogConfig(wyrm_cfg);

        // Get Data Descriptions from server and build Rigid Body name table
        sDataDescriptions* descriptions = nullptr;
        client.GetDataDescriptionList(&descriptions);
        NameTable name_table = BuildNameTable(descriptions);

        // Setup Zenoh Session
        zenoh::init_log_from_env_or("error");
        zenoh::Config zenoh_cfg = zenoh::Config::create_default();
        auto session = zenoh::Session::open(std::move(zenoh_cfg));

        // Create a queryable for data descriptions
        auto queryable = session.declare_queryable(WyrmDescKeyexpr,
            [&ctx](const zenoh::Query& query) {
                auto payload = serialize_descriptions(ctx.descriptions);
                query.reply(
                    WyrmDescKeyexpr,
                    zenoh::Bytes::copy(payload.data(), payload.size())
                );
            }
        );

        // Create a publisher for Frames
        auto framePub = session.declare_publisher(KeyExpr(WyrmFrameKeyexpr));

        while (true) {
            // 1. Mocap callback converts from natnet->wyrm, then puts data into buffer
            // 2. Pull from buffer, serialize, publish
        }

        client.Disconnect();
        io.LogMessage("[Success] Disconnected From Server!");

    } catch (const ZException& e) {
        fmt::print(stderr, bg(fmt::color::crimson), "[Zenoh Error] {}\n", e.what());
        return EXIT_FAILURE;
    } catch (const std::runtime_error& e) {
        fmt::print(stderr, bg(fmt::color::coral), "[Runtime error] {}\n", e.what());
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        fmt::print(stderr, bg(fmt::color::crimson), "[Exception] {}\n", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}