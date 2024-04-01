/*============================================================================*/
/**  @file      json2database.cpp
 **  @ingroup   dialogs
 **  @brief		Json command for the database.
 **
 **  Send command to the database using json
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cjson2Database
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 23 may 2021
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/
#include <assert.h>
#include "json2database.hpp"

/*------------- Program includes ---------------------------------------------*/

/*----------------------------------------------------------------------------*/
Cjson2Database::Cjson2Database( const std::string &description)
: m_features()
, m_reader()
, m_root()
{
	bool parsingSuccessful = m_reader.parse( description, m_root);
	if ( !parsingSuccessful )
	{
		// "Failed to parse %s", m_reader.getFormatedErrorMessages().c_str() );
		return;
	}
	if ( m_root.isArray())
	{
		for ( int n=0; n<(int)m_root.size(); n++)
		{
			decode(m_root[n]);
		}
	}
}

/** @brief Decode the object
 *  @param parent [in] Dialog to use it
 *  @param value [out] Initial value
 */
void Cjson2Database::decode(const Json::Value &value)
{
	Json::Value v( value["type"]);
	if ( v.isString())
	{
		m_lastType =v.asString();
	}
//	if ( m_lastType.compare("image")==0)
//	{
//		decodeImage( parent, value);
//	}
//	else if ( m_lastType.compare("button")==0)
//	{
//		decodeButton( parent, value);
//	}
//	else if ( m_lastType.compare("bottombutton")==0)
//	{
//		decodeBottomButton( parent, value);
//	}
//	else if ( m_lastType.compare("calculatorbutton")==0)
//	{
//		decodeCalculatorButton( value);
//	}
//	else if ( m_lastType.compare("header")==0)
//	{
//		decodeHeaderButton( value);
//	}
//	else if ( m_lastType.compare("headerbutton")==0)
//	{
//		decodeHeaderButton( value);
//	}
//	else if ( m_lastType.compare("background")==0)
//	{
//		decodeBackground( parent, value);
//	}
//	else if ( m_lastType.compare("text")==0)
//	{
//		decodeText( parent, value);
//	}
	//else "CjsonObject::CjsonObject Wrong type!!");
}
