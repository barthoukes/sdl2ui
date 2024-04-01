/*============================================================================*/
/**  @file       graphics_sdl.cpp
 **  @ingroup    sdl2ui
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

/*------------- Standard includes --------------------------------------------*/
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "sdl_graphics.h"
#include <SDL_image.h>
#include "SDL_ttf.h"

#define SWAP(A,B,TYPE) {TYPE temp=A; A=B; B=temp;}

/// Main layer on bottom, closed as last in the program.
Cgraphics *m_mainGraph =NULL;

/// Defaults graphics
SdlDefaults Cgraphics::m_defaults =
{
	800, // .width
	600, // .height
	800, // width alloc
	600, // height alloc
    RESIZE_ON, // auto resize
	0, // .text_button_text black
	0x00FFFFFF, // .text_button_background1
	0x00DDDDDD, // .text_button_background2
	0x000000, // .text_button_border
	0x000000, // .button_text =
	0x000000, // button_background1
	0x000000, // button_background2
	0x000000, // .menu_button_text =
	0x000000, // menu_button_background1
	0x000000, // menu_button_background2
	0x404040, // button_shadow
	0x000000, // line_dark
	0xeeeeee, // line_bright
	0xe0e000, // image_button_text
	0, // function_button_text
	0xffffff, // button_greyed
	0x803518, // function_button_background1
	0x903d1b, // function_button_background2
	0xeeeeee, // header_text
	0x500000, // header_background1
	0x700000, // header_background2
	0xffdd1c, // matrix_header_text
	0xc2b2e, // matrix_header_background1
	0x81a1c, // matrix_header_background2
	0, // graph_text
	0xfffae6, // graph_background1
	0xfffae6, // graph_background2
	0xe0e000, // calculator_text
	0xc86464, // calculator_background1
	0xe77676, // calculator_background2
	0x964646, // calculator_border
	0xd3d3d3, // background
	0xd3d3d3, // item_text
	0xf0f080, // handwriting_background
	0xe0e000, // label_background
	0xc86464, // label_text
	0xeeeeee, // messagebox_text
	0xe0e000, // messagebox_background1
	0xcf2020, // messagebox_background2
	0, // slider_lines
	0xfffae6, //.slider_background =
	"vloer14.png", // .image_button_background=
	"background01.png", // full_screen_image_background
	"enter48.png", // icon_ok48
	"cancel48.png", // ICON_CANCEL
	"languages48.png", // ICON_LANGUGE
	"printer48.png", // ICON_PRINTER
	"slider48.png", // ICON_SLIDER
	LANG_DUTCH, // country
	15, // drag_start_pixels
	75, // repeat_speed
	1000, // repeat_delay
	1000, // minimum_drag_time
	200, // minimum_click_time
	"/home/mensfort/zhongcan/font", // .font_path
	"/home/mensfort/zhongcan/buttons/", // .image_path
    "/home/mensfort/zhongcan/buttons_mono/", // .image_path
	"/home/mensfort/zhongcan/data", // .data_path
	1, // full_screen
	true, // handwriting_detection_enabled
	false, // enable_record_keyfile
	true, // show_labels
	false, // use_mono_images
	500, // messagebox_time
	"popup.wav", // audio_popup
	20, // swype_friction
	6, // button height
	5, // touch_debounce_time;
	8, // touch_debounce_distance
	0, // display coordinates
	3000, // debounce long press

    false, // use_indonesian
    false, // use_simplified
    false, // use_traditional
    true, // use_dutch
    false, // use_english
    false, // use_german

	NULL, // get_test_event
	NULL, // get_font
	NULL, // find_text_id
	NULL, //.find_colour
	NULL, //.start_from_main
	NULL, //.log_func
	NULL, //.external loop
};

/*----------------------------------------------------------------------------*/
/** @brief Constructor for graphics to paint on.
 *  @param width [in] Width of graphics.
 *  @param height [in] Height of graphics.
 *  @param mainScreen [in] boolean, true for the main screen.
 * */
Cgraphics::Cgraphics( const Csize &size, bool mainScreen):
	m_allocatedSurface(NULL),
	m_renderSurface(NULL),
    m_colour(0),
    m_topLayer(0),
	m_cx(0),
	m_cy(0),
	m_R(0),
	m_A(0),
	m_G(0),
	m_B(0),
	m_pixelOffset(0,0),
	m_size( size),
	m_init(false),
	m_mainScreen( mainScreen),
	m_lock_keycode(false),
	m_bits(32),
    m_option(0),
	m_pixels(NULL),
	m_disableUpdate(false)
{
	m_touch.setSize( Csize(size.width()/8, size.height()/8));
}

/*----------------------------------------------------------------------------*/
Cgraphics::~Cgraphics()
{
	clean();
	close();
}

/*----------------------------------------------------------------------------*/
void Cgraphics::close()
{
//#ifdef USE_SDL2
//	if ( m_texture)
//	{
//		SDL_DestroyTexture( m_texture);
//		m_texture =NULL;
//	}
//	if ( m_mainScreen)
//	{
//		SDL_Quit();
//	}
//#else
	if (m_allocatedSurface)
	{
		SDL_FreeSurface(m_allocatedSurface);
		if ( m_allocatedSurface ==m_renderSurface)
		{
			m_renderSurface = NULL;
		}
		m_allocatedSurface = NULL;
	}
	//else
	if ( m_mainScreen)
	{
		SDL_Quit();
	}
//#endif
}

/*----------------------------------------------------------------------------*/
/** @brief Initialise the graphics field.
 *  @param caption [in] What to show above the dialog
 *  @param pixels [in] Should be 32 for each pixel.
 *  @param colour [in] Background colour. -1 for default.
 */
bool Cgraphics::init( const std::string &caption, int pixels, int colour)
{
#ifdef USE_SDL2
	(void)pixels;
	(void)colour;

	m_option =Cgraphics::m_defaults.full_screen; // 0=window, 1=full, 2=invisible
	if ( colour ==-1)
	{
		colour =Cgraphics::m_defaults.background;
	}
	close();
	// To set up video in SDL, you need the SDL_SetVideoMode function.
	// This takes parameters specifying the kind of setup you want and returns
	// a pointer to the display surface. This is what you will draw onto to put
	// anything on the screen
    int options =SDL_SWSURFACE;

    if ( m_mainScreen)
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			return false;
		}

		switch ( m_option)
		{
		case 0:
			break;
		case 1:
			options =SDL_WINDOW_FULLSCREEN ;
			break;
		case 2:
			options =SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
			break;
		default:
			break;
	    }
		m_window = SDL_CreateWindow( caption.c_str(),
		                          SDL_WINDOWPOS_UNDEFINED,
		                          SDL_WINDOWPOS_UNDEFINED,
		                          m_size.width(), m_size.height(),
		                          options);
		m_renderer = SDL_CreateRenderer( m_window, -1, 0);

		m_renderSurface =NULL;
		m_pixels =NULL;
		//SDL_SetAlpha(m_renderSurface, SDL_SRCALPHA, 0);
		m_init =true;
	}
	else // Not main screen
	{
		//options =SDL_SWSURFACE|SDL_NOFRAME;
		options =SDL_SWSURFACE; //|SDL_NOFRAME;
		//options =SDL_SWSURFACE|SDL_NOFRAME;
	    Uint32 rmask, gmask, bmask, amask;

	    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
	       on the endianness (byte order) of the machine */
		#if 0 //SDL_BYTEORDER ==SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		#else
			bmask = 0x000000ff;
			gmask = 0x0000ff00;
			rmask = 0x00ff0000;
			amask = 0; //0xff000000;
		#endif

	    m_allocatedSurface =SDL_CreateRGBSurface( options, m_size.width(), m_size.height(),
	    		                         ::m_mainGraph->m_renderSurface->format->BitsPerPixel,
	                                     rmask, gmask, bmask, amask);
	    m_renderSurface =m_allocatedSurface;
	    m_pixels =(Uint32*)m_renderSurface->pixels;
		SDL_Rect rect;
		rect.h =(Uint16)(m_renderSurface->h);
		rect.w =(Uint16)(m_renderSurface->w);
		rect.x =0;
		rect.y =0;
		SDL_FillRect( m_renderSurface, &rect, colour);
		m_init =true;
	}
