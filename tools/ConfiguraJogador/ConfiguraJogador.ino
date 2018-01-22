#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9
#define SS_PIN         10

MFRC522 mfrc522(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key = {
    .keyByte = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
};

void setup()
{
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
}

void loop()
{
    char playerName[16];
    bool cardAvailable = false;
    unsigned long cardTimeout;

    Serial.println(F("Cadastrando novo cartão de jogador"));

    if (!readPlayerName(playerName, sizeof(playerName))) {
        Serial.println(F("Nenhum dado recebido em 30 segundos, cancelando..."));
    }
    else {
        Serial.print(F("Nome do jogador: "));
        Serial.println(playerName);
    
        Serial.println("Aproxime o cartão do leitor");

        cardTimeout = millis() + 30000L;
        while (!cardAvailable && (millis() < cardTimeout)) {
            cardAvailable = isCardAvailable();
        }
        
        if (cardAvailable && writeOnCard(playerName, sizeof(playerName))) {
            Serial.println("Nome do jogador foi salvo com sucesso!");
        }
        else {
            Serial.println("ERRO: Não foi possível salvar o nome do jogador!");
        }
    }

    Serial.println("Finalizado!");
    Serial.println("---------------------------------");
}

bool readPlayerName(char *outputBuffer, size_t bufferSize)
{
    byte length;

    // Limpa o buffer setando todos os bytes para 0
    memset(outputBuffer, 0, bufferSize);

    Serial.println(F("Digite o nome do jogador e adicione # no final: "));

    // Aguarda até 30 segundos para o nome ser digitado
    Serial.setTimeout(30000L);
    length = Serial.readBytesUntil('#', outputBuffer, bufferSize);

    return (length > 0);
}

// Funções utilitárias para acessar leitor RFID

bool writeOnCard(char *playerName, size_t bufferSize)
{    
    byte block = 1;
    bool success = false;

    if (!authenticate(block)) {
        return false;
    }

    if (writeBlock(block, (byte*) playerName, (byte) bufferSize)) {
        success = true;
    }

    deauthenticate();

    return success;
}

bool isCardAvailable()
{
    // Verifica se existe um cartão na frente do leitor
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return false;
    }

    // Verifica se é possível ler o UID do cartão
    return mfrc522.PICC_ReadCardSerial();
}

bool writeBlock(byte blockAddr, byte *buffer, byte bufferSize)
{
    MFRC522::StatusCode status;

    status = mfrc522.MIFARE_Write(blockAddr, buffer, bufferSize);

    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));

        return false;
    }

    return true;
}

bool authenticate(byte blockAddr)
{
    MFRC522::StatusCode status;

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &key, &(mfrc522.uid));

    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));

        return false;
    }

    return true;
}

void deauthenticate()
{
    MFRC522::StatusCode status;

    status = mfrc522.PICC_HaltA();

    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PICC_HaltA failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }

    mfrc522.PCD_StopCrypto1();
}


