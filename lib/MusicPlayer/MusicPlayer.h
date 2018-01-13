#ifndef PLACARDUINO_MUSIC_PLAYER_H
#define PLACARDUINO_MUSIC_PLAYER_H

#include <Tone.h>

class MusicPlayer
{
public:
    MusicPlayer(Tone *buzzer);

    void cMajorScale();
    void superMarioTheme();
    void sweetChildOMine();

    void playNote(uint16_t frequency, uint32_t duration);
    void pause(uint32_t duration);

private:
    Tone *buzzer;
};

#endif // PLACARDUINO_MUSIC_PLAYER_H
