#include "RisingEdgeButton.h"

RisingEdgeButton::RisingEdgeButton()
{
}

RisingEdgeButton::RisingEdgeButton(const int pin)
{
    this->setPin(pin);
}

void RisingEdgeButton::setPin(const int pin)
{
    this->pin = pin;
}

bool RisingEdgeButton::pressed()
{
    return this->read() && this->state == HIGH;
}

bool RisingEdgeButton::read()
{
    int newState = digitalRead(this->pin);
    bool changed = false;

    if (newState != this->state) {
        this->state = newState;
        changed = true;
    }

    return changed;
}