/*
 *  sound.c
 *
 */

#include <FC/sound.h>
#include <FC/defines.h>

internal unsigned int SoundBufferSize = 0;
internal Music *SoundBuffer[10];

void
AddSoundToBuffer(Music *sound)
{
    // TODO(nick): handle overflow of add request by removing first
    SoundBuffer[SoundBufferSize] = sound;
    SoundBufferSize++;
}

void
RemoveSoundFromBuffer()
{
    // TODO(nick):
    // NOTE: for now, always remove from the front
}

void
PlaySounds()
{
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
    for (int i = 0; i < len(SoundBuffer); i++)
    {
        if (SoundBuffer[i] != NULL)
        {
            UpdateMusicStream(*SoundBuffer[i]);
        }
    }
}
