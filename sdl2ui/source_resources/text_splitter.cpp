/*
 * text_splitter.cpp
 *
 *  Created on: 14 apr. 2012
 *      Author: mensfort
 */

#include "text_splitter.hpp"

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
CtextSplitter::CtextSplitter( const std::string &string,
                              const std::string &sign1,
                              const std::string &sign2,
                              bool trimWords)
: m_size(0)
, m_trim(trimWords)
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
		    if (trim(word))
		    {
			    push_back( utf8string(word));
		    }
			break;
		}
		word.assign( string, index, (int)loc-index);
        if (trim(word))
        {
    		push_back( utf8string(word));
        }
    	index =(int)loc+1;
	}
}

/*----------------------------------------------------------------------------*/
bool CtextSplitter::trim(std::string &word) const
{
    if (!m_trim)
    {
        return true;
    }
    while (word.length()>0 && word[0]==' ')
    {
        word.erase(word.begin());
    }
    while (word.length()>1 && word[word.length()-1]==' ')
    {
        word.erase(word.end()-1);
    }
    return word.length()>0;
}

/*----------------------------------------------------------------------------*/
int CtextSplitter::size() const
{
    return m_size;
}

/*----------------------------------------------------------------------------*/
utf8string CtextSplitter::operator[]( int index) const
{
    return m_array[index];
}

/*----------------------------------------------------------------------------*/
void CtextSplitter::first_character_capital()
{
	for (int a=0; a<(int)size(); a++)
	{
		m_array[a].firstCharacterCapital();
	}
}

/*----------------------------------------------------------------------------*/
void CtextSplitter::all_capitals()
{
	for (int a=0; a<(int)size(); a++)
	{
		m_array[a].toUpper();
	}
}

/*----------------------------------------------------------------------------*/
void CtextSplitter::all_lower_case()
{
	for (int a=0; a<(int)size(); a++)
	{
		m_array[a].toLower();
	}
}

/*----------------------------------------------------------------------------*/
void CtextSplitter::push_back( const utf8string &str)
{
    m_array.push_back(str);
    m_size++;
}

/*----------------------------------------------------------------------------*/
void CtextSplitter::trim_tags()
{
    for (utf8string &str : m_array)
    {
        str.trimTags();
    }
}

/*----------------------------------------------------------------------------*/
void CtextSplitter::remove_left(int nr_characters)
{
    for (utf8string &str : m_array)
    {
        str.replace("&quot;", "-");
        for (int n=0; n<nr_characters; ++n)
        {
            str.erase(0);
        }
    }
}

/*----------------------------------------------------------------------------*/
void CtextSplitter::trim_round_brackets()
{
    for (utf8string &str : m_array)
    {
        str.trimRoundBrackets();
    }
}

/*----------------------------------------------------------------------------*/
bool CtextSplitter::empty() const
{
    return m_array.empty();
}

/*----------------------------------------------------------------------------*/
void CtextSplitter::erase(int index)
{
    if ((int)m_array.size()>index)
    {
        m_array.erase(m_array.begin()+index);
    }
}

/*----------------------------------------------------------------------------*/
void CtextSplitter::trim()
{
    for (utf8string &str : m_array)
    {
        str.trim();
    }
}

/*----------------------------------------------------------------------------*/
void CtextSplitter::insert( int y, const utf8string &str)
{
	while ( y>=m_size)
	{
		push_back( utf8string(""));
	}
	m_array[y].insert( 0, str);
}

/*----------------------------------------------------------------------------*/
std::string CtextSplitter::join( int start, const std::string &join) const
{
    std::string result;
    bool first = true;
    while (start < (int)m_array.size())
    {
        if (first)
        {
            first = false;
        }
        else
        {
            result +=join;
        }
        result +=m_array[start].m_string;
        start++;
    }
    return result;
}