#else
	m_option =Cgraphics::m_defaults.full_screen; // 0=window, 1=full, 2=invisible
	if ( colour ==-1)
	{
		colour =Cgraphics::m_defaults.background;
	}
	close();
	// To set up video in SDL, you need the SDL_SetVideoMode function.
	// This takes parameters specifying the kind of setup you want and returns
	// a pointer to the display surface. This is what you will draw onto to put
	// anything on the screen
    int options =SDL_SWSURFACE;

    if ( m_mainScreen)
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			return false;
		}

		switch ( m_option)
		{
		case 0:
			break;
		case 1:
			options =SDL_SWSURFACE|SDL_FULLSCREEN;
			break;
		case 2:
			options =SDL_SWSURFACE;
			break;
		default:
			break;
	    }
		m_allocatedSurface = SDL_SetVideoMode(m_size.width(), m_size.height(), pixels, options);
		m_renderSurface =m_allocatedSurface;
		m_pixels =(Uint32*)m_allocatedSurface->pixels;
		//SDL_SetAlpha(m_renderSurface, SDL_SRCALPHA, 0);
		m_init =true;
    	if ( m_option<2) SDL_WM_SetCaption( caption.c_str(), NULL );
	}
	else
	{
		//options =SDL_SWSURFACE|SDL_NOFRAME;
		options =SDL_SWSURFACE; //|SDL_NOFRAME;
		//options =SDL_SWSURFACE|SDL_NOFRAME;
	    Uint32 rmask, gmask, bmask, amask;

	    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
	       on the endianness (byte order) of the machine */
		#if 0 //SDL_BYTEORDER ==SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		#else
			bmask = 0x000000ff;
			gmask = 0x0000ff00;
			rmask = 0x00ff0000;
			amask = 0; //0xff000000;
		#endif

	    m_allocatedSurface =SDL_CreateRGBSurface( options, m_size.width(), m_size.height(),
	    		                         ::m_mainGraph->m_renderSurface->format->BitsPerPixel,
	                                     rmask, gmask, bmask, amask);
	    m_renderSurface =m_allocatedSurface;
	    m_pixels =(Uint32*)m_renderSurface->pixels;
		SDL_Rect rect;
		rect.h =(Uint16)(m_renderSurface->h);
		rect.w =(Uint16)(m_renderSurface->w);
		rect.x =0;
		rect.y =0;
		SDL_FillRect( m_renderSurface, &rect, colour);
		m_init =true;
	}
#endif
    return true;
}

/*----------------------------------------------------------------------------*/
/** @brief Lock the surface for painting. */
void Cgraphics::lock()
{
	//Log.write("Cgraphics::lock");
	if (m_init)
	{
		SDL_LockSurface( m_renderSurface );
	}
}

/*----------------------------------------------------------------------------*/
/** @brief Unlock the surface for others to paint.
 */
void Cgraphics::unlock()
{
	if (m_init)
	{
		SDL_UnlockSurface( m_renderSurface );
	}
}

#ifdef USE_SDL2
/*----------------------------------------------------------------------------*/
/**
 * Update screen with one sprite
 * @param texture
 * @param destination
 */
void Cgraphics::update( SDL_Texture *texture, SDL_Rect *destination)
{
	SDL_SetRenderTarget( m_renderer, NULL);
	// Copy main background
	SDL_RenderCopy( m_renderer, m_texture, NULL, NULL);
	if ( texture && m_texture)
	{
		SDL_RenderCopy( m_renderer, m_texture, NULL, destination);
	}
	SDL_RenderPresent( m_renderer);
	SDL_SetRenderTarget( m_renderer, m_texture);
}
#endif

/*----------------------------------------------------------------------------*/
/** @brief Update the output graphics now */
void Cgraphics::update()
{
	if (m_init && m_option<2 && m_disableUpdate ==false)
	{
#ifdef USE_SDL2
		SDL_SetRenderTarget( m_renderer, NULL);
		// Copy main background
		SDL_RenderCopy( m_renderer, m_texture, NULL, NULL);
		SDL_RenderPresent( m_renderer);
		SDL_SetRenderTarget( m_renderer, m_texture);
#else
		SDL_UpdateRect( m_renderSurface,0,0,0,0);
#endif
	}
}

/*----------------------------------------------------------------------------*/
/** @brief Update the display now
 *  @param x1 [in] Left position to update
 *  @param y1 [in] Top position to update
 *  @param w [in] Width to update
 *  @param h [in] Height to update
 */
void Cgraphics::update( int x1, int y1, int w, int h)
{
	if (m_init && m_option<2 && m_disableUpdate == false)
	{
#ifdef USE_SDL2
		SDL_RenderPresent( m_renderer);
#else
		//SDL_UpdateRect( m_renderSurface,0,0,0,0);
		SDL_UpdateRect( m_renderSurface,x1,y1,w,h);
#endif
	}
}

/*----------------------------------------------------------------------------*/
/** @brief Draw a bar on screen.
 *  @param x1 [in] Left position
 *  @param y1 [in] Top position
 *  @param x2 [in] Right position
 *  @param y2 [in] Bottom position
 *  @param radius [in] Corner rounded diameter
 */
void Cgraphics::bar(int x1, int y1, int x2, int y2, int radius)
{
	x1 +=m_pixelOffset.x;
	y1 +=m_pixelOffset.y;
	x2 +=m_pixelOffset.x;
	y2 +=m_pixelOffset.y;
	SDL_Rect sr;
	if ( y2<y1)
	{
		int temp=y2;
		y2=y1;
		y1=temp;
	}
	if ( x2<x1)
	{
		int temp=x2;
		x2=x1;
		x1=temp;
	}
	int width =x2-x1;
	int height =y2-y1;
	sr.x =(Sint16)x1;
	sr.y =(Sint16)y1;
	sr.w =(Uint16)width;
	sr.h =(Uint16)height;
	if ( !m_init)
	{
		return;
	}
	//int col=SDL_MapRGB( m_renderSurface->format, m_R, m_G, m_B);
	int col=SDL_MapRGBA( m_renderSurface->format, m_R, m_G, m_B, m_A);
	if ( radius ==0)
	{
		SDL_FillRect(m_renderSurface, &sr, col);
		return;
	}
	for ( int x=0; x<=radius; x++)
	{
		int v=(int)( sqrt( (double)(radius*radius-(radius-x)*(radius-x))+0.5) );
		v =radius-v;
		sr.x =(Sint16)(x1+v);
		sr.y =(Sint16)(y1+x);
		sr.w =(Uint16)(x2-x1-v-v);
		sr.h =1;
		SDL_FillRect(m_renderSurface, &sr, col);
		sr.y =(Sint16)(y2-1-x);
		SDL_FillRect(m_renderSurface, &sr, col);
	}
	sr.y=(Sint16)(y1+radius+1);
	height =y2-y1-2*radius-2;
	if ( height>0)
	{
		sr.h =(Uint16)height;
		SDL_FillRect( m_renderSurface, &sr, col);
	}
}

