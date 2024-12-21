#include "assertions.h"
#include "Log.h"

void framework_assert(std::string msg) {
    if (!msg.empty()) {
        WARN(msg);
    }

    __breakpoint();
}


void framework_warn(std::string msg) {
    if (!msg.empty()) {
        std::cout << msg << std::endl;
    }
}