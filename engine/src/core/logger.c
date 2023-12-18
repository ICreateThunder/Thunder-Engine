#include "core/logger.h"

#include "core/asserts.h"
#include "platform/platform.h"

// TODO: Temporary usage of standard libraries to write out
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

typedef struct logger_system_state {
    b8 initialised;
}logger_system_state;

static logger_system_state* state_ptr;

void report_assertion_failure(const char *expression, const char *message, const char *file, i32 line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: %s, in file: %s, line: %d\n", expression, message, file, line);
}

b8 initialise_logging(u64* memory_requirement, void* state) {
    *memory_requirement = sizeof(logger_system_state);
    if (state == 0) {
        return true;
    }

    state_ptr = state;
    state_ptr->initialised = true;

    KTRACE("Logger level test:\n\n");
    KFATAL("Something went wrong: %f", 3.14f);
    KERROR("Something went wrong: %f", 3.14f)
    KWARN("Something went wrong: %f", 3.14f);
    KINFO("Something went wrong: %f", 3.14f);
    KDEBUG("Something went wrong: %f", 3.14f);
    KTRACE("Something went wrong: %f\n\n", 3.14f);

    KINFO("Logging subsystem initialised");

    // TODO: Create logfile
    return true;
}

void shutdown_logging(void* state) {
    // TODO: Cleanup logging/write queued entries
    state_ptr = 0;
}

void log_output(log_level level, const char *message, ...) {
    const char *level_strings[6] = {"[!] FATAL :: ", "[!] ERROR :: ", "[-] WARN :: ", "[-] INFO :: ", "[-] DEBUG :: ", "[-] TRACE :: "};
    b8 is_error = level < LOG_LEVEL_WARN;

    const i32 msg_length = 32000;
    char out_message[msg_length];
    memset(out_message, 0, sizeof(out_message));

    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, msg_length, message, arg_ptr);
    va_end(arg_ptr);

    char out_message2[msg_length];
    sprintf(out_message2, "%s%s\n", level_strings[level], out_message);

    if (is_error) {
        platform_console_write_error(out_message2, level);
    } else {
        platform_console_write(out_message2, level);
    }
}