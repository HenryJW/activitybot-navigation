#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "logger.h"

static short _log(const char * message, LogLevel level, const FILE * file);
static const char* _get_log_level_tag(LogLevel level);
static const char* _get_current_time();


short logger_log_to_file(const char * message, LogLevel level, const char * filepath) {
	FILE *file = fopen(filepath, "a");
	int success = 0;
	if (file) {
		success = _log(message, level, file);
		fclose(file);
	}
	return success;	
}

short logger_log_to_console(const char * message, LogLevel level) {
	const FILE *console_fp = level != ERROR ? stdout : stderr;
	return _log(message, level, console_fp);
}


static short _log(const char * message, LogLevel level, const FILE * file) {
	if(!file) return 0;

	const char *tag = _get_log_level_tag(level);
	const char *current_time = _get_current_time();
	char *log_message = malloc(strlen(tag) + strlen(current_time) + strlen(message) + 
		strlen("  [] \n") + 1); //Account for additional characters in the string.
	sprintf(log_message, "%s  [%s] %s\n", current_time, tag, message);

 	int success = fputs(log_message, file) != EOF;

	return success;
}

static const char* _get_log_level_tag(LogLevel level) {
	switch(level) {
		case INFO:
			return "INFO";
		case WARNING:
			return "WARNING";
		case ERROR:
			return "ERROR";
		default:
			return "OTHER";
	}
}

static const char* _get_current_time() {
	time_t now;
	time(&now);
	char* current_time = ctime(&now);
	current_time[strlen(current_time) - 1] = '\0'; //Remove newline
	return current_time;
}