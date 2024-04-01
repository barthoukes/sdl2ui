/*
 * paste_buffer.cpp
 *
 *  Created on: 5 jan 2022
 *      Author: mensfort
 */

#include <stdio.h>

#include "paste_buffer.hpp"

CpasteBuffer::CpasteBuffer()
{
}

CpasteBuffer::~CpasteBuffer()
{}

std::string CpasteBuffer::getPaste()
{
	Display *display = XOpenDisplay(NULL);
	unsigned long color = BlackPixel(display, DefaultScreen(display));
	Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0,0, 1,1, 0, color, color);
	const std::string retVal = getString(display, window, "CLIPBOARD", "UTF8_STRING");  // or STRING
	XDestroyWindow(display, window);
	XCloseDisplay(display);
	return retVal;
}

std::string CpasteBuffer::getString(Display *display, Window window, const char *bufname, const char *fmtname)
{
	std::string retVal;
	char *result;
	unsigned long ressize, restail;
	int resbits;
	Atom bufid = XInternAtom(display, bufname, False),
	     fmtid = XInternAtom(display, fmtname, False),
	     propid = XInternAtom(display, "XSEL_DATA", False),
	     incrid = XInternAtom(display, "INCR", False);
	XEvent event;

	XConvertSelection(display, bufid, fmtid, propid, window, CurrentTime);
	do
	{
		XNextEvent(display, &event);
	} while (event.type != SelectionNotify || event.xselection.selection != bufid);

	if (event.xselection.property)
	{
	    XGetWindowProperty(display, window, propid, 0, LONG_MAX/4, False, AnyPropertyType,
	                       &fmtid, &resbits, &ressize, &restail, (unsigned char**)&result);

	    if (fmtid == incrid)
	    {
	    	// Buffer is too large and INCR reading is not implemented yet.
	    }
	    else
	    {
	    	retVal =result;
	    	//printf("%.*s", (int)ressize, result);
	    }
	    XFree(result);
	}
    return retVal;
}
