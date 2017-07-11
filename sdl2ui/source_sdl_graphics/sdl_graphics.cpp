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
#include <sdl_graphics.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <SDL_image.h>
#include "SDL_ttf.h"

#define SWAP(A,B,TYPE) {TYPE temp=A; A=B; B=temp;}

/// Defaults graphics
Sdefaults Cgraphics::m_defaults =
{
	800, // .width
	600, // .height
	800,
	600,
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
	"printer48.png", // ICON_PRINTER
	"slider48.png", // ICON_SLIDER
	32, // country
	15, // drag_start_pixels
	75, // repeat_speed
	1000, // repeat_delay
	1000, // minimum_drag_time
	200, // minimum_click_time
	"/home/mensfort/zhongcan/font", // .font_path
	"/home.mensfort/zhongcan/buttons", // .image_path
	"/home/mensfort/zhongcan/data", // .data_path
	1, // full_screen
	true, // handwriting_detection_enabled
	false, // enable_record_keyfile
	true, // show_labels
	500, // messagebox_time
	"popup.wav", // audio_popup
	20, // swype_friction
	6, // button height
	5, // touch_debounce_time;
	8, // touch_debounce_distance
	0, // display coordinates
	3000, // debounce long press
	NULL, // get_translation
	NULL, // next_language
	NULL, // get_test_event
	NULL, // get_font
	NULL, // find_text_id
	NULL, //.find_colour
	NULL, //.start_from_main
	NULL, //.log_func
	NULL, //.external loop
};

/** @brief Constructor for graphics to paint on.
 *  @param width [in] Width of graphics.
 *  @param height [in] Height of graphics.
 *  @param mainScreen [in] boolean, true for the main screen.
 * */
Cgraphics::Cgraphics( const Csize &size, bool mainScreen):
#ifdef USE_SDL2
	m_window(NULL),
	m_renderer(NULL),
	m_texture(NULL),
#else
	m_allocatedSurface(NULL),
	m_renderSurface(NULL),
#endif
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
	m_pixels(NULL)
{
	m_touch.setSize( Csize(size.width()/8, size.height()/8));
}

Cgraphics::~Cgraphics()
{
	clean();
	close();
}

void Cgraphics::close()
{
#ifdef USE_SDL2
	if ( m_texture)
	{
		SDL_DestroyTexture( m_texture);
		m_texture = NULL;
	}
    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = NULL;
    }
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = NULL;
    }

	if ( m_mainScreen)
	{
		SDL_Quit();
	}
#else
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
#endif
}

/** @brief Initialise the graphics field.
 *  @param caption [in] What to show above the dialog
 *  @param pixels [in] Should be 32 for each pixel.
 *  @param colour [in] Background colour. -1 for default.
 */
