/*============================================================================*/
/**  @file      sdl_checkbox.h
 **  @ingroup   user_interface
 **  @brief		Draw buttons.
 **
 **  Create and show buttons.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Ccheckbox
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
#include <string>
#include <memory>
#include "sdl_button.h"

/// @brief  Create and display buttons.
class Ccheckbox : public Cbutton
{
public:
	Ccheckbox( Cdialog *parent,
			 const Crect &rect,
			 keybutton code,
			 textId id=INVALID_TEXT_ID,
			 EborderType border=BORDER_THIN_SHADOW,
			 EfillType fill=FILL_UNICOLOURED,
			 const std::string &icon="");
	Ccheckbox( Cdialog *parent,
			 const Crect &rect,
			 keybutton code,
			 const std::string &text,
			 EborderType border=BORDER_THIN_SHADOW,
			 EfillType fill=FILL_UNICOLOURED,
			 const std::string &icon="");
	Ccheckbox( Cdialog *parent,
			 const Crect &rect,
			 keybutton code,
			 const Sfont &font,
			 textId id=INVALID_TEXT_ID,
			 EborderType border=BORDER_THIN_SHADOW,
			 Egravity gravity=GRAVITY_CENTER,
			 int radius=6,
	         EfillType fill=FILL_UNICOLOURED);
	Ccheckbox( Cdialog *parent,
			 const Crect &rect,
			 keybutton code,
			 const Sfont &font,
			 const std::string &text="",
			 EborderType border=BORDER_THIN_SHADOW,
			 Egravity gravity=GRAVITY_CENTER,
			 int radius=6,
			 EfillType fill=FILL_UNICOLOURED);
	virtual ~Ccheckbox();

public:
	bool isChecked() { return m_checked; }
	void setChecked(bool state) { m_checked=state; }
	void onPaint(int touch);

private:
	bool m_checked;
};


/// @brief  Create and display buttons.
class CbottomCheckbox : public CbottomButton
{
public:
	CbottomCheckbox( Cdialog *parent,
			 const Crect &rect,
			 keybutton code,
			 textId id=INVALID_TEXT_ID);
	CbottomCheckbox( Cdialog *parent,
			 const Crect &rect,
			 keybutton code,
			 const std::string &text);
	virtual ~CbottomCheckbox();

public:
	bool isChecked() { return m_checked; }
	void setChecked(bool state) { m_checked=state; }
	void onPaint(int touch);

private:
	bool m_checked;
};

typedef std::shared_ptr<Ccheckbox>  CcheckboxPtr;
typedef std::shared_ptr<CbottomCheckbox>  CbottomCheckboxPtr;