/*----------------------------------------------------------------------------*/
/** @brief Set rectangle in a colour
 *  @param rect [in] Rectangle position on screen
 *  @param color [in] What colour to use
 *  @param radius [in] Corner radius to use (optional)
 */
void Cgraphics::bar( const Crect &rect, colour color, int radius)
{
	int x1=rect.left()*8; // Should consider pixelOffset
	int y1=rect.top()*8;
	int x2=rect.right()*8;
	int y2=rect.bottom()*8;
	gLimit( x1,0, m_mainGraph->width());
	gLimit( x2,0, m_mainGraph->width());
	gLimit( y1,0, m_mainGraph->height());
	gLimit( y2,0, m_mainGraph->height());

	if ( color!=(int)COLOUR_NONE) setColour( color );
	bar( x1,y1, x2,y2, radius);
}

/*----------------------------------------------------------------------------*/
/** @brief Set a new colour to paint with.
 *  @param col [in] Colour as in most software to use.
 */
void Cgraphics::setColour(colour col)
{
	m_colour =col;
	m_B = (Uint8)(col & 0x0000FF);
	m_G = (Uint8)((col & 0x00FF00) >> 8);
	m_R = (Uint8)((col & 0xFF0000) >> 16);
	m_A = 0; //(col & 0xFF000000) >> 24;
	//m_colour =(m_R<<8) + (m_B) + (m_G<<16)
#ifdef USE_SDL2
	SDL_SetRenderDrawColor( m_renderer, m_R, m_G, m_B, 0xff);
#endif
}

/*----------------------------------------------------------------------------*/
/** @brief Set rectangle code to use.
 *  @param rect [in] Rectangle with key code.
 *  @param key_code [in] Code to use for rectangle.
 */
void Cgraphics::setKey( const Crect &rect, keybutton key_code)
/* Define a keyblock with the same key_code for input touchpanel            */
/* PC uses Touchbuttons, PX uses hardware interrupt and table               */
{
	if ( m_lock_keycode ==false && key_code !=KEY_NOCHANGE)
	{
		m_touch.setKey( rect, key_code);
	}
}

/*----------------------------------------------------------------------------*/
/** @brief Change the picture, make if brighter.
 *  @param col [in] Which colour to use.
 *  @param percentage [in] How much more bright.
 */
colour Cgraphics::brighter( colour col, int percentage)
{
	int b = (col & 0x0000FF);
	int g = (col & 0x00FF00)>>8;
	int r = (col & 0xFF0000)>>16;
	r =r+(r*percentage/100);
	g =g+(g*percentage/100);
	b =b+(b*percentage/100);
	if ( r<0) r=0;
	if ( g<0) g=0;
	if ( b<0) b=0;
	if ( r>255) r=255;
	if ( g>255) g=255;
	if ( b>255) b=255;
	return (b)+(g<<8)+(r<<16);
}

/*----------------------------------------------------------------------------*/
/** @brief Find an image in the list or load from hard disk.
 *  @param fname [in] File to search.
 *  @return Surface to the picture.
 */
SDL_Surface *Cgraphics::findImage( const std::string &fname, bool isMonoIcon)
{
	if ( fname.size()==0)
	{
		return NULL;
	}
	auto it = m_imageMap.find(fname);
	if (it != m_imageMap.end())
	{
		return it->second;
	}
    SDL_Surface *surface = NULL;
	try
	{
		if ( fname.find( "/")==std::string::npos)
		{
		    // No slashes in name, use default path.
			std::string s=isMonoIcon ? Cgraphics::m_defaults.image_path_mono + fname:
			                           Cgraphics::m_defaults.image_path + fname;
			surface = IMG_Load( s.c_str() );
		}
		else
		{
		    // Path in filename
			surface = IMG_Load( fname.c_str() );
		}
        m_imageMap[fname] = surface;
	}
	catch (...)
	{
		return NULL;
	}
    return surface;
}

/*----------------------------------------------------------------------------*/
/** @brief Clean the surface to black. */
void Cgraphics::clean()
{
    for (auto image : m_imageMap)
    {
        SDL_FreeSurface( image.second);
    }
    m_imageMap.clear();
}

/*----------------------------------------------------------------------------*/
/** @brief Clean the surface to black. */
void Cgraphics::cleanBigSurfaces()
{
    bool found(true);
	while (found)
	{
	    found = false;
	    for (auto image : m_imageMap)
	    {
	    	if (!image.second)
	    	{
	            m_imageMap.erase(image.first);
	    	}
	    	else if (image.second->w > 400)
	        {
	            found = true;
	            SDL_FreeSurface(image.second);
	            m_imageMap.erase(image.first);
	        }
	    }
	}
}

/*----------------------------------------------------------------------------*/
/** @brief Free image used by the graphics layer.
 *  @param image [in] Name of image to release.
 */
void Cgraphics::freeImage( const std::string &image)
{
    auto it = m_imageMap.find(image);
    if (it != m_imageMap.end())
    {
    	if (it->second)
    	{
    		SDL_FreeSurface(it->second);
    	}
        m_imageMap.erase(it);
    }
}

/*----------------------------------------------------------------------------*/
bool Cgraphics::imageColour( const std::string &fname, int x1, int y1, int x2, int y2, colour col)
{
	(void)x2;
	(void)y2;
	SDL_Surface *bitmap =findImage( fname, true);
	if ( bitmap ==NULL || m_renderSurface ==NULL)
	{
		return false;
	}
	setColour( col);
	Uint32 *pixels =(Uint32*)bitmap->pixels;
	for ( int y=0; y<bitmap->h; y++)
	for ( int x=0; x<bitmap->w; x++)
	{
		int pix =*pixels;
		pixels++;
		if ((pix&0x80000000)!=0)
		{
			setPixel( x1+x, y1+y);
		}
	}
	return true;
}

