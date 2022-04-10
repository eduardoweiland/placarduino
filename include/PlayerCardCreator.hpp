#pragma once

#include "App.hpp"
#include "RisingEdgeButton.h"
#include "SmartCard.h"

#include <LiquidCrystal_I2C.h>
#include <stdint.h>
#include <WString.h>


class PlayerCardCreator : public App
{
public:
    PlayerCardCreator();
    void init();
    void run();

private:
    LiquidCrystal_I2C lcd;
    RisingEdgeButton charUpButton;
    RisingEdgeButton charDownButton;
    RisingEdgeButton walkLeftButton;
    RisingEdgeButton walkRightButton;
    RisingEdgeButton writeButton;
    uint8_t rfidKey[MFRC522::MF_KEY_SIZE];
    SmartCard smartCard;
    String playerName;
    uint8_t charIndexUnderCursor;
    char *statusLine;

    void setupLCD();
    void showControlHints();
    void showName();
    bool executeNameChanges();
    void resetName();
    bool writeNameToCard();
    void clearLine(const int number);
    char nextChar(const char current);
    char prevChar(const char current);
};
