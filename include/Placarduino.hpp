#pragma once

#include "App.hpp"
#include "LcdBigNumbers.h"
#include "MusicPlayer.h"
#include "RisingEdgeButton.h"
#include "SmartCard.h"
#include "PlayerControl.h"
#include "PlayerCard.h"

#include <LiquidCrystal_I2C.h>
#include <Tone.h>
#include <stdint.h>

class Placarduino : public App
{
public:
    Placarduino();
    void init();
    void run();

private:
    LiquidCrystal_I2C lcd;
    LcdBigNumbers bigNumbers;
    PlayerControl player1;
    PlayerControl player2;
    uint8_t rfidKey[MFRC522::MF_KEY_SIZE];
    SmartCard smartCard;
    PlayerCard playerCard;
    RisingEdgeButton gameOverButton;
    Tone buzzer;
    MusicPlayer musicPlayer;
    PlayerControl *playerToConfigure;

    void setupInputs();
    void setupLCD();
    void setupBuzzer();
    void printWelcome();
    void playWelcome();
    void printScore();
    void readPlayerCard();
    void checkButtons();
    void gameOver();
    void playFeedbackPositive();
    void playFeedbackNegative();
    void printTime();
};
