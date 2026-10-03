#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifndef LOG_BUFFER_SIZE
#define LOG_BUFFER_SIZE 128
#endif

static LogLevel g_logLevel = LOG_LEVEL_INFO;

extern void loggerPlatformPrint(const char* msg);

void loggerInit(LogLevel level) {
    g_logLevel = level;
}

void loggerSetLevel(LogLevel level) {
    g_logLevel = level;
}

LogLevel loggerGetLevel(void) {
    return g_logLevel;
}

void loggerPrint(LogLevel level, const char* tag, const char* fmt, ...) {
    if (level > g_logLevel) {
        return;
    }

    char buf[LOG_BUFFER_SIZE];
    int offset = 0;

    offset += snprintf(buf + offset, sizeof(buf) - offset, "[%s] ", tag);

    va_list args;
    va_start(args, fmt);
    offset += vsnprintf(buf + offset, sizeof(buf) - offset, fmt, args);
    va_end(args);

    if (offset < (int)sizeof(buf) - 2) {
        buf[offset++] = '\r';
        buf[offset++] = '\n';
        buf[offset]   = '\0';
    } else {
        buf[sizeof(buf) - 1] = '\n';
        buf[sizeof(buf) - 2] = '\r';
    }

    loggerPlatformPrint(buf);
}