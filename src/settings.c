#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <simpletext.h>

#include "settings.h"
#include "sdcard.h"

short LEFT_LED_PIN_NUM = 7;
short RIGHT_LED_PIN_NUM = 2;
short PING_PIN_NUM = 16;
short PING_MOUNT_PIN_NUM = 17;
short LEFT_IR_RECEIVER_PIN_NUM = 8;
short LEFT_IR_FLASHLIGHT_PIN_NUM = 9;
short RIGHT_IR_FLASHLIGHT_PIN_NUM = 0;
short RIGHT_IR_RECEIVER_PIN_NUM = 1;
short OTHER_IR_RECEIVER_PIN_NUM = 6;


static short _extract_key_value(char * input, char ** key, char ** value);
static short _set_value(const char * key, const char * value);
static char * _getline(FILE *fp);
static char _fpeek(FILE *fp);

short settings_init(const char * filepath) {
	if(!sdcard_mount()) {
		return SETTINGS_ERROR_NO_SD_CARD;
	}
	
	FILE *fp = fopen(filepath, "r");
		
	if(fp) {
		char *line = NULL;
		char *key = NULL;
		char *value = NULL;

		while( (line = _getline(fp)) != NULL) {
			if(!_extract_key_value(line, &key, &value)) {
				free(line);
				continue;
			}
			_set_value(key, value);
			free(key);
			free(value);
		}

		fclose(fp);
		return 0;
	} else {
		return SETTINGS_ERRROR_NO_SETTINGS_FILE;
	}
}

short _extract_key_value(char * input, char ** key, char ** value) {
	*key = strtok(input, "=");
	if(*key == NULL) return 0;
	*value = strtok(NULL, NULL);
	if(*value == NULL) return 0;

	return 1;
}

static short _set_value(const char * key, const char * value) {
	short success = 1;
	int val = atoi(value);

	if(strcmp(key, "left_led") == 0) {
		LEFT_LED_PIN_NUM = val;
	} else if(strcmp(key, "right_led") == 0) {
		RIGHT_LED_PIN_NUM = val;
	} else if(strcmp(key, "ping") == 0) {
		PING_PIN_NUM = val;
	} else if(strcmp(key, "ping_mount") == 0) {
		PING_MOUNT_PIN_NUM = val;
	} else if(strcmp(key, "left_ir_flashlight") == 0) {
		LEFT_IR_FLASHLIGHT_PIN_NUM = val;
	} else if(strcmp(key, "left_ir_receiver") == 0) {
		LEFT_IR_RECEIVER_PIN_NUM = val;
	} else if(strcmp(key, "right_ir_flashlight") == 0) {
		RIGHT_IR_FLASHLIGHT_PIN_NUM = val;
	} else if(strcmp(key, "right_ir_flashlight") == 0) {
		RIGHT_IR_RECEIVER_PIN_NUM = val;
	} else if(strcmp(key, "other_ir_receiver") == 0) {
		OTHER_IR_RECEIVER_PIN_NUM = val;
	} else {
		success = 0;
	}

	return success;
}

static char * _getline(FILE *fp) {
	char first_char = _fpeek(fp);
	if(first_char == EOF || (!isprint(first_char)) && !isspace(first_char)) return NULL;

	int line_size = 16;
	char *line = (char *) malloc(line_size);
	line[0] = '\0';
	int c = '\0';
	int i = 0;
	while ( (c = fgetc(fp)) != EOF && c != '\n') {
		if(strlen(line) >= line_size) {
			//Double allocated memory for string
			char *temp = line;
			line_size *= 2;
			line = (char*) malloc(line_size);
			strcpy(line, temp);
			free(temp);
		}
		line[i] = c;
		line[i + 1] = '\0';
		i++;
	}

	if(strlen(line) < line_size) {
		//Shrink allocated memory to exactly fit string
		char *temp = line;
		line = (char *) malloc(strlen(line) + 1);
		line = strcpy(line, temp);
		free(temp);
	}
	return line;
}

static char _fpeek(FILE *fp) {
	char c = fgetc(fp);
	ungetc(c, fp);
	return c;
}