#ifndef SETTINGS_H
#define SETTINGS_H

/*
  @brief Module for working with settings in settings file.
  Expected format below:
	  key1=val1
	  key2=val2
	  ...
	  keyn=valn
*/

extern short LEFT_LED_PIN_NUM;
extern short RIGHT_LED_PIN_NUM;
extern short PING_PIN_NUM;
extern short PING_MOUNT_PIN_NUM;
extern short LEFT_IR_FLASHLIGHT_PIN_NUM;
extern short LEFT_IR_RECEIVER_PIN_NUM;
extern short RIGHT_IR_FLASHLIGHT_PIN_NUM;
extern short RIGHT_IR_RECEIVER_PIN_NUM;
extern short OTHER_IR_RECEIVER_PIN_NUM;

typedef enum {
	SETTINGS_ERROR_NO_SD_CARD = 1,
	SETTINGS_ERRROR_NO_SETTINGS_FILE = 2
} SettingsError;
	

/* @brief Initializes settings from settings file
 * @param filepath Path of the file that contains the settings from which to initialize
 * @returns 0 if successfull. Positive int otherwise.
 */
short settings_init(const char *filepath);

#endif