#include <inttypes.h>
#include <binary.h>

#include <LiquidCrystal_I2C.h>

#include "LcdBigNumbers.h"

#define CHAR_TOP_LEFT       0
#define CHAR_TOP_RIGHT      1
#define CHAR_BOTTOM_LEFT    2
#define CHAR_BOTTOM_RIGHT   3
#define CHAR_HALF_TOP       4
#define CHAR_HALF_BOTTOM    5
#define CHAR_TOP_BOTTOM     6
#define CHAR_MIDDLE         7
#define CHAR_EMPTY        254
#define CHAR_FULL         255

static uint8_t customChars[8][8] =
{
    // CHAR_TOP_LEFT
    {
        B00001,
        B00111,
        B01111,
        B01111,
        B11111,
        B11111,
        B11111,
        B11111
    },

    // CHAR_TOP_RIGHT
    {
        B10000,
        B11100,
        B11110,
        B11110,
        B11111,
        B11111,
        B11111,
        B11111
    },

    // CHAR_BOTTOM_LEFT
    {
        B11111,
        B11111,
        B11111,
        B11111,
        B01111,
        B01111,
        B00111,
        B00001
    },

    // CHAR_BOTTOM_RIGHT
    {
        B11111,
        B11111,
        B11111,
        B11111,
        B11110,
        B11110,
        B11100,
        B10000
    },

    // CHAR_HALF_TOP
    {
        B11111,
        B11111,
        B11111,
        B11111,
        B00000,
        B00000,
        B00000,
        B00000
    },

    // CHAR_HALF_BOTTOM
    {
        B00000,
        B00000,
        B00000,
        B00000,
        B11111,
        B11111,
        B11111,
        B11111
    },

    // CHAR_TOP_BOTTOM
    {
        B11111,
        B11111,
        B11111,
        B00000,
        B00000,
        B11111,
        B11111,
        B11111
    },

    // CHAR_MIDDLE
    {
        B00000,
        B00000,
        B00000,
        B11111,
        B11111,
        B00000,
        B00000,
        B00000
    }
};

static uint8_t numberChars[10][3][3] =
{
    // 0
    {
        { CHAR_TOP_LEFT, CHAR_HALF_TOP, CHAR_TOP_RIGHT },
        { CHAR_FULL, CHAR_EMPTY, CHAR_FULL },
        { CHAR_BOTTOM_LEFT, CHAR_HALF_BOTTOM, CHAR_BOTTOM_RIGHT }
    },

    // 1
    {
        { CHAR_HALF_BOTTOM, CHAR_FULL, CHAR_EMPTY },
        { CHAR_EMPTY, CHAR_FULL, CHAR_EMPTY },
        { CHAR_HALF_BOTTOM, CHAR_FULL, CHAR_HALF_BOTTOM }
    },

    // 2
    {
        { CHAR_HALF_TOP, CHAR_HALF_TOP, CHAR_HALF_BOTTOM },
        { CHAR_HALF_BOTTOM, CHAR_MIDDLE, CHAR_HALF_TOP },
        { CHAR_FULL, CHAR_HALF_BOTTOM, CHAR_HALF_BOTTOM }
    },

    // 3
    {
        { CHAR_HALF_TOP, CHAR_HALF_TOP, CHAR_HALF_BOTTOM },
        { CHAR_EMPTY, CHAR_MIDDLE, CHAR_TOP_BOTTOM },
        { CHAR_HALF_BOTTOM, CHAR_HALF_BOTTOM, CHAR_HALF_TOP }
    },

    // 4
    {
        { CHAR_FULL, CHAR_EMPTY, CHAR_HALF_BOTTOM },
        { CHAR_FULL, CHAR_HALF_BOTTOM, CHAR_FULL },
        { CHAR_EMPTY, CHAR_EMPTY, CHAR_FULL }
    },

    // 5
    {
        { CHAR_FULL, CHAR_HALF_TOP, CHAR_HALF_TOP },
        { CHAR_HALF_TOP, CHAR_MIDDLE, CHAR_HALF_BOTTOM },
        { CHAR_HALF_BOTTOM, CHAR_HALF_BOTTOM, CHAR_BOTTOM_RIGHT }
    },

    // 6
    {
        { CHAR_TOP_LEFT, CHAR_HALF_TOP, CHAR_HALF_TOP },
        { CHAR_FULL, CHAR_HALF_TOP, CHAR_TOP_RIGHT },
        { CHAR_BOTTOM_LEFT, CHAR_HALF_BOTTOM, CHAR_BOTTOM_RIGHT }
    },

    // 7
    {
        { CHAR_HALF_TOP, CHAR_HALF_TOP, CHAR_FULL },
        { CHAR_EMPTY, CHAR_HALF_BOTTOM, CHAR_HALF_TOP },
        { CHAR_EMPTY, CHAR_FULL, CHAR_EMPTY }
    },

    // 8
    {
        { CHAR_HALF_BOTTOM, CHAR_HALF_TOP, CHAR_HALF_BOTTOM },
        { CHAR_TOP_BOTTOM, CHAR_MIDDLE, CHAR_TOP_BOTTOM },
        { CHAR_HALF_TOP, CHAR_HALF_BOTTOM, CHAR_HALF_TOP }
    },

    // 9
    {
        { CHAR_TOP_LEFT, CHAR_HALF_TOP, CHAR_TOP_RIGHT },
        { CHAR_BOTTOM_LEFT, CHAR_HALF_BOTTOM, CHAR_FULL },
        { CHAR_HALF_BOTTOM, CHAR_HALF_BOTTOM, CHAR_BOTTOM_RIGHT }
    }
};

LcdBigNumbers::LcdBigNumbers(LiquidCrystal_I2C *lcd)
{
    this->lcd = lcd;
}

void LcdBigNumbers::init()
{
    for (uint8_t i = 0; i < 8; i++) {
        this->lcd->createChar(i, customChars[i]);
    }
}

void LcdBigNumbers::printNumber(const unsigned long int number, uint8_t column, uint8_t row)
{
    uint8_t digits[10];  // Hard limit de 10 dígitos
    uint8_t count = 0, i;
    unsigned long int moreNumbers = number;

    do {
        digits[count++] = moreNumbers % 10;
        moreNumbers /= 10;
    } while (count <= 10 && moreNumbers > 0);

    for (i = 0; i < count; i++) {
        this->printDigit(digits[count - i - 1], column + 4 * i, row);
    }
}

void LcdBigNumbers::printDigit(const uint8_t digit, uint8_t column, uint8_t row)
{
    if (digit > 9) {
        return;
    }

    uint8_t r, c;

    for (r = 0; r < 3; r++) {
        this->lcd->setCursor(column, row + r);

        for (c = 0; c < 3; c++) {
            this->lcd->write(numberChars[digit][r][c]);
        }
    }
}
