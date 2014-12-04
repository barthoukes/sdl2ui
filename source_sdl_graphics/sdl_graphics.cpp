/*============================================================================*/
/**  @file       graphics_sdl.cpp
 **  @ingroup    zhongcan_sdl
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
 **  Copyright (c) Bart Houkes, 7 mrt 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/
//#include <SDL/SDL_draw.h>
#include <stddef.h>
#include <assert.h>
#include <math.h>
#include "sdl_graphics.h"
#include <SDL_image.h>
#include "SDL_ttf.h"

#define SWAP(A,B,TYPE) {TYPE temp=A; A=B; B=temp;}

Cgraphics *m_mainGraph =NULL; ///< Used in graphics_widgets.c, icon.c, image_layer.c, CmessageBoxBTW.cpp

/** @brief Constructor for graphics to paint on.
 *  @param width [in] Width of graphics.
 *  @param height [in] Height of graphics.
 *  @param mainScreen [in] boolean, true for the main screen.
 * */
Cgraphics::Cgraphics( const Csize &size, bool mainScreen) :

#ifdef USE_SDL2
	m_window(NULL),
	m_renderer(NULL),
	m_texture(NULL),
#else
	m_surface(NULL),
#endif
    m_colour(0),
    m_topLayer(0),
	m_cx(0),
	m_cy(0),
	m_R(0),
	m_G(0),
	m_B(0),
	m_offset(0,0),
	m_size( size),
	m_init(false),
	m_mainScreen( mainScreen),
	m_lock_keycode(false),
	m_bits(32),
    m_option(0)
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
		m_texture =NULL;
	}
	if ( m_mainScreen)
	{
		SDL_Quit();
	}
#else
	if (m_surface)
	{
		SDL_FreeSurface(m_surface);
		m_surface = NULL;
	}
	//else
	if ( m_mainScreen)
	{
		SDL_Quit();
	}
#endif
}

/** @brief Initialise the graphics field.
 *  @param pixels [in] Should be 32 for each pixel.
 *  @param colour [in] Background colour. -1 for default.
 */
bool Cgraphics::init( int pixels, int colour)
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
		m_surface = SDL_SetVideoMode(m_size.width(), m_size.height(), pixels, options);
		//SDL_SetAlpha(m_surface, SDL_SRCALPHA, 0);
		m_init =true;
    	if ( m_option<2) SDL_WM_SetCaption( "Houkes Horeca Applications", NULL );
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

	    m_surface =SDL_CreateRGBSurface( options, m_size.width(), m_size.height(),
	    		                         ::m_mainGraph->m_surface->format->BitsPerPixel,
	                                     rmask, gmask, bmask, amask);
		SDL_Rect rect;
		rect.h =m_surface->h;
		rect.w =m_surface->w;
		rect.x =0;
		rect.y =0;
		SDL_FillRect( m_surface, &rect, colour);
		m_init =true;
	}

    return true;
}

/** @brief Lock the surface for painting. */
void Cgraphics::lock()
{
	//Log.write("Cgraphics::lock");
	if (m_init)
	{
		SDL_LockSurface( m_surface );
	}
}

/** @brief Unlock the surface for others to paint.
 */
void Cgraphics::unlock()
{
	//Log.write("Cgraphics::unlock");
	if (m_init)
	{
		SDL_UnlockSurface( m_surface );
	}
}

/** @brief Update the output graphics now */
void Cgraphics::update()
{
	//Log.write("Cgraphics::update");
	if (m_init && m_option<2)
	{
		//lock();
		//SDL_Flip( m_surface);
		//unlock();
		SDL_UpdateRect( m_surface,0,0,0,0);
	}
}

/** @brief Update the display now
 *  @param x1 [in] Left position to update
 *  @param y1 [in] Top position to update
 *  @param w [in] Width to update
 *  @param h [in] Height to update
 */
