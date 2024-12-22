/**
 * Memory management toolkit
 *
 * @details The Framework provides its own version of smart pointers. These
 * features automatic memory management by reference counting.
 */

#pragma once

#include <memory>
#include <utility>

namespace FW {
    /**
     * Delete a raw pointer
     * @tparam T The pointer's typename
     * @param ptr Pointer to delete
     */
    template<typename T>
    void DELETE(T* ptr)
    {
        delete ptr;
        ptr = nullptr;
    }

    template<typename T>
    using ref = std::shared_ptr<T>;

    /**
     * Create a shared reference
     * @tparam T The typename of the referenced object.
     * @tparam Args Typename for the arguments to pass to the object's
     * constructor
     * @param args Arguments to redirect to the object's constructor.
     * @return A shared reference to the new object.
     */
    template<typename T, typename... Args>
    constexpr ref<T> createRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using scope = std::unique_ptr<T>;

    /**
     * Create a unique reference.
     * @tparam T The typename of the referenced object.
     * @tparam Args Typename for the arguments to pass to the object's
     * constructor
     * @param args Arguments to redirect to the object's constructor.
     * @return A unique reference to the new object.
     */
    template<typename T, typename... Args>
    constexpr scope<T> createScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
} // FW