// Colour conversion table:
// made with http://colorsafe.co/
static const int colourChart[125] =
{
    0xfde3a7,  // 000000
    0x89c4f4,  // 000040
    0x00bfff,  // 000080
    0xf1f227,  // 0000c0
    0xf1f227,  // 0000ff
    0x3cfc3c,  // 004000
    0x00d46a,  // 004040
    0x89c4f4,  // 004080
    0xf1f227,  // 0040c0
    0xf1f227,  // 0040ff
    0xc8f7c5,  // 008000
    0xbaf73c,  // 008040
    0xf1f227,  // 008080
    0xc5eff7,  // 0080c0
    0xfffacd,  // 0080ff
    0x634806,  // 00c000
    0x2a5527,  // 00c040
    0x205e3b,  // 00c080
    0x0000b5,  // 00c0c0
    0x3a539b,  // 00c0ff
    0x005500,  // 00ff00
    0x005031,  // 00ff40
    0x3e3e3e,  // 00ff80
    0x008000,  // 00ffc0
    0x006080,  // 00ffff
    0xe76e3c,  // 400000
    0xe000e0,  // 400040
    0x53b2d9,  // 400080
    0x53b2d9,  // 4000c0
    0x00ffff,  // 4000ff
    0xd4b300,  // 404000
    0xe6cc22,  // 404040
    0x89c4f4,  // 404080
    0x81cfe0,  // 4040c0
    0x00f8fb,  // 4040ff
    0xbaf73c,  // 408000
    0x98fb98,  // 408040
    0xa2ded0,  // 408080
    0x00f8fb,  // 4080c0
    0x2c3e50,  // 4080ff
    0x345a5e,  // 40c000
    0x134d13,  // 40c040
    0x114c2a,  // 40c080
    0x0000b5,  // 40c0c0
    0x1f3a93,  // 40c0ff
    0x205e3b,  // 40ff00
    0x4b6319,  // 40ff40
    0x00552a,  // 40ff80
    0x205e3b,  // 40ffc0
    0x3a539b,  // 40ffff
    0xff6347,  // 800000
    0xfc6399,  // 800040
    0xfc6399,  // 800080
    0xdcc6e0,  // 8000c0
    0xc5eff7,  // 8000ff
    0xf9bf3b,  // 804000
    0xffa07a,  // 804040
    0xdda0dd,  // 804080
    0xffecdb,  // 8040c0
    0xffecdb,  // 8040ff
    0xffecdb,  // 808000
    0xfde3a7,  // 808040
    0x551700,  // 808080
    0x3b0053,  // 8080c0
    0x3b0053,  // 8080ff
    0x005500,  // 80c000
    0x00552a,  // 80c040
    0x134d13,  // 80c080
    0x005051,  // 80c0c0
    0x0000b5,  // 80c0ff
    0x1d7a1d,  // 80ff00
    0xd43900,  // 80ff40
    0x1d781d,  // 80ff80
    0x134d13,  // 80ffc0
    0x2e456d,  // 80ffff
    0xf4b350,  // c00000
    0x2b0000,  // c00040
    0xf1f227,  // c00080
    0xf1f227,  // c000c0
    0x3b0053,  // c000ff
    0xf9bf3b,  // c04000
    0xe6cc22,  // c04040
    0xd5d5d5,  // c04080
    0xf1f227,  // c040c0
    0x3d2f5b,  // c040ff
    0x2a2400,  // c08000
    0x551700,  // c08040
    0x553529,  // c08080
    0x550055,  // c080c0
    0x3c1362,  // c080ff
    0x00008b,  // c0c000
    0x634806,  // c0c040
    0xb11030,  // c0c080
    0xcf000f,  // c0c0c0
    0x9400d3,  // c0c0ff
    0x007a4b,  // c0ff00
    0x1b7742,  // c0ff40
    0x1e824c,  // c0ff80
    0x1d781d,  // c0ffc0
    0x1460aa,  // c0ffff
    0xf1f227,  // ff0000
    0xf2f1ef,  // ff0040
    0x551700,  // ff0080
    0x600060,  // ff00c0
    0x600060,  // ff00ff
    0x5d1212,  // ff4000
    0x5d1212,  // ff4040
    0x551700,  // ff4080
    0x561b8d,  // ff40c0
    0x550055,  // ff40ff
    0x55ff20,  // ff8000
    0x923026,  // ff8040
    0x923026,  // ff8080
    0x800080,  // ff80c0
    0x76008a,  // ff80ff
    0x726012,  // ffc000
    0x382903,  // ffc040
    0x67221b,  // ffc080
    0x870c25,  // ffc0c0
    0xb200fd,  // ffc0ff
    0xaf851a,  // ffff00
    0xb8860b,  // ffff40
    0xf22613,  // ffff80
    0x9d8319,  // ffffc0
    0xe65722   // ffffff
};

static short digitToOffset[16] = { 0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4 };

/*----------------------------------------------------------------------------*/
bool Cgraphics::imageWithColourChart( const std::string &fname, int x1, int y1, int x2, int y2)
{
    (void)x2;
    (void)y2;
    SDL_Surface *bitmap =findImage( fname, true);
    if ( bitmap ==NULL || m_renderSurface ==NULL)
    {
        return false;
    }
    colour col = pixel((x1+x2)/2, (y1+y2)/2);
    // 0-1f =0 , 20-5f=1, 60-9f=2, a0-df=3, e0-ff=4
    int R= digitToOffset[(col>>20)&0xf]; // 0-4
    int G= digitToOffset[(col>>12)&0xf];
    int B= digitToOffset[(col>>4)&0xf];
    int index = R*25+G*5+B;
    setColour( colourChart[index]);
    Uint32 *pixels =(Uint32*)bitmap->pixels;

    for ( int y=0; y<bitmap->h; y++)
    for ( int x=0; x<bitmap->w; x++)
    {
        int pix =*pixels;
        pixels++;
        if ((pix&0x80000000)!=0)
        {
            setPixel( x1+x, y1+y);
        }
    }
    return true;
}

/*----------------------------------------------------------------------------*/
bool Cgraphics::imageWithBackgroundColour( const std::string &fname, int x1, int y1, int x2, int y2)
{
    (void)x2;
    (void)y2;
    SDL_Surface *bitmap =findImage( fname, true);
    if ( bitmap ==NULL || m_renderSurface ==NULL)
    {
        return false;
    }
    colour col = pixel((x1+x2)/2, (y1+y2)/2);
    int R= (col>>16)&0xff;
    int G= (col>>8)&0xff;
    int B= (col&0xff);
    col = ((R+G+B)>=0xc0*3) ? brighter(col, -40):brighter(col, 95);
    setColour( col);
    Uint32 *pixels =(Uint32*)bitmap->pixels;

    for ( int y=0; y<bitmap->h; y++)
    for ( int x=0; x<bitmap->w; x++)
    {
        int pix =*pixels;
        pixels++;
        if ((pix&0x80000000)!=0)
        {
            setPixel( x1+x, y1+y);
        }
    }
    return true;
}

bool Cgraphics::imageInverse( const std::string &fname, int x1, int y1, int x2, int y2, colour col)
{
	(void)x2;
	(void)y2;
	SDL_Surface *bitmap =findImage( fname, true);
	if ( bitmap ==NULL || m_renderSurface ==NULL)
	{
		return false;
	}
	setColour( col);
    Uint32 *pixels =(Uint32*)bitmap->pixels;
	for ( int y=0; y<bitmap->h; y++)
	for ( int x=0; x<bitmap->w; x++)
	{
		int pix =*pixels;
		pixels++;
        if ((pix&0x80000000)==0)
        {
            setPixel( x1+x, y1+y);
        }
	}
	return true;
}

/*----------------------------------------------------------------------------*/
/** @brief Set an image on screen.
 *  @param fname [in] Name of the image.
 *  @return true on success.
 */
