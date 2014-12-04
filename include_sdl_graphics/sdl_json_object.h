/*
 * sdl_json_object.h
 *
 *  Created on: 18 feb. 2014
 *      Author: mensfort
 */

#pragma once

#include "sdl_button.h"
#include "sdl_image.h"
#include "json_value.h"
#include "json_reader.h"
#include "json_features.h"

/// Class to get an object from text strings.
class CjsonObject : public CdialogObject
{
public:
	CjsonObject( Cdialog *parent, const std::string &description);
	void onPaint( int touch);
	void onPaint( const Cpoint &p, int touch);

	virtual ~CjsonObject();

	operator Cbutton*() const;
	operator Cimage*() const;
	CdialogObject *operator[]( int index);
	CdialogObject *find( keybutton key);
	void add( const std::string &description);

private:
	void decode( Cdialog *parent, const Json::Value &root);
	void decodeText( Cdialog *parent, const Json::Value &root);
	void decodeImage( Cdialog *parent, const Json::Value &root);
	void decodeBackground( Cdialog *parent, const Json::Value &root);
	void decodeButton( Cdialog *parent, const Json::Value &root);
	void decodeBottomButton( Cdialog *parent, const Json::Value &root);
	void decodeCalculatorButton( const Json::Value &root);
	void decodeHeaderButton( const Json::Value &root);

	// Find variables of one type
	Egravity decodeGravity( const Json::Value &root, Egravity &gravity);
	EborderType decodeBorder( const Json::Value &root, EborderType &border);
	Crect decodeRect( const Json::Value &root);
	keybutton decodeKeybutton( const Json::Value &root);
	int decodeMargin( const Json::Value &root);
	std::string decodeIcon( const Json::Value &root);
	std::string decodeLabel( const Json::Value &root);
	textId decodeTextId( const Json::Value &value);
	EfillType decodeFillType( const Json::Value &root, EfillType &fill);
	int decodeBorderWidth( const Json::Value &root, int &borderWidth);
	Sfont decodeFont( const Json::Value &root, const std::string &name);
	colour decodeColour1( const Json::Value &root);
	colour decodeColour2( const Json::Value &root);
	int decodeRadius( const Json::Value &root, int &lastvalue);

private:
	Json::Features 	m_features;		///< Needed for parsing json.
	Json::Reader 	m_reader;		///< Needed for parsing json.
	Json::Value 	m_root;			///< Needed for parsing json.
	int				m_index;		///< Which object we want.
	std::vector<CdialogObject*> m_object;	///< List of objects.

private:
	std::string		m_lastName;		///< Last name for next button
	int				m_lastCode;		///< Last code for next button
	Crect			m_lastRect;		///< Last rectangle for next button
	EborderType 	m_buttonBorder;	///< Default button border
	EborderType		m_lastBorder;	///< Last border for next button
	Egravity		m_lastGravity;	///< Last gravity
	Egravity		m_buttonGravity;///< Default button gravity
	Egravity		m_imageGravity; ///< Default image gravity
	int				m_lastMargin;	///< Last margin for next button
	std::string		m_lastLabel;	///< Last label
	Sfont			m_lastFont;		///< Last font used
	std::string		m_lastType;     ///< Last type
	int				m_lastColour1;	///< Last colour type
	int				m_lastColour2;	///< Last colour type
	int				m_lastRadius; 	///< Radius found
	EfillType		m_lastFill;		///< Last fill type
	EfillType		m_lastButtonFill;		///< Last fill type
	int				m_lastBorderWidth; ///< Last width of border
	int				m_lastBackgroundRadius; ///< Radius for backgrounds
	int				m_lastBackgroundBorderWidth; ///< Width border
	EfillType		m_lastBackgroundFill; ///< Fill type
	int				m_lastButtonRadius; ///< Radius calculate
};

/* SDL_JSON_OBJECT_H_ */
