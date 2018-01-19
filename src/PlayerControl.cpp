#include <string.h>

#include "PlayerControl.h"

PlayerControl::PlayerControl(const int addScorePin, const int subScorePin)
{
    this->addScoreButton.setPin(addScorePin);
    this->subScoreButton.setPin(subScorePin);

    memset(this->name, 0, sizeof(this->name));
}

void PlayerControl::setName(const char *name)
{
    strncpy(this->name, name, PlayerControl::MAX_NAME_LENGTH);
}

const char* PlayerControl::getName() const
{
    return this->name;
}

const int PlayerControl::getScore() const
{
    return this->score;
}

const int PlayerControl::updateScore()
{
    int change = 0;

    if (this->addScoreButton.pressed()) {
        change = +1;
    }
    else if (this->score > 0 && this->subScoreButton.pressed()) {
        change = -1;
    }

    this->score += change;

    return change;
}