bool Cgraphics::image(const std::string &fname, int x1, int y1, int x2, int y2)
{
	SDL_Surface *bitmap =findImage( fname, false);
	int w=x2-x1;
	int h=y2-y1;
	if ( bitmap ==NULL || m_renderSurface ==NULL)
	{
		return false;
	}
	if ( w>bitmap->w)
	{
		w=bitmap->w;
	}
	if ( h>bitmap->h)
	{
		h=bitmap->h;
	}
	SDL_Rect dst;
	dst.x = (Sint16)x1;
	dst.y = (Sint16)y1;
	dst.w = (Uint16)w;
	dst.h = (Uint16)h;

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = (Uint16)w; //x2-x1;
	src.h = (Uint16)h; //y2-y1;
	SDL_BlitSurface( bitmap, &src, m_renderSurface, &dst);
	//SDL_SoftStretch( bitmap, &src, m_renderSurface, &dst);
	//clean();
	return true;
}

/*----------------------------------------------------------------------------*/
void Cgraphics::setPixelOffset( int x, int y )
{
	m_pixelOffset =Cpoint( x,y );
}

/*----------------------------------------------------------------------------*/
/**
 * @brief Where to render to.
 * @param texture [in] Destination for lines, images, painting instructions etc.
 * @return true on success
 */
bool Cgraphics::setRenderArea( SDL_Surface *texture )
{
	if ( !texture)
	{
		m_renderSurface =m_allocatedSurface;
	}
	else
	{
		m_renderSurface =texture;
	}
	//int retv =SDL_SetRenderTarget( m_renderer, texture);
	//return (retv ==0);
	m_pixels =(Uint32*)m_renderSurface->pixels;
	return true;
}

/*----------------------------------------------------------------------------*/
/** @brief Insert a surface on our image.
 *  @param surface [in] What to insert.
 *  @param
 */
bool Cgraphics::surface( SDL_Surface *surface, int x1, int y1)
{
	if ( surface ==NULL)
	{
		return false;
	}
	SDL_Rect dst;
	dst.x =(Sint16)(x1+m_pixelOffset.x);
	dst.y =(Sint16)(y1+m_pixelOffset.y);
	dst.w =(Uint16)surface->w;
	dst.h =(Uint16)surface->h;
	//SDL_Surface *optimiser;
	//if (surface->flags & SDL_SRCALPHA)
	//        optimiser = SDL_DisplayFormatAlpha(surface);
	//    else
	//        optimiser = SDL_DisplayFormat(surface);

	//SDL_SetAlpha(surface,  SDL_SRCALPHA, SDL_ALPHA_TRANSPARENT);
	//SDL_SetAlpha(m_renderSurface,  SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
	SDL_BlitSurface( surface, NULL, m_renderSurface, &dst);
	//SDL_FreeSurface(optimiser);
	return true;
}

/*----------------------------------------------------------------------------*/
/** @brief Copy a piece of graphic.
 *  @param graphics [in] What image to paint.
 *  @param destination [in] What crop rectangle to paint the image.
 *  @param surface [in] Where to insert in our graphic.
 */
bool Cgraphics::copy( Cgraphics *graphics, const Crect &destination, const Crect &surface)
{
	if ( graphics ==NULL)
	{
		return false;
	}
	SDL_Rect src;
	src.x =(Sint16)surface.left();
	src.y =(Sint16)surface.top();
	src.w =(Uint16)surface.width();
	src.h =(Uint16)surface.height();
	SDL_Rect dst;
	dst.x =(Sint16)(destination.left()+m_pixelOffset.x);
	dst.y =(Sint16)(destination.top()+m_pixelOffset.y);
	dst.w =(Uint16)destination.width();
	dst.h =(Uint16)destination.height();
	//setColour(0);
	//bar(dst.x, dst.y, dst.x+dst.w, dst.y+dst.h, 0);
	//SDL_SetAlpha( graphics->m_renderSurface,  SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
	//SDL_FillRect( m_renderSurface, &dst, 0x004000);
	//SDL_FillRect( graphics->m_renderSurface, &src, 0xff000060);
	//SDL_SetAlpha( graphics->m_renderSurface, SDL_SRCALPHA, 255);
	//SDL_LockSurface(graphics->m_renderSurface);
	//SDL_LockSurface(m_renderSurface);
	//SDL_SetClipRect( m_renderSurface, &dst);
	SDL_BlitSurface( graphics->m_renderSurface, &src, m_renderSurface, &dst);


	//SDL_UnlockSurface(m_renderSurface);
	//SDL_UnlockSurface(graphics->m_renderSurface);
	return true;
}

/*----------------------------------------------------------------------------*/
/** @brief Copy a piece of graphic.
 *  @param graphics [in] What image to paint.
 *  @param destination [in] What crop rectangle to paint the image.
 *  @param surface [in] Where to insert in our graphic.
 */
bool Cgraphics::copySurface( SDL_Surface *surface, const Crect &destination, const Crect &source)
{
	if ( surface ==NULL)
	{
		return false;
	}
	SDL_Rect src;
	src.x =(Sint16)source.left();
	src.y =(Sint16)source.top();
	src.w =(Uint16)source.width();
	src.h =(Uint16)source.height();
	SDL_Rect dst;
	dst.x =(Sint16)(destination.left()+m_pixelOffset.x);
	dst.y =(Sint16)(destination.top()+m_pixelOffset.y);
	dst.w =(Uint16)destination.width();
	dst.h =(Uint16)destination.height();
	SDL_BlitSurface( surface, &src, m_renderSurface, &dst);
	return true;
}

/*----------------------------------------------------------------------------*/
/** @brief Insert a surface onto the background.
 *  @param surface [in] What surface to insert.
 *  @param x [in] Left position
 *  @param y [in] Top position
 *  @param w [in] Width
 *  @param h [in] Height
 */
bool Cgraphics::surface( SDL_Surface *surface, int x, int y, int w, int h)
{
	if ( surface ==NULL)
	{
		return false;
	}
	SDL_Rect dst;
	dst.x =(Sint16)(x+m_pixelOffset.x);
	dst.y =(Sint16)(y+m_pixelOffset.y);
	dst.w =(Uint16)w;
	dst.h =(Uint16)h;
	//SDL_Surface *optimiser;
	//if (surface->flags & SDL_SRCALPHA)
	//        optimiser = SDL_DisplayFormatAlpha(surface);
	//    else
	//        optimiser = SDL_DisplayFormat(surface);

	//SDL_SetAlpha(surface,  SDL_SRCALPHA, SDL_ALPHA_TRANSPARENT);
	//SDL_SetAlpha(m_renderSurface,  SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
	SDL_BlitSurface( surface, NULL, m_renderSurface, &dst);
	//SDL_FreeSurface(optimiser);
	return true;
}

