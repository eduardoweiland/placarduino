# Placarduino

Projeto de mini placar eletrônico com Arduino.

Esse repositório contém o sketch do [Arduino](https://www.arduino.cc) e o esquemático de ligações na protoboard criado
com o [Fritzing](https://fritzing.org).

Veja as postagens no meu blog sobre esse projeto: https://ew.dev.br/tags/placarduino/.

## Compilando

Esse projeto utiliza o [PlatformIO](https://platformio.org/). Com apenas um comando, todas as bibliotecas necessárias
são baixadas, o projeto é compilado e o firmware é enviado para o microcontrolador (necessário ter um Arduino UNO
conectado no computador):

```sh
platformio run --target upload
```
