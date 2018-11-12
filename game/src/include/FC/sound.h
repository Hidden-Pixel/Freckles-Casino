/*
 *  sound.h
 *
 */

#include "raylib.h"

#define INFINITE_PLAY -1
#define KILL_IMMEDIATELY -2

typedef struct _soundMeta
{
    unsigned int    totalPlays;
    int             playLimit;
    float           entirePlayLength;
    float           lastPlayTime;
} SoundMeta;

void
InitializeSoundMeta(Music *soundArray, SoundMeta *soundMetaArray, int length);

SoundMeta
CreateSoundMeta(Music *sound);

void
AddSoundToBuffer(Music *sound, SoundMeta *soundMeta);

void
RemoveSoundFromBufferByIndex(int index);

void
PlaySounds();

void
UpdateSounds();
