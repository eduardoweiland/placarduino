#include <SPI.h>
#include <string.h>

#include "SmartCard.h"

SmartCard::SmartCard(const uint8_t *key, const int chipSelectPin, const int resetPin)
{
    memcpy(this->key.keyByte, key, MFRC522::MF_KEY_SIZE);
    this->chipSelectPin = chipSelectPin;
    this->resetPin = resetPin;
}

void SmartCard::begin()
{
    SPI.begin();
    this->mfrc522.PCD_Init(this->chipSelectPin, this->resetPin);
}

bool SmartCard::isCardAvailable()
{
    // Verifica se existe um cartão na frente do leitor
    if (!this->mfrc522.PICC_IsNewCardPresent()) {
        return false;
    }

    // Verifica se é possível ler o UID do cartão
    return this->mfrc522.PICC_ReadCardSerial();
}

bool SmartCard::read(const uint8_t blockAddr, uint8_t *buffer, uint8_t *bufferSize)
{
    if (!this->isCardAvailable()) {
        return false;
    }

    if (!this->authenticate(blockAddr)) {
        return false;
    }

    bool success = this->readBlock(blockAddr, buffer, bufferSize);

    this->deauthenticate();

    return success;
}

bool SmartCard::readBlock(const uint8_t blockAddr, uint8_t *buffer, uint8_t *bufferSize)
{
    MFRC522::StatusCode status;

    status = this->mfrc522.MIFARE_Read(blockAddr, buffer, bufferSize);

    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read failed: "));
        Serial.println(this->mfrc522.GetStatusCodeName(status));

        return false;
    }

    // Limpa CRC adicionado pela biblioteca no buffer, não é mais necessário
    // (CRC já é validado dentro do MIFARE_Read, que retorna MFRC522::STATUS_CRC_WRONG em caso de erro)
    buffer[16] = '\0';
    buffer[17] = '\0';

    return true;
}

bool SmartCard::write(const uint8_t blockAddr, const uint8_t *buffer, const uint8_t bufferSize)
{
    if (!this->isCardAvailable()) {
        return false;
    }

    if (!this->authenticate(blockAddr)) {
        return false;
    }

    bool success = this->writeBlock(blockAddr, buffer, bufferSize);

    this->deauthenticate();

    return success;
}

bool SmartCard::writeBlock(const uint8_t blockAddr, const uint8_t *buffer, const uint8_t bufferSize)
{
    MFRC522::StatusCode status;

    status = this->mfrc522.MIFARE_Write(blockAddr, (uint8_t *)buffer, (uint8_t)bufferSize);

    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write failed: "));
        Serial.println(this->mfrc522.GetStatusCodeName(status));

        return false;
    }

    return true;
}

bool SmartCard::authenticate(const uint8_t blockAddr)
{
    MFRC522::StatusCode status;

    status = this->mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &(this->key), &(this->mfrc522.uid));

    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate failed: "));
        Serial.println(this->mfrc522.GetStatusCodeName(status));

        return false;
    }

    return true;
}

void SmartCard::deauthenticate()
{
    MFRC522::StatusCode status;

    status = this->mfrc522.PICC_HaltA();

    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PICC_HaltA failed: "));
        Serial.println(this->mfrc522.GetStatusCodeName(status));
    }

    this->mfrc522.PCD_StopCrypto1();
}
