#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Tone.h>

#include "LcdBigNumbers.h"
#include "MusicPlayer.h"
#include "PlayerControl.h"

/*
 * CONSTANTES
 */

// Configurações do LCD
#define LCD_I2C_ADDR   0x3F
#define LCD_COLS         20
#define LCD_ROWS          4

// Configurações de pinos
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

// Objeto para controle de um buzzer
Tone buzzer;
MusicPlayer musicPlayer(&buzzer);


/*
 * SETUP
 */

void setup()
{
    player1.setName("Jogador 1");
    player2.setName("Jogador 2");

    setupInputs();
    setupLCD();
    setupBuzzer();

    printWelcome();
    playWelcome();

    delay(1000);

    printScore();
}


/*
 * LOOP
 */

void loop()
{
    checkButtons();
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
    lcd.print("PLACARDUINO v1.0");

    // Posiciona o cursor na quinta coluna da terceira linha
    lcd.setCursor(4, 2);
    lcd.print("a.k.a. DAMN");
}

void playWelcome()
{
    musicPlayer.cMajorScale();
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
    lcd.print(player1.getName());

    bigNumbers.printNumber(player1.getScore(), 0, 1);

    // Altera coluna inicial de acordo com valor da pontuação para
    // manter alinhado à direita
    if (player2.getScore() >= 10) {
        bigNumbers.printNumber(player2.getScore(), LCD_COLS - 7, 1);
    }
    else {
        bigNumbers.printNumber(player2.getScore(), LCD_COLS - 3, 1);
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
