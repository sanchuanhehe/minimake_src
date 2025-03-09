#ifndef LOGGER_H
#define LOGGER_H
// 日志级别枚举
typedef enum { LOG_ERROR, LOG_WARNING, LOG_INFO, LOG_DEBUG } LogLevel_t;

struct logger_config_t {
  LogLevel_t level;
  int useColor;
};

extern struct logger_config_t logger_config;

void LogMessage(LogLevel_t level, const char *file, int line, const char *fmt,
                ...);

// 封装宏简化调用
#define LogDebug(...) LogMessage(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LogInfo(...) LogMessage(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LogWarning(...) LogMessage(LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define LogError(...) LogMessage(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#endif