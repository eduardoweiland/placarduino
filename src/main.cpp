#include "config.h"
#include "App.hpp"
#include "Placarduino.hpp"
#include "PlayerCardCreator.hpp"

Placarduino placarduino;
PlayerCardCreator cardCreator;
App *app = &placarduino;

void setup()
{
    if (digitalRead(PIN_BTN_FRONT) == HIGH) {
        app = &cardCreator;
    }

    app->init();
}

void loop()
{
    app->run();
}
