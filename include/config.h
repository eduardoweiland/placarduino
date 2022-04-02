#ifndef PLACARDUINO_CONFIG_H
#define PLACARDUINO_CONFIG_H

// LCD
#define LCD_I2C_ADDR   0x3F

// RFID
#define RFID_PIN_SS      10
#define RFID_PIN_RESET    9
#define RFID_KEY          { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }

// BUTTONS
#define PIN_BTN_GAME_OVER 4
#define PIN_BTN_SUB_PL1   5
#define PIN_BTN_ADD_PL1   6
#define PIN_BTN_SUB_PL2   8
#define PIN_BTN_ADD_PL2   7

// BUZZER
#define PIN_BUZZER        3

#endif // PLACARDUINO_CONFIG_H
