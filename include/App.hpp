#pragma once

#include <Arduino.h>

class App
{
public:
    virtual void init() = 0;
    virtual void run() = 0;
};
