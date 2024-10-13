#include "pch.h"
#include "JSONParser.h"

using json = nlohmann::json;

namespace FW {
    bool JSONParser::parse(std::string filepath) {
        fileName = filepath;

        if (isParsed) {
            ERROR("The file '{0}' is already parsed.", fileName);
            return false;
        }

        if (fileName == "") {
            ERROR("Attempted to parse JSON configuration file, but it was "
                 "empty.");
            return false;
        }

        if (!std::filesystem::exists(fileName)) {
            ERROR("Attempted to read JSON configuration file \'%s\', but "
                 "it was not found.",
                 fileName);
            return false;
        }


        std::ifstream infile(fileName);
        jObject = json::parse(infile);
        infile.close();

        isParsed = true;

        return true;
    }

    bool JSONParser::write(bool prettified) {
        if (!isParsed) {
            ERROR("You must parse the file '{0}' before writing to it.",
                  fileName);
        }

        std::ofstream outfile(fileName);

        if (!outfile) {
            ERROR("Failed to create outfile '{0}'", fileName);
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
