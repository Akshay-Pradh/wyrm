#include <iostream>
#include <sstream>
#include <fmt/format.h>
#include <fmt/color.h>
#include "NatNetTypes.h"
#include "NatNetClient.h"
#include "include/types.hpp"
#include "include/io.hpp"

void connectToServer(NatNetClient& client, const WyrmConfig wyrmCfg) {
    // Connect to Server
    switch (client.Connect(wyrmCfg.toNatNet())) {
        case ErrorCode_OK:                  break;
        case ErrorCode_Network:             throw std::runtime_error("Unable to Connect to Server.");
        case ErrorCode_InvalidArgument:     throw std::runtime_error("Invalid connection params.");
        case ErrorCode_InvalidOperation:    throw std::runtime_error("Invalid operation.");
        default:                            throw std::runtime_error("Unknown NatNet error.");
    }

    // Test Server Connection
    sServerDescription serverDesc;
    client.GetServerDescription(&serverDesc);

    if (!serverDesc.bConnectionInfoValid) {
        throw std::runtime_error("Invalid connection params. Server predates NatNet 3.0");
    }

    if (!serverDesc.HostPresent) {
        throw std::runtime_error("Unable to Connect to Server. Host not present.");
    }

    // Check that the connection types match
    if (serverDesc.ConnectionMulticast && 
        wyrmCfg.connectionType != ConnectionType_Multicast) {
            throw std::runtime_error("Invalid connection params. Connection Types do not match.");
    }

    if (!serverDesc.ConnectionMulticast && 
        wyrmCfg.connectionType != ConnectionType_Unicast) {
            throw std::runtime_error("Invalid connection params. Connection Types do not match.");
    }

    // Convert serverDesc IP to a c++ string
    std::ostringstream ipOss;
    ipOss << static_cast<int>(serverDesc.HostComputerAddress[0]) << '.'
        << static_cast<int>(serverDesc.HostComputerAddress[1]) << '.'
        << static_cast<int>(serverDesc.HostComputerAddress[2]) << '.'
        << static_cast<int>(serverDesc.HostComputerAddress[3]);

    std::string ipString = ipOss.str();

    if (ipString != wyrmCfg.serverAddress) {
        throw std::runtime_error("Unable to Connect to Server. Host IP Addresses do not Match.");
    }

    // Convert serverDesc Multicast Id to a c++ string
    std::ostringstream mOss;
    mOss << static_cast<int>(serverDesc.ConnectionMulticastAddress[0]) << '.'
        << static_cast<int>(serverDesc.ConnectionMulticastAddress[1]) << '.'
        << static_cast<int>(serverDesc.ConnectionMulticastAddress[2]) << '.'
        << static_cast<int>(serverDesc.ConnectionMulticastAddress[3]);

    std::string multicastString = mOss.str();

    if (multicastString != wyrmCfg.multicastAddress) {
        throw std::runtime_error("Unable to Connect to Server. Multicast Addresses do not match.");
    }
    
    if (serverDesc.ConnectionDataPort != wyrmCfg.serverDataPort) {
        throw std::runtime_error("Invalid connection params. Data ports do not match.");
    }
}

int main(int argc, char *argv[]) {
    try {

        IOHandler io(argc, argv);

        // Read config file
        WyrmConfig wyrmCfg = parseMotiveConfig();

        // Create a client
        NatNetClient client;
        connectToServer(client, wyrmCfg);

        io.logConfig(wyrmCfg);
        io.logMessage(fmt::format("[Success] Connected to Server at {}!", wyrmCfg.serverAddress));

        // Get Data Descriptions from the server
        

        // Connect callbacks
        // TODO

        // Setup DDS Session
        // TODO

        // Loop to receive data from server
        //  Read data from server
        //  Package data for Zenoh
        //  Publish data to Zenoh
        //  Gracefull cleanup on CRTL-C

        // Disconnect from the server
        client.Disconnect();
        io.logMessage("[Success] Disconnected From Server!");

    } catch (const std::runtime_error& e) {
        fmt::print(stderr, bg(fmt::color::coral), "[Runtime error] {}\n", e.what());
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        fmt::print(stderr, bg(fmt::color::crimson), "[Exception] {}\n", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
