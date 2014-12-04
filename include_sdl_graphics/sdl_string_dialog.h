/*============================================================================*/
/**  @file      sdl_string_dialog.h
 **  @ingroup   user_interface
 **  @brief		Edit string dialog.
 **
 **  Edit a string, e.g. to enter a table name.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CstringDialog
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 10 aug 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <string>
#include "sdl_dialog.h"
#include "sdl_button.h"
#include "keyboard.h"
#include "string_edit.h"
#include "pinyin.h"
#include "sql_menu_translation_iterator.h"
#include "sdl_hand_writer.h"
#include "symbol_choose_dialog.h"

#define		STRING_EDIT_CHINESE  	1 ///< Edit chinese.
#define 	STRING_EDIT_LOCAL		2 ///< Edit local language.

/** @brief What type of layout.
 */
typedef enum
{
	LAYOUT_LOCAL,   		///< Local characters layout a..z,
	LAYOUT_LOCAL_CAPITAL, 	///< Local characters layout A..Z,
	LAYOUT_RADICAL,			///< Radicals layout.
	LAYOUT_PINYIN,			///< Write Pinyin layout.
	LAYOUT_HANDWRITING,		///< Handwriting layout.
	LAYOUT_FOOD1,			///< Food method layout per character.
	LAYOUT_FOOD2			///< Food method per word.
} Elayout;

#define STR_DLG_HEIGHT  60 ///< Height of the string dialog.
#define STR_DLG_WIDTH   72 ///< Width of the string dialog.
#define STRING_DIALOG_RECT   Crect(14,8, STR_DLG_WIDTH, STR_DLG_HEIGHT) ///< Rectangle string dialog.

/// @brief Class to ask a string.
class CstringDialog : public Cdialog
{
public:
	CstringDialog( int maxChars, int w, int h,  int flags);
	virtual Estatus onButton( SDLMod mod, keybutton sym);
	virtual ~CstringDialog();
	virtual bool onInit(const std::string &string, int flags);
	virtual void onUpdate();
	virtual void onPaint();
	virtual void onCleanup();
	keybutton key();
	std::string getString() { return m_string; }
	std::string getString2() { return m_string2; }
	virtual bool valid() { return m_valid; }
	void insertChinese( const std::string &key);
	void replaceChinese( const std::string &key);
	void setLayout( Elayout newLayout);
	void setTone( int tone);
	void setStringLength( int width) { m_max_string_width =width; }
	virtual void onDrag( const Cpoint &position);
	virtual void onDragRelease( const Cpoint &position, CdialogObject * movingObject);
	virtual void touchPress( const Cpoint &p) { onDrag(p); }
	void setHint( const std::string &hint) { m_hint =hint; }
	void handleBackspace();

private:
	void paintLocal();
	void paintWords();
	void touchRadical( int radical);
	void findPinyinSymbols();
	void findFoodSymbols();
	void findFoodStrings();
	void handleInput( SDLMod mod, keybutton c);
	void onClearScreen();
	void createTranslationNames();

private:
	std::string		m_string;
	std::string		m_string2;
	std::string		m_hint;
	std::string		m_pinyinString;
	int				m_flags;
	bool			m_show;
	bool			m_amountKey;
	bool			m_returnAtZero;
	bool			m_capital;
	bool			m_ascii;
	bool			m_show0to9;
	keybutton		m_lastKey;
	bool			m_valid;
	int				m_edit_width;
	int				m_edit_height;
	int				m_max_string_width;
	int				m_position_left;
	int				m_cursor;
	char			m_radicals[8192][6];
	bool			m_local; ///< Can we add local characters?
	bool			m_chinese; ///< Can we add chinese characters?
	Cimage			m_goLeft; ///< Go to left.
	Cimage			m_goRight; ///< Go to right.
	Cimage			m_erase; ///< Erase the character.
	CkeyboardButton	m_clear; ///< Clear all
	Cbutton			m_ok;		///< Button for ok.
	Cbutton			m_esc;		///< Button to stop.
	Cbutton			m_shift; ///< Shift button.
	Cbutton			m_hand; ///< Handwriting.
	Cbutton			m_pinyin; ///< Pinyin input.
	Cbutton			m_food; ///< Food input method button.
	Cbutton			m_radical; ///< Radical input method.
	Ckeyboard		*m_keyboard; ///< Keyboard to use.
	//int				m_radicalsCursor; ///< Cursor inside the radicals.
	Elayout			m_layout; ///< What layout do we display.
	Cimage			m_tone1; ///< Tone 1.
	Cimage			m_tone2; ///< Tone 2.
	Cimage			m_tone3; ///< Tone 3.
	Cimage			m_tone4; ///< Tone 4.
	int				m_selectedTone; ///< Tone selected.
	Cbutton			m_searchString; ///< Editor for string.
	Cimage			m_gum; ///< Gum for erasing.
	Cpinyin			m_pinyinMethod; ///< Search method.
	std::string		m_foundSymbols; ///< All found symbols.
	Cimage			m_moveUp; ///< Up button.
	Cimage			m_moveDown; ///< Down button.
	int 			m_symbolsHorizontal; ///< Number of icons horizontal.
	int				m_symbolsVertical; ///< Number of icons vertical.
	CsqlMenuTranslationIterator m_translations; ///< Translations to use.
	ChandWriter		m_handWriter;
	CsymbolChooseDialog m_radicalList; ///< List of all radicals.
	CsymbolChooseDialog m_symbolList; ///< List of output icons.
};
