/*
 *  sound.h
 *
 */

#include "raylib.h"

typedef struct _soundMeta
{
    unsigned int    totalPlays;
    unsigned int    playLimit;
    float           entirePlayLength;
    float           lastPlayTime;
} SoundMeta;

void
InitializeSoundMeta(Music *soundArray, SoundMeta *soundMetaArray, int length);

SoundMeta
CreateSoundMeta(Music *sound);

void
AddSoundToBuffer(Music *Sound);

void
RemoveSoundFromBuffer();

void
PlaySounds();

void
UpdateSounds();
