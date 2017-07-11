/*
 * text_splitter.cpp
 *
 *  Created on: 14 apr. 2012
 *      Author: mensfort
 */

#include "text_splitter.h"

/*==============================================================================
**              CtextSplitter::CtextSplitter
**============================================================================*/
///
/// @brief		Split text into an array of parts.
///
///	@brief		sign1 [in] Seperator between 2 substring, e.g. space character.
/// @brief		sign2 [in] Seperator between 2 substring, e.g. tab character.
///
/// @post       Sentence seperated.
///
/// @remarks    TYPED
///
/*============================================================================*/
CtextSplitter::CtextSplitter( const std::string &string, const std::string &sign1, const std::string &sign2)
: m_size(0)
{
	std::string word;
	int index =0;
	for (;;)
	{
		std::string::size_type loc =string.find( sign1, index);
		std::string::size_type loc2 =string.find( sign2, index);
		if ( loc==std::string::npos)
		{
			loc =loc2;
		}
		else if ( loc2 !=std::string::npos && loc2<loc)
		{
			loc =loc2;
		}

		if ( loc ==std::string::npos || loc<=0)
		{
			// Add remainder of string.
			word.assign( string, index, string.length()-index);
			push_back( utf8string(word));
			break;
		}
		word.assign( string, index, (int)loc-index);
		push_back( utf8string(word));
		index =(int)loc+1;
	}
}

void CtextSplitter::first_character_capital()
{
	//for ( auto ?)
	for ( auto &x : m_array)
	{
		x.firstCharacterCapital();
	}
//	for (int a=0; a<(int)size(); a++)
//	{
//		m_array[a].firstCharacterCapital();
//	}
}

void CtextSplitter::all_capitals()
{
	for ( auto &x : m_array )
	{
		x.toUpper();
	}
}

void CtextSplitter::all_lower_case()
{
	for ( auto &x : m_array )
	{
		x.toLower();
	}
}

void CtextSplitter::insert( int y, const utf8string &str)
{
	while ( y>=m_size)
	{
		push_back( utf8string(""));
	}
	m_array[y].insert( 0, str);
}
