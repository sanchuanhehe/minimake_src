#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include "loger.h"

// 全局日志配置
struct logger_config_t logger_config = {LOG_INFO, 1}; // 默认级别为 INFO，启用颜色

void log_message(log_level_t level, const char *file, int line, const char *fmt, ...)
{
    if (level > logger_config.level)
        return;

    // 获取时间戳
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[64] = {0};  // 修改为字符数组并初始化
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    // 颜色定义
    const char *color = "";
    const char *level_str = "";
    switch (level)
    {
    case LOG_DEBUG:
        color = "\x1b[36m";
        level_str = "DEBUG";
        break;
    case LOG_INFO:
        color = "\x1b[32m";
        level_str = "INFO";
        break;
    case LOG_WARNING:
        color = "\x1b[33m";
        level_str = "WARN";
        break;
    case LOG_ERROR:
        color = "\x1b[31m";
        level_str = "ERROR";
        break;
    }

    // 输出格式
    fprintf(stderr, "%s[%s][%s%s\x1b[0m][%s:%d] ",
            logger_config.use_color ? "" : "",
            timestamp,
            logger_config.use_color ? color : "",
            level_str,
            file, line);

    // 输出用户消息
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}
