#ifndef PLACARDUINO_PLAYER_CARD_H
#define PLACARDUINO_PLAYER_CARD_H

#include <Arduino.h>

#include "SmartCard.h"
#include "PlayerControl.h"

class PlayerCard
{
public:
    PlayerCard(SmartCard *smartCard, const byte blockNumber);

    bool readPlayerNameFromCard(PlayerControl *player);

private:
    SmartCard *smartCard;
    byte blockNumber;

    bool isValidPlayerName(const byte *name, const size_t length) const;
};

#endif // PLACARDUINO_PLAYER_CARD_H
