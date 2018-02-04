#include <Arduino.h>
#include <Tone.h>

#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(Tone *tone)
{
    this->buzzer = tone;
}

void MusicPlayer::playNote(uint16_t frequency, uint32_t duration)
{
    this->buzzer->play(frequency, duration * 0.85);
    delay(duration);
}

void MusicPlayer::pause(uint32_t duration)
{
    delay(duration);
}

void MusicPlayer::cMajorScale()
{
    int i;
    int notes[] = { NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6, NOTE_C7 };

    for (i = 0; i < 8; ++i) {
        this->playNote(notes[i], 35 + (i * 5));
    }
}

void MusicPlayer::superMarioTheme()
{
    this->playNote(NOTE_E6, 140);
    this->playNote(NOTE_E6, 140);
    this->pause(140);
    this->playNote(NOTE_E6, 140);
    this->pause(140);
    this->playNote(NOTE_C6, 140);
    this->playNote(NOTE_E6, 260);
    this->playNote(NOTE_G6, 260);
    this->pause(260);
    this->playNote(NOTE_G5, 260);
}

void MusicPlayer::sweetChildOMine()
{
    int i, j;
    int firstNote[] = { NOTE_CS5, NOTE_DS5, NOTE_FS5 };

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 2; ++j) {
            this->playNote(firstNote[i], 260);
            this->playNote(NOTE_CS6, 260);
            this->playNote(NOTE_GS5, 260);
            this->playNote(NOTE_FS5, 260);
            this->playNote(NOTE_FS6, 260);
            this->playNote(NOTE_GS5, 260);
            this->playNote(NOTE_F6, 260);
            this->playNote(NOTE_GS5, 260);
        }
    }

    this->playNote(NOTE_CS5, 260);
}

void MusicPlayer::victory()
{
    this->playNote(NOTE_C6, 140);
    this->playNote(NOTE_D6, 140);
    this->playNote(NOTE_E6, 140);
    this->playNote(NOTE_G6, 260);
    this->pause(140);
    this->playNote(NOTE_E6, 140);
    this->playNote(NOTE_G6, 550);
}
