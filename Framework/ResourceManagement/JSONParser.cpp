#include "JSONParser.h"
#include "Log.h"
#include "Files.h"
#include "assert.h"

#include <cstring>

using json = nlohmann::json;

namespace FW {
    JSONParser::JSONParser() {
        // TODO add version to config to patch it with new keys and values.
        // TODO scan for outdated or illegal values.

        // We should create new config home directory if one does not exist,
        // otherwise the location we save to does not exist.
        std::filesystem::path configHome = getDataDir().value();
        std::filesystem::create_directories(configHome);

        // We also have to copy the configuration to this directory if it does
        // not exist, so the user can save their own configurations and persist
        // the data.
        userConfigPath = configHome / "editor.json";

        std::ifstream file(userConfigPath);

        if (!std::filesystem::exists(userConfigPath)) {
            INFO("Copying configuration files");
            // https://stackoverflow.com/a/10195497
            std::ifstream src(std::string(CONFIG_DIR) + "editor.json",
                              std::ios::binary);
            std::ofstream dst(userConfigPath, std::ios::binary);

            dst << src.rdbuf();
            file.open(userConfigPath);

            if (!file) {
                FATAL("Failed to parse file: {}", userConfigPath.string());
            }

            src.close();
            dst.close();
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
                  userConfigPath.string());
        }

        std::ofstream outfile(userConfigPath);

        if (!outfile) {
            ERROR("Failed to create outfile '{0}'", userConfigPath.string());
            return false;
        }

        if (prettified) {
            outfile << std::setw(4);
        }
        outfile << jObject << std::endl;
        outfile.close();
        return true;
    }
} // namespace FW
