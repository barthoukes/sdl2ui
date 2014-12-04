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
 **  Copyright (c) Bart Houkes, 11 jan 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <SDL/SDL_image.h>
#include "singleton.h"
#include "sdl_types.h"
#include "sdl_rect.h"
#include "sdl_touch.h"

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
		              int &chinesePixels);
typedef textId (*find_text_id_func)( const std::string &name);
typedef colour (*find_colour_func)( const std::string &colour_name);
typedef std::string (*get_translation_func)(int id, language country);

/// We keep a record of all images inside our program and release it at exit.
typedef struct
{
	SDL_Surface  *image;	///< Surface to paint.
	std::string	name;		///< Filename without path.
} SimageSurface;

typedef struct
{
	int width;	// in pixels.
	int height;	// in pixels.
	colour text_button_text;
	colour text_button_background1;
	colour text_button_background2;
	colour text_button_border;
	colour button_text;
	colour button_background1;
	colour button_background2;
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
	std::string image_button_background;
	language country;
	get_translation_func get_translation;
	next_language_func next_language;
	get_test_event_func get_test_event;
	get_font_func get_font;
	find_text_id_func find_text_id;
	find_colour_func find_colour;
	std::string full_screen_image_background;
	int drag_start_pixels;
	int repeat_speed;
	int repeat_delay;
	int minimum_drag_time;
	int minimum_click_time;
	colour item_text;
	std::string font_path;
	std::string image_path;
	std::string data_path;
	int full_screen;
	bool handwriting_detection_enabled;
	colour handwriting_background;
	bool enable_record_keyfile;
	bool show_labels;
	colour label_background;
	colour label_text;
	std::string icon_ok48;
	std::string icon_cancel48;
} Sdefaults;

/// @brief Graphics routines for SDL layer.
class Cgraphics
{
public:
	Cgraphics( const Csize &size, bool mainScreen);
	virtual ~Cgraphics();
	bool init( int pixels=32, int colour=-1);
	void close();
	void setPixel(int x, int y);
	int pixel( int x, int y);
	void bar( int x1, int y1, int x2, int y2, int radius=0);
	void line( int x1, int y1, int x2, int y2);
	void imageLine( const std::string &image, int x1, int y1, int x2, int y2);
	//void paint();
	void lock();
	void unlock();
	void lock_keycodes() { m_lock_keycode =true; }
	void unlock_keycodes() { m_lock_keycode =false; }
	void update();
	bool front();
	bool pop_back();
	bool push_back();
	void setCode( const Crect &rect, keybutton key_code);
	void darken( int x1, int y1, int x2, int y2);
	void darken( int x, int y);
	void cleardevice();
	//void addLayer(Ipainter *layer);
	//void removeLayer(Ipainter *layer);
	void moverel(int dx, int dy);
	void moveto(int x, int y);
	bool image( const std::string &fname, int x1, int y1, int x2, int y2);
	bool surface( SDL_Surface *surface, int x1, int y1);
	bool surface( SDL_Surface *surface, int x, int y, int w, int h);
	bool copy( Cgraphics *graphics, const Crect &destination, const Crect &surface);
	void linerel(int dx, int dy);
	void lineto(int x, int y);
	void setColour(colour col);
	static colour brighter( colour col, int percentage);
	void rectangle(int left, int top, int right, int bottom, int edge=1, int radius=0);
	void ellipse(int xc, int yc, int stangle, int endangle,
			 int rx, int ry);
	int height() { return m_size.height(); }
	int width() { return m_size.width(); }
	void clean();
	SDL_Surface *findImage( const std::string &fname);
	void update( int x1, int y1, int w, int h);
	Ctouch m_touch;	///< Touch control.
	void freeImage( const std::string &image);
	int bitsPerPixel() { return m_surface? m_surface->format->BitsPerPixel:0; }

protected:
	void mapword(int x1, int x2, int y);
	void mapvword(int x, int y1, int y2);

private:
	std::vector<SDL_Surface*> m_bitmap; ///< Bitmaps to paint on.
	SDL_Surface *m_surface; ///< Surface to paint on.
	std::deque<Uint32*> m_background; ///< Backgrounds to use.
	std::vector<SimageSurface> m_images; ///< Images to use.
	int m_colour; ///< Colour.
	int m_topLayer; ///< Top layer.
	int m_cx,m_cy;
	Uint8 m_R;
	Uint8 m_G;
	Uint8 m_B;
	Cpoint m_offset;
	Csize m_size;
	bool m_init;
	bool m_mainScreen;
	bool m_lock_keycode;
	int m_bits; ///< Bits per pixel.
	int m_option; ///< 0=window, 1=full, 2=invisible

public:
	static Sdefaults m_defaults; ///< All defaults for SDL
};

/// @brief Main graphics layer.
extern Cgraphics *m_mainGraph;