/*----------------------------------------------------------------------------*/
void Cgraphics::settings( SdlDefaults *settings)
{
    m_defaults = *settings;
/*
	m_defaults.width =settings->width;	// in pixels.
	m_defaults.height =settings->height;	// in pixels.
	m_defaults.width_allocated =settings->width_allocated;
	m_defaults.height_allocated =settings->height_allocated;
	m_defaults.text_button_text =settings->text_button_text;
	m_defaults.text_button_background1 =settings->text_button_background1;
	m_defaults.text_button_background2 =settings->text_button_background2;
	m_defaults.text_button_border =settings->text_button_border;
	m_defaults.button_text =settings->button_text;
	m_defaults.button_background1 =settings->button_background1;
	m_defaults.button_background2 =settings->button_background2;
	m_defaults.menu_button_text =settings->menu_button_text;
	m_defaults.menu_button_background1 =settings->menu_button_background1;
	m_defaults.menu_button_background2 =settings->menu_button_background2;
	m_defaults.button_shadow =settings->button_shadow;
	m_defaults.line_dark =settings->line_dark;
	m_defaults.line_bright =settings->line_bright;
	m_defaults.image_button_text =settings->image_button_text;
	m_defaults.function_button_text =settings->function_button_text;
	m_defaults.function_button_background1 =settings->function_button_background1;
	m_defaults.function_button_background2 =settings->function_button_background2;
	m_defaults.header_text =settings->header_text;
	m_defaults.header_background1 =settings->header_background1;
	m_defaults.header_background2 =settings->header_background2;
	m_defaults.matrix_header_text =settings->matrix_header_text;
	m_defaults.button_height =settings->button_height;
	m_defaults.matrix_header_background1 =settings->matrix_header_background1;
	m_defaults.matrix_header_background2 =settings->matrix_header_background2;
	m_defaults.graph_text =settings->graph_text;
	m_defaults.graph_background1 =settings->graph_background1;
	m_defaults.graph_background2 =settings->graph_background2;
	m_defaults.calculator_text =settings->calculator_text;
	m_defaults.calculator_background1 =settings->calculator_background1;
	m_defaults.calculator_background2 =settings->calculator_background2;
	m_defaults.calculator_border =settings->calculator_border;
	m_defaults.background =settings->background;
	m_defaults.item_text =settings->item_text;
	m_defaults.handwriting_background =settings->handwriting_background;
	m_defaults.label_background =settings->label_background;
	m_defaults.label_text =settings->label_text;
	m_defaults.messagebox_text =settings->messagebox_text;
	m_defaults.messagebox_background1 =settings->messagebox_background1;
	m_defaults.messagebox_background2 =settings->messagebox_background2;
	m_defaults.slider_lines =settings->slider_lines;
	m_defaults.slider_background =settings->slider_background;
	m_defaults.touch_debounce_long_time =settings->touch_debounce_long_time;

	// images
	m_defaults.image_button_background =settings->image_button_background;
	m_defaults.full_screen_image_background =settings->full_screen_image_background;
	m_defaults.icon_ok48 =settings->icon_ok48;
	m_defaults.icon_cancel48 =settings->icon_cancel48;
	m_defaults.icon_language48 =settings->icon_language48;
	m_defaults.icon_printer48 =settings->icon_printer48;
	m_defaults.icon_slider48 =settings->icon_slider48;

	// settings
	m_defaults.country =settings->country;
	m_defaults.drag_start_pixels =settings->drag_start_pixels;
	m_defaults.repeat_speed =settings->repeat_speed;
	m_defaults.repeat_delay =settings->repeat_delay;
	m_defaults.minimum_drag_time =settings->minimum_drag_time;
	m_defaults.minimum_click_time =settings->minimum_click_time;
	m_defaults.font_path =settings->font_path;
	m_defaults.image_path =settings->image_path;
	m_defaults.data_path =settings->data_path;
	m_defaults.full_screen =settings->full_screen;
	m_defaults.handwriting_detection_enabled =settings->handwriting_detection_enabled;
	m_defaults.enable_record_keyfile =settings->enable_record_keyfile;
	m_defaults.show_labels =settings->show_labels;
	m_defaults.messagebox_time =settings->messagebox_time;
	m_defaults.audio_popup =settings->audio_popup;
	m_defaults.swype_friction =settings->swype_friction;
	m_defaults.touch_debounce_time =settings->touch_debounce_time;
	m_defaults.touch_debounce_distance =settings->touch_debounce_distance;
	m_defaults.debug_coordinates =settings->debug_coordinates;

	// functions
	m_defaults.get_translation =settings->get_translation;
	m_defaults.next_language =settings->next_language;
	m_defaults.get_test_event =settings->get_test_event;
	m_defaults.get_font =settings->get_font;
	m_defaults.find_text_id =settings->find_text_id;
	m_defaults.find_colour =settings->find_colour;
	m_defaults.start_from_main =settings->start_from_main;
	m_defaults.log =settings->log ? settings->log:printf;
	m_defaults.external_loop =settings->external_loop;
*/
}

/*----------------------------------------------------------------------------*/
/** @brief Set a pixel.
 *  @param x [in] Left position.
 *  @param y [in] Top position.
 */
void Cgraphics::setPixel(int x, int y)
{
	x +=m_pixelOffset.x;
	y +=m_pixelOffset.y;
#ifdef USE_SDL2
	SDL_RenderDrawPoint( m_renderer, x,y);
#else
	if (y>=m_renderSurface->h || x>=m_renderSurface->w || x<0 || y<0)
	{
		return;
	}
    //assert( x<m_renderSurface->w && x>=0);
    //assert( y<m_renderSurface->h && y>=0);

	Uint32 *pixels = (Uint32 *) m_pixels; //m_renderSurface->pixels;
	pixels[y * m_renderSurface->w + x] = m_colour;
#endif
}

/*----------------------------------------------------------------------------*/
/** @brief Set a pixel.
 *  @param x [in] Left position.
 *  @param y [in] Top position.
 */
void Cgraphics::darkenPixel(int x, int y, double part)
{
	if (y>=m_renderSurface->h || x>=m_renderSurface->w || x<0 || y<0)
	{
		return;
	}
    //assert( x<m_renderSurface->w && x>=0);
    //assert( y<m_renderSurface->h && y>=0);

	Uint32 *pixels = (Uint32 *) m_pixels; //m_renderSurface->pixels;
	colour p =pixels[y * m_renderSurface->w + x];
	int B = p & 0x0000FF;
	int G = (p & 0x00FF00) >> 8;
	int R = (p & 0xFF0000) >> 16;
	R =(int)(R*part);
	G =(int)(G*part);
	B =(int)(B*part);
	R =gLimit( R,0,255);
	G =gLimit( G,0,255);
	B =gLimit( B,0,255);
	p =B+(G<<8)+(R<<16);
    pixels[y * m_renderSurface->w + x] =p;
}

/*----------------------------------------------------------------------------*/
/** @brief Set a pixel.
 *  @param x [in] Left position.
 *  @param y [in] Top position.
 */
void Cgraphics::transparantPixel(int x, int y, double part)
{
	if (y>=m_renderSurface->h || x>=m_renderSurface->w || x<0 || y<0)
	{
		return;
	}
    //assert( x<m_renderSurface->w && x>=0);
    //assert( y<m_renderSurface->h && y>=0);
	part =gLimit(part,0,1);
	Uint32 *pixels = (Uint32 *) m_pixels; // m_renderSurface->pixels;
	colour p =pixels[y * m_renderSurface->w + x];
	int B = p & 0x0000FF;
	int G = (p & 0x00FF00) >> 8;
	int R = (p & 0xFF0000) >> 16;
	R =(int)(R*part+m_R*(1-part));
	G =(int)(G*part+m_G*(1-part));
	B =(int)(B*part+m_B*(1-part));
	R =gLimit( R,0,255);
	G =gLimit( G,0,255);
	B =gLimit( B,0,255);
	p =B+(G<<8)+(R<<16);
    pixels[y * m_renderSurface->w + x] =p;
}

/*----------------------------------------------------------------------------*/
/** @brief Get pixel data.
 *	@param x [in] Left position.
 *	@param y [in] Top position.
 */
