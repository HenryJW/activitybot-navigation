#ifndef LOG_H
#define LOG_H

typedef enum {
	INFO,
	WARNING,
	ERROR
}LogLevel;

/**
 * @brief Logs message to file
 * @param message Message to be logged
 * @param level Log level
 * @param filepath File path of log file
 * @returns int 1 if operation was successful. 0 otherwise.
 */
short logger_log_to_file(const char * message, LogLevel level, const char * filepath);

/**
 * @brief Logs message console (stdout)
 * @param message Message to be logged
 * @param level Log level
 * @returns int 1 if operation was successful. 0 otherwise.
 */
short logger_log_to_console(const char * message, LogLevel level);

#endif