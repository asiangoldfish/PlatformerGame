#include "Log.h"

#include <iostream>

namespace Framework {
    void Log::log(const std::string &msg) {
        std::cout << msg << std::endl;
    }
}