int Cgraphics::pixel(int x, int y)
{
	if (y>=m_renderSurface->h || x>=m_renderSurface->w || x<0 || y<0)
	{
		return 0;
	}
    //assert( x<m_renderSurface->w && x>=0);
    //assert( y<m_renderSurface->h && y>=0);

	Uint32 *pixels = (Uint32 *) m_pixels; //m_renderSurface->pixels;
	return pixels[y * m_renderSurface->w + x];
}

/*----------------------------------------------------------------------------*/
/** @brief Darken a certain pixel.
 *  @param x [in] Left position.
 *  @param y [in] Top position.
 */
void Cgraphics::darken(int x, int y)
{
	if (y>=m_renderSurface->h || x>=m_renderSurface->w || x<0 || y<0)
	{
		return;
	}
	Uint32 *pixels = (Uint32 *) m_pixels; // m_renderSurface->pixels;
	int col =pixels[y * m_renderSurface->w + x];

	int B = (col & 0x0000FF)/2;
	int G = ((col & 0x00FF00) >> 8)/2;
	int R = ((col & 0xFF0000) >> 16)/2;
	int value =(R<<16) +(G<<8) + B;
	pixels[y * m_renderSurface->w + x] =value;
}

/*----------------------------------------------------------------------------*/
/** @brief Tell where we are allowed to paint inside the graphics display
 *  @param rect [in] Location. NULL is remove clipping
 */
void Cgraphics::setViewport( SDL_Rect *rect)
{
// Tell where we can paint.
	SDL_SetClipRect( m_allocatedSurface, rect);
}

/*----------------------------------------------------------------------------*/
/** @brief Make a piece of display more dark (to highlight the rest).
 *  @param x1 [in] Left position.
 *  @param y1 [in] Top position.
 *  @param x2 [in] Right position.
 *  @param y2 [in] Bottom position.
 */
void Cgraphics::darken( int x1, int y1, int x2, int y2)
{
#ifdef USE_SDL2
	SDL_Texture *texture =SDL_CreateTexture( m_renderer,
                                   SDL_PIXELFORMAT_ARGB8888,
                                   SDL_TEXTUREACCESS_TARGET,
                                   1, 1);
	if ( !texture)
	{
		x2 =Cgraphics::m_defaults.width;
		y2 =Cgraphics::m_defaults.height;
	}
	if ( this!=m_mainGraph)
	{
		m_mainGraph->darken( x1,y1,x2,y2);
		return;
	}
	SDL_SetRenderTarget( m_renderer, texture);
	SDL_SetRenderDrawColor( m_renderer, 0x10, 0x10, 0x10, 0x30);
	SDL_RenderDrawPoint( m_renderer, 0,0);
	//SDL_SetTextureAlphaMod( texture, 0x80);
	SDL_SetRenderTarget( m_renderer, m_texture);
	SDL_Rect rect;
	rect.x =x1;
	rect.y =y1;
	rect.w =x2-x1;
	rect.h =y2-y1;
	SDL_RenderCopy( m_renderer, texture, NULL, &rect);
	SDL_DestroyTexture( texture);
#else
	if ( x2==0 && y2==0)
	{
		x2 =Cgraphics::m_defaults.width;
		y2 =Cgraphics::m_defaults.height;
	}
	if ( this!=m_mainGraph)
	{
		m_mainGraph->darken( x1,y1,x2,y2);
		return;
	}
	for (int x=x1; x<x2; ++x)
	for (int y=y1; y<y2; ++y)
	{
		darken(x,y);
	}
#endif
}

/*----------------------------------------------------------------------------*/
/** @brief Push a graphic onto stack.
 */
bool Cgraphics::push_back()
{
	Uint32 *background =new Uint32[ m_size.area()];
	if (!background)
	{
		return false;
	}
	lock();
 	Uint32 *pixels =(Uint32 *)  m_renderSurface->pixels;
 	memcpy( background, pixels, m_size.area()*sizeof(Uint32));
 	m_background.push_back(background);
	unlock();
	return true;
}

/*----------------------------------------------------------------------------*/
/** @brief Pop a graphic plane from stack (not used now).
 */
bool Cgraphics::pop_back()
{
	if ( m_background.empty() )
	{
		return false;
	}
	Uint32 *background =m_background[ m_background.size()-1 ];
	m_background.pop_back();
	if (!background)
	{
		return false;
	}
	lock();
 	Uint32 *pixels =(Uint32 *) m_renderSurface->pixels;
 	memcpy( pixels, background, m_size.area()*sizeof(Uint32));
	unlock();
	update();
	delete background;
	return true;
}

/*----------------------------------------------------------------------------*/
/// @brief Get front screen without remove it from stack.
bool Cgraphics::front()
{
	if ( m_background.empty() )
	{
		return false;
	}
	Uint32 *background =m_background[ m_background.size()-1 ];
	if (!background)
	{
		return false;
	}
	lock();
 	Uint32 *pixels =(Uint32 *) m_renderSurface->pixels;
 	memcpy( pixels, background, m_size.area()*sizeof(Uint32));
	unlock();
	update();
	return true;
}

/*----------------------------------------------------------------------------*/
void Cgraphics::cleardevice()
{

}

/*----------------------------------------------------------------------------*/
void Cgraphics::drawArc(int x, int y, int start_corner, int end_corner, int radius1, int radius2)
{
	double corner1=start_corner*2*M_PI/360.0;
	double corner2=end_corner*2*M_PI/360.0;
	double step = 0.03/(double)radius2;
	double rad1=radius1;
	double rad2=radius2;
	for (double corner =corner1; corner<=corner2; corner+=step)
	{
		double sinx=sin(corner);
		double cosx=cos(corner);
		line( x+(int)(sinx*rad1+0.5), y-(int)(cosx*rad1+0.5), x+(int)(sinx*rad2+0.5), y-(int)(cosx*rad2+0.5));
	}
}

/*----------------------------------------------------------------------------*/
/** @brief Paint any line.
 *  @param x1 [in] Left position start point.
 *  @param y1 [in] Top position start point.
 *  @param x2 [in] Left position end point.
 *  @param y2 [in] Top position end point.
 */
void Cgraphics::line(int x1, int y1, int x2, int y2)
{
#ifdef USE_SDL2
	SDL_RenderDrawLine( m_renderer, x1, y1, x2,y2);
	return;
#else
	//Log.write( "Cgraphics::line");
	int dx = abs(x1 - x2), dy = abs(y1 - y2), p;
	int inc;
	if (!dy) /* Horizontal line - use mapword */
	    mapword(x1, x2, y1);
	else if (!dx) /*Vertical line - use mapvword  */
		mapvword(x1, y1, y2);
	else if (dx >= dy)
	{
		p = 2 * dy - dx;
		if (x1 >= x2)
		{
			SWAP(x1, x2, int)
			SWAP(y1, y2, int)
		}
		setPixel(x1, y1);
		if (y2 >= y1)
			inc = 1;
		else
			inc = -1;

		while (x1 < x2)
		{
			x1++;
			if (p < 0)
				p += 2 * dy;
			else
			{
				y1 += inc;
				p += 2 * (dy - dx);
			}
			setPixel(x1, y1);
		}
	}
	else
	{
		p = 2 * dx - dy;
		if (y1 >= y2)
		{
			SWAP(x1, x2, int)
			SWAP(y1, y2, int)
		}
		setPixel(x1, y1);

		if (x2 >= x1)
			inc = 1;
		else
			inc = -1;

		while (y1 < y2)
		{
			y1++;
			if (p < 0)
				p += 2 * dx;
			else
			{
				x1 += inc;
				p += 2 * (dx - dy);
			}
			setPixel(x1, y1);
		}
	}
	m_cx = x2;
	m_cy = y2;
#endif	
}

