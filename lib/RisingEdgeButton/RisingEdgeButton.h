#ifndef PLACARDUINO_RISING_EDGE_BUTTON_H
#define PLACARDUINO_RISING_EDGE_BUTTON_H

#include <Arduino.h>

class RisingEdgeButton
{
public:
    RisingEdgeButton();
    RisingEdgeButton(const int pin);

    void setPin(const int pin);
    bool pressed();

private:
    int pin;
    int state = LOW;

    bool read();
};

#endif // PLACARDUINO_RISING_EDGE_BUTTON_H