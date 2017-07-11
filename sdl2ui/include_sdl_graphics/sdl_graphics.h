/*============================================================================*/
/**  @file       graphics_sdl.h
 **  @ingroup    user_interface
 **  @brief		 Low level graph
 **
 **  Init the SDL library, draw lines, circles and characters in colour.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cgraphics
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

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <SDL_image.h>
#include <SDL_video.h>
#ifdef USE_SDL2
#include <SDL_surface.h>
#include <SDL_render.h>
#endif
#include "sdl_types.h"
#include "sdl_rect.h"
#include "sdl_touch.h"

#ifdef USE_SDL2
typedef SDL_Texture sdlTexture;
#else
typedef SDL_Surface sdlTexture;
#endif

/// All colours.
typedef int colour;

/// All languages.
typedef int language;

/// All identified text strings
typedef int textId;
#define INVALID_TEXT_ID -1

/// Colours by default.
enum Ecolours
{
    COLOUR_BLACK 		= 0x00000000,          /* dark colors */
    COLOUR_DARKGRAY     = 0x00404040,       /* light colors */
    COLOUR_MEDIUMGREY   = 0x00989898,
    COLOUR_GREYEDOUT    = 0x00c4c4c4,
    COLOUR_LIGHTGREY	= 0x00D3D3D3,
    COLOUR_WHITE		= 0x00EEEEEE,
    COLOUR_NONE			= 0x8FFFFFFF,
    COLOUR_DARKBLUE		= 0x000000C0,
    COLOUR_BLUE  		= 0x000000FF,
    COLOUR_LIGHTBLUE	= 0x00ADD8E6,
    COLOUR_VERY_LIGHTBLUE = 0x00CDECF3,

    COLOUR_GREEN 		= 0x00008000,
    COLOUR_DARKGREEN 	= 0x00002000,
    COLOUR_LIGHTGREEN		= 0x0020B2AA,
    COLOUR_VERY_LIGHTGREEN	= 0x0090DCD8,

    COLOUR_DARKCYAN		= 0x00006060,
    COLOUR_CYAN			= 0x0000F0F0,
    COLOUR_LIGHT_CYAN  	= 0x0060FFFF,
    COLOUR_VERY_LIGHTCYAN	= 0x00D0FFFF,

    COLOUR_BORDEAUX			= 0x0060000,
    COLOUR_RED				= 0x00E00000,
    COLOUR_LIGHTRED	    	= 0x00FF3030,
    COLOUR_VERY_LIGHTRED 	= 0x00FFC0C0,
    COLOUR_INDIAN_LIGHTRED 	= 0x00DE7272,
    COLOUR_INDIAN_RED   	= 0x00C86464,

    COLOUR_DARKMAGENTA	= 0x00600060,
    COLOUR_MAGENTA		= 0x00C000C0,
    COLOUR_LIGHTMAGENTA= 0x00FF30FF,
    COLOUR_VERY_LIGHTMAGENTA =0x00FFB0FF,

    COLOUR_DARKBROWN	= 0x00521515,
    COLOUR_BROWN		= 0x00A52A2A,
    COLOUR_LIGHTBROWN	= 0x00B54A4A,
    COLOUR_VERY_LIGHTBROWN	= 0x00C56A6A,

    COLOUR_DARKYELLOW   = 0x00A0A000,
    COLOUR_YELLOW		= 0x00E0E000,
    COLOUR_LIGHTYELLOW			= 0x00FFFF20,
    COLOUR_VERY_LIGHTYELLOW		= 0x00FFFF80
};

/// Colour sets for a different look.
enum EcolourMode
{
	MODE_ZUIHAO,
	MODE_BLACK_WHITE,
	MODE_MINT,
	MODE_3,
	MODE_4,
	MODE_5,
	MODE_6,
	MAX_COLOUR_MODE,
};

#define SOLID_FILL 		1
#define INTERLEAVE_FILL 2