bool Cgraphics::init( const std::string &caption, int pixels, int colour)
{
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

#ifdef USE_SDL2
    (void)pixels;
    if ( m_mainScreen)
    {
		if(SDL_Init( SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO
				   | SDL_INIT_EVENTS | SDL_INIT_HAPTIC
				   | SDL_INIT_GAMECONTROLLER) < 0)
		{
			return false;
		}
		m_window = SDL_CreateWindow( caption.c_str(),
		                          SDL_WINDOWPOS_UNDEFINED,
		                          SDL_WINDOWPOS_UNDEFINED,
		                          m_size.width(), m_size.height(),
		                          (m_option ==1) ?
		                        		  SDL_WINDOW_FULLSCREEN|SDL_WINDOW_SHOWN:SDL_WINDOW_BORDERLESS|SDL_WINDOW_SHOWN
		                          //SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL:SDL_WINDOW_OPENGL
		                          );
		m_renderer = SDL_CreateRenderer( m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
		//m_windowRenderer =m_renderer;
		SDL_SetRelativeMouseMode(SDL_FALSE);

#if 0
		const SDL_VideoInfo *info =SDL_GetVideoInfo();
		if ( info)
		{
			Log.write("hw_available =%d", info->hw_available);
			Log.write("wm_available =%d", info->wm_available);
		}
#endif
		SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, 0xff);
		SDL_RenderClear( m_renderer);
		SDL_RenderPresent( m_renderer);
		SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
		SDL_RenderSetLogicalSize( m_renderer, m_defaults.width, m_defaults.height);
		SDL_Rect view;
		view.h =m_defaults.height;
		view.w =m_defaults.width;
		view.x =0;
		view.y =0;
		SDL_RenderSetViewport( m_renderer, &view);
		m_init =true;
    } // if main windows
    else
    {
    	//m_window =m_mainGraph->m_window;
    	m_renderer =NULL; //m_mainGraph->m_renderer;
    	//m_windowRenderer =m_renderer;
    }
    m_texture =SDL_CreateTexture( m_renderer,
                                   SDL_PIXELFORMAT_ARGB8888,
                                   SDL_TEXTUREACCESS_TARGET,
                                   m_size.width(), m_size.height());
    if ( m_texture)
    {
    	SDL_SetTextureBlendMode( m_texture, SDL_BLENDMODE_BLEND);
    }
#else
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
		const SDL_VideoInfo *info =SDL_GetVideoInfo();
		if ( info)
		{
			Log.write("hw_available =%d", info->hw_available);
			Log.write("wm_available =%d", info->wm_available);
		}
		m_renderSurface =m_allocatedSurface;
		m_pixels =(Uint32*)m_allocatedSurface->pixels;
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

/** @brief Lock the surface for painting. */
void Cgraphics::lock()
{
#ifdef USE_SDL2
	//assert(0);
#else
	if (m_init)
	{
		SDL_LockSurface( m_renderSurface );
	}
#endif
}

Csize Cgraphics::textureSize( sdlTexture *texture)
{
#ifdef USE_SDL2
	if ( texture ==NULL)
	{
		return Csize(0,0);
	}
	Uint32 format;
	int w,h;
	int access;
	SDL_QueryTexture( texture, &format, &access, &w, &h);
	return Csize(w,h);
#else
	return Csize(texture->w, texture->h);
#endif
}

/** @brief Unlock the surface for others to paint.
 */
void Cgraphics::unlock()
{
#ifndef USE_SDL2
	if (m_init)
	{
		SDL_UnlockSurface( m_renderSurface );
	}
#endif
}

#if 0
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

#ifdef USE_SDL2
bool Cgraphics::insertTexture( SDL_Texture *texture, int left, int top, int w, int h)
{
	if (texture)
	{
		setRenderArea();
		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = w;
		src.h = h;
		SDL_Rect rect;
		rect.x = left;
		rect.y = top;
		rect.w = m_size.width();
		rect.h = m_size.height();
		SDL_RenderCopy(m_renderer, m_texture, &src, &rect);
		return true;
	}
	return false;
}
#endif

/** @brief Update the output graphics now */
void Cgraphics::update()
{
	//Log.write("Cgraphics::update");
	if (m_init && m_option<2)
	{
#ifdef USE_SDL2
		SDL_SetRenderTarget( m_renderer, NULL);
		SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
		SDL_RenderPresent( m_renderer);
		SDL_SetRenderTarget( m_renderer, m_texture);
#else
		SDL_UpdateRect( m_renderSurface,0,0,0,0);
#endif
	}
}

/** @brief Update the display now
 *  @param x1 [in] Left position to update
 *  @param y1 [in] Top position to update
 *  @param w [in] Width to update
 *  @param h [in] Height to update
 */
void Cgraphics::update( const Crect &rect)
{
	if (m_init && m_option<2)
	{
#ifdef USE_SDL2
		if (m_texture)
		{
			SDL_Rect rct;
			rct.x = rect.left();
			rct.y = rect.top();
			rct.w = rect.width();
			rct.h = rect.height();
			SDL_SetRenderTarget( m_renderer, NULL);
			SDL_RenderCopy(m_renderer, m_texture, &rct, &rct);
		}
		SDL_RenderPresent( m_renderer);
		SDL_SetRenderTarget( m_renderer, m_texture);
#else
		SDL_UpdateRect( m_renderSurface,rect.left(),rect.top(),rect.width(),rect.height());
#endif
	}
}

/** @brief Draw a bar on screen.
 *  @param x1 [in] Left position
 *  @param y1 [in] Top position
 *  @param x2 [in] Right position
 *  @param y2 [in] Bottom position
 *  @param radius [in] Corner rounded diameter
 */
void Cgraphics::bar(int x1, int y1, int x2, int y2, int radius)
{
#ifdef USE_SDL2
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
    SDL_SetRenderDrawColor( m_renderer, m_R, m_G, m_B, SDL_ALPHA_OPAQUE);
	if ( radius ==0)
	{
		SDL_RenderFillRect( m_renderer, &sr);
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
		SDL_RenderFillRect( m_renderer, &sr);
		sr.y =(Sint16)(y2-1-x);
		SDL_RenderFillRect( m_renderer, &sr);
	}
	sr.y=(Sint16)(y1+radius+1);
	height =y2-y1-2*radius-2;
	if ( height>0)
	{
		sr.h =(Uint16)height;
		SDL_RenderFillRect( m_renderer,&sr);
	}
#else
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
#endif
}

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
	//gLimit( x1,0, m_mainGraph->width());
	//gLimit( x2,0, m_mainGraph->width());
	//gLimit( y1,0, m_mainGraph->height());
	//gLimit( y2,0, m_mainGraph->height());

	if ( color!=(int)COLOUR_NONE) setColour( color );
	bar( x1,y1, x2,y2, radius);
}

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
#ifdef USE_SDL2
	SDL_SetRenderDrawColor( m_renderer, m_R, m_G, m_B, SDL_ALPHA_OPAQUE);
