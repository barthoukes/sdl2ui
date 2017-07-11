/*============================================================================*/
/**  @file       demo.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Low level audio
 **
 **  Demo for sdl2ui
 **
 **  @author     mensfort
 **
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2011, 2014, 2015
 ** Houkes Horeca Applications
 **
 ** This file is part of the SDL2UI Demo.  This library is free
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
#include <progress_dlg.h>
#include <unistd.h>
#include <memory>
#include "SDL.h"
#include "view_main_dialog.h"
#include "sdl_graphics.h"
#include "sdl_world.h"
#include "timeout.h"
#include "lingual.h"
#include "image_dlg.h"
#include "scroll_horizontal_dlg.h"
#include "scroll_vertical_dlg.h"
#include "button_dlg.h"
#include "check_box_dlg.h"
#include "swype_2d_vertical_dlg.h"
#include "swype_2d_horizontal_dlg.h"
#include "hand_writer_dlg.h"
#include "number_dlg.h"
#include "edit_dlg.h"
#include "drag_dlg.h"
#include "message_dlg.h"
#include "font_dlg.h"
#include "audio_dlg.h"
#include "bar_dlg.h"

bool mainLoop( Iworld *world);

// Graphical defaults
Sdefaults defaults;

/*--Bart Houkes-------------------------------------------------------------*/



void get_font( const std::string &index,
		       std::string &localName,
		       std::string &chineseName,
		       int &localPixels,
		       int &chinesePixels,
			   bool relative)
{
	// Here you can create all the fonts for local, chinese language and pixel sizes
	localName ="Ubuntu-M.ttf";
	chineseName ="BabelStoneHan.ttf";
	localPixels =16;
	chinesePixels =24;
	if (index == "title")
	{
		localPixels =24;
		chinesePixels =32;
	}
	else if (index == "alias")
	{
		localPixels =16;
	}
	else if (index == "small_button")
	{
		localPixels =20;
		chinesePixels =20;
	}
	relative =true;
}

void createSettings()
{

	int x=512,y=320;

	defaults.width  =x;
	defaults.height =y;

	defaults.full_screen_image_background ="";

	defaults.header_text =COLOUR_YELLOW;
	defaults.header_background1 =COLOUR_DARKBLUE;
	defaults.header_background2 =Cgraphics::brighter( COLOUR_DARKBLUE, -20);

	// Colours
	defaults.text_button_text =COLOUR_WHITE;
	defaults.text_button_background1 =COLOUR_CYAN;
	defaults.text_button_background2 =COLOUR_LIGHT_CYAN;
	defaults.text_button_border =COLOUR_BORDEAUX;
	defaults.button_text =COLOUR_WHITE;
	defaults.button_background1 =COLOUR_LIGHTBLUE;
	defaults.button_background2 =COLOUR_LIGHT_CYAN;
	defaults.menu_button_text =COLOUR_WHITE;
	defaults.menu_button_background1 =COLOUR_BORDEAUX;
	defaults.menu_button_background2 =COLOUR_DARKGRAY;
    defaults.button_shadow =COLOUR_DARKGRAY;
    defaults.image_button_text =COLOUR_BLACK;
    defaults.line_dark =COLOUR_DARKMAGENTA;
    defaults.line_bright =COLOUR_WHITE;
    defaults.function_button_background1 =COLOUR_DARKBLUE;
    defaults.function_button_background2 =COLOUR_BLUE;
    defaults.function_button_text =COLOUR_BLACK;
    defaults.matrix_header_text =COLOUR_BLACK;
    defaults.matrix_header_background1 =COLOUR_BORDEAUX;
    defaults.matrix_header_background2 =COLOUR_LIGHTGREY;
    defaults.graph_text =COLOUR_BLACK;
    defaults.graph_background1 =COLOUR_DARKGREEN;
    defaults.graph_background2 =COLOUR_LIGHTGREEN;
    defaults.calculator_text =0xffffff;
	defaults.calculator_background1 =0xdd0000;
	defaults.calculator_background2 =0xcc0000;
	defaults.calculator_border =0x0;
	defaults.background =0xcccccc;
	defaults.handwriting_background =0x000000;
	defaults.item_text =0xffffff;
	defaults.label_background =0x0266b4;
	defaults.label_text =0xffffff;
	defaults.messagebox_text =0x444444;
	defaults.messagebox_background1 =0xaaaaaa;
	defaults.messagebox_background2 =0x888888;
	defaults.slider_background =0xffffff;
	defaults.slider_lines =0x00000;
	// Audio
	defaults.audio_popup ="";
	//Images
	defaults.image_button_background ="";
	defaults.icon_slider48 ="";
	defaults.icon_ok48 ="enter*.png";
	defaults.icon_cancel48 ="cancel*.png";
	defaults.icon_printer48 ="";
	defaults.button_height =6;
	// Timers
	defaults.messagebox_time =500;
	// Options
	defaults.drag_start_pixels =30;
	defaults.repeat_speed =50;
	defaults.repeat_delay =3000;
	defaults.minimum_drag_time =2500;
	defaults.minimum_click_time =10;
	defaults.enable_record_keyfile =false;
	defaults.swype_friction =50;
	defaults.show_labels =true;
	defaults.full_screen =false;
	defaults.handwriting_detection_enabled =false;
	defaults.country =0;
	// Functions
	defaults.next_language =NULL;
	defaults.find_text_id =NULL;
	defaults.get_translation =find_some_translation;
	defaults.get_test_event =NULL;
	defaults.find_colour =NULL;
	defaults.get_font=get_font;
	defaults.start_from_main=NULL;
	defaults.log =NULL;
	// Path
	defaults.data_path ="../";
	defaults.font_path="../font/";
	defaults.image_path ="../images/";
}

