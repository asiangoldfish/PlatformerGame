#include "Files.h"

#include <cstring>
#include <filesystem>

#if __linux__
#include <unistd.h>
#include <pwd.h>
#endif // __linux__

namespace FW {

    std::expected<std::filesystem::path, std::string> getDataDir() {
        std::string userDataPath;

        std::string appName = "PhysicsDemo";

#ifdef _WIN32
        // TODO error handling when APPDATA doesnt exist
        char* appdata = std::getenv("APPDATA");
        if (appdata) {
            return std::filesystem::path(appdata);
        } else {
            return std::unexpected("Could not determine the user data "
                                   "directory: APPDATA is undefined");
        }
#elif __linux__
        if (const char* xdgData = std::getenv("XDG_DATA_HOME")) {
            return std::filesystem::path(xdgData) / appName;
        } else if (const char* home = std::getenv("HOME")) {
            std::filesystem::path userHome =
              std::filesystem::path(home) / ".local/share" / appName;
            return userHome;
        } else if (struct passwd* pw = getpwuid(getuid())) {
            std::filesystem::path userHome =
              std::filesystem::path(pw->pw_dir) / ".local/share" / appName;
            return userHome;
        } else {
            return std::unexpected(
              "Could not determine the user data directory: XDG_DATA_HOME, "
              "HOME, and getpwuid() failed");
        }
#elif __APPLE__
        FATAL("We do not support Apple devices");
#endif
    }

}

// -------------------------
// Doctest Unit Tests
// -------------------------