/*----------------------------------------------------------------------------*/
/** @brief Paint an image.
 *  @param image [in] Image to draw.
 *  @param x1 [in] Left position.
 *  @param y1 [in] Top position.
 *  @param x2 [in] Right position.
 *  @param y2 [in] Bottom position.
 */
void Cgraphics::imageLine( const std::string &image, int x1, int y1, int x2, int y2)
{
	SDL_Surface *bitmap =findImage( image, false);
	if ( bitmap ==NULL || m_renderSurface ==NULL)
	{
		return;
	}
	SDL_Rect dst;
	dst.x = (Sint16)(x1+m_pixelOffset.x);
	dst.y = (Sint16)(y1+m_pixelOffset.y);
	dst.w = (Uint16)(x2 - x1);
	dst.h = (Uint16)(y2 - y1);

	SDL_Rect src;
	src.x = (Sint16)x1;
	src.y = (Sint16)y1;
	src.w = (Uint16)(x2 - x1);
	src.h = (Uint16)(y2 - y1);
	SDL_BlitSurface( bitmap, &src, m_renderSurface, &dst);
}

/*----------------------------------------------------------------------------*/
/** @brief Paint line to graphics.
 *  @param dx [in] relative X-position.
 *  @param dy [in] relative Y-position.
 */
void Cgraphics::linerel(int dx, int dy)
{
	line(m_cx, m_cy, m_cx + dx, m_cy + dy);
}

/*----------------------------------------------------------------------------*/
/** @brief Paint line to graphics.
 *  @param x [in] X-position.
 *  @param y [in] Y-position.
 */
void Cgraphics::lineto(int x, int y)
{
	line(m_cx, m_cy, x, y);
	m_cx = x;
	m_cy = y;
}

/*----------------------------------------------------------------------------*/
/** @brief Display rectangle.
 *  @param left [in] Left position.
 *  @param top [in] top position.
 *  @param right [in] right position.
 *  @param bottom [in] bottom position.
 *  @param edge [in] width of edge.
 *  @param radius [in] round corners diameter.
 */
void Cgraphics::rectangle(int left, int top, int right, int bottom, int edge, int radius)
{
	int width =(right-left);
	int height =(bottom-top);

	for ( int a=0; a<edge; a++)
	{
		if ( width>radius*2)
		{
			line(left+radius, top+a, right-radius-1, top+a);
			line(right-radius-1, bottom-a-1, left+radius, bottom-a-1);
		}
		if ( height>radius*2)
		{
			line(right-a-1, top+radius, right-a-1, bottom-radius-1);
			line(left+a, bottom-radius-1, left+a, top+radius);
		}
	}
	if (radius ==0)
	{
		return;
	}
	SDL_Rect sr;
	int radius2=radius-edge;
	for ( int x=radius-1; x>=0; x--)
	{
		int v=(int)( sqrt( (double)(radius*radius-(radius-x)*(radius-x))+0.5) );
		//int v2=(int)((double)radius*sqrt((double)x/(double)radius)+0.5);
		v =radius-v;
		if ( x<edge)
		{
			sr.w =(Uint16)(radius-v);
		}
		else
		{
			int v2=(int)( sqrt( (double)(radius2*radius2-(radius-x)*(radius-x))+0.5) );
			v2 =edge+radius2-v2;
			sr.w =(Uint16)((v2>v) ? v2-v:v-v2);
		}
		sr.x =(Sint16)(left+v);
		sr.y =(Sint16)(top+x);
		sr.h =1;
		SDL_FillRect(m_renderSurface, &sr, m_colour);
		sr.x =(Sint16)(right-v-sr.w);
		SDL_FillRect(m_renderSurface, &sr, m_colour);
		sr.y =(Sint16)(bottom-x-1);
		SDL_FillRect(m_renderSurface, &sr, m_colour);
		sr.x =(Sint16)(left+v);
		SDL_FillRect(m_renderSurface, &sr, m_colour);
	}
}

/*----------------------------------------------------------------------------*/
void Cgraphics::ellipse(int xc, int yc, int stangle, int endangle,
		 int rx, int ry)
{
	float xold, yold, xnew, ynew;
	float sin_rxbyry, sin_rybyrx, costheta, theta;
	int num;

	theta = (float)gMin(1.00 / (gMax(rx, ry)), 0.001);
	num = (int) (rint(M_PI * (endangle - stangle) / 180.0 / theta));

	sin_rxbyry = (float) (rx / ry * sin(theta));
	sin_rybyrx = (float) (ry / rx * sin(theta));
	costheta = (float)cos(theta);

	xold = (float)(rx * cos(M_PI * stangle / 180.0));
	yold = (float)(ry * sin(M_PI * stangle / 180.0));

	for (; num; num--)
	{
		xnew = xold * costheta - yold * sin_rxbyry;
		ynew = xold * sin_rybyrx + yold * costheta;
		setPixel( (int)(rintf(xnew + (float)xc)), (int)(rintf(ynew + (float)yc)));
		xold = xnew, yold = ynew;
	}
}

/*----------------------------------------------------------------------------*/
void Cgraphics::moveto(int x, int y)
{
	m_cx = gMin(m_renderSurface->w, x);
	m_cy = gMin(m_renderSurface->h, y);
	m_cx = gMax(x, 0);
	m_cy = gMax(0, y);
}

/*----------------------------------------------------------------------------*/
void Cgraphics::moverel(int dx, int dy)
{
	m_cx += dx;
	m_cy += dy;
	moveto(m_cx, m_cy);
}

/*----------------------------------------------------------------------------*/
void Cgraphics::mapword(int x1, int x2, int y)
{
	if (x1<0 || x2<0 || y<0 || x1>=m_renderSurface->w || x2>=m_renderSurface->w || y>=m_renderSurface->h)
	{
		return;
	}
	assert( x1>=0 && x1<m_renderSurface->w);
	assert( x2>=0 && x2<m_renderSurface->w);
	assert( y>=0 && y<m_renderSurface->h);

	if ( x1>x2) SWAP(x1,x2,int);

	Uint32 *pixels = m_pixels+y * m_renderSurface->w+x1;
	// m_renderSurface->pixels+y*m_renderSurface->w+x1;

	while (x1<=x2)
	{
		*pixels =m_colour;
		pixels++;
		x1++;
	}
	return;
}

/*----------------------------------------------------------------------------*/
void Cgraphics::mapvword(int x, int y1, int y2)
{
	if ( m_renderSurface ==NULL)
	{
		return;
	}
	if (x<0 || y1<0 || y2<0 || x>=m_renderSurface->w || y1>=m_renderSurface->h || y2>=m_renderSurface->h )
	{
		return;
	}
	if ( y1>y2) SWAP(y1,y2,int);

	//Uint32 *pixels = (Uint32 *) m_renderSurface->pixels+y1*m_renderSurface->w+x;
	Uint32 *pixels = (Uint32 *) m_pixels+y1*m_renderSurface->w+x;

	while (y1<=y2)
	{
		*pixels =m_colour;
		pixels+=m_renderSurface->w;
		y1++;
	}
	return;
}
