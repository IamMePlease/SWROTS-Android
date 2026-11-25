#include "log_utils.h"
#include "../common.h"

namespace LogUtils {
    static int g_log_level = ANDROID_LOG_INFO;
    
    void logInfo(const std::string& message) {
        if (g_log_level <= ANDROID_LOG_INFO) {
            LOGI("%s", message.c_str());
        }
    }
    
    void logError(const std::string& message) {
        LOGE("%s", message.c_str());
    }
    
    void logDebug(const std::string& message) {
        if (g_log_level <= ANDROID_LOG_DEBUG) {
            LOGD("%s", message.c_str());
        }
    }
    
    void setLogLevel(int level) {
        g_log_level = level;
    }
    
    int getLogLevel() {
        return g_log_level;
    }
}
