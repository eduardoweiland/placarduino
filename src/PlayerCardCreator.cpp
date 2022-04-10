#include "config.h"
#include "PlayerCardCreator.hpp"
#include "PlayerControl.h"

#include <Arduino.h>
#include <avr/wdt.h>
#include <string.h>

static char alphabet[] = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

static uint8_t charArrowUpDown[8] = {
    B00100,
    B01110,
    B11111,
    B00000,
    B11111,
    B01110,
    B00100,
    B00000,
};

#define LCD_COLS     20
#define LCD_ROWS      4
#define CARD_BLOCK    1
#define CHAR_ARROWS   1

PlayerCardCreator::PlayerCardCreator() : App(),
    lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS),
    charUpButton(PIN_BTN_ADD_PL1),
    charDownButton(PIN_BTN_SUB_PL1),
    walkLeftButton(PIN_BTN_ADD_PL2),
    walkRightButton(PIN_BTN_SUB_PL2),
    writeButton(PIN_BTN_FRONT),
    rfidKey RFID_KEY,
    smartCard(rfidKey, RFID_PIN_SS, RFID_PIN_RESET),
    charIndexUnderCursor(0),
    statusLine(nullptr)
{
}

void PlayerCardCreator::init()
{
    setupLCD();
    resetName();

    showControlHints();
    showName();

    smartCard.begin();
    Serial.begin(9600);

    delay(2000);
}

void PlayerCardCreator::run()
{
    if (executeNameChanges()) {
        showName();
        delay(500);
    }

    if (writeButton.pressed()) {
        if (writeNameToCard()) {
            resetName();
        }
    }
}

void PlayerCardCreator::setupLCD()
{
    lcd.init();
    lcd.backlight();
    lcd.blink();
    lcd.createChar(CHAR_ARROWS, charArrowUpDown);
}

void PlayerCardCreator::showControlHints()
{
    lcd.setCursor(0, 0);
    lcd.write(CHAR_ARROWS);
    lcd.print(F("CHARS"));

    lcd.setCursor(13, 0);
    lcd.print(F("CURSOR"));
    lcd.write(CHAR_ARROWS);
}

void PlayerCardCreator::showName()
{
    // Terceira linha: nome
    lcd.setCursor(5, 2);
    lcd.print(playerName);

    // Mostra cursor onde está sendo editado
    lcd.setCursor(charIndexUnderCursor + 5, 2);
}

bool PlayerCardCreator::executeNameChanges()
{
    if (charUpButton.pressed()) {
        playerName[charIndexUnderCursor] = nextChar(playerName[charIndexUnderCursor]);
        return true;
    }
    if (charDownButton.pressed()) {
        playerName[charIndexUnderCursor] = prevChar(playerName[charIndexUnderCursor]);
        return true;
    }

    if (walkLeftButton.pressed() && charIndexUnderCursor > 0) {
        --charIndexUnderCursor;
        return true;
    }
    if (walkRightButton.pressed() && charIndexUnderCursor < PlayerControl::MAX_NAME_LENGTH - 1) {
        ++charIndexUnderCursor;
        return true;
    }

    return false;
}

void PlayerCardCreator::resetName()
{
    playerName = F("__________");
}

bool PlayerCardCreator::writeNameToCard()
{
    clearLine(3);
    lcd.setCursor(0, 3);

    auto cleanName = playerName;
    cleanName.replace('_', ' ');
    cleanName.trim();

    byte dataToWrite[16];
    memset(dataToWrite, 0, sizeof(dataToWrite));
    memcpy(dataToWrite, cleanName.c_str(), cleanName.length());

    if (smartCard.write(CARD_BLOCK, dataToWrite, sizeof(dataToWrite))) {
        lcd.print(F("Cartao escrito OK"));
        return true;
    }
    else {
        lcd.print(F("Falha na escrita"));
        return false;
    }
}

void PlayerCardCreator::clearLine(const int number) {
    lcd.setCursor(0, number);
    lcd.print(F("                    "));
}

char PlayerCardCreator::nextChar(const char current)
{
    char *ptr;
    for (ptr = alphabet; *ptr && *ptr != current; ++ptr);

    // Not found
    if (!*ptr) {
        return alphabet[0];
    }

    ++ptr;

    // At end
    if (!*ptr) {
        return alphabet[0];
    }

    return *ptr;
}

char PlayerCardCreator::prevChar(const char current)
{
    char *ptr;
    for (ptr = alphabet; *ptr && *ptr != current; ++ptr);

    // Not found
    if (!*ptr) {
        return alphabet[0];
    }

    // At start
    if (ptr == alphabet) {
        return alphabet[sizeof(alphabet) - 2];
    }

    return *--ptr;
}
