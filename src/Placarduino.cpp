#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Tone.h>
#include <string.h>
#include <avr/wdt.h>

#include "LcdBigNumbers.h"
#include "MusicPlayer.h"
#include "RisingEdgeButton.h"
#include "SmartCard.h"

#include "PlayerControl.h"
#include "PlayerCard.h"

/*
 * CONSTANTES
 */

#define PLACARDUINO_VERSION   "v2.2"

// Configurações do LCD
#define LCD_I2C_ADDR   0x3F
#define LCD_COLS         20
#define LCD_ROWS          4

// Configurações de pinos
#define PIN_RFID_SS      10
#define PIN_RFID_RESET    9

#define PIN_BTN_GAME_OVER 8

#define PIN_BTN_SUB_PL1   7
#define PIN_BTN_ADD_PL1   6
#define PIN_BTN_SUB_PL2   5
#define PIN_BTN_ADD_PL2   4

#define PIN_BUZZER        3

/*
 * GLOBAIS
 */

// Inicializa o objeto LCD configurando o endereço de comunicação e tamanho
LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);
LcdBigNumbers bigNumbers(&lcd);

// Configurações dos jogadores e placar
PlayerControl player1(PIN_BTN_ADD_PL1, PIN_BTN_SUB_PL1);
PlayerControl player2(PIN_BTN_ADD_PL2, PIN_BTN_SUB_PL2);

// Leitor RFID
byte rfidKey[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
SmartCard smartCard(rfidKey, PIN_RFID_SS, PIN_RFID_RESET);
PlayerCard playerCard(&smartCard, 1);
PlayerControl *playerToConfigure = &player1;

RisingEdgeButton gameOverButton(PIN_BTN_GAME_OVER);

// Objeto para controle de um buzzer
Tone buzzer;
MusicPlayer musicPlayer(&buzzer);

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

/*
 * SETUP
 */

void setup()
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


/*
 * LOOP
 */

void loop()
{
    readPlayerCard();
    checkButtons();
    printTime();

    if (gameOverButton.pressed()) {
        gameOver();
    }
}

/*
 * FUNÇÕES
 */

void setupInputs()
{
    pinMode(PIN_BTN_SUB_PL1, INPUT);
    pinMode(PIN_BTN_ADD_PL1, INPUT);
    pinMode(PIN_BTN_SUB_PL2, INPUT);
    pinMode(PIN_BTN_ADD_PL2, INPUT);
}

void setupLCD()
{
    lcd.init();
    bigNumbers.init();

    lcd.backlight();
}

void setupBuzzer()
{
    buzzer.begin(PIN_BUZZER);
}

void printWelcome()
{
    lcd.clear();

    // Posiciona o cursor na terceira coluna da segunda linha
    lcd.setCursor(2, 1);
    lcd.print("PLACARDUINO " PLACARDUINO_VERSION);

    // Posiciona o cursor na quinta coluna da terceira linha
    lcd.setCursor(4, 2);
    lcd.print("a.k.a. DAMN");
}

void playWelcome()
{
    musicPlayer.superMarioTheme();
}

void checkButtons()
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

void printScore()
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

void readPlayerCard()
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

void playFeedbackPositive()
{
    musicPlayer.playNote(NOTE_C6, 140);
    musicPlayer.playNote(NOTE_C7, 480);
}

void playFeedbackNegative()
{
    musicPlayer.playNote(NOTE_C7, 140);
    musicPlayer.playNote(NOTE_C6, 480);
}

void gameOver()
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

void printTime()
{
    char buffer[3];

    sprintf(buffer, "%02lu", millis() / 60000);
    lcd.setCursor(9, 2);
    lcd.print(buffer);

    sprintf(buffer, "%02lu", millis() / 1000 % 60);
    lcd.setCursor(9, 3);
    lcd.print(buffer);
}