#endif
}

void Cgraphics::setTransparant(double level)
{
	m_A =gLimit((int)(level*256.0), 0,255);
#ifdef USE_SDL2
	SDL_SetRenderDrawColor( m_renderer, m_R, m_G, m_B, m_A);
#endif
}

/** @brief Set rectangle code to use.
 *  @param rect [in] Rectangle with key code.
 *  @param key_code [in] Code to use for rectangle.
 */
void Cgraphics::setCode( const Crect &rect, keybutton key_code)
/* Define a keyblock with the same key_code for input touchpanel            */
/* PC uses Touchbuttons, PX uses hardware interrupt and table               */
{
	if ( m_lock_keycode ==false && key_code !=KEY_NOCHANGE)
	{
		m_touch.setKey( rect, key_code);
	}
}

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

/** @brief Find an image in the list or load from hard disk.
 *  @param fname [in] File to search.
 *  @return Surface to the picture.
 */
sdlTexture *Cgraphics::findImage( const std::string &fname)
{
	if ( fname.size()==0)
	{
		return NULL;
	}
	for ( int a=0; a<(int)m_images.size(); a++)
	{
		if ( fname==m_images[a].name)
		{
			if ( m_images[a].image ==NULL)
			{
				printf("Image not found %s!!\n", m_images[a].name.c_str());
			}
			return m_images[a].image;
		}
	}
	SimageSurface c;
	try
	{
		c.name =fname;
		if ( c.name.find( "/")==std::string::npos)
		{
			std::string s=Cgraphics::m_defaults.image_path + c.name;
#ifdef USE_SDL2
			c.image = IMG_LoadTexture( m_renderer, s.c_str());
#else
			c.image = IMG_Load( s.c_str() );
#endif
		}
		else
		{
#ifdef USE_SDL2
			c.image = IMG_LoadTexture( m_renderer, c.name.c_str());
#else
			c.image = IMG_Load( c.name.c_str() );
#endif
		}
	}
	catch (...)
	{
		return NULL;
	}
	m_images.push_back( c);
	return c.image;
}

/** @brief Clean the surface to black. */
void Cgraphics::clean()
{
	for ( int a=0; a<(int)m_images.size(); a++)
	{
#ifdef USE_SDL2
		SDL_DestroyTexture( m_images[a].image);
#else
		SDL_FreeSurface(m_images[a].image);
#endif
	}
	m_images.clear();
}

/** @brief Free image used by the graphics layer.
 *  @param image [in] Name of image to release.
 */
