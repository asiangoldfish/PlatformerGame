/**
 * Messaging and debugging print outs to console.
 *
 * @details The macros in this file provide the same formatting options as the underlying logging system does. Visit
 * <a href="https://github.com/gabime/spdlog">spdlog</a> for details.
 *
 * @example
 * @code
 * std::string hello = "Hello";
 * INFO("{}, this is an example", hello);
 *
 * int someNumber = 3;
 * ERROR("Error {}. A bad error", someNumber);
 * @endcode
 *
 * @details Consider adding an error number system for <u>ERROR</u> and <u>FATAL</u> to precisely identify the cause of
 * the corresponding errors or crashes.
 */

#pragma once

#include "spdlog/spdlog.h"
#include "assertions.h"

/**
 * Trace code flow.
 *
 * @details Use this to learn the code flow of a particular part of the code base. It should be removed after use to
 * avoid polluting the console.
 */
#define TRACE(...) spdlog::trace(__VA_ARGS__)

/**
 * Print some useful information.
 *
 * @details Use this to inform the user about something. It may help debugging a runtime error, but for the most part
 * is used to inform the user about a process that has started or finished.
 */
#define INFO(...) spdlog::info(__VA_ARGS__)

/**
 * Print information for debugging.
 *
 * @details As the code for printing debugging log is stripped out of the release build, this macro serves as a log for
 * developers only.
 */
#define DEBUG(...) spdlog::debug(__VA_ARGS__)

/**
 * Print warning message.
 *
 * @details Warnings may be worth checking out, but is not necessarily critical to the operation of the application.
 * It should be clear where the warning is produced, so it may bring value to tracing where an error might have occurred.
 * While warnings may not always be optimal, it may also log when an action was unsuccessful, like a HTTP request.
 * Despite this, as a general rule of thumb, try to limit the use of warnings.
 */
#define WARN(...) spdlog::warn(__VA_ARGS__)

/**
 * Critical errors for developers and administrators.
 *
 * @details ERROR is reserved to log critical errors that should never occur. It may be a side effect of a function that
 * should never happen. Developers should look into this and fix the error quickly, as it is disruptive for the
 * operations of the application.
 *
 * @details While FATAL will cause a runtime error, ERROR will not. This is by design, so users can report what
 * happens after the ERROR occurs. This may help developers debugging and fixing the problem.
 */
#define ERROR(...) spdlog::error(__VA_ARGS__)

/**
 * Fatal errors and runtime error.
 *
 * @details Use only when commands or functions fails and the application can no longer run because of this. Use cases
 * can be to immediately crash the program to prevent data corruption or security vulnerabilities. These FATAL errors
 * must be fixed quickly as possible.
 */
#define FATAL(...) spdlog::critical(__VA_ARGS__); __breakpoint()

#define INFO_GLM_VEC2(value) INFO("X: {} | Y: {}", value.x, value.y)
#define INFO_GLM_VEC3(value) INFO("X: {} | Y: {} | Z: {}", value.x, value.y, value.z);
#define INFO_GLM_VEC4(value) INFO("X: {} | Y: {} | Z: {} | W: {}", value.x, value.y, value.z, value.w);
