#pragma once

#include <memory>
#include <utility>

namespace Framework {
    // Macro for shared_ptr and unique_ptr
    template<typename T>
    using ref = std::shared_ptr<T>;
    template<typename T, typename... Args>
    constexpr ref<T> createRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using scope = std::unique_ptr<T>;
    template<typename T, typename... Args>
    constexpr scope<T> createScope(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