void Cgraphics::freeImage( const std::string &image)
{
	for ( int a=0; a<(int)m_images.size(); a++)
	{
		if ( image ==m_images[a].name)
		{
#ifdef USE_SDL2
			SDL_DestroyTexture( m_images[a].image);
#else
			SDL_FreeSurface(m_images[a].image);
#endif
			m_images.erase( m_images.begin()+a);
			break;
		}
	}
}

#if 0
/**
 * Paint an image on location with a margin
 * @param texture
 * @param rect [in] Rectangle in pixels.
 * @param gravity
 * @param margin
 * @return true on success
 */
bool Cgraphics::imageAdd( const std::string &fname, const Crect &rect, Egravity gravity, int margin)
{
	int w,h;
	SDL_Rect src;
	SDL_Rect dst;

	SDL_Texture *texture =findImage( fname);
	if (!texture)
	{
		return false;
	}
	SDL_QueryTexture( texture, NULL, NULL, &w, &h);
	src.x =0; src.y=0; src.w=w; src.h=h;
	dst.x =rect.left()+margin; dst.y=rect.top()+margin; dst.w=rect.width()-2*margin; dst.h=rect.height()-2*margin;

	// Horizontal calculation + Vertical calculation
	switch (gravity)
	{
	case GRAVITY_LEFT: // left top
		dst.w =w;
		dst.h =h;
		break;

	case GRAVITY_LEFT_CENTER:
		dst.w =w;
		dst.h =h;
		dst.y =(rect.top()+rect.bottom()-h)/2;
		break;

	case GRAVITY_LEFT_BOTTOM:
		dst.w =w;
		dst.h =h;
		dst.y =rect.bottom()-margin-h;
		break;

	case GRAVITY_RIGHT:
		dst.x =rect.right()-margin-w;
		dst.w =w;
		dst.h =h;
		break;

	case GRAVITY_RIGHT_CENTER:
		dst.x =rect.right()-margin-w;
		dst.w =w;
		dst.h =h;
		dst.y =(rect.top()+rect.bottom()-h)/2;
		break;

	case GRAVITY_RIGHT_BOTTOM:
		dst.x =rect.right()-margin-w;
		dst.w =w;
		dst.h =h;
		dst.y =rect.bottom()-margin-h;
		break;

	case GRAVITY_CENTER:
		dst.x =(rect.left()+rect.right()-w)/2;
		dst.w =w;
		dst.h =h;
		dst.y =(rect.top()+rect.bottom()-h)/2;
		break;

	case GRAVITY_TOP_CENTER:
		dst.x =(rect.left()+rect.right()-w)/2;
		dst.w =w;
		dst.h =h;
		break;

	case GRAVITY_BOTTOM_CENTER:
		dst.x =(rect.left()+rect.right()-w)/2;
		dst.w =w;
		dst.h =h;
		dst.y =rect.bottom()-margin-h;
		break;

	case GRAVITY_RESIZE:
		break;
	case GRAVITY_NO_CHANGE:
		break;
	default:
	  	break;
	}
	SDL_RenderCopy( m_renderer, texture, &src, &dst);
	return true;
}
#endif

bool Cgraphics::imageColour( const std::string &fname, int x1, int y1, int x2, int y2, colour col)
{
	(void)x2;
	(void)y2;
#ifdef USE_SDL2
	(void)x1;
	(void)y1;
	(void)col;
	(void)fname;
	assert(0);
#else
	sdlTexture *bitmap =findImage( fname);
	if ( bitmap ==NULL || m_renderSurface ==NULL)
	{
		return false;
	}
	setColour( col);
	Uint32 *pixels =(Uint32*)m_pixels;
	for ( int y=0; y<bitmap->h; y++)
	for ( int x=0; x<bitmap->w; x++)
	{
		int pix =*pixels;
		pixels++;
		if (pix!=0xffffff)
		{
			setPixel( x1+x, y1+y);
		}
	}
#endif
	return true;
}

