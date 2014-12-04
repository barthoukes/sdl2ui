/*============================================================================*/
/**  @file       sdl_audio.cpp
 **  @ingroup    zhongcan_sdl
 **  @brief		 Low level audio
 **
 **  Create sound without stopping the CPU.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Caudio
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 24 aug 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

#include "sdl_audio.h"

/** @brief Constructor audio */
Caudio::Caudio()
: m_ready(false)
, wav_length(0)
, sound(NULL)
, channel(0)
{
	wav_spec.size=0;

	channel =-1;
	sound =NULL;
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;
    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
    {
        return;
    }
}

/** @brief Destructor audio */
Caudio::~Caudio()
{
	if ( channel !=-1)
	{
		Mix_HaltChannel(channel);
		Mix_FreeChunk(sound);
		Mix_CloseAudio();
		sound =NULL;
		channel =-1;
	}
    Mix_CloseAudio();
}

void Caudio::play( const char *wavFile)
{
	std::string wav =m_path;
	wav +=wavFile;
	if ( channel!=-1)
	{
		Mix_HaltChannel(channel);
		Mix_FreeChunk(sound);

		sound =NULL;
		channel =-1;
	}

	sound = Mix_LoadWAV(wav.c_str());
    if( sound == NULL)
    {
		return;
    }
    channel = Mix_PlayChannel(-1, sound, 0);
    if(channel == -1)
    {
		Mix_FreeChunk(sound);
		sound =NULL;
		return;
    }
}
