/*============================================================================*/
/** @file        utf8string.h
 **  @ingroup    group
 **  @brief		 something...
 **
 **  Longer description.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Clogfile
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 1 mei 2011
 ** 
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

/*------------- Standard includes --------------------------------------------*/

#pragma once

#include <string>
#include <vector>

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
	std::string subStr( size_t left, size_t right=-1);
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
    void push_front( const std::string &s);
	void pop_back();
	bool startWith( int cursor, const std::string &compare);
	static int characterLength( const char *value);
	static int utf8character( const char *value);
    void trimTags();
    void trimRoundBrackets();
    void trim();
    int length() const;
    void replace( const std::string &find_what, const std::string &replace_with);

private:
	int utf8character( size_t index) const;
	int getOffset( size_t cursor) const;

public:
	std::string			m_string;
};

int memicmp( const void *a, const void *b, size_t len);
void replace( std::string &str, const std::string &find_what, const std::string &replace_with);
std::string toSql( const std::string &str);
bool strfind( const char *s1, const char *s2);
std::string utf8ToUtf8mb4String(const std::string &bufferIn);
