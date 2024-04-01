/*============================================================================*/
/**  @file      utf8string.cpp
 **  @ingroup   drivers
 **  @brief		Handle chinese string and convert between:
 **             1. gb format. (memory)
 **             2. unicode format. (sql)
 **             3. numbers (printers, font)
 **
 **  Convert chinese strings.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              zhongstring
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 30 apr 2011
 ** 
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

/*------------- Standard includes --------------------------------------------*/
#include <sstream>
#include <string.h>
#include <assert.h>
#include "utf8string.hpp"

/*----------------------------------------------------------------------------*/
utf8string::utf8string(const std::string & s)
: m_string(s)
{
}

/*----------------------------------------------------------------------------*/
utf8string::utf8string(const char *s)
: m_string(s)
{
}

/*----------------------------------------------------------------------------*/
utf8string::~utf8string()
{}

/*----------------------------------------------------------------------------*/
void utf8string::set( size_t index, int nr)
{
	// Offset at cursor.
	int pos =getOffset( index);
	int len = characterLength( m_string.c_str()+pos);
	char s[2];
	s[1]=0;

	int newLen =(nr<0x80) ? 1:(nr<0x800) ? 2:3;
	if ( newLen <len)
	{
		m_string.erase( pos, len-newLen);
	}

	if ( newLen==1)
	{
		m_string[pos] =static_cast<char>(nr);
	}
	else if ( newLen==2)
	{
		m_string[pos] =static_cast<char>(0xc0+(nr>>5));
		s[0] =static_cast<char>(0x80+(nr&0x3f));
		if ( len<2)
		{
			m_string.insert( pos+1, s);
		}
		else
		{
			m_string[ pos+1]=s[0];
		}
	}
	else
	{
		m_string[pos] =static_cast<char>(0xe0 | ((nr&0xF000)>>12));
		s[0] =static_cast<char>(0x80 | ((nr&0x0FC0)>>6));
		if ( len<2)
		{
			m_string.insert( pos+1, s);
		}
		else
		{
			m_string[pos+1]=s[0];
		}
		s[0] =static_cast<char>(0x80 | (nr&0x003F));
		if ( len<3)
		{
			m_string.insert( pos+2, s);
		}
		else
		{
			m_string[pos+2]=s[0];
		}
	}
}

/*----------------------------------------------------------------------------*/
std::string utf8string::subStr( size_t left, size_t right)
{
	left =getOffset( left);
	if ( ((int)right)<=0)
	{
		right =m_string.length();
	}
	else
	{
		right =getOffset( right);
	}
	if ( left<right)
	{
		std::string t;
		t.assign( m_string, left, right-left);
		return t;
	}
	std::string u;
	return u;
}

/*----------------------------------------------------------------------------*/
int utf8string::characterLength( const char *value)
{
	int first;

	first =*value++;
	if (( first & 0x80) ==0)
	{
		// single byte
		return 1;
	}
	int second =*value++;
	if ( second ==0)
	{
		return 1;
	}
	if ( (first & 0xe0)==0xc0)
	{
		return 2;
	}
	int third =*value++;
	if ( third ==0)
	{
		return 2;
	}
	if ( (first & 0xf0) ==0xe0)
	{
		// three bytes.
		return 3;
	}
	int fourth =*value++;
	if ( fourth ==0)
	{
		return 3;
	}
	if ( (first & 0xf8) ==0xf0)
	{
		// four bytes.
		return 4;
	}
	return 1;
}

