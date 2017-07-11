/*============================================================================*/
/**  @file       sdl_audio.cpp
 **  @ingroup    sdl2ui
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
 ** Copyright (C) 2011, 2014, 2015
 ** Houkes Horeca Applications
 **
 ** This file is part of the SDL2UI Library.  This library is free
 ** software; you can redistribute it and/or modify it under the
 ** terms of the GNU General Public License as published by the
 ** Free Software Foundation; either version 3, or (at your option)
 ** any later version.

 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.

 ** Under Section 7 of GPL version 3, you are granted additional
 ** permissions described in the GCC Runtime Library Exception, version
 ** 3.1, as published by the Free Software Foundation.

 ** You should have received a copy of the GNU General Public License and
 ** a copy of the GCC Runtime Library Exception along with this program;
 ** see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 ** <http://www.gnu.org/licenses/>
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/
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

void Caudio::play( const std::string &wavFile)
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