bool Cgraphics::imageInverse( const std::string &fname, int x1, int y1, int x2, int y2, colour col)
{
#ifdef USE_SDL2
	(void)fname;
	(void)x1;
	(void)y1;
	(void)x2;
	(void)y2;
	(void)col;
	assert(0);
	return true;
#else
	sdlTexture *bitmap =findImage( fname);
	if ( bitmap ==NULL || m_renderSurface ==NULL)
	{
		return false;
	}
	setColour( col);
	Uint32 *pixels =(Uint32*)m_pixels;
	for ( int y=0; y<bitmap->h; y++)
	for ( int x=0; x<bitmap->w; x++)
	{
		int pix =*pixels;
		pixels++;
		if (pix!=0xffffff)
		{
			setPixel( x1+x, y1+y);
		}
	}
	return true;
#endif
}

/** @brief Set an image on screen.
 *  @param fname [in] Name of the image.
 *  @return true on success.
 */
bool Cgraphics::image(const std::string &fname, int x1, int y1, int x2, int y2)
{
#ifdef USE_SDL2
	SDL_Texture *bitmap =findImage( fname);
	if ( bitmap ==NULL || m_renderer ==NULL)
	{
		return false;
	}
	SDL_Rect dst;
	dst.x =x1;
	dst.y =y1;
	dst.w =x2-x1;
	dst.h =y2-y1;
	SDL_RenderCopy( m_renderer, bitmap, NULL, &dst);
#else
	sdlTexture *bitmap =findImage( fname);
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
#endif
	return true;
}

/**
 * @brief Add a texture to my view at a cetain position.
 * @param texture [in] What texture to insert
 * @param rect [in] Destination rectangle
 */
void Cgraphics::setPixelOffset( int x, int y )
{
	m_pixelOffset =Cpoint( x,y );
}

/**
 * @brief Where to render to.
 * @param texture [in] Destination for lines, images, painting instructions etc.
 * @return true on success
 */
bool Cgraphics::setRenderArea( sdlTexture *texture )
{
#ifdef USE_SDL2
	SDL_SetRenderTarget( m_renderer, texture);
#else
	if ( !texture)
	{
		m_renderSurface =m_allocatedSurface;
	}
	else
	{
		m_renderSurface =texture;
	}
	m_pixels =(Uint32*)m_renderSurface->pixels;
#endif
	return true;
}

int Cgraphics::bitsPerPixel()
{
#ifdef USE_SDL2
	return 24;
#else
	return m_renderSurface ? m_renderSurface->format->BitsPerPixel:0;
#endif
}

#ifdef USE_SDL2
/** @brief Insert a texture on our image.
 *  @param surface [in] What to insert.
 *  @param
 */
bool Cgraphics::renderTexture( SDL_Texture *texture, int x1, int y1)
{
	if ( texture)
	{
		SDL_Rect rect;
		Csize size = this->textureSize( texture);
		rect.h =size.height();
		rect.w =size.width();
		rect.x=x1;
		rect.y=y1;
		SDL_RenderCopy( m_renderer, texture, NULL, &rect);
		return true;
	}
	return false;
}

/** @brief Insert a texture on our image.
 *  @param graphics [in] Destination graph
 *  @param
 */
bool Cgraphics::renderGraphics( Cgraphics *graphics, const Crect &rect)
{
	if ( graphics)
	{
		SDL_Rect dst;
		Csize size = this->textureSize( m_texture);
		setRenderArea( graphics->m_texture);
		dst.h =rect.height();
		dst.w =rect.width();
		dst.x=rect.left();
		dst.y=rect.top();
		SDL_RenderCopy( m_renderer, m_texture, NULL, &dst);
		setRenderArea();
		return true;
	}
	return false;
}

/** @brief Insert a texture on our image.
 *  @param surface [in] What to insert.
 *  @param
 */
bool Cgraphics::renderTexture( SDL_Texture *texture, int x1, int y1, int w, int h)
{
	if ( texture)
	{
		SDL_Rect rect;
		rect.h =h;
		rect.w =w;
		rect.x=x1;
		rect.y=y1;
		SDL_RenderCopy( m_renderer, texture, NULL, &rect);
		return true;
	}
	return false;
}
#endif

/** @brief Insert a surface on our image.
 *  @param surface [in] What to insert.
 *  @param
 */
