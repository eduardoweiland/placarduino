#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Tone.h>

/*
 * CONSTANTES
 */

// Configurações do LCD
#define LCD_I2C_ADDR   0x3F
#define LCD_COLS         20
#define LCD_ROWS          4

// Configurações de pinos
#define PIN_BTN_1         7
#define PIN_BTN_2         6

#define PIN_BUZZER        5

/*
 * GLOBAIS
 */

// Inicializa o objeto LCD configurando o endereço de comunicação e tamanho
LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);

// Configurações dos jogadores e placar
const char firstPlayerName[] = "Jogador 1";
unsigned int firstPlayerScore = 0;
int firstButtonState = LOW;

const char secondPlayerName[] = "Jogador 2";
unsigned int secondPlayerScore = 0;
int secondButtonState = LOW;

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
    pinMode(PIN_BTN_1, INPUT);
    pinMode(PIN_BTN_2, INPUT);
}

void setupLCD()
{
    lcd.init();
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

void checkButtons()
{
    int newButtonState;

    newButtonState = digitalRead(PIN_BTN_1);
    if (newButtonState != firstButtonState && newButtonState == HIGH) {
        // Se o botão mudou de estado e o novo estado é pressionado, soma um ponto
        firstPlayerScore++;
        printScore();
        playFeedback();
    }
    firstButtonState = newButtonState;

    newButtonState = digitalRead(PIN_BTN_2);
    if (newButtonState != secondButtonState && newButtonState == HIGH) {
        // Se o botão mudou de estado e o novo estado é pressionado, soma um ponto
        secondPlayerScore++;
        printScore();
        playFeedback();
    }
    secondButtonState = newButtonState;
}

void printScore()
{
    // Buffer para saída do dtostrf
    // Usado para formatar os pontos alinhados à direita
    char outScore[4];

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(firstPlayerName);
    lcd.setCursor(13, 0);
    lcd.print(dtostrf(firstPlayerScore, 3, 0, outScore));

    lcd.setCursor(0, 1);
    lcd.print(secondPlayerName);
    lcd.setCursor(13, 1);
    lcd.print(dtostrf(secondPlayerScore, 3, 0, outScore));
}

void playFeedback()
{
    buzzer.play(NOTE_C6, 120);
    delay(140);

    buzzer.play(NOTE_C7, 420);
    delay(500);
}
