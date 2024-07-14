#include "pch.h"
#include "Files.h"

using json = nlohmann::json;

namespace FW {
namespace Files {
    
    bool Config::parse()
    {
        if (fileName == "") {
            FATAL("Attempted to parse JSON configuration file, but it was empty.");
            return false;
        }

        if (!std::filesystem::exists(fileName)) {
            FATAL("Attempted to read JSON configuration file \'%s\', but it was not found.", fileName);
        }

        std::ifstream infile(fileName);
        jObject = json::parse(infile);
        infile.close();
        return true;
    }

    bool Config::write()
    {
        if (fileName == "") {
            FATAL("Attempted to parse JSON configuration file, but it was empty.");
            return false;
        }

        std::ofstream outfile(fileName);
        if (prettified) {
            outfile << std::setw(4);
        }
        outfile << jObject << std::endl;
        outfile.close();
        return true;
    }
}
}