void Cgraphics::update( int x1, int y1, int w, int h)
{
	if (m_init && m_option<2)
	{
		//SDL_UpdateRect( m_surface,0,0,0,0);
		SDL_UpdateRect( m_surface,x1,y1,w,h);
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
	sr.x =x1;
	sr.y =y1;
	sr.w =width;
	sr.h =height;
	if ( !m_init)
	{
		return;
	}
	int col=SDL_MapRGB( m_surface->format, m_R, m_G, m_B);
	if ( radius ==0)
	{
		SDL_FillRect(m_surface, &sr, col);
		return;
	}
	for ( int x=0; x<=radius; x++)
	{
		//int v=(int)((double)radius*sqrt((double)(radius-x)/(double)radius)+0.5);
		int v=(int)( sqrt( (double)(radius*radius-(radius-x)*(radius-x))+0.5) );
		//int v2=(int)((double)radius*sqrt((double)x/(double)radius)+0.5);
		v =radius-v;
		sr.x =x1+v;
		sr.y =y1+x;
		sr.w =x2-x1-v-v;
		sr.h =1;
		SDL_FillRect(m_surface, &sr, col);
		sr.y =y2-1-x;
		SDL_FillRect(m_surface, &sr, col);
	}
	sr.y=y1+radius+1;
	height =y2-y1-2*radius-2;
	if ( height>0)
	{
		sr.h =height;
		SDL_FillRect( m_surface, &sr, col);
	}
}

/** @brief Set a new colour to paint with.
 *  @param col [in] Colour as in most software to use.
 */
void Cgraphics::setColour(colour col)
{
	m_colour =col;
	m_B = (col & 0x0000FF);
	m_G = (col & 0x00FF00) >> 8;
	m_R = (col & 0xFF0000) >> 16;
	//m_colour =(m_R<<8) + (m_B) + (m_G<<16);
}

/** @brief Set rectangle code to use.
 *  @param rect [in] Rectangle with key code.
 *  @param key_code [in] Code to use for rectangle.
 */
void Cgraphics::setCode( const Crect &rect, keybutton key_code)
/* Define a keyblock with the same key_code for input touchpanel            */
/* PC uses Touchbuttons, PX uses hardware interrupt and table               */
{
	if ( m_lock_keycode ==false)
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
SDL_Surface *Cgraphics::findImage( const std::string &fname)
{
	for ( int a=0; a<(int)m_images.size(); a++)
	{
		if ( fname==m_images[a].name)
		{
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
			c.image = IMG_Load( s.c_str() );
		}
		else
		{
			c.image = IMG_Load( c.name.c_str() );
		}
	}
	catch (...)
	{
		//Log.error( "Cgraphics::findImage  cannot load image %s", fname.c_str());
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
		SDL_FreeSurface(m_images[a].image);
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
			SDL_FreeSurface(m_images[a].image);
			m_images.erase( m_images.begin()+a);
			break;
		}
	}
}

/** @brief Set an image on screen.
 *  @param fname [in] Name of the image.
 *  @return true on success.
 */
bool Cgraphics::image(const std::string &fname, int x1, int y1, int x2, int y2)
{
	SDL_Surface *bitmap =findImage( fname);
	if ( bitmap ==NULL || m_surface ==NULL)
	{
		return false;
	}
	SDL_Rect dst;
	dst.x = x1;
	dst.y = y1;
	dst.w = x2 - x1;
	dst.h = y2 - y1;

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = bitmap->w; //x2-x1;
	src.h = bitmap->h; //y2-y1;
	SDL_BlitSurface( bitmap, &src, m_surface, &dst);
	//SDL_SoftStretch( bitmap, &src, m_surface, &dst);
	//clean();
	return true;
}

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
	dst.x =x1;
	dst.y =y1;
	dst.w =surface->w;
	dst.h =surface->h;
	//SDL_Surface *optimiser;
	//if (surface->flags & SDL_SRCALPHA)
	//        optimiser = SDL_DisplayFormatAlpha(surface);
	//    else
	//        optimiser = SDL_DisplayFormat(surface);

	//SDL_SetAlpha(surface,  SDL_SRCALPHA, SDL_ALPHA_TRANSPARENT);
	//SDL_SetAlpha(m_surface,  SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
	SDL_BlitSurface( surface, NULL, m_surface, &dst);
	//SDL_FreeSurface(optimiser);
	return true;
}

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
	src.x =surface.left();
	src.y =surface.top();
	src.w =surface.width();
	src.h =surface.height();
	SDL_Rect dst;
	dst.x =destination.left();
	dst.y =destination.top();
	dst.w =destination.width();
	dst.h =destination.height();
	//setColour(0);
	//bar(dst.x, dst.y, dst.x+dst.w, dst.y+dst.h, 0);
	//SDL_SetAlpha( graphics->m_surface,  SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
	//SDL_FillRect( m_surface, &dst, 0x004000);
	//SDL_FillRect( graphics->m_surface, &src, 0xff000060);
	//SDL_SetAlpha( graphics->m_surface, SDL_SRCALPHA, 255);
	//SDL_LockSurface(graphics->m_surface);
	//SDL_LockSurface(m_surface);
	//SDL_SetClipRect( m_surface, &dst);
	SDL_BlitSurface( graphics->m_surface, &src, m_surface, &dst);


	//SDL_UnlockSurface(m_surface);
	//SDL_UnlockSurface(graphics->m_surface);
	return true;
}

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
	dst.x =x;
	dst.y =y;
	dst.w =w;
	dst.h =h;
	//SDL_Surface *optimiser;
	//if (surface->flags & SDL_SRCALPHA)
	//        optimiser = SDL_DisplayFormatAlpha(surface);
	//    else
	//        optimiser = SDL_DisplayFormat(surface);

	//SDL_SetAlpha(surface,  SDL_SRCALPHA, SDL_ALPHA_TRANSPARENT);
	//SDL_SetAlpha(m_surface,  SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
	SDL_BlitSurface( surface, NULL, m_surface, &dst);
	//SDL_FreeSurface(optimiser);
	return true;
}

