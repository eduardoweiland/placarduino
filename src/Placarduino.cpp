#include "config.h"
#include "Placarduino.hpp"

#include <string.h>
#include <avr/wdt.h>

#define PLACARDUINO_VERSION   "v2.2"

#define LCD_COLS         20
#define LCD_ROWS          4

Placarduino::Placarduino() : App(),
    lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS),
    bigNumbers(&lcd),
    player1(PIN_BTN_ADD_PL1, PIN_BTN_SUB_PL1),
    player2(PIN_BTN_ADD_PL2, PIN_BTN_SUB_PL2),
    rfidKey RFID_KEY,
    smartCard(rfidKey, RFID_PIN_SS, RFID_PIN_RESET),
    playerCard(&smartCard, 1),
    gameOverButton(PIN_BTN_GAME_OVER),
    musicPlayer(&buzzer),
    playerToConfigure(&player1)
{
}

void Placarduino::init()
{
    MCUSR = 0;
    wdt_disable();

    player1.setName("Jogador 1");
    player2.setName("Jogador 2");

    setupInputs();
    setupLCD();
    setupBuzzer();

    printWelcome();
    playWelcome();

    delay(1000);

    printScore();

    smartCard.begin();
    Serial.begin(9600);
}

void Placarduino::run()
{
    readPlayerCard();
    checkButtons();
    printTime();

    if (gameOverButton.pressed()) {
        gameOver();
    }
}

void Placarduino::setupInputs()
{
    pinMode(PIN_BTN_SUB_PL1, INPUT);
    pinMode(PIN_BTN_ADD_PL1, INPUT);
    pinMode(PIN_BTN_SUB_PL2, INPUT);
    pinMode(PIN_BTN_ADD_PL2, INPUT);
}

void Placarduino::setupLCD()
{
    lcd.init();
    bigNumbers.init();

    lcd.backlight();
}

void Placarduino::setupBuzzer()
{
    buzzer.begin(PIN_BUZZER);
}

void Placarduino::printWelcome()
{
    lcd.clear();

    // Posiciona o cursor na terceira coluna da segunda linha
    lcd.setCursor(2, 1);
    lcd.print("PLACARDUINO " PLACARDUINO_VERSION);

    // Posiciona o cursor na quinta coluna da terceira linha
    lcd.setCursor(4, 2);
    lcd.print("a.k.a. DAMN");
}

void Placarduino::playWelcome()
{
    musicPlayer.superMarioTheme();
}

void Placarduino::checkButtons()
{
    int player1Changed = player1.updateScore();
    int player2Changed = player2.updateScore();

    if (player1Changed > 0 || player2Changed > 0) {
        printScore();
        playFeedbackPositive();
    }
    else if (player1Changed < 0 || player2Changed < 0) {
        printScore();
        playFeedbackNegative();
    }
}

void Placarduino::printScore()
{
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(player1.getName());

    // Alinha o nome do jogador à direita
    lcd.setCursor(LCD_COLS - strlen(player2.getName()), 0);
    lcd.print(player2.getName());

    bigNumbers.printNumber(player1.getScore(), 0, 1);

    // Altera coluna inicial de acordo com valor da pontuação para
    // manter alinhado à direita
    if (player2.getScore() >= 10) {
        bigNumbers.printNumber(player2.getScore(), LCD_COLS - 7, 1);
    }
    else {
        bigNumbers.printNumber(player2.getScore(), LCD_COLS - 3, 1);
    }

    printTime();
}

void Placarduino::readPlayerCard()
{
    if (playerCard.readPlayerNameFromCard(playerToConfigure)) {
        if (playerToConfigure == &player1) {
            playerToConfigure = &player2;
        }
        else {
            playerToConfigure = &player1;
        }

        printScore();
    }
}

void Placarduino::playFeedbackPositive()
{
    musicPlayer.playNote(NOTE_C6, 140);
    musicPlayer.playNote(NOTE_C7, 480);
}

void Placarduino::playFeedbackNegative()
{
    musicPlayer.playNote(NOTE_C7, 140);
    musicPlayer.playNote(NOTE_C6, 480);
}

void Placarduino::gameOver()
{
    bool hasWinner = false;
    const char *winnerName;
    size_t winnerNameLength;

    lcd.clear();

    if (player1.getScore() > player2.getScore()) {
        winnerName = player1.getName();
        hasWinner = true;
    }
    else if (player2.getScore() > player1.getScore()) {
        winnerName = player2.getName();
        hasWinner = true;
    }

    if (!hasWinner) {
        lcd.setCursor(7, 1);
        lcd.print("EMPATE");
    }
    else {
        // \o/
        lcd.setCursor(1, 1); lcd.write(255);
        lcd.setCursor(1, 2); lcd.write(255);
        lcd.setCursor(2, 3); lcd.write(255);

        lcd.setCursor(3, 2); lcd.write(0);
        lcd.setCursor(4, 2); lcd.write(4);
        lcd.setCursor(5, 2); lcd.write(1);
        lcd.setCursor(3, 3); lcd.write(2);
        lcd.setCursor(4, 3); lcd.write(5);
        lcd.setCursor(5, 3); lcd.write(3);

        lcd.setCursor(7, 1); lcd.write(255);
        lcd.setCursor(7, 2); lcd.write(255);
        lcd.setCursor(6, 3); lcd.write(255);

        winnerNameLength = strnlen(winnerName, PlayerControl::MAX_NAME_LENGTH);
        lcd.setCursor(9 + (11 - winnerNameLength) / 2, 1);
        lcd.print(winnerName);

        lcd.setCursor(11, 2);
        lcd.print("GANHOU!");

        musicPlayer.victory();
    }

    wdt_enable(WDTO_4S);
    while (1);
}

void Placarduino::printTime()
{
    char buffer[3];

    sprintf(buffer, "%02lu", millis() / 60000);
    lcd.setCursor(9, 2);
    lcd.print(buffer);

    sprintf(buffer, "%02lu", millis() / 1000 % 60);
    lcd.setCursor(9, 3);
    lcd.print(buffer);
}
