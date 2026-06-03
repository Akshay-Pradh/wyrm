#pragma once

#include <string>
#include "NatNetTypes.h"
#include "NatNetClient.h"
#include "include/types.hpp"

class IOHandler {
 private:
    bool        printing = false;
    bool        logging  = false;
    std::string log_path;

 public:
    IOHandler(int argc, char* argv[]);
    bool IsPrinting() const;
    void LogMessage(const std::string& msg, const std::string& level = "INFO") const;
    void LogConfig(const WyrmConfig& cfg) const;

 private:
    std::string GenerateLogPath(const std::string& log_dir) const;
};

WyrmConfig ParseMotiveConfig(const std::string& path = "./config/config.toml");