#include <chrono>
#include <fstream>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <argparse/argparse.hpp>
#include <toml++/toml.hpp>
#include "io.hpp"

IOHandler::IOHandler(int argc, char *argv[]) {
    argparse::ArgumentParser program("wyrm");
    program.add_argument("-p", "--printing")
        .help("Print information to the console.")
        .flag();
    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& e) {
        fmt::println(stderr, "{}", e.what());
        std::cerr << program;
        std::exit(EXIT_FAILURE);
    }
    printing = program.get<bool>("--printing");
    if (!printing) {
        logPath = generateLogPath("./logs");
    }
}

bool IOHandler::isPrinting() const {
    return printing;
}

void IOHandler::logMessage(const std::string& msg, const std::string& level) const {
    if (isPrinting()) {
        fmt::println("{}", msg);
    } else {
        std::ofstream file(logPath, std::ios::app);
        file << fmt::format("[{}] {}\n", level, msg);
    }
}

void IOHandler::logConfig(const WyrmConfig wyrmCfg) const {
    std::string cfgString = fmt::format("[-----Wyrm Configuration-----]\nConnection Type: {}\nServer Address: {}\nLocal Address: {}\nMulticast Address: {}\nData Port: {}\nCommand Port: {}\n",
                                            wyrmCfg.connectionType == ConnectionType_Multicast ? "Multicast" : "Unicast",
                                            wyrmCfg.serverAddress,
                                            wyrmCfg.localAddress,
                                            wyrmCfg.multicastAddress,
                                            wyrmCfg.serverDataPort,
                                            wyrmCfg.serverCommandPort);
    if (isPrinting()) {
        fmt::print(cfgString);
    } else {
        std::ofstream file(logPath, std::ios::app);
        file << fmt::format(cfgString);
    }
}

std::string IOHandler::generateLogPath(const std::string& logDir) const {
    auto now = std::chrono::system_clock::now();
    return fmt::format("{}/wyrm_{:%Y-%m-%d_%H-%M-%S}.log", logDir, now);
}

WyrmConfig parseMotiveConfig(const std::string& path) {
    toml::table tbl = toml::parse_file(path);

    WyrmConfig cfg;
    cfg.serverCommandPort = tbl["networking"]["command_port"].value<uint16_t>().value_or(0);
    cfg.serverDataPort    = tbl["networking"]["data_port"].value<uint16_t>().value_or(0);
    cfg.serverAddress     = tbl["networking"]["server_address"].value<std::string>().value_or("");
    cfg.localAddress      = tbl["networking"]["client_address"].value<std::string>().value_or("");
    cfg.multicastAddress  = tbl["networking"]["multicast_address"].value<std::string>().value_or("");
    cfg.connectionType    = tbl["networking"]["use_multicast"].value<bool>().value_or(false)
                            ? ConnectionType_Multicast
                            : ConnectionType_Unicast;
    return cfg;
}