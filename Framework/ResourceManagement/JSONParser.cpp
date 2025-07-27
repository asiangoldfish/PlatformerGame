#include "pch.h"
#include "JSONParser.h"
#include <cstring>
#include "Log.h"

using json = nlohmann::json;

namespace FW {
    JSONParser::JSONParser() {
        // TODO add version to config to patch it with new keys and values.
        // TODO scan for outdated or illegal values.

        // We should create new config home directory if one does not exist,
        // otherwise the location we save to does not exist.
        std::string configHome = getUserDataDirectory();
        std::filesystem::create_directories(configHome);

        // We also have to copy the configuration to this directory if it does
        // not exist, so the user can save their own configurations and persist
        // the data.
        userConfigPath = configHome + "/editor.json";

        // We should convert / to \\ if we are on Windows, because Windows file
        // paths require \\.
        #if _WIN32
        std::replace(userConfigPath.begin(), userConfigPath.end(), '/', '\\'); 
        #endif


        std::ifstream file(userConfigPath);
        if (!file) {
            INFO("Copying configuration files");
            // https://stackoverflow.com/a/10195497
            std::ifstream  src(std::string(CONFIG_DIR) + "editor.json", std::ios::binary);
            std::ofstream  dst(userConfigPath,   std::ios::binary);

            dst << src.rdbuf();
            file.open(userConfigPath);

            if (!file) {
                FATAL("Failed to parse file: {}", userConfigPath);
            }
        }
        jObject = json::parse(file);
        isParsed = true;
        file.close();
    }

    bool JSONParser::parse() {
        std::ifstream infile;

        infile.open(userConfigPath);

        if (infile) {
            jObject = json::parse(infile);
            infile.close();

            isParsed = true;
            return true;
        } else {
            return false;
        }
    }

    bool JSONParser::write(bool prettified) {
        if (!isParsed) {
            ERROR("You must parse the file '{0}' before writing to it.",
                  userConfigPath);
        }

        std::ofstream outfile(userConfigPath);

        if (!outfile) {
            ERROR("Failed to create outfile '{0}'", userConfigPath);
            return false;
        }

        if (prettified) {
            outfile << std::setw(4);
        }
        outfile << jObject << std::endl;
        outfile.close();
        return true;
    }
    std::string JSONParser::getUserDataDirectory() {
        std::string userDataPath;

        std::string appName = "PhysicsDemo";

        #ifdef _WIN32
            // TODO error handling when APPDATA doesnt exist
            char* appdata = std::getenv("APPDATA");
            if (appdata) {
                userDataPath = std::string(appdata) + "\\" + appName;
            }
        #elif __linux__
            // TODO needs testing
            // TODO needs error handling
            char* configHome = std::getenv("XDG_DATA_HOME");
            char* userHome = std::getenv("HOME");
            char overrideXdgHome[14] = "/.local/share";
            if (!configHome) {
                configHome = strcat(userHome, overrideXdgHome);
            }
            
            userDataPath = std::string(configHome) + "/" + appName;
        #elif __APPLE__
            FATAL("We do not support Apple devices");
        #endif

        return userDataPath;
    }
} // namespace FW
