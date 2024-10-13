#pragma once

#include <nlohmann/json.hpp>
#include "pch.h"

namespace FW {
    /*
    class Singleton
    {
    public:
        Singleton(Singleton const&) = delete;
        Singleton& operator=(Singleton const&) = delete;

        static std::shared_ptr<Singleton> instance()
        {
            static std::shared_ptr<Singleton> s{new Singleton};
            return s;
        }

    private:
        Singleton() {}
    };
    */

    /**
     * JSONParser is a wrapper class and provides convinient API to parse and
     * write to JSON files.
     *
     * @tutorial:
     * // Initialise parser
     * FW::JSONParser* jsonParser = new FW::JSONParser(filename);
     *
     * // Read a variable
     * int width = jsonParser->get()["width"];
     * 
     * // Set new width to 2.0f;
     * jsonParser->get()["width"] = 2.0f;
     * 
     * // Write to the file
     * bool success = jsonParser->write();
     */
    class JSONParser {
    public:
        /**
         * Default constructor.
         *
         * This constructor does nothing. The user is responsible for manually
         * parsing JSON configuration files accordingly.
         */
        JSONParser() = default;

        /**
         * Deconstructor.
         *
         * Free memory whenever this object is deallocated or deleted.
         */
        virtual ~JSONParser() = default;

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
        bool parse(std::string filepath);

        /**
         * Write the jObject to file.
         *
         * This instance's jObject is written to the JSON configuration file
         * whose name is provided by Config::fileName.
         *
         * If the operation fails, then the function returns false. In DEBUG
         * mode, a traceback is additionally called and the program halts.
         * 
         * @param prettified if true, then write to file with prettified format.
         */
        bool write(bool prettified);

        nlohmann::json get() { return jObject; }

    public:
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
        nlohmann::json jObject;

    private:
        bool isParsed = false;
        std::string fileName;
    };
}