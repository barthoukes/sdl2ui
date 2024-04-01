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
#include <map>
#include <deque>
#include <vector>
#include <string>
#include <memory>

#include "sdl_rect.h"
#include "sdl_types.h"
#include "sdl_touch.h"
#include "taal_type.h"

#define SOLID_FILL 		1
#define INTERLEAVE_FILL 2

class Cdialog;
class SDL_Surface;

#define gMin(a,b)    	(((a) < (b)) ? (a) : (b))
#define gMax(a,b)    	(((a) > (b)) ? (a) : (b))
#define gMax3(a,b,c) 	(((a) > (b)) ? (gMax(a,c)) : (gMax(b,c)))
#define gMin3(a,b,c) 	(((a) < (b)) ? (gMin(a,c)) : (gMin(b,c)))
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

typedef struct
{
	int width;	// in pixels.
	int height;	// in pixels.
	int width_allocated;
	int height_allocated;
	Eresize resize_font;
	colour text_button_text;
	colour text_button_background1;
	colour text_button_background2;
	colour text_button_border;
	colour button_text;
	colour button_greyed;
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
	std::string icon_language48;
	std::string icon_printer48;
	std::string icon_slider48;

	// settings
	Taal country;
	int drag_start_pixels;
	int repeat_speed;
	int repeat_delay;
	int minimum_drag_time;
	int minimum_click_time;
	std::string font_path;
	std::string image_path;
    std::string image_path_mono;
	std::string data_path;
	int full_screen;
	bool handwriting_detection_enabled;
	bool enable_record_keyfile;
	bool show_labels;
	bool use_mono_images;
	int messagebox_time;
	std::string audio_popup;
	int swype_friction;
	int button_height;

	int touch_debounce_time;
	int touch_debounce_distance;
	int debug_coordinates;
	int touch_debounce_long_time;

    // language settings
    bool use_indonesian;
    bool use_simplified;
    bool use_traditional;
    bool use_dutch;
    bool use_english;
    bool use_german;

	// functions
	get_test_event_func get_test_event;
	get_font_func get_font;
	find_text_id_func find_text_id;
	find_colour_func find_colour;
	start_from_main_func start_from_main;
	log_func log;
	external_loop_func external_loop; /// External main loop over all dialogs
} SdlDefaults;

/// @brief Graphics routines for SDL layer.
class Cgraphics
{
public:
	Cgraphics( const Csize &size, bool mainScreen);
	virtual ~Cgraphics();
	bool init( const std::string &caption ="", int pixels=32, int colour=-1 );
	void close();
	void settings( SdlDefaults *settings);
	void setPixel(int x, int y);
	int pixel( int x, int y);
	void darkenPixel(int x, int y, double part);
	void bar( int x1, int y1, int x2, int y2, int radius=0);
	void bar( const Crect &rect, colour color, int radius=0);
	void setPixelOffset( int x, int y );
	void cleanBigSurfaces();
	void setVisible(bool visible);
	void line( int x1, int y1, int x2, int y2);
	void imageLine( const std::string &image, int x1, int y1, int x2, int y2);
	void lock();
	void unlock();
	void lock_keycodes() { m_lock_keycode =true; }
	void unlock_keycodes() { m_lock_keycode =false; }
	void update();
	bool front();
	bool pop_back();
	bool push_back();
	void setKey( const Crect &rect, keybutton key_code);
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
	bool imageWithBackgroundColour( const std::string &fname, int x1, int y1, int x2, int y2);
	bool imageWithColourChart( const std::string &fname, int x1, int y1, int x2, int y2);

	bool surface( SDL_Surface *surface, int x1, int y1);
	bool surface( SDL_Surface *surface, int x, int y, int w, int h);
	bool copySurface( SDL_Surface *surface, const Crect &destination, const Crect &source);
	bool copy( Cgraphics *graphics, const Crect &destination, const Crect &surface);
	void linerel(int dx, int dy);
	void lineto(int x, int y);
	void setColour(colour col);
	static colour brighter( colour col, int percentage);
	void rectangle(int left, int top, int right, int bottom, int edge=1, int radius=0);
	void ellipse(int xc, int yc, int stangle, int endangle,
			 int rx, int ry);
	void drawArc(int xc, int yc, int start_corner, int end_corner, int radius1, int radius2);
	bool isMainScreen() { return m_mainScreen; }
	int height() { return m_size.height(); }
	int width() { return m_size.width(); }
	void clean();
	bool setRenderArea( SDL_Surface *texture );
	int getOffsetX() { return m_pixelOffset.x; }
	SDL_Surface *findImage( const std::string &fname, bool isMonoIcon);
	void update( int x1, int y1, int w, int h);
	Ctouch m_touch;	///< Touch control.
	void freeImage( const std::string &image);
	int bitsPerPixel() { return m_renderSurface ? m_renderSurface->format->BitsPerPixel:0; }

protected:
	void mapword(int x1, int x2, int y);
	void mapvword(int x, int y1, int y2);

private:
#ifdef USE_SDL2
	SDL_Window	*m_window;
	SDL_Renderer *m_renderer;
#endif
	std::vector<SDL_Surface*> m_bitmap; ///< Bitmaps to paint on.
	SDL_Surface *m_allocatedSurface; ///< Surface to paint on.
	SDL_Surface *m_renderSurface; ///< Where to render to.
	std::deque<Uint32*> m_background; ///< Backgrounds to use.
	std::map<std::string, SDL_Surface*> m_imageMap; ///< Images to use.
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
	bool m_disableUpdate;

public:
	static SdlDefaults m_defaults; ///< All defaults for SDL
};

/// @brief Main graphics layer.
extern Cgraphics *m_mainGraph;