/*----------------------------------------------------------------------------*/
int utf8string::utf8character( const char *value)
{
	int first;

	first =*value++;
	if (( first & 0x80) ==0)
	{
		// single byte
		return first;
	}
	int second =*value++;
	if ( second ==0)
	{
		// single byte
		return first;
	}
	if ( (first & 0xe0)==0xc0)
	{
		int value=((first&0x1F)<<6) + (second&0x3F);
		return value;
	}
	int third =*value++;
	if ( third ==0)
	{
		return 2;
	}
	if ( (first & 0xf0) ==0xe0)
	{
		// three bytes.
		int value=((first&0x0F)<<12) | ((second&0x3F)<<6) | (third&0x3F);
		return value;
	}
	int fourth =*value++;
	if ( fourth ==0)
	{
		int value=((first&0x0F)<<12) | ((second&0x3F)<<6) | (third&0x3F);
		return value;
	}
	if ( (first & 0xf8) ==0xf0)
	{
		// four bytes.
		int value =((first&0x07)<<18) | ((second&0x3F)<<12) | ((third&0x3F)<<6) | (fourth&0x3F);
		return value;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
int utf8string::operator[](size_t index) const
{
	return utf8character(index);
}

/*----------------------------------------------------------------------------*/
int utf8string::utf8character( size_t index) const
{
	index =getOffset( index);
	return utf8character( m_string.c_str()+index);
}

/*----------------------------------------------------------------------------*/
size_t utf8string::size() const
{
	const char *s =m_string.c_str();
	int a;
	for ( a=0;;++a)
	{
		if (*s ==0)
		{
			break;
		}
		int len =characterLength(s);
		s +=len;
	}
	return a;
}

/*----------------------------------------------------------------------------*/
int utf8string::length() const
{
    return (int)size();
}

/*----------------------------------------------------------------------------*/
void utf8string::replace(const std::string &find_what, const std::string &replace_with)
{
    ::replace(m_string, find_what, replace_with);
}

/*----------------------------------------------------------------------------*/
int utf8string::getOffset( size_t cursor) const
{
	const char *s=m_string.c_str();
	int offset =0;
	while ( cursor--)
	{
		if ( *(s+offset)==0)
		{
			return offset;
		}
		offset +=characterLength(s+offset);
	}
	return offset;
}

/*----------------------------------------------------------------------------*/
void utf8string::erase( size_t cursor)
{
	// Offset at cursor.
	int pos =getOffset(cursor);
	int len = characterLength( m_string.c_str()+pos);
	if (len)
	{
		m_string.erase(pos, len);
	}
}

/*----------------------------------------------------------------------------*/
void utf8string::insert( size_t cursor, const utf8string &str)
{
	int pos =getOffset(cursor);
	//int len = str.length();
	m_string.insert( pos, str.m_string);

}

/*----------------------------------------------------------------------------*/
void utf8string::insert( size_t cursor, int nr)
{
	char s[4];
	s[0]=0;
	int pos =getOffset( cursor);

	if (nr<0x80)
	{
		s[0] =static_cast<char>(nr);
		s[1] =0;
	}
	else if (nr<0x800)
	{
		s[0] =static_cast<char>(0xc0+(nr>>5));
		s[1] =static_cast<char>(0x80+(nr&0x3f));
		s[2] =0;
	}
	else
	{
		s[0] =static_cast<char>(0xe0 | ((nr&0xF000)>>12));
		s[1] =static_cast<char>(0x80 | ((nr&0x0FC0)>>6));
		s[2] =static_cast<char>(0x80 | (nr&0x003F));
		s[3] =0;
	}	m_string.insert( pos, s);
}

/*----------------------------------------------------------------------------*/
/** @brief Remove last character. */
void utf8string::pop_back()
{
	size_t sz =size();
	if ( sz ==1)
	{
		clear();
	}
	else
	if ( sz >1)
	{
		erase( sz-1);
	}
}

/*----------------------------------------------------------------------------*/
/** @brief Add character at the end.
 *  @param nr [in] What utf-8 character.
 */
void utf8string::push_back( int nr)
{
	if (nr<0x80)
	{
		m_string.push_back( static_cast<char>(nr));
	}
	else if (nr<0x800)
	{
		m_string.push_back( static_cast<char>(0xc0+(nr>>5)));
		m_string.push_back(static_cast<char>(0x80+(nr&0x3f)));
	}
	else
	{
		m_string.push_back( static_cast<char>(0xe0 | ((nr&0xF000)>>12)));
		m_string.push_back( static_cast<char>(0x80 | ((nr&0x0FC0)>>6)));
		m_string.push_back( static_cast<char>(0x80 | (nr&0x003F)));
	}
}

/*----------------------------------------------------------------------------*/
/// @brief Compare strings.
int memicmp( const void *a, const void *b, size_t len)
{
	const char *aa=static_cast<const char*>(a);
	const char *bb=static_cast<const char*>(b);
	while (len--)
	{
		char aaa=toupper(*aa);
		char bbb=toupper(*bb);
		if (aaa<bbb) return -1;
		if (aaa>bbb) return 1;
		aa++; bb++;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
bool utf8string::startWith( int cursor, const std::string &compare)
{
	int pos =getOffset( cursor);
	if ( memicmp( m_string.c_str()+pos, compare.c_str(), compare.length()) ==0)
	{
		return true;
	}
	return false;
}

/*----------------------------------------------------------------------------*/
void utf8string::trimTags()
{
    for (;;)
    {
        int c = m_string.find("<");
        if (c>=0)
        {
            int d= m_string.find(">", c+1);
            if (d>c)
            {
                m_string.erase(c, d-c+1);
                continue;
            }
        }
        break;
    }
}

/*----------------------------------------------------------------------------*/
void utf8string::trim()
{
    m_string.erase(0, m_string.find_first_not_of(' '));       //prefixing spaces
    m_string.erase(m_string.find_last_not_of(' ')+1);         //surfixing spaces
}

/*----------------------------------------------------------------------------*/
void utf8string::trimRoundBrackets()
{
    for (;;)
    {
        int c = m_string.find(")");
        if (c>=0)
        {
            int d= m_string.rfind(")", c-1);
            if (d>c)
            {
                m_string.erase(c, d-c+1);
                continue;
            }
        }
        break;
    }
}

/*----------------------------------------------------------------------------*/
void utf8string::push_back( const std::string &s)
{
	m_string +=s;
}

/*----------------------------------------------------------------------------*/
void utf8string::push_front( const std::string &s)
{
    m_string =s+m_string;
}

#define LAST_ASCII_CHAR 255
/*----------------------------------------------------------------------------*/
void utf8string::onlyAscii()
{
	for ( size_t a=0; a<size(); a++)
	{
		int index =getOffset( a);
		unsigned short val =utf8character( m_string.c_str()+index);
		if (val>LAST_ASCII_CHAR)
		{
			// remove this character!!
			erase(a);
		}
		else a++;
	}
}

/*----------------------------------------------------------------------------*/
void utf8string::firstCharacterCapital()
{
	if ( m_string.size() ==0)
	{
		return;
	}
	if ( m_string[0]>='a' && m_string[0]<='z')
	{
		m_string[0] =toupper( m_string[0]);
	}
}

/*----------------------------------------------------------------------------*/
const char *utf8string::c_str() const
{
	return m_string.c_str();
}

/*----------------------------------------------------------------------------*/
void utf8string::toUpper()
{
	const char *s =m_string.c_str();
	int a;
	int p =0;
	for ( a=0;;++a)
	{
		if (*s ==0)
		{
			break;
		}
		int len =characterLength(s);
		if ( len ==1)
		{
			m_string[p] =toupper( m_string[p]);
		}
		p +=len;
		s +=len;
	}
}

/*----------------------------------------------------------------------------*/
void utf8string::toLower()
{
	const char *s =m_string.c_str();
	int a;
	int p =0;
	for ( a=0;;++a)
	{
		if (*s ==0)
		{
			break;
		}
		int len =characterLength(s);
		if ( len ==1)
		{
			m_string[p] =tolower( m_string[p]);
		}
		p +=len;
		s +=len;
	}
}

/*----------------------------------------------------------------------------*/
void utf8string::sort()
{
	int sz=size();
	std::vector<int> quantity;
	for ( int l=0; l<sz; l++)
	{
		quantity.push_back(1);
	}
	// Find doubles.
	for ( int a=0; a<sz; a++)
	for ( int b=a+1; b<sz;)
	{
		if ( utf8character(a) ==utf8character(b))
		{
			quantity[a]++;
			quantity.erase( quantity.begin()+b);
			erase( b);
			sz--;
		}
		else b++;
	}
	// Sort counters.
	for ( int c=0; c<sz; c++)
	for ( int d=c+1; d<sz;)
	{
		if (quantity[c] <quantity[d])
		{
			// Exchange
			int nr =utf8character(c);
			set( c, utf8character(d));
			set( d, nr);
			nr =quantity[c];
			quantity[c] =quantity[d];
			quantity[d] =nr;
		} else d++;
	}
}


/*============================================================================*/
///
/// @brief 		Replace text in string.
///
/// @param		str 		 [in] string.
/// @param		find_what 	 [in] search for.
/// @param		replace_with [in] replace with.
///
/*============================================================================*/
void replace( std::string &str, const std::string &find_what, const std::string &replace_with)
{
	size_t pos=0;
	while((pos=str.find(find_what, pos))!=std::string::npos)
	{
		str.erase(pos, find_what.length());
		str.insert(pos, replace_with);
		pos+=replace_with.length();
	}
}

/*============================================================================*/
///
/// @brief 		Convert string to sql.
///
/// @param		str [in] string.
///
/// @return		Writable sql string.
///
/*============================================================================*/
std::string toSql( const std::string &str)
{
	std::string tmp=str;
	replace(tmp, "'", "''");
	replace(tmp, "%", "%%");
	return tmp;
}

/*--Bart Houkes--2/5/1995----------------------------------Status:Debugged--*/
bool strfind( const char *s1, const char *s2)
// if s2 is element in s1 return OK
{
	if (!*s2)
		return true;
	while (*s1)
	{
		int g = 0;
		while (tolower(s2[g]) == tolower(s1[g]))
		{
			g++;
			if (!s2[g])
				return true;
		}
		s1++;
	}
	return false;
}

std::string utf8ToUtf8mb4String(const std::string &bufferIn)
{
    size_t n=bufferIn.length();
    const unsigned char* buffer =(const unsigned char*) bufferIn.c_str();
    static const unsigned char utf8_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 0, 0,
        0xFF, 0x1F, 0x0F, 0x07, 0x03, 0x01
    };
    static const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
    static const unsigned char *masks = utf8_lengths + 0x100;
    if (n >= 3 && !memcmp(buffer, bom, 3))
    {
        buffer += 3;
        n -= 3;
    }
    //int cursor = 0;
    //unsigned char output[4096];
    std::string output;
    //size_t writer = 0;
    for (size_t i = 0; i != n;)
    {
        unsigned char byte = buffer[i++];
        unsigned char length = utf8_lengths[byte];
        if (length > n - i)
        {
            break;
        }
        unsigned char wc = byte & masks[length];
        for (;length; length--)
        {
            wc <<= 6;
            wc |= (buffer[i++] & 0x3F);
        }
        output +=wc;
        //output[cursor++] = wc;
        //output[cursor] =0;
    }
    //dst.assign(temp_pointer, temp_pointer + writer);
    return output;
}
