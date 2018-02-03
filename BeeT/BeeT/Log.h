#ifndef __LOG_H__
#define __LOG_H__

/*
 * DEBUG_LEVEL defines the type of messages that will be printed in the output console.
 * There are 3 levels:
 * 0 - Shows informative, warning and error messages
 * 1 - Shows warning and error messages
 * 2 - Shows error messages
*/
#define DEBUG_LEVEL 0 // TODO: Change debug level to be a customizable option

#define LOGI(format, ...) log(__FILE__, __LINE__, 0, format, __VA_ARGS__); // Information log	Level 0
#define LOGW(format, ...) log(__FILE__, __LINE__, 1, format, __VA_ARGS__); // Warning log		Level 1
#define LOGE(format, ...) log(__FILE__, __LINE__, 2, format, __VA_ARGS__); // Error log			Level 2

void log(const char file[], int line, int level, const char* format, ...);

#endif