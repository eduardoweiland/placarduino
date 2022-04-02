#include "App.hpp"
#include "Placarduino.hpp"

Placarduino placarduino;

void setup()
{
    placarduino.init();
}

void loop()
{
    placarduino.run();
}
