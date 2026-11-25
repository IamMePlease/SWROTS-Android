#pragma once
#include <string>

namespace LogUtils {
    void logInfo(const std::string& message);
    void logError(const std::string& message);
    void logDebug(const std::string& message);
    
    void setLogLevel(int level);
    int getLogLevel();
}