/** @brief Set a pixel.
 *  @param x [in] Left position.
 *  @param y [in] Top position.
 */
void Cgraphics::setPixel(int x, int y)
{
	if (y>=m_size.height() || x>=m_size.width() || x<0 || y<0)
	{
		return;
	}
    //assert( x<m_size.width() && x>=0);
    //assert( y<m_size.height() && y>=0);

	Uint32 *pixels = (Uint32 *) m_surface->pixels;
	pixels[y * m_size.width() + x] = m_colour;
}

/** @brief Get pixel data.
 *	@param x [in] Left position.
 *	@param y [in] Top position.
 */
int Cgraphics::pixel(int x, int y)
{
	if (y>=m_size.height() || x>=m_size.width() || x<0 || y<0)
	{
		return 0;
	}
    //assert( x<m_size.width() && x>=0);
    //assert( y<m_size.height() && y>=0);

	Uint32 *pixels = (Uint32 *) m_surface->pixels;
	return pixels[y * m_size.width() + x];
}

/** @brief Darken a certain pixel.
 *  @param x [in] Left position.
 *  @param y [in] Top position.
 */
void Cgraphics::darken(int x, int y)
{
	if (y>=m_size.height() || x>=m_size.width() || x<0 || y<0)
	{
		return;
	}
	Uint32 *pixels = (Uint32 *) m_surface->pixels;
	int col =pixels[y * m_size.width() + x];

	int B = (col & 0x0000FF)/2;
	int G = ((col & 0x00FF00) >> 8)/2;
	int R = ((col & 0xFF0000) >> 16)/2;
	int value =(R<<16) +(G<<8) + B;
	pixels[y * m_size.width() + x] =value;
}

/** @brief Make a piece of display more dark (to highlight the rest).
 *  @param x1 [in] Left position.
 *  @param y1 [in] Top position.
 *  @param x2 [in] Right position.
 *  @param y2 [in] Bottom position.
 */
void Cgraphics::darken( int x1, int y1, int x2, int y2)
{
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
}

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
 	Uint32 *pixels =(Uint32 *) m_surface->pixels;
 	memcpy( background, pixels, m_size.area()*sizeof(Uint32));
 	m_background.push_back(background);
	unlock();
	return true;
}

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
 	Uint32 *pixels =(Uint32 *) m_surface->pixels;
 	memcpy( pixels, background, m_size.area()*sizeof(Uint32));
	unlock();
	update();
	delete background;
	return true;
}

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
 	Uint32 *pixels =(Uint32 *) m_surface->pixels;
 	memcpy( pixels, background, m_size.area()*sizeof(Uint32));
	unlock();
	update();
	return true;
}

void Cgraphics::cleardevice()
{

}

/** @brief Paint any line.
 *  @param x1 [in] Left position start point.
 *  @param y1 [in] Top position start point.
 *  @param x2 [in] Left position end point.
 *  @param y2 [in] Top position end point.
 */
