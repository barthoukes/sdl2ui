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

#pragma once

#include <string>
#ifdef USE_SDL
#include "SDL.h"
#include "SDL_mixer.h"

class Caudio
{
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

extern Caudio    *m_mainAudio;

#define AUDIO_ERROR			"error.wav"
#define AUDIO_BILL			"bill.wav"
#define AUDIO_CLICK			"click.wav"
#define AUDIO_GONG			"gong.wav"
#define AUDIO_POPUP			"popup.wav"
#define AUDIO_ERROR			"error.wav"
#define AUDIO_ERASER		"eraser.wav"
#define AUDIO_MOVING		"moving.wav"
#define AUDIO_KEY			"sonar.wav"
#define AUDIO_DESTROY		"slide_down.wav"

#define ERROR_SOUND()  		if (m_mainAudio) m_mainAudio->play( AUDIO_ERROR)
#define CREATE_SOUND(x)     if (m_mainAudio) m_mainAudio->play( x)
#define CLICK_SOUND()       if (m_mainAudio) m_mainAudio->click()
#else
#define ERROR_SOUND()
#define CREATE_SOUND(x)
#define CLICK_SOUND()
#endif

/* AUDIO_H_ */
