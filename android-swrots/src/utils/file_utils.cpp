#include "common.h"
#include <string>

static AppState* g_state = nullptr;

AppState* getAppState() {
    if (!g_state) {
        g_state = new AppState();
    }
    return g_state;
}

// File utilities
namespace FileUtils {
    std::string readFile(const std::string& path) {
        FILE* f = fopen(path.c_str(), "rb");
        if (!f) return "";
        
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        fseek(f, 0, SEEK_SET);
        
        std::string content(size, '\0');
        fread(&content[0], 1, size, f);
        fclose(f);
        
        return content;
    }
    
    bool writeFile(const std::string& path, const std::string& content) {
        FILE* f = fopen(path.c_str(), "wb");
        if (!f) return false;
        
        fwrite(content.c_str(), 1, content.size(), f);
        fclose(f);
        
        return true;
    }
    
    bool exists(const std::string& path) {
        FILE* f = fopen(path.c_str(), "r");
        if (f) {
            fclose(f);
            return true;
        }
        return false;
    }
}