bool Cgraphics::renderSurface( SDL_Surface *surface, int x1, int y1)
{
#ifdef USE_SDL2
	SDL_Texture *texture =SDL_CreateTextureFromSurface( m_renderer, surface);
	if ( texture)
	{
		SDL_Rect rect;
		rect.h =surface->h;
		rect.w =surface->w;
		rect.x=x1;
		rect.y=y1;
		SDL_RenderCopy( m_renderer, texture, NULL, &rect);
		SDL_DestroyTexture( texture);
		return true;
	}
	return false;
#else
	if ( surface ==NULL)
	{
		return false;
	}
	SDL_Rect dst;
	dst.x =(Sint16)(x1+m_pixelOffset.x);
	dst.y =(Sint16)(y1+m_pixelOffset.y);
	dst.w =(Uint16)surface->w;
	dst.h =(Uint16)surface->h;
	SDL_BlitSurface( surface, NULL, m_renderSurface, &dst);
#endif
	return true;
}

/** @brief Insert a surface onto the background.
 *  @param surface [in] What surface to insert.
 *  @param x [in] Left position
 *  @param y [in] Top position
 *  @param w [in] Width
 *  @param h [in] Height
 */
bool Cgraphics::renderSurface( SDL_Surface *surface, int x, int y, int w, int h)
{
#ifdef USE_SDL2
	SDL_Texture *texture =SDL_CreateTextureFromSurface( m_renderer, surface);
	if ( texture)
	{
		SDL_Rect rect;
		rect.h =h;
		rect.w =w;
		rect.x=(Sint16)(x+m_pixelOffset.x);
		rect.y=(Sint16)(y+m_pixelOffset.y);
		SDL_RenderCopy( m_renderer, texture, NULL, &rect);
		SDL_DestroyTexture( texture);
		return true;
	}
	return false;
#else
	if ( surface ==NULL)
	{
		return false;
	}
	SDL_Rect dst;
	dst.x =(Sint16)(x+m_pixelOffset.x);
	dst.y =(Sint16)(y+m_pixelOffset.y);
	dst.w =(Uint16)surface->w;
	dst.h =(Uint16)surface->h;
	SDL_BlitSurface( surface, NULL, m_renderSurface, &dst);
#endif
	return true;
}

void Cgraphics::settings( Sdefaults *settings)
{
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
}

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

/** @brief Set a pixel.
 *  @param x [in] Left position.
 *  @param y [in] Top position.
 */
void Cgraphics::darkenPixel(int x, int y, double part)
{
#ifdef USE_SDL2
	(void)x;
	(void)y;
	(void)part;
	assert(0);
#else
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
	G =(int)(R*part);
	B =(int)(B*part);
	R =gLimit( R,0,255);
	G =gLimit( G,0,255);
	B =gLimit( B,0,255);
	p =B+(G<<8)+(R<<16);
    pixels[y * m_renderSurface->w + x] =p;
#endif
}

/** @brief Set a pixel.
 *  @param x [in] Left position.
 *  @param y [in] Top position.
 */
void Cgraphics::transparantPixel(int x, int y, double part)
{
#ifdef USE_SDL2
	setTransparant(part);
	setPixel(x,y);
#else
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
#endif
}

/** @brief Get pixel data.
 *	@param x [in] Left position.
 *	@param y [in] Top position.
 */
int Cgraphics::pixel(int x, int y)
{
#ifdef USE_SDL2
	(void)x;
	(void)y;
	assert(0);
	return 0;
#else
	if (y>=m_renderSurface->h || x>=m_renderSurface->w || x<0 || y<0)
	{
		return 0;
	}
    //assert( x<m_renderSurface->w && x>=0);
    //assert( y<m_renderSurface->h && y>=0);

	Uint32 *pixels = (Uint32 *) m_pixels; //m_renderSurface->pixels;
	return pixels[y * m_renderSurface->w + x];
#endif
}

/** @brief Darken a certain pixel.
 *  @param x [in] Left position.
 *  @param y [in] Top position.
 */
void Cgraphics::darken(int x, int y)
{
#ifdef USE_SDL2
	(void)x;
	(void)y;
	assert(0);
#else
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
#endif
}

/** @brief Tell where we are allowed to paint inside the graphics display
 *  @param rect [in] Location. NULL is remove clipping
 */
