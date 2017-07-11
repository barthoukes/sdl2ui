/*============================================================================*/
/** @file       glue_tinyxml.cpp
 *  @ingroup    WTC
 *  @brief		XML wrapper to make life easy.
 *  
 *  Creation Date: Tue - 11 september 2007
 * 
 *  @author Barthelomeus Houkes  
 *
 *  @par Project:
 *  Security Systems: WTC
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
#include <string>
#include "tinyxml.h"

class CGLUE_tinyXML
{
public:
	CGLUE_tinyXML(void);
public:
	~CGLUE_tinyXML(void);
public:
public:
	void add( TiXmlElement *root, const char *group, const char *item, int value);
	void add( TiXmlElement *root, const char *group, const char *item, bool value);
	void add( TiXmlElement *root, const char *group, const char *item, const char *value);
	void add( TiXmlElement *root, const char *group, const char *item, std::string & value);
	void add( TiXmlElement *root, const char *item, const std::string &value);

	void add( TiXmlElement *root, const char *item, int value);
	void add( TiXmlElement *root, const char *item, long value);
	void add( TiXmlElement *root, const char *item, bool value);
	void add( TiXmlElement *root, const char *item, const char *value);
	void add( TiXmlElement *root, const char *item, std::string & value);
	TiXmlElement *add( TiXmlElement *root, const char *name);
	bool addComment( TiXmlElement *root, const char *comment);

	bool find( TiXmlHandle handle, const char *group, const char *item, char *value, int maxLength =256);
	bool find( TiXmlHandle handle, const char *group, const char *item, std::string & value);
	bool find( TiXmlHandle handle, const char *item, std::string & value);
	bool find( TiXmlHandle handle, const char *group, const char *item, int *value);
	bool find( TiXmlHandle handle, const char *group, const char *item, bool *value);
	bool find( TiXmlHandle handle, const char *group, const char *item, long *value);

	TiXmlHandle find( TiXmlHandle handle, const char *name, int index=0);
	bool find( TiXmlHandle handle, const char *item, char *value, int maxLength =256);
	bool find( TiXmlHandle handle, const char *item, int *value);
	bool find( TiXmlHandle handle, const char *item, long *value);
	bool find( TiXmlHandle handle, const char *item, bool *value);

public:
	TiXmlElement *fileCreate( const char *title);
public:
	TiXmlHandle fileOpen( const char *fileName, const char *title);
	bool fileSave( const char *fileName);

	TiXmlDocument *m_doc; ///< document.
	//TiXmlHandle *mm_root; ///< Root.
};

/*======-----==================================================================
**      end of file
**===========-----===========================================================*/
