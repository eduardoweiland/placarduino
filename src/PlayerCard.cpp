#include <string.h>

#include "PlayerCard.h"

PlayerCard::PlayerCard(SmartCard *smartCard, const byte blockNumber)
{
    this->smartCard = smartCard;
    this->blockNumber = blockNumber;
}

bool PlayerCard::readPlayerNameFromCard(PlayerControl *player)
{
    byte buffer[18];
    byte length = sizeof(buffer);

    if (!this->smartCard->read(this->blockNumber, buffer, &length)) {
        return false;
    }

    Serial.print(F("Dados lidos do cartão: "));
    Serial.println((char*)buffer);

    if (!this->isValidPlayerName(buffer, strnlen((const char*)buffer, length))) {
        Serial.println(F("Nome contém caracteres inválidos, ignorando"));
        return false;
    }

    player->setName((const char*)buffer);
    return true;
}

bool PlayerCard::isValidPlayerName(const byte *name, const size_t length) const
{
    size_t i;

    for (i = 0; i < length; ++i) {
        if (!isAlphaNumeric(name[i])) {
            return false;
        }
    }

    return true;
}
