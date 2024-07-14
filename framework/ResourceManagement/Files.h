#pragma once

#include <string>

namespace FW {
namespace Files {
    class Config {
    public:
        /**
         * Default constructor.
         * 
         * This constructor does nothing. The user is responsible for manually
         * parsing JSON configuration files accordingly.
         */
        Config() {}

        /**
         * Deconstructor.
         * 
         * Free memory whenever this object is deallocated or deleted.
         */
        virtual ~Config() {}

        /**
         * Initialise and set JSON configuration name.
         * 
         * Whenever this object is initialised, the JSON file's name is
         * initialised. This WILL NOT parse the file and must be done with
         * Config::parse().
         * 
         * @param name the configuration file name
         */
        Config(std::string name) {
            fileName = name;
        }

        /**
         * Read JSON configuration file.
         * 
         * After parsing the file, the JSON object is accessible through the
         * jObject variable.
         * 
         * If the file name is invalid or the file does not exist, then the
         * program will throw a traceback in DEBUG mode. This is disabled in
         * RELEASE mode, but both modes will return false.
         */
        bool parse();

        /**
         * Write the jObject to file.
         * 
         * This instance's jObject is written to the JSON configuration file
         * whose name is provided by Config::fileName.
         * 
         * If the operation fails, then the function returns false. In DEBUG
         * mode, a traceback is additionally called and the program halts.
         */
        bool write();

        /**
         * JSON object with the stored configurations.
         * 
         * To get data, access them with key-values.
         * Examples:
         * j["pi"] = 3.141;
         * 
         * More examples at:
         * https://github.com/nlohmann/json?tab=readme-ov-file#json-as-first-
         * class-data-type
         */
        json jObject;

        /// If true, then the output for Config::write() is prettified.
        bool prettified = true;

        std::string fileName;

    private:
    };
}
}