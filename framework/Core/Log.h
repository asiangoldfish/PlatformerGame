#ifndef LOG_H
#define LOG_H

#include <string>

namespace Framework {
    /**
     * Logger class for logging messages to file or console
     */
    class Log
    {
    public:
        /**
         * Log a message to console
         * @param msg Message to log to console
         */
        static void log(const std::string& msg);
    };

}

#endif // LOG_H