/*
 * text_splitter.hpp
 *
 *  Created on: 14 apr. 2012
 *      Author: mensfort
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
#include "utf8string.hpp"

/// @brief  Split a string into nice pieces to handle.
class CtextSplitter
{
public:
	CtextSplitter( const std::string &string, const std::string &sign1, const std::string &sign2, bool trim);
	~CtextSplitter() {}
	int size() const;
	utf8string operator[]( int index) const;
	void first_character_capital();
	void all_capitals();
	void all_lower_case();
	void push_back( const utf8string &str);
	void insert( int y, const utf8string &str);
    void trim_tags();
    void remove_left(int nr_characters);
    void trim_round_brackets();
    void trim();
    bool empty() const;
    void erase(int index);
    std::string join( int start, const std::string &join) const;

private:
	bool trim(std::string &word) const;

private:
	std::vector<utf8string> m_array;
	int m_size;
	bool m_trim;
};

/* TEXT_SPLITTER_H_ */
