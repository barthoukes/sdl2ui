/*============================================================================*/
/**  @file      keybutton.cpp
 **  @ingroup   source
 **  @brief		keybuttons and conversion.
 **
 **  Conversion of text to keyboard.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 15 dec 2012
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

#include "sdl_keybutton.h"

typedef struct
{
	keybutton key;
	std::string txt;
} Skey;

const Skey keys[]=
{
	{ 	KEY_ESCAPE		, "ESC" },
	{   KEY_UP    		, "UP" },
	{	KEY_CR				, "CR" },
	{	KEY_UP				, "UP" },
	{	KEY_DOWN			, "DOWN" },
	{	KEY_PAGEUP			, "PAGEUP" },
	{	KEY_PAGEDOWN		, "PAGEDOWN" },
	{	KEY_EQUALS			, "=" },
	{	KEY_F1				, "F1" },
	{	KEY_F2				, "F2" },
	{	KEY_F3				, "F3" },
	{	KEY_F4				, "F4" },
	{	KEY_F5				, "F5" },
	{	KEY_F6				, "F6" },
	{	KEY_F7				, "F7" },
	{	KEY_F8				, "F8" },
	{	KEY_F9				, "F9" },
	{	KEY_F10				, "F10" },
	{	KEY_F11				, "F11" },
	{	KEY_F12				, "F12" },
	{	KEY_QUESTION		, "?" },
	{	KEY_AMPERSAND		, "&" },
	{	KEY_HASH			, "HASH" },
	{	KEY_BIGGER			, ">" },
	{	KEY_SMALLER			, "<" },
	{	KEY_BRACKET_OPEN	, "[" },
	{	KEY_BRACKET_CLOSE	, "]" },
	{	KEY_EXCLAMATION		, "!" },
	{	KEY_0				, "0" },
	{	KEY_1				, "1" },
	{	KEY_2				, "2" },
	{	KEY_3	, "3" },
	{	KEY_4	, "4" },
	{	KEY_5	, "5" },
	{	KEY_6	, "6" },
	{	KEY_7	, "7" },
	{	KEY_8	, "8" },
	{	KEY_9	, "9" },
	{	KEY_AT	, "AT" },
	{	KEY_A	, "A" },
	{	KEY_B	, "B" },
	{	KEY_C	, "C" },
	{	KEY_D	, "D" },
	{	KEY_E	, "E" },
	{	KEY_F	, "F" },
	{	KEY_G	, "G" },
	{	KEY_H	, "H" },
	{	KEY_I	, "I" },
	{	KEY_J	, "J" },
	{	KEY_K	, "K" },
	{	KEY_L	, "L" },
	{	KEY_M	, "M" },
	{	KEY_N	, "N" },
	{	KEY_O	, "O" },
	{	KEY_P	, "P" },
	{	KEY_Q	, "Q" },
	{	KEY_R	, "R" },
	{	KEY_S	, "S" },
	{	KEY_T	, "T" },
	{	KEY_U	, "U" },
	{	KEY_V	, "V" },
	{	KEY_W	, "W" },
	{	KEY_X	, "X" },
	{	KEY_Y				, "Y" },
	{	KEY_Z				, "Z" },
	{	KEY_BACKSPACE		, "BACKSPACE" },
	{	KEY_LEFT_ALT		, "LEFT_ALT" },
	{	KEY_RIGHT_ALT		, "RIGHT_ALT" },
	{	KEY_RIGHT_ALT		, "PORTION" },
	{	KEY_CONTROL			, "CONTROL" },
	{	KEY_SHIFT_F1		, "SHIFT_F1" },
	{	KEY_SHIFT_F2		, "SHIFT_F2" },
	{	KEY_SHIFT_F3	, "SHIFT_F3" },
	{	KEY_SHIFT_F4	, "SHIFT_F4" },
	{	KEY_SHIFT_F5	, "SHIFT_F5" },
	{	KEY_SHIFT_F6	, "SHIFT_F6" },
	{	KEY_SHIFT_F7	, "SHIFT_F7" },
	{	KEY_SHIFT_F8	, "SHIFT_F8" },
	{	KEY_SHIFT_F9	, "SHIFT_F9" },
	{	KEY_SHIFT_F10	, "SHIFT_F10" },
	{	KEY_SHIFT_F11	, "SHIFT_F11" },
	{	KEY_SHIFT_F12	, "SHIFT_F12" },
	{	KEY_TABLE		, "TABLE" },
	{	KEY_GROUP_1		, "GROUP_1" },
	{	KEY_GROUP_2		, "GROUP_2" },
	{	KEY_GROUP_3		, "GROUP_3" },
	{	KEY_GROUP_4		, "GROUP_4" },
	{	KEY_GROUP_5		, "GROUP_5" },
	{	KEY_GROUP_6		, "GROUP_6" },
	{	KEY_GROUP_7		, "GROUP_7" },
	{	KEY_GROUP_8		, "GROUP_8" },
	{	KEY_GROUP_9		, "GROUP_9" },
	{	KEY_GROUP_10	, "GROUP_10" },
	{	KEY_GROUP_11	, "GROUP_11" },
	{	KEY_GROUP_12	, "GROUP_12" },
	{	KEY_GROUP_13	, "GROUP_13" },
	{	KEY_GROUP_14	, "GROUP_14" },
	{	KEY_GROUP_15	, "GROUP_15" },
	{	KEY_GROUP_16	, "GROUP_16" },
	{	KEY_GROUP_17	, "GROUP_17" },
	{	KEY_GROUP_18	, "GROUP_18" },
	{	KEY_GROUP_19	, "GROUP_19" },
	{	KEY_GROUP_20	, "GROUP_20" },
	{	KEY_GROUP_21	, "GROUP_21" },
	{	KEY_GROUP_22	, "GROUP_22" },
	{	KEY_GROUP_23	, "GROUP_23" },
	{	KEY_GROUP_24	, "GROUP_24" },
	{	KEY_ITEM_1		, "ITEM_1" },
	{	KEY_ITEM_2		, "ITEM_2" },
	{	KEY_ITEM_3		, "ITEM_3" },
	{	KEY_ITEM_4		, "ITEM_4" },
	{	KEY_ITEM_5		, "ITEM_5" },
	{	KEY_ITEM_6		, "ITEM_6" },
	{	KEY_ITEM_7		, "ITEM_7" },
	{	KEY_ITEM_8		, "ITEM_8" },
	{	KEY_ITEM_9		, "ITEM_9" },
	{	KEY_ITEM_10		, "ITEM_10" },
	{	KEY_ITEM_11		, "ITEM_11" },
	{	KEY_ITEM_12		, "ITEM_12" },
	{	KEY_ITEM_13		, "ITEM_13" },
	{	KEY_ITEM_14		, "ITEM_14" },
	{	KEY_ITEM_15		, "ITEM_15" },
	{	KEY_ITEM_16		, "ITEM_16" },
	{	KEY_ITEM_17		, "ITEM_17" },
	{	KEY_ITEM_18		, "ITEM_18" },
	{	KEY_ITEM_19		, "ITEM_19" },
	{	KEY_ITEM_20		, "ITEM_20" },
	{	KEY_ITEM_21		, "ITEM_21" },
	{	KEY_ITEM_22		, "ITEM_22" },
	{	KEY_ITEM_23		, "ITEM_23" },
	{	KEY_ITEM_24		, "ITEM_24" },
	{	KEY_ITEM_25		, "ITEM_25" },
	{	KEY_ITEM_26		, "ITEM_26" },
	{	KEY_ITEM_27		, "ITEM_27" },
	{	KEY_ITEM_28		, "ITEM_28" },
	{	KEY_ITEM_29		, "ITEM_29" },
	{	KEY_ITEM_30		, "ITEM_30" },
	{	KEY_RSHIFT		, "RSHIFT" },
	{	KEY_HOME		, "HOME" },
	{	KEY_END			, "END" },
	{	KEY_SPACE		, "SPACE" },
	{	KEY_SEMICOLON	, "SEMICOLON" },
	{	KEY_TILDE		, "TILDE" },
	{	KEY_COMMA		, "COMMA" },
	{	KEY_STAR		, "STAR" },
	{	KEY_STAR		, "*" },
	{	KEY_FORWARDSLASH, "FORWARDSLASH" },
	{	KEY_PLUS		, "+" },
	{	KEY_PLUS		, "PLUS" },
	{	KEY_MINUS		, "-" },
	{	KEY_MINUS		, "MINUS" },
	{	KEY_DOT			, "." },
	{	KEY_DOT			, "DOT" },
	{	KEY_FORWARD_SLASH, "/" },
	{	KEY_FORWARD_SLASH, "FORWARD_SLASH" },

	{	KEY_LEFT		, "LEFT" },
	{	KEY_RIGHT		, "RIGHT" },
	{	KEY_TOTAL		, "TOTAL" },
	{	KEY_EXAMPLE		, "EXAMPLE" },
	{	KEY_STOP		, "STOP" },
	{	KEY_QUIT		, "QUIT" },
	{	KEY_MORE		, "MORE" },
	{	KEY_UNDO		, "UNDO" },
	{	KEY_YES			, "YES" },
	{	KEY_NO			, "NO" },
	{	KEY_CANCEL		, "CANCEL" },
	{	KEY_TELEPHONE	, "TELEPHONE" },
	{	KEY_DIRECTION	, "DIRECTION" },
	{ 	KEY_NUMBER		, "-" },
	{	KEY_AMOUNT		, "AMOUNT" },
	{ 	KEY_DISCOUNT	, "DISCOUNT" },
	{	KEY_OLD_BILL	, "OLD_BILL" },
	{	KEY_LAST		, "LAST" },
	{	KEY_CORR		, "CORR" },
	{	KEY_CHN_BILL	, "CHN_BILL" },
	{	KEY_BILL		, "BILL" },
	{	KEY_BTW			, "B" },
	{	KEY_DELETE		, "DELETE" },
	{	KEY_BARCODE		, "BARCODE" },
	{	KEY_TAB			, "TAB" },
	{	KEY_CALC		, "CALC" },
	{	KEY_ALIAS		, "ALIAS" },
	{	KEY_TA			, "TA" },
	{	KEY_SI			, "SI" },
	{	KEY_OK			, "OK" },
	{	KEY_LANG		, "LANG" },
	{	KEY_QUANTITY	, "QUANTITY" },
	{	KEY_PAGEORDER	, "PAGEORDER" },
	{	KEY_FB			, "FB" },
    { 	KEY_MERGE 		, "+" },
	{	KEY_DOLLAR		, "$" },
	{	KEY_PERCENT		, "PERCENT" },
	{	KEY_LOAD		, "LOAD" },
	{	KEY_SAVE		, "SAVE" },
	{	KEY_FORMAT		, "FORMAT" },
	{	KEY_CHECK		, "CHECK" },
	{	KEY_COUNT		, "COUNT" },
	{	KEY_YEAR		, "YEAR" },
	{	KEY_LEVEL		, "LEVEL" },
	{	KEY_BLOCK		, "BLOCK" },
	{	KEY_ID			, "ID" },
	{	KEY_VALID		, "VALID" },
	{	KEY_RESTOID		, "RESTOID" },
	{	KEY_RCTRL		, "RCTRL" },
	{	KEY_LCTRL		, "LCTRL" },
	{	KEY_NAME		, "NAME" },
	{	KEY_STREET		, "STREET" },
	{	KEY_HOUSENR		, "HOUSENR" },
	{	KEY_POSTCODE	, "POSTCODE" },
	{	KEY_PLACE		, "PLACE" },
	{	KEY_JANUAR		, "JANUAR" },
	{	KEY_FEBRUAR		, "FEBRUAR" },
	{	KEY_MARCH		, "MARCH" },
	{	KEY_APRIL		, "APRIL" },
	{	KEY_MAY			, "MAY" },
	{	KEY_JUNE		, "JUNE" },
	{	KEY_JULY		, "JULY" },
	{	KEY_AUGUST		, "AUGUST" },
	{	KEY_SEPTEMBER	, "SEPTEMBER" },
	{	KEY_OCTOBER		, "OCTOBER" },
	{	KEY_NOVEMBER	, "NOVEMBER" },
	{	KEY_DECEMBER	, "DECEMBER" },
	{	KEY_HOURS		, "HOURS" },
	{	KEY_MINUTS		, "MINUTS" },
	{	KEY_DAYS		, "DAYS" },
	{	KEY_MONTHS		, "MONTHS" },
	{	KEY_YEARS		, "YEARS" },
	{	KEY_TRANSPORT	, "TRANSPORT" },
	{	KEY_INIT		, "INIT" },
	{	KEY_REPLACE		, "REPLACE" },
	{	KEY_EXTRA		, "EXTRA" },
	{	KEY_INCLUDED	, "INCLUDED" },
	{	KEY_INSERT		, "INSERT" },
	{	KEY_SMARTCARD	, "SMARTCARD" },
	{	KEY_LIST		, "LIST" },
	{	KEY_NETWORK		, "NETWORK" },
	{	KEY_ARROWUP		, "ARROWUP" },
	{	KEY_ARROWDOWN	, "ARROWDOWN" },
	{	KEY_LSHIFT		, "LSHIFT" },

	{	WT_CANCEL		, "CANCEL" },
	{	WT_CONFIRM		, "CONFIRM" },
	{	WT_NEXT			, "NEXT" },
	{	WT_PREVIOUS		, "PREVIOUS" },
	{	WT_PRINT		, "PRINT" },
	{	KEY_BACK		, "BACK" },
	{	KEY_NEXT		, "NEXT" },
	{	KEY_CTRL_F7		, "CTRL_F7" },
	{   FIRST_ITEM      , "FIRST_ITEM" },
	{	KEY_NOCHANGE	, "NOCHANGE" },
	{   KEY_RADICAL1    , "RADICAL1" },
	{   KEY_FOUND1      , "FOUND1" },
	{	KEY_FOUND142	, "FOUND142" },
	{  	KEY_CTRL_F1     , "CTRL_F1" },
	{	KEY_CTRL_F2		, "CTRL_F2" },
	{	KEY_CTRL_F3		, "CTRL_F3" },
	{	KEY_CTRL_F4		, "CTRL_F4" },
	{	KEY_CTRL_F5		, "CTRL_F5" },
	{	KEY_CTRL_F6		, "CTRL_F6" },
	{	KEY_CTRL_F8		, "CTRL_F8" },
	{	KEY_CTRL_F9		, "CTRL_F9" },
	{	KEY_CTRL_F10	, "CTRL_F10" },
	{	KEY_CTRL_F11	, "CTRL_F11" },
	{	KEY_CTRL_F12	, "CTRL_F12" },
	{	KEY_CTRL_F13	, "CTRL_F13" },
	{	KEY_CTRL_F14	, "CTRL_F14" },
	{	KEY_CTRL_F15	, "CTRL_F15" },
	{	KEY_CTRL_F16	, "CTRL_F16" },
	{	KEY_COLOUR_1	, "COLOUR_1" },
	{	KEY_COLOUR_48	, "COLOUR_48" },
	{	KEY_MAP_ITEM1	, "MAP_ITEM1" },
	{	KEY_SLIDER		, "SLIDER" },
	{ 	KEY_ESCAPE		, "ESCAPE" },
	{ 	KEY_ESCAPE		, "ESC" },
	{	KEY_UNDEFINED	, "UNDEFINED" },
    {   KEY_UNDEFINED   , "" }
};

/*----------------------------------------------------------------------------*/
std::string key2string( keybutton key)
{
	for ( int p=0;;p++)
	{
		if ( keys[p].key ==KEY_UNDEFINED)
		{
			break;
		}
		if ( keys[p].key ==key)
		{
			// Found key.
			return keys[p].txt;
		}
	}
	if ( key>=KEY_RADICAL1 && key<=KEY_RADICAL189)
	{
		char s[32];
		sprintf( s, "KEY_RADICAL%d", key-KEY_RADICAL1+1);
		return s;
	}
	if ( key>=KEY_FOUND1 && key<=KEY_FOUND142)
	{
		char s[32];
		sprintf( s, "KEY_FOUND%d", key-KEY_FOUND1+1);
		return s;
	}
	if ( key>=KEY_COLOUR_1 && key<=KEY_COLOUR_48)
	{
		char s[32];
		sprintf( s, "KEY_COLOUR_%d", key-KEY_COLOUR_1+1);
		return s;
	}
	if ( key>=KEY_MAP_ITEM1 && key<=KEY_MAP_ITEM1000)
	{
		char s[32];
		sprintf( s, "KEY_MAP_ITEM%d", key-KEY_MAP_ITEM1+1);
		return s;
	}
	return "";
}

