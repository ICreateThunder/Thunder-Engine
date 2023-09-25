#include "logger.h"

// TODO: Temporary usage of standard libraries to write out
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 initialse_logging() {
    // TODO: Create logfile
    return TRUE;
}

void shutdown_logging() {
    // TODO: Cleanup logging/write queued entries
}

KAPI void log_output(log_level level, const char* message, ...) {
    const char* level_strings[6] = {"[!] FATAL :: ", "[!] ERROR :: ", "[~] WARN :: ", "[-] INFO :: ", "[-] DEBUG :: ", "[-] TRACE :: "};
    // b8 is_error = level < 2;

    char out_message[32000];
    memset(out_message, 0, sizeof(out_message));

    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, 32000, message, arg_ptr);
    va_end(arg_ptr);

    char out_message2[32000];
    sprintf(out_message2, "%s%s\n", level_strings[level], out_message);

    // TODO: Platform specific output
    printf("%s", out_message2);
}