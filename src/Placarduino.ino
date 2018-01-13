#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Tone.h>

#include "LcdBigNumbers.h"

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
const char firstPlayerName[] = "Jogador 1";
int firstPlayerScore = 0;
int subPlayer1ButtonState = LOW;
int addPlayer1ButtonState = LOW;

const char secondPlayerName[] = "Jogador 2";
int secondPlayerScore = 0;
int subPlayer2ButtonState = LOW;
int addPlayer2ButtonState = LOW;

// Objeto para controle de um buzzer
Tone buzzer;


/*
 * SETUP
 */

void setup() {
    setupInputs();
    setupLCD();
    setupBuzzer();

    printWelcome();
    playWelcome();

    delay(3000);

    printScore();
}


/*
 * LOOP
 */

void loop() {
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

// Escala de Dó maior
void playWelcome()
{
    // Básico
    int i;
    int notes[] = { NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6, NOTE_C7 };

    for (i = 0; i < 8; ++i) {
        buzzer.play(notes[i], 30 + (i * 5));
        delay(35 + (i * 5));
    }
}

// Super Mário
//void playWelcome()
//{
//    buzzer.play(NOTE_E6, 120);
//    delay(140);
//    buzzer.play(NOTE_E6, 120);
//    delay(140);
//    delay(140);
//    buzzer.play(NOTE_E6, 120);
//    delay(140);
//    delay(140);
//    buzzer.play(NOTE_C6, 120);
//    delay(140);
//    buzzer.play(NOTE_E6, 240);
//    delay(260);
//    buzzer.play(NOTE_G6, 240);
//    delay(260);
//    delay(260);
//    buzzer.play(NOTE_G5, 240);
//    delay(260);
//}

// Sweet Child O' Mine
//void playWelcome()
//{
//    int i, j;
//    int firstNote[] = { NOTE_CS5, NOTE_DS5, NOTE_FS5 };
//
//    for (i = 0; i < 3; ++i) {
//        for (j = 0; j < 2; ++j) {
//            buzzer.play(firstNote[i], 240);
//            delay(260);
//            buzzer.play(NOTE_CS6, 240);
//            delay(260);
//            buzzer.play(NOTE_GS5, 240);
//            delay(260);
//            buzzer.play(NOTE_FS5, 240);
//            delay(260);
//            buzzer.play(NOTE_FS6, 240);
//            delay(260);
//            buzzer.play(NOTE_GS5, 240);
//            delay(260);
//            buzzer.play(NOTE_F6, 240);
//            delay(260);
//            buzzer.play(NOTE_GS5, 240);
//            delay(260);
//        }
//    }
//
//    buzzer.play(NOTE_CS5, 240);
//    delay(260);
//}

void checkScoreInput(int inputPin, int *stateStore, int *scoreStore, int op, void(*feedbackFunction)())
{
    int newButtonState = digitalRead(inputPin);

    if (newButtonState != *stateStore && newButtonState == HIGH) {
        // Placar não deve ficar negativo, então a operação só é feita se o placar for continuar >= 0
        if (*scoreStore + op >= 0) {
            *scoreStore += op;
            printScore();
            feedbackFunction();
        }
    }

    *stateStore = newButtonState;
}

void checkButtons()
{
    checkScoreInput(PIN_BTN_ADD_PL1, &addPlayer1ButtonState, &firstPlayerScore, +1, playFeedbackPositive);
    checkScoreInput(PIN_BTN_SUB_PL1, &subPlayer1ButtonState, &firstPlayerScore, -1, playFeedbackNegative);
    checkScoreInput(PIN_BTN_ADD_PL2, &addPlayer2ButtonState, &secondPlayerScore, +1, playFeedbackPositive);
    checkScoreInput(PIN_BTN_SUB_PL2, &subPlayer2ButtonState, &secondPlayerScore, -1, playFeedbackNegative);
}

void printScore()
{
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(firstPlayerName);

    // Alinha o nome do jogador à direita
    lcd.setCursor(LCD_COLS - strlen(secondPlayerName), 0);
    lcd.print(secondPlayerName);

    bigNumbers.printNumber(firstPlayerScore, 0, 1);

    // Altera coluna inicial de acordo com valor da pontuação para
    // manter alinhado à direita
    if (secondPlayerScore >= 10) {
        bigNumbers.printNumber(secondPlayerScore, LCD_COLS - 7, 1);
    }
    else {
        bigNumbers.printNumber(secondPlayerScore, LCD_COLS - 3, 1);
    }
}

void playFeedbackPositive()
{
    buzzer.play(NOTE_C6, 120);
    delay(140);

    buzzer.play(NOTE_C7, 420);
    delay(500);
}

void playFeedbackNegative()
{
    buzzer.play(NOTE_C7, 120);
    delay(140);

    buzzer.play(NOTE_C6, 420);
    delay(500);
}