/*----------------------------------------------------------------------------*/
keybutton string2key( std::string s)
{
	for ( int p=0;;p++)
	{
		if ( keys[p].key ==KEY_UNDEFINED)
		{
			break;
		}
		if ( keys[p].txt ==s)
		{
			// Found key.
			return keys[p].key;
		}
	}
	// KEY_RADICAL1 ..89
	// KEY_FOUND1 ..42
	// KEY_COLOUR_1 ..48
	// KEY_MAP_ITEM1 ..1000
	if ( s.find( "KEY_RADICAL")==0)
	{
		return (keybutton)( KEY_RADICAL1+(atoi( s.c_str()+11))-1);
	}
	if ( s.find( "KEY_FOUND")==0)
	{
		return (keybutton)( KEY_FOUND1+(atoi( s.c_str()+9))-1);
	}
	if ( s.find( "KEY_COLOUR_")==0)
	{
		return (keybutton)( KEY_COLOUR_1+(atoi( s.c_str()+11))-1);
	}
	if ( s.find( "KEY_MAP_ITEM")==0)
	{
		return (keybutton)( KEY_MAP_ITEM1+(atoi( s.c_str()+12))-1);
	}
	return KEY_UNDEFINED;
}

/*----------------------------------------------------------------------------*/
Ckeybutton::operator std::string()
{
	return key2string( m_key);
}

/*----------------------------------------------------------------------------*/
Ckeybutton::operator keybutton()
{
	return m_key;
}

/*----------------------------------------------------------------------------*/
Ckeybutton::Ckeybutton( const std::string &str)
: m_key( string2key(str))
{
}

/*----------------------------------------------------------------------------*/
Ckeybutton::Ckeybutton( keybutton n)
{
	m_key =n;
}

/*----------------------------------------------------------------------------*/
std::string Ckeybutton::label()
{
	if ( m_key>=KEY_RADICAL1 && m_key<=KEY_RADICAL189)
	{
		return "";
	}
	if ( m_key>=KEY_FOUND1 && m_key<=KEY_FOUND142)
	{
		return "";
	}
	if ( m_key>=KEY_GROUP_1 && m_key<=KEY_GROUP_24)
	{
		return "";
	}
	if ( m_key>=KEY_ITEM_1 && m_key<=KEY_ITEM_30)
	{
		return "";
	}
	if ( m_key>=KEY_COLOUR_1 && m_key<=KEY_COLOUR_48)
	{
		return "";
	}
	return key2string( m_key);
}
