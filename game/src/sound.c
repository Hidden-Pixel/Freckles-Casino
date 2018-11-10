/*
 *  sound.c
 *
 */

#include <FC/sound.h>
#include <FC/defines.h>

#define SOUND_BUFFER_MAX 10

internal unsigned int SoundBufferSize = 0;
internal Music *SoundBuffer[SOUND_BUFFER_MAX];

SoundMeta
CreateSoundMeta(Music *sound)
{
    SoundMeta s = (SoundMeta)
    {
        .totalPlays         = 0,
        .playLimit          = 0,
        .entirePlayLength   = GetMusicTimeLength(*sound),
        .lastPlayTime       = 0,
    };
    return s;
}

void
AddSoundToBuffer(Music *sound)
{
    // TODO(nick): handle overflow of add request by removing first
    if (len(SoundBuffer) >= SOUND_BUFFER_MAX)
    {
        RemoveSoundFromBuffer();
    }
    SoundBuffer[SoundBufferSize] = sound;
    SoundBufferSize++;
}

void
RemoveSoundFromBuffer()
{
    // TODO(nick):
}

void
PlaySounds()
{
    // TODO(nick): add meta data checking here ...
    for (int i = 0; i < len(SoundBuffer); i++)
    {
        if (SoundBuffer[i] != NULL)
        {
            PlayMusicStream(*SoundBuffer[i]);
        }
    }
}

void
UpdateSounds()
{
    // TODO(nick): add meta data checking here ...
    for (int i = 0; i < len(SoundBuffer); i++)
    {
        if (SoundBuffer[i] != NULL)
        {
            UpdateMusicStream(*SoundBuffer[i]);
        }
    }
}
