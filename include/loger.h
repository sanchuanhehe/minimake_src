#ifndef LOGGER_H
#define LOGGER_H
// 日志级别枚举
typedef enum {
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG
} log_level_t;

// 全局日志配置
static struct {
    log_level_t level;
    int use_color;
} logger_config = {LOG_INFO, 1}; // 默认级别为 INFO，启用颜色

void log_message(log_level_t level, const char *file, int line, const char *fmt, ...);

// 封装宏简化调用
#define log_debug(...)   log_message(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)    log_message(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warning(...) log_message(LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...)   log_message(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#endif