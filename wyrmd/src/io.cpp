/**
 * Copyright [2026] [Nicholas Sutton]
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <chrono>
#include <fstream>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <argparse/argparse.hpp>
#include <toml++/toml.hpp>
#include "io.hpp"

/**
* Constructor for the IOHandler Class
* @param argc number of CLI args
* @param argv an array of CLI arguments
*/
IOHandler::IOHandler(int argc, char* argv[]) {
    argparse::ArgumentParser program("wyrm");
    program.add_argument("-p", "--printing")
        .help("Print information to the console.")
        .flag();
    program.add_argument("-c", "--config")
        .default_value(std::string("./wyrmd/config/config.toml"))
        .help("Path to a config.toml file.");
    program.add_argument("-l", "--logs")
        .default_value(std::string("./wyrmd/logs"))
        .help("Path to a logs directory.");
    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& e) {
        fmt::println(stderr, "{}", e.what());
        std::cerr << program;
        std::exit(EXIT_FAILURE);
    }

    config_path = program.get<std::string>("--config");
    log_dir = program.get<std::string>("--logs");

    printing = program.get<bool>("--printing");
    if (!printing) {
        log_path = GenerateLogPath(log_dir);
    }
}

/**
* Returns true if Wyrm should print logs to the terminal
* @return true if the printing flag is set
*/
bool IOHandler::IsPrinting() const {
    return printing;
}

/**
* Logs a message and its information level.
* @param msg message to log
* @param level the type of message being logged
*/
void IOHandler::LogMessage(const std::string& msg, const std::string_view level) const {
    if (IsPrinting()) {
        fmt::println("[{}] {}", level, msg);
    } else {
        std::ofstream file(log_path, std::ios::app);
        file << fmt::format("[{}] {}\n", level, msg);
    }
}

/**
* Logs the provided Wyrm configuration.
* @param cfg configuration to log
*/
void IOHandler::LogConfig(const WyrmConfig& cfg) const {
    std::string cfg_string = fmt::format(
        "[-----Wyrm Configuration-----]\n"
        "Connection Type: {}\nServer Address: {}\nLocal Address: {}\n"
        "Multicast Address: {}\nData Port: {}\nCommand Port: {}\n",
        cfg.connection_type == ConnectionType_Multicast ? "Multicast" : "Unicast",
        cfg.server_address,
        cfg.local_address,
        cfg.multicast_address,
        cfg.server_data_port,
        cfg.server_command_port);

    if (IsPrinting()) {
        fmt::print(cfg_string);
    } else {
        std::ofstream file(log_path, std::ios::app);
        file << cfg_string;
    }
}

/**
* Creates a Log file in the specified log directory
* @param log_dir path to the log directory
* @return the path to the log file
*/
std::string IOHandler::GenerateLogPath(const std::string& log_dir) const {
    auto now = std::chrono::system_clock::now();
    return fmt::format("{}/wyrm_{:%Y-%m-%d_%H-%M-%S}.log", log_dir, now);
}

/**
* Parses the wyrm configuration file and saves its contents to a WyrmConfig struct.
* @return a WyrmConfig struct that stores the config settings
*/
WyrmConfig IOHandler::ParseMotiveConfig() {
    toml::table tbl = toml::parse_file(config_path);
    WyrmConfig cfg;
    cfg.server_command_port = tbl["networking"]["command_port"].value<uint16_t>().value_or(0);
    cfg.server_data_port    = tbl["networking"]["data_port"].value<uint16_t>().value_or(0);
    cfg.server_address      = tbl["networking"]["server_address"].value<std::string>().value_or("");
    cfg.local_address       = tbl["networking"]["client_address"].value<std::string>().value_or("");
    cfg.multicast_address   = tbl["networking"]["multicast_address"].value<std::string>().value_or("");
    cfg.connection_type     = tbl["networking"]["use_multicast"].value<bool>().value_or(false)
        ? ConnectionType_Multicast
        : ConnectionType_Unicast;
    return cfg;
}