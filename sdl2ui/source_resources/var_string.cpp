/*============================================================================*/
/**  @file      var_string.cpp
 **  @ingroup   drivers
 **  @brief		Create strings with variables
 **
 **  Convert strings
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              var_string
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 08 feb 2022
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

#include <cstdarg>
#include <algorithm>
#include <cctype>
#include <stdlib.h>
//#include <stdarg.h>
#include "log_base.hpp"
#include "var_string.hpp"

var_string::var_string(const char *cmd, ...)
{
//	va_list args;
//	va_start(args, cmd);
//
//	while (*cmd)
//	{
//		if (*cmd == '%')
//		{
//			switch(cmd[1])
//			{
//			case 'd':
//				(*this) << va_arg(args, int);
//				break;
//			case 'f':
//				(*this) << va_arg(args, double);
//				break;
//			case 's':
//				(*this) << va_arg(args, char)
//			}
//		}
//
//
//	}
    char name[4096];
	va_list args;
	va_start(args, cmd);
	vsnprintf(name, sizeof(name) - 1, cmd, args);
	m_buffer = name;
}

var_string::var_string(const std::string &name)
{
	m_buffer = name;
}

var_string::~var_string()
{}

var_string::operator std::string()
{
	return m_buffer;
}

std::string var_string::string()
{
	return m_buffer;
}

var_string::operator const char*()
{
	return m_buffer.c_str();
}

int var_string::length()
{
	return m_buffer.length();
}

const char *var_string::c_str()
{
	return m_buffer.c_str();
}

int var_string::system()
{
	(*this) << "system::%s" << m_buffer << Endl;
	return ::system(m_buffer.c_str());
}

void var_string::toLower()
{
    std::transform(m_buffer.begin(), m_buffer.end(), m_buffer.begin(),
        [](unsigned char c){ return std::tolower(c); });
}

size_t var_string::find_last_not_of(const char *value, size_t pos) const
{
	return(m_buffer.find_last_not_of(value, pos));
}

void var_string::erase(size_t index)
{
	m_buffer.erase(index);
}

std::string var_string::to_string()
{
	return m_buffer;
}

/// Operator to add logging.
var_string& var_string::operator<<(unsigned char by)
{
    m_buffer.push_back(by);
    return *this;
}

/// Operator to add logging.
var_string& var_string::operator<<(short w)
{
    std::stringstream ss;
    ss << w;
    m_buffer += ss.str();
    return *this;
}

/// Operator to add logging.
var_string& var_string::operator<<(int l)
{
    std::stringstream ss;
    ss << l;
    m_buffer += ss.str();
    return *this;
}

/// Operator to add logging.
var_string& var_string::operator<<(unsigned int l)
{
    std::stringstream ss;
    ss << l;
    m_buffer += ss.str();
    return *this;
}
/// Operator to add logging.
var_string& var_string::operator<<(long l)
{
    std::stringstream ss;
    ss << l;
    m_buffer += ss.str();
    return *this;
}

/// Operator to add logging.
var_string& var_string::operator<<(unsigned long l)
{
    std::stringstream ss;
    ss << l;
    m_buffer +=ss.str();
    return *this;
}

/// Operator to add logging.
var_string& var_string::operator<<(double f)
{
    std::stringstream ss;
    ss << f;
    m_buffer +=ss.str();
    return *this;
}

/// Operator to add logging.
var_string& var_string::operator<<(char ch)
{
    m_buffer.push_back(ch);
    return *this;
}

/// Operator to add logging.
var_string& var_string::operator<<(char *ch)
{
    m_buffer += std::string(ch);
    return *this;
}

/// Operator to add logging.
var_string& var_string::operator<<(const char *ch)
{
    m_buffer += std::string(ch);
    return *this;
}

/// Operator to add logging.
var_string& var_string::operator<<(const std::string &ch)
{
    m_buffer += ch;
    return *this;
}

/// Operator to add logging.
var_string& var_string::operator<<(bool u)
{
    m_buffer += (u ? "true":"false");
    return *this;
}
