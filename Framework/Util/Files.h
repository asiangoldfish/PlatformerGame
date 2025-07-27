#pragma once

#include <string>
#include <expected>
#include <filesystem>

namespace FW {
    /**
     * Get the data directory based on the platform.
     * 
     * The following directories would be found on these platforms:
     * - Windows: %APPDATA%
     * - Linux: $XDG_DATA_HOME
     * 
     * If the file cannnot be accessed, then an error is valid and the reason
     * behind the error is returned.
     * 
     * For Linux, the order of priority is the following (from highest to
     * lowest):
     * - $XDG_DATA_HOME
     * - $HOME/.local/share
     * - POSIX fallback to using getpwuid
     */
    std::expected<std::filesystem::path, std::string> getDataDir();
}