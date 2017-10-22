#include <LiquidCrystal.h>

/*
 * CONSTANTES
 */

// Configurações de pinos
#define PIN_LCD_D7        8
#define PIN_LCD_D6        9
#define PIN_LCD_D5       10
#define PIN_LCD_D4       11
#define PIN_LCD_EN       12
#define PIN_LCD_RS       13

#define PIN_BTN_1         7
#define PIN_BTN_2         6


/*
 * GLOBAIS
 */

// Inicializa o objeto LCD configurando os pinos de comunicação
LiquidCrystal lcd(
    PIN_LCD_RS,
    PIN_LCD_EN,
    PIN_LCD_D4,
    PIN_LCD_D5,
    PIN_LCD_D6,
    PIN_LCD_D7
);

// Configurações dos jogadores e placar
const char firstPlayerName[] = "Jogador 1";
unsigned int firstPlayerScore = 0;
int firstButtonState = LOW;

const char secondPlayerName[] = "Jogador 2";
unsigned int secondPlayerScore = 0;
int secondButtonState = LOW;


/*
 * SETUP
 */

void setup() {
    setupInputs();
    setupLCD();
    printWelcome();

    delay(3000);
}


/*
 * LOOP
 */

void loop() {
    checkButtons();
    printScore();

    delay(100);
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
    // Configura o tamanho do LCD: 16 colunas e 2 linhas
    lcd.begin(16, 2);
}

void printWelcome()
{
    lcd.clear();

    // Posiciona o cursor na primeira coluna da primeira linha
    lcd.setCursor(0, 0);
    lcd.print("PLACARDUINO v1.0");

    // Posiciona o cursor na terceira coluna da segunda linha
    lcd.setCursor(2, 1);
    lcd.print("a.k.a. DAMN");
}

void checkButtons()
{
    int newButtonState;

    newButtonState = digitalRead(PIN_BTN_1);
    if (newButtonState != firstButtonState && newButtonState == HIGH) {
        // Se o botão mudou de estado e o novo estado é pressionado, soma um ponto
        firstPlayerScore++;
    }
    firstButtonState = newButtonState;

    newButtonState = digitalRead(PIN_BTN_2);
    if (newButtonState != secondButtonState && newButtonState == HIGH) {
        // Se o botão mudou de estado e o novo estado é pressionado, soma um ponto
        secondPlayerScore++;
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