/** @brief Main loop running all the dialogs.
 *  @return true The PC should shutdown.
 */
bool mainLoop( Iworld *world)
{
	bool retVal =false;
	int screen;
	CmainDialog mainDlg(world);
	CimageDlg imageDlg(world);
	CscrollHorizontalDlg scrollHorizontalDlg(world);
	CscrollVerticalDlg  scrollVerticalDlg(world);
	CbuttonDlg buttonDlg(world);
	CcheckBoxDlg checkBoxDlg(world);
	Cswype2DverticalDlg  swype2Dvertical(world);
	Cswype2DhorizontalDlg swype2Dhorizontal(world);
	ChandWriterDlg  handwriterDlg(world);
	CnumberDlg  numberDlg(world);
	CeditDlg	editDlg(world);
	CdragDlg    dragDlg(world);
	CprogressDlg progressDlg(world);
	CmessageDlg	 messageDlg(world);
	CfontDlg	fontDlg(world);
	CaudioDlg	audioDlg(world);
	CbarDlg		barDlg(world);

	for (screen=1;screen!=0;)
	{
		switch (screen)
		{
		case 1:
			screen = mainDlg.onExecute( NULL);
			break;
		case 2:
			screen = imageDlg.onExecute( NULL);
			break;
		case 3:
			screen = scrollHorizontalDlg.onExecute( NULL);
			break;
		case 4:
			screen = scrollVerticalDlg.onExecute( NULL);
			break;
		case 5:
			screen = buttonDlg.onExecute( NULL);
			break;
		case 6:
			screen = checkBoxDlg.onExecute( NULL);
			break;
		case 7:
			screen = swype2Dvertical.onExecute( NULL);
			break;
		case 8:
			screen = swype2Dhorizontal.onExecute( NULL);
			break;
		case 9:
			screen = handwriterDlg.onExecute( NULL);
			break;
		case 10:
			screen = numberDlg.onExecute( NULL);
			break;
		case 11:
			screen = editDlg.onExecute( NULL);
			break;
		case 12:
			screen = dragDlg.onExecute( NULL);
			break;
		case 13:
			screen = progressDlg.onExecute( NULL);
			break;
		case 14:
			screen = messageDlg.onExecute( NULL);
			break;
		case 15:
			screen = fontDlg.onExecute( NULL);
			break;
		case 16:
			screen = audioDlg.onExecute( NULL);
			break;
		case 17:
			screen = barDlg.onExecute( NULL);
			break;
		default:
			screen =0;
			break;
		}
	}
	return retVal;
}

/*============================================================================*/
///
///  @brief 	Start program.
///
///  @param		argc [in] Start # parameters.
///  @param		argv [in] Start parameters.
///
///  @post      Program stopped.
///
/*============================================================================*/
int main(int argc, char *argv[])
{
	std::string argv_str(argv[0]);
	std::string base = argv_str.substr(0, argv_str.find_last_of("/"));
	base = argv_str.substr(0, base.find_last_of("/"));
    createSettings();
    TTF_Init();
    std::shared_ptr<Cgraphics> mainGraph =std::shared_ptr<Cgraphics>(new Cgraphics( Csize(defaults.width, defaults.height), true));
    defaults.image_path = base + "/images/";
    defaults.font_path = base + "/font/";
    defaults.data_path = base + "/data/";
    mainGraph->settings(&defaults);
    mainGraph->init();
    Cworld world(mainGraph);
    world.init();

    mainLoop(&world);
	return 0;
}


/* End file */
