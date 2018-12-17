/*
 *  sound.c
 *
 */

#include <stddef.h>
#include <FC/sound.h>
#include <FC/defines.h>

#define SOUND_BUFFER_MAX 10

internal unsigned int SoundBufferSize = 0;
internal Music *SoundBuffer[SOUND_BUFFER_MAX];
internal SoundMeta *SoundMetaBuffer[SOUND_BUFFER_MAX];

void
InitializeSoundMeta(Music *soundArray, SoundMeta *soundMetaArray, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (soundArray[i] != NULL)
        {
            soundMetaArray[i] = CreateSoundMeta(&soundArray[i]);
        }
    }
}

SoundMeta
CreateSoundMeta(Music *sound)
{
    SoundMeta s = (SoundMeta)
    {
        .totalPlays             = 0,
        .playLimit              = 0,
        .entirePlayLength       = GetMusicTimeLength(*sound),
        .lastPlayTime           = 0.0f,
    };
    return s;
}

void
UpdateSoundMeta(Music *sound, SoundMeta *soundMeta)
{
    float currentPlayTime = GetMusicTimePlayed(*sound);
    float deltaPlayTime = currentPlayTime - soundMeta->lastPlayTime;
    if (deltaPlayTime < 0.0f)
    {
        soundMeta->totalPlays++;
    }
    soundMeta->lastPlayTime = currentPlayTime;
}

// TODO(nick): handle overflow of add request by removing
void
AddSoundToBuffer(Music *sound, SoundMeta *soundMeta)
{
    SoundBuffer[SoundBufferSize] = sound;
    SoundMetaBuffer[SoundBufferSize] = soundMeta;
    SoundBufferSize++;
}

void
RemoveSoundFromBufferByIndex(int index)
{
    if (index <= SOUND_BUFFER_MAX)
    {
        SoundBuffer[index] = NULL;
        SoundMetaBuffer[index] = NULL;
    }
}

void
PlaySounds()
{
    for (int i = 0; i < len(SoundBuffer); i++)
    {
        if (SoundBuffer[i] != NULL)
        {
            PlayMusicStream(*SoundBuffer[i]);
            UpdateSoundMeta(SoundBuffer[i], SoundMetaBuffer[i]);
        }
    }
}

void
UpdateSounds()
{
    for (int i = 0; i < len(SoundBuffer); i++)
    {
        if (SoundBuffer[i] != NULL)
        {
            UpdateMusicStream(*SoundBuffer[i]);
            UpdateSoundMeta(SoundBuffer[i], SoundMetaBuffer[i]);
            if ((SoundMetaBuffer[i]->playLimit <= SoundMetaBuffer[i]->totalPlays) &&
                (SoundMetaBuffer[i]->playLimit != INFINITE_PLAY) ||
                (SoundMetaBuffer[i]->playLimit == KILL_IMMEDIATELY))
            {
                RemoveSoundFromBufferByIndex(i);
            }
        }
    }
}
