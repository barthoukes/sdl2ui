/*============================================================================*/
/**  @file       sdl_audio.h
 **  @ingroup    user_interface
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

#ifndef AUDIO_H_
#define AUDIO_H_

#include <string>
#include "SDL.h"
#include "SDL_mixer.h"
#include "singleton.h"

class Caudio : public Tsingleton<Caudio>
{
friend class Tsingleton<Caudio>;

public:
	Caudio();
	virtual ~Caudio();
	void play( const std::string &wavFile);
	void click() { if ( m_click.size()) play( m_click.c_str()); }
	void setClick( const std::string &click) { m_click=click; }
	void setAudioPath( const std::string &path) { m_path=path; }

	bool m_ready;
	SDL_AudioSpec wav_spec;
	Uint32 wav_length;
	Mix_Chunk *sound;
	int channel;
	std::string m_click;
	std::string m_path;
};

#define ERROR_SOUND()  		Caudio::Instance()->play( AUDIO_ERROR)

#endif /* AUDIO_H_ */
