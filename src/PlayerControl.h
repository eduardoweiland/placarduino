#ifndef PLACARDUINO_PLAYER_CONTROL_H
#define PLACARDUINO_PLAYER_CONTROL_H

#include <Arduino.h>

#include "RisingEdgeButton.h"

class PlayerControl
{
public:
    PlayerControl(const int addScorePin, const int subScorePin);

    static const size_t MAX_NAME_LENGTH = 10;

    void setName(const char *name);
    const char* getName() const;

    const int getScore() const;

    const int updateScore();

private:
    char name[PlayerControl::MAX_NAME_LENGTH + 1];
    int score = 0;

    // Buttons
    RisingEdgeButton addScoreButton;
    RisingEdgeButton subScoreButton;
};

#endif // PLACARDUINO_PLAYER_CONTROL_H