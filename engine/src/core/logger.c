#include "core/logger.h"

#include "core/asserts.h"
#include "platform/platform.h"
#include "platform/filesystem.h"
#include "core/kstring.h"
#include "core/kmemory.h"

// TODO: Temporary usage of standard libraries to write out
#include <stdarg.h>

typedef struct logger_system_state {
    file_handle log_file_handle;
}logger_system_state;

static logger_system_state* state_ptr;

void append_to_log_file(const char* message) {
    if (state_ptr && state_ptr->log_file_handle.is_valid) {
        u64 length = string_length(message);
        u64 written = 0;
        if (!filesystem_write(&state_ptr->log_file_handle, length, message, &written)) {
            platform_console_write_error("ERROR writing to console.log", LOG_LEVEL_ERROR);
        }
    }
}

void report_assertion_failure(const char *expression, const char *message, const char *file, i32 line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: %s, in file: %s, line: %d\n", expression, message, file, line);
}

b8 initialise_logging(u64* memory_requirement, void* state) {
    *memory_requirement = sizeof(logger_system_state);
    if (state == 0) {
        return true;
    }

    state_ptr = state;

    if (!filesystem_open("console.log", FILE_MODE_WRITE, false, &state_ptr->log_file_handle)) {
        platform_console_write_error("ERROR: Unable to open console.log for writing", LOG_LEVEL_ERROR);
        return false;
    }

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
    const char* level_strings[6] = {"[!] FATAL :: ", "[!] ERROR :: ", "[-] WARN :: ", "[-] INFO :: ", "[-] DEBUG :: ", "[-] TRACE :: "};
    b8 is_error = level < LOG_LEVEL_WARN;

    const i32 msg_length = 32000;
    char out_message[msg_length];
    kzero_memory(out_message, sizeof(out_message));

    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    string_format_v(out_message, message, arg_ptr);
    va_end(arg_ptr);

    string_format(out_message, "%s%s\n", level_strings[level], out_message);

    if (is_error) {
        platform_console_write_error(out_message, level);
    } else {
        platform_console_write(out_message, level);
    }

    append_to_log_file(out_message);
}