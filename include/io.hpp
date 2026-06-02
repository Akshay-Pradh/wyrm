#pragma once
#include <string>
#include "NatNetTypes.h"
#include "NatNetClient.h"
#include "include/types.hpp"

class IOHandler {
private:
    bool printing = false;
    bool logging = false;
    std::string logPath;

public:
    IOHandler(int argc, char *argv[]);
    bool isPrinting() const;
    void logMessage(const std::string& msg, const std::string& level = "INFO") const;
    void logConfig(const WyrmConfig cfg) const;

private:
    std::string generateLogPath(const std::string& logDir) const;
};

WyrmConfig parseMotiveConfig(const std::string& path = "./config/config.toml");