void Cgraphics::line(int x1, int y1, int x2, int y2)
{
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
	SDL_Surface *bitmap =findImage( image);
	if ( bitmap ==NULL || m_surface ==NULL)
	{
		return;
	}
	SDL_Rect dst;
	dst.x = x1;
	dst.y = y1;
	dst.w = x2 - x1;
	dst.h = y2 - y1;

	SDL_Rect src;
	src.x = x1;
	src.y = y1;
	src.w = x2 - x1;
	src.h = y2 - y1;
	SDL_BlitSurface( bitmap, &src, m_surface, &dst);
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
		//int v2=(int)((double)radius*sqrt((double)x/(double)radius)+0.5);
		v =radius-v;
		if ( x<edge)
		{
			sr.w =radius-v;
		}
		else
		{
			int v2=(int)( sqrt( (double)(radius2*radius2-(radius-x)*(radius-x))+0.5) );
			v2 =edge+radius2-v2;
			sr.w =(v2>v) ? v2-v:v-v2;
		}
		sr.x =left+v;
		sr.y =top+x;
		sr.h =1;
		SDL_FillRect(m_surface, &sr, m_colour);
		sr.x =right-v-sr.w;
		SDL_FillRect(m_surface, &sr, m_colour);
		sr.y =bottom-x-1;
		SDL_FillRect(m_surface, &sr, m_colour);
		sr.x =left+v;
		SDL_FillRect(m_surface, &sr, m_colour);
	}
}


void Cgraphics::ellipse(int xc, int yc, int stangle, int endangle,
		 int rx, int ry)
{
	float xold, yold, xnew, ynew;
	float sin_rxbyry, sin_rybyrx, costheta, theta;
	int num;

	theta = gMin(1.00 / (gMax(rx, ry)), 0.001);
	num = (int) rint(M_PI * (endangle - stangle) / 180.0 / theta);

	sin_rxbyry = (float) rx / ry * sin(theta);
	sin_rybyrx = (float) ry / rx * sin(theta);
	costheta = cos(theta);

	xold = rx * cos(M_PI * stangle / 180.0);
	yold = ry * sin(M_PI * stangle / 180.0);

	for (; num; num--)
	{
		xnew = xold * costheta - yold * sin_rxbyry;
		ynew = xold * sin_rybyrx + yold * costheta;
		setPixel((int) rint(xnew + xc), (int) rint(ynew + yc));
		xold = xnew, yold = ynew;
	}
}

void Cgraphics::moveto(int x, int y)
{
	m_cx = gMin(m_size.width(), x);
	m_cy = gMin(m_size.height(), y);
	m_cx = gMax(x, 0);
	m_cy = gMax(0, y);
}

void Cgraphics::moverel(int dx, int dy)
{
	m_cx += dx;
	m_cy += dy;
	moveto(m_cx, m_cy);
}

void Cgraphics::mapword(int x1, int x2, int y)
{
	if (x1<0 || x2<0 || y<0 || x1>=m_size.width() || x2>=m_size.width() || y>=m_size.height())
	{
		return;
	}
	assert( x1>=0 && x1<m_size.width());
	assert( x2>=0 && x2<m_size.width());
	assert( y>=0 && y<m_size.height());

	if ( x1>x2) SWAP(x1,x2,int);

	Uint32 *pixels = (Uint32 *) m_surface->pixels+y*m_size.width()+x1;

	while (x1<=x2)
	{
		*pixels =m_colour;
		pixels++;
		x1++;
	}
	return;
}

void Cgraphics::mapvword(int x, int y1, int y2)
{
	if (x<0 || y1<0 || y2<0 || x>=m_size.width() || y1>=m_size.height() || y2>=m_size.height() )
	{
		return;
	}
	assert( x>=0 && x<=m_size.width());
	assert( y1>=0 && y1<m_size.height());
	assert( y2>=0 && y2<m_size.height());

	if ( y1>y2) SWAP(y1,y2,int);

	Uint32 *pixels = (Uint32 *) m_surface->pixels+y1*m_size.width()+x;

	while (y1<=y2)
	{
		*pixels =m_colour;
		pixels+=m_size.width();
		y1++;
	}
	return;
}
