#ifndef PLACARDUINO_LCD_BIG_NUMBERS_H
#define PLACARDUINO_LCD_BIG_NUMBERS_H

class LiquidCrystal_I2C;

class LcdBigNumbers
{
public:
    LcdBigNumbers(LiquidCrystal_I2C *lcd);

    void init();
    void printNumber(const unsigned long int number, uint8_t column, uint8_t row);
    void printDigit(const uint8_t digit, uint8_t column, uint8_t row);

private:
    LiquidCrystal_I2C *lcd;
};

#endif // PLACARDUINO_LCD_BIG_NUMBERS_H
