/*============================================================================*/
/** @file       zhongstring.h
 **  @ingroup    group
 **  @brief		something...
 **
 **  Longer description.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Clogfile
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

class utf8string
{
public:
	utf8string( const std::string & s="");
	utf8string( const char *s);
	virtual ~utf8string();
	int operator[](size_t index) const;
	size_t size() const;
	void erase( size_t cursor);
	void insert( size_t cursor, int number);
	void insert( size_t cursor, const utf8string &str);
	void set( size_t index, int value);
	std::string sub( size_t left, size_t right=-1);
	void onlyAscii();
	void sort();
	operator std::string() { return m_string; }
	void firstCharacterCapital();
	void toUpper();
	void toLower();
	const char *c_str() const;
	void clear() { m_string.clear(); }
	void push_back( int n);
	void push_back( const std::string &s);
	void pop_back();
	bool startWith( int cursor, const std::string &compare);
	static int characterLength( const char *value);
	static int utf8character( const char *value);

private:
	int utf8character( size_t index) const;
	int getOffset( size_t cursor) const;

public:
	std::string			m_string;
};

bool strfind( const char *s1, const char *s2);

#if 0
/// Class to handle chinese strings
class zhongstring
{
public:
	zhongstring(const std::string & s="");
	zhongstring(const char *s);
	virtual ~zhongstring();

	void setGB( int index, unsigned short characterNr);
	unsigned short operator[](size_t offset) const;
	unsigned short value(size_t offset)
	{
		if ( offset<m_numbers.size())
		{
			return m_numbers[offset];
		}
		return 0;
	}
	operator const std::string ();
	zhongstring & operator =( const std::string & s);
	zhongstring & operator +=( const std::string & s);
	zhongstring & operator  =( const char *t);
	size_t size() { return m_numbers.size(); }
	void erase( int cursor);
	void insert( int cursor, int number);
	void onlyAscii();
	void sort();
private:
	std::vector<unsigned short> m_numbers;
};
#endif

extern void replace(std::string &str, const std::string &find_what, const std::string &replace_with);
extern std::string tosql(std::string str);
extern int memicmp( const void *a, const void *b, size_t len);

/* End ZHONGSTRING_H_ */