class Cdialog;

#define gMin(a,b)    	(((a) < (b)) ? (a) : (b))
#define gMax(a,b)    	(((a) > (b)) ? (a) : (b))
#define gMax3(a,b,c) 	(((a) > (b)) ? (gMax(a,c)) : (gMax(b,c)))
#define gLimit(a,mn,mx) (((a) > (mn)) ? gMin(mx,a) : mn)
typedef language (*next_language_func)( language country);
typedef bool (*get_test_event_func)( SDL_Event &event);
typedef void (*get_font_func)(const std::string &index,
		              std::string &localName,
		              std::string &chineseName,
		              int &localPixels,
		              int &chinesePixels,
		              bool relative_to_screen);
typedef textId (*find_text_id_func)( const std::string &name);
typedef colour (*find_colour_func)( const std::string &colour_name);
typedef std::string (*get_translation_func)(int id, language country);
typedef void (*start_from_main_func)( Cdialog *me);
typedef int (*log_func)(const char *__restrict __format, ...);
typedef void (*external_loop_func)();

/// We keep a record of all images inside our program and release it at exit.
typedef struct
{
	sdlTexture *image;	///< Surface to paint.
	std::string	name;		///< Filename without path.
} SimageSurface;

typedef struct
{
	int width;	// in pixels.
	int height;	// in pixels.
	int width_allocated;
	int height_allocated;
	colour text_button_text;
	colour text_button_background1;
	colour text_button_background2;
	colour text_button_border;
	colour button_text;
	colour button_background1;
	colour button_background2;
	colour menu_button_text;
	colour menu_button_background1;
	colour menu_button_background2;
	colour button_shadow;
	colour line_dark;
	colour line_bright;
	colour image_button_text;
	colour function_button_text;
	colour function_button_background1;
	colour function_button_background2;
	colour header_text;
	colour header_background1;
	colour header_background2;
	colour matrix_header_text;
	colour matrix_header_background1;
	colour matrix_header_background2;
	colour graph_text;
	colour graph_background1;
	colour graph_background2;
	colour calculator_text;
	colour calculator_background1;
	colour calculator_background2;
	colour calculator_border;
	colour background;
	colour item_text;
	colour handwriting_background;
	colour label_background;
	colour label_text;
	colour messagebox_text;
	colour messagebox_background1;
	colour messagebox_background2;
	colour slider_lines;
	colour slider_background;

	// images
	std::string image_button_background;
	std::string full_screen_image_background;
	std::string icon_ok48;
	std::string icon_cancel48;
	std::string icon_printer48;
	std::string icon_slider48;

	// settings
	language country;
	int drag_start_pixels;
	int repeat_speed;
	int repeat_delay;
	int minimum_drag_time;
	int minimum_click_time;
	std::string font_path;
	std::string image_path;
	std::string data_path;
	int full_screen;
	bool handwriting_detection_enabled;
	bool enable_record_keyfile;
	bool show_labels;
	int messagebox_time;
	std::string audio_popup;
	int swype_friction;
	int button_height;

	int touch_debounce_time;
	int touch_debounce_distance;
	int debug_coordinates;
	int touch_debounce_long_time;

	// functions
	get_translation_func get_translation;
	next_language_func next_language;
	get_test_event_func get_test_event;
	get_font_func get_font;
	find_text_id_func find_text_id;
	find_colour_func find_colour;
	start_from_main_func start_from_main;
	log_func log;
	external_loop_func external_loop; /// External main loop over all dialogs
} Sdefaults;

/// @brief Graphics routines for SDL layer.
class Cgraphics
{
public:
	Cgraphics( const Csize &size, bool mainScreen);
	virtual ~Cgraphics();
	bool init( const std::string &caption ="", int pixels=32, int colour=-1 );
	void close();
	void settings( Sdefaults *settings);
	void setPixel(int x, int y);
	int pixel( int x, int y);
	void darkenPixel(int x, int y, double part);
	void bar( int x1, int y1, int x2, int y2, int radius=0);
	void bar( const Crect &rect, colour color, int radius=0);
	void setPixelOffset( int x, int y );

