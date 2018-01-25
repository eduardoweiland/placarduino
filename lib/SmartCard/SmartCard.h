#ifndef PLACARDUINO_SMART_CARD_H
#define PLACARDUINO_SMART_CARD_H

#include <Arduino.h>
#include <MFRC522.h>

class SmartCard
{
public:
    SmartCard(const byte *key, const int chipSelectPin = SS, const int resetPin = UINT8_MAX);

    void begin();
    bool isCardAvailable();

    bool read(const uint8_t blockAddr, uint8_t *buffer, uint8_t *bufferSize);
    bool write(const uint8_t blockAddr, const uint8_t *buffer, const uint8_t bufferSize);

protected:
    MFRC522 mfrc522;

    bool authenticate(const byte blockAddr);
    void deauthenticate();

private:
    MFRC522::MIFARE_Key key;

    int chipSelectPin;
    int resetPin;

    bool readBlock(const uint8_t blockAddr, uint8_t *buffer, uint8_t *bufferSize);
    bool writeBlock(const uint8_t blockAddr, const uint8_t *buffer, const uint8_t bufferSize);
};

#endif // PLACARDUINO_SMART_CARD_H
