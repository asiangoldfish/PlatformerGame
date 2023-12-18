/**
 * These assertions are made to cause a breakpoint during debug. It lets us
 * monitor data and find the roots of bugs.
 *
 * The functions in this file should be stripped out before release.
 */

#ifndef __ASSERTIONS_H__
#define __ASSERTIONS_H__

#include <iostream>

// Debug trace/breakpoint trap
#define DEBUG_DISABLED
#ifdef DEBUG_DISABLED
    #define __breakpoint()
#elif defined(_WIN32) || defined(_WIN64)
#define __breakpoint() __debugbreak()
#elif __linux__
#include <csignal>
#define __breakpoint() raise(SIGTRAP)
#endif

/**
 * Raise a breakpoint whenever called.
 * @param msg Debug message to print on console.
 */
void framework_assert(std::string msg = "");

/**
 * Print debug message on screen, but do not raise a breakpoint.
 * @param msg Debug message to print on console.
 */
void framework_warn(std::string msg);

#endif // __ASSERTIONS_H__