void Cgraphics::setViewport( SDL_Rect *rect)
{
#ifdef USE_SDL2
	SDL_RenderSetClipRect( m_renderer, rect);
#else
// Tell where we can paint.
	SDL_SetClipRect( m_allocatedSurface, rect);
#endif
}

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
	assert(0);
//	if ( this!=m_mainGraph)
//	{
//		m_mainGraph->darken( x1,y1,x2,y2);
//		return;
//	}
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

/** @brief Push a graphic onto stack.
 */
bool Cgraphics::push_back()
{
#ifdef USE_SDL2
	assert(0);
#else
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
#endif
	return true;
}

/** @brief Pop a graphic plane from stack (not used now).
 */
bool Cgraphics::pop_back()
{
#ifdef USE_SDL2
	assert(0);
#else
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
#endif
	return true;
}

/// @brief Get front screen without remove it from stack.
bool Cgraphics::front()
{
#ifdef USE_SDL2
	assert(0);
#else
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
#endif
	return true;
}

void Cgraphics::setRenderArea()
{
#ifdef USE_SDL2
	SDL_SetRenderTarget( m_renderer, m_texture);
#endif
}

void Cgraphics::cleardevice()
{
#ifdef USE_SDL2
	//setRenderArea(NULL);
    //SDL_SetRenderDrawColor( m_renderer, m_R, m_G, m_B, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);
#endif
}

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

/** @brief Paint an image.
 *  @param image [in] Image to draw.
 *  @param x1 [in] Left position.
 *  @param y1 [in] Top position.
 *  @param x2 [in] Right position.
 *  @param y2 [in] Bottom position.
 */
void Cgraphics::imageLine( const std::string &image, int x1, int y1, int x2, int y2)
{
#ifdef USE_SDL2
	sdlTexture *bitmap =findImage( image);
	if ( bitmap ==NULL || m_renderer ==NULL)
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
	SDL_RenderCopy( m_renderer, bitmap, &src, &dst);
#else
	SDL_Surface *bitmap =findImage( image);
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
#endif
}

/** @brief Paint line to graphics.
 *  @param dx [in] relative X-position.
 *  @param dy [in] relative Y-position.
 */
void Cgraphics::linerel(int dx, int dy)
{
	line(m_cx, m_cy, m_cx + dx, m_cy + dy);
}

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
#ifdef USE_SDL2
		SDL_RenderFillRect( m_renderer, &sr);
		sr.x =(Sint16)(right-v-sr.w);
		SDL_RenderFillRect(m_renderer, &sr);
		sr.y =(Sint16)(bottom-x-1);
		SDL_RenderFillRect(m_renderer, &sr);
		sr.x =(Sint16)(left+v);
		SDL_RenderFillRect(m_renderer, &sr);
#else
		SDL_FillRect(m_renderSurface, &sr, m_colour);
		sr.x =(Sint16)(right-v-sr.w);
		SDL_FillRect(m_renderSurface, &sr, m_colour);
		sr.y =(Sint16)(bottom-x-1);
		SDL_FillRect(m_renderSurface, &sr, m_colour);
		sr.x =(Sint16)(left+v);
		SDL_FillRect(m_renderSurface, &sr, m_colour);
#endif
	}
}


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

void Cgraphics::moveto(int x, int y)
{
#ifdef USE_SDL2
	(void)x;
	(void)y;
	assert(0);
#else
	m_cx = gMin(m_renderSurface->w, x);
	m_cy = gMin(m_renderSurface->h, y);
	m_cx = gMax(x, 0);
	m_cy = gMax(0, y);
#endif
}

void Cgraphics::moverel(int dx, int dy)
{
	m_cx += dx;
	m_cy += dy;
	moveto(m_cx, m_cy);
}

void Cgraphics::mapword(int x1, int x2, int y)
{
#ifdef USE_SDL2
	(void)x1;
	(void)x2;
	(void)y;
	assert(0);
#else
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
#endif
}

void Cgraphics::mapvword(int x, int y1, int y2)
{
#ifdef USE_SDL2
	(void)x;
	(void)y1;
	(void)y2;
	assert(0);
#else
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
#endif
}
