#pragma once

#include "defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if KRELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

/**
 * @brief
 *
 * States for logger messages
 */
typedef enum log_level {
    LOG_LEVEL_FATAL = 0, /**< Critical errors that cause application crash */
    LOG_LEVEL_ERROR = 1, /**< Errors that may keep application running, however expect unexpected behaviour */
    LOG_LEVEL_WARN = 2,  /**< Warnings refer to performance degradation, program should still run as expected */
    LOG_LEVEL_INFO = 3,  /**< Basic level of logging */
    LOG_LEVEL_DEBUG = 4, /**< Debug information */
    LOG_LEVEL_TRACE = 5  /**< Trace is verbose debugging */
} log_level;

/**
 * @brief Initialises logging subsystem. Call twice, once with state = 0 for required memory,
 * then a second time passing allocated memory to state
 * 
 * @param memory_requirement A pointer to hold the required memory size of internal state
 * @param state 0 for memory requirement request, otherwise pointer to allocated block of memory
 * @return b8 true on success; otherwise false
 */
b8 initialise_logging(u64* memory_requirement, void* state);
void shutdown_logging(void* state);

KAPI void log_output(log_level level, const char* message, ...);

#define KFATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

#ifndef KERROR
#define KERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
#define KWARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
#define KWARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define KINFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define KINFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define KDEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define KDEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define KTRACE(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define KTRACE(message, ...)
#endif