#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOG_LEVEL_NONE  = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_INFO  = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} LogLevel;

void loggerInit(LogLevel level);
void loggerSetLevel(LogLevel level);
LogLevel loggerGetLevel(void);

void loggerPrint(LogLevel level, const char* tag, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

// ---------------------------------------------------------
// ANSI 颜色代码定义
// ---------------------------------------------------------
#define COLOR_RESET "\033[0m"
#define COLOR_RED   "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_GREEN "\033[32m"
#define COLOR_CYAN  "\033[36m"
#define COLOR_BLUE  "\033[34m"

#if defined(LOG_ENABLE) && LOG_ENABLE

// ==================== 开启日志与颜色 ====================

#if defined(COLOR_ENABLE) && COLOR_ENABLE
    // 开启颜色时的宏（不同等级对应不同颜色）
    #define LOG_E(fmt, ...)  loggerPrint(LOG_LEVEL_ERROR, COLOR_RED "E" COLOR_RESET, fmt, ##__VA_ARGS__)
    #define LOG_W(fmt, ...)  loggerPrint(LOG_LEVEL_WARN,  COLOR_YELLOW "W" COLOR_RESET, fmt, ##__VA_ARGS__)
    #define LOG_I(fmt, ...)  loggerPrint(LOG_LEVEL_INFO,  COLOR_GREEN "I" COLOR_RESET, fmt, ##__VA_ARGS__)
    #define LOG_D(fmt, ...)  loggerPrint(LOG_LEVEL_DEBUG, COLOR_CYAN "D" COLOR_RESET, fmt, ##__VA_ARGS__)
    #define LOG_T(fmt, ...)  loggerPrint(LOG_LEVEL_TRACE, COLOR_BLUE "T" COLOR_RESET, fmt, ##__VA_ARGS__)
#else
    // 关闭颜色时的宏（只有黑白）
    #define LOG_E(fmt, ...)  loggerPrint(LOG_LEVEL_ERROR, "E", fmt, ##__VA_ARGS__)
    #define LOG_W(fmt, ...)  loggerPrint(LOG_LEVEL_WARN,  "W", fmt, ##__VA_ARGS__)
    #define LOG_I(fmt, ...)  loggerPrint(LOG_LEVEL_INFO,  "I", fmt, ##__VA_ARGS__)
    #define LOG_D(fmt, ...)  loggerPrint(LOG_LEVEL_DEBUG, "D", fmt, ##__VA_ARGS__)
    #define LOG_T(fmt, ...)  loggerPrint(LOG_LEVEL_TRACE, "T", fmt, ##__VA_ARGS__)
#endif

#else
// ==================== 关闭日志 ====================
    #define LOG_E(...)
    #define LOG_W(...)
    #define LOG_I(...)
    #define LOG_D(...)
    #define LOG_T(...)

#endif

#endif