	void line( int x1, int y1, int x2, int y2);
	void imageLine( const std::string &image, int x1, int y1, int x2, int y2);
	//void paint();
	void lock();
	void unlock();
#ifdef USE_SDL2
	SDL_Renderer *getRenderer() { return m_renderer; }
	Csize textureSize( sdlTexture *texture);
	bool renderTexture( SDL_Texture *surface, int x1, int y1);
	bool renderTexture( SDL_Texture *surface, int x, int y, int w, int h);
	bool insertTexture( SDL_Texture *texture, int left, int top, int w, int h);
	bool renderGraphics( Cgraphics *graphics, const Crect &rect);
#endif
	bool renderSurface( SDL_Surface *surface, int x1, int y1);
	bool renderSurface( SDL_Surface *surface, int x, int y, int w, int h);
	void lock_keycodes() { m_lock_keycode =true; }
	void unlock_keycodes() { m_lock_keycode =false; }
	void update();
	bool front();
	bool pop_back();
	bool push_back();
	void setCode( const Crect &rect, keybutton key_code);
	void darken( int x1, int y1, int x2, int y2);
	void transparantPixel(int x, int y, double part);
	void setViewport( SDL_Rect *rect);

	void darken( int x, int y);
	void cleardevice();
	void moverel(int dx, int dy);
	void moveto(int x, int y);
	bool image( const std::string &fname, int x1, int y1, int x2, int y2);
	bool imageColour( const std::string &fname, int x1, int y1, int x2, int y2, colour col);
	bool imageInverse( const std::string &fname, int x1, int y1, int x2, int y2, colour col);
	void linerel(int dx, int dy);
	void lineto(int x, int y);
	void setColour(colour col);
	void setTransparant(double level);
	static colour brighter( colour col, int percentage);
	void rectangle(int left, int top, int right, int bottom, int edge=1, int radius=0);
	void ellipse(int xc, int yc, int stangle, int endangle,
			 int rx, int ry);
	int height() { return m_size.height(); }
	int width() { return m_size.width(); }
	void clean();
	bool setRenderArea( sdlTexture *texture );
	int getOffsetX() { return m_pixelOffset.x; }
	sdlTexture *findImage( const std::string &fname);
	void update( const Crect &rect);
	Ctouch m_touch;	///< Touch control.
	void freeImage( const std::string &image);
	int bitsPerPixel();
	void setRenderArea();

protected:
	void mapword(int x1, int x2, int y);
	void mapvword(int x, int y1, int y2);

private:
	std::vector<SDL_Surface*> m_bitmap; ///< Bitmaps to paint on.
#ifdef USE_SDL2
	SDL_Window   *m_window; ///< Video screen.
	SDL_Renderer *m_renderer; ///< Renders a window.
	SDL_Texture  *m_texture; ///< Where to paint on.
#else
	SDL_Surface *m_allocatedSurface; ///< Surface to paint on.
	SDL_Surface *m_renderSurface; ///< Where to render to.
#endif

	std::deque<Uint32*> m_background; ///< Backgrounds to use.
	std::vector<SimageSurface> m_images; ///< Images to use.
	int m_colour; ///< Colour.
	int m_topLayer; ///< Top layer.
	int m_cx,m_cy;
	Uint8 m_R;
	Uint8 m_A;
	Uint8 m_G;
	Uint8 m_B;
	Cpoint m_pixelOffset;
	Csize m_size;
	bool m_init;
	bool m_mainScreen;
	bool m_lock_keycode;
	int m_bits; ///< Bits per pixel.
	int m_option; ///< 0=window, 1=full, 2=invisible
	Uint32 *m_pixels;

public:
	static Sdefaults m_defaults; ///< All defaults for SDL
};

