/*============================================================================*/
/**  @file      money.cpp
 **  @ingroup   source
 **  @brief		Money conversion in cents.
 **
 **  Conversion of money.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **				Cmoney
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 15 dec 1994
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/
#include <stdio.h>
#include <sstream>
#include <string.h>
#include "money.hpp"

/*----------------------------------------------------------------------------*/
Cmoney::Cmoney( const std::string &m)
{
	m_cents=0;
	const char *x =m.c_str();
	bool negative =false;
	if (*x=='-')
	{
		negative =!negative;
		x++;
	}
	while ( isdigit(*x))
	{
		m_cents =m_cents*10 + (*x-'0');
		x++;
	}
	m_cents*=100;
	if ( *x=='.' || *x==',')
	{
		x++;
		if ( isdigit(*x))
		{
			m_cents +=10*(*x-'0');
			x++;
			if ( isdigit(*x))
			{
				m_cents +=(*x-'0');
			}
		}
	}
	if (negative)
	{
		m_cents =-m_cents;
	}
}

/*----------------------------------------------------------------------------*/
/** @brief Compare 2 amounts.
 *  @param a [in] amount.
 *  @param b [in] amount.
 *  @return true if a<b.
 */
bool operator <(const Cmoney &a, const Cmoney &b)
{
	return (long) a < (long) b;
}

/*----------------------------------------------------------------------------*/
/** @brief Subtract with a value.
 *  @param value [in] amount.
 *  @return myself-value
 */
Cmoney & Cmoney::operator -=(const Cmoney &value)
{
	m_cents -=value.m_cents;
	return *this;
}

/*----------------------------------------------------------------------------*/
/** @brief Multiply with a value.
 *  @param value [in] amount.
 *  @return value*myself
 */
Cmoney & Cmoney::operator *=(int value)
{
	m_cents *=value;
	return *this;
}

/*----------------------------------------------------------------------------*/
/** @brief Multiply with a value.
 *  @param value [in] amount.
 *  @return value*myself
 */
Cmoney & Cmoney::operator *=(double value)
{
	m_cents =(long)(m_cents*value);
	return *this;
}

/*----------------------------------------------------------------------------*/
/** @brief Divide with a number.
 *  @param x [in] number.
 *  @return v/x
 */
Cmoney & Cmoney::operator /=(long x)
{
	m_cents =(long)(m_cents/x);
	return *this;
}

/*----------------------------------------------------------------------------*/
/** @brief Add with an amount.
 *  @param value [in] amount.
 *  @return value+myself
 */
Cmoney & Cmoney::operator +=(const Cmoney &value)
{
	m_cents +=value.m_cents;
	return *this;
}

/*----------------------------------------------------------------------------*/
/** @brief Multiply with an amount.
 *  @param v [in] amount.
 *  @return v*myself
 */
Cmoney & Cmoney::operator *=(const Cmoney &v)
{
	m_cents *=v.m_cents;
	return *this;
}

/*----------------------------------------------------------------------------*/
Cmoney & Cmoney::operator =(int x)
{
	m_cents = x;
	return *this;
}

/*----------------------------------------------------------------------------*/
Cmoney & Cmoney::operator =(long x)
{
	m_cents = x;
	return *this;
}

/*----------------------------------------------------------------------------*/
/** @brief Compare 2 amounts.
 *  @param x [in] Any number.
 *  @param b [in] First amount.
 *  @return x*b
 */
const Cmoney operator *(int x, const Cmoney &b)
{
	Cmoney a(x*b.Long());
	return a;
}

/*----------------------------------------------------------------------------*/
/** @brief Compare 2 amounts.
 *  @param b [in] First amount.
 *  @param x [in] Any number.
 *  @return true if the amounts are not equal.
 */
const Cmoney operator *( const Cmoney &b, int x)
{
	Cmoney a(x*b.Long());
	return a;
}

/*----------------------------------------------------------------------------*/
/** @brief Compare 2 amounts.
 *  @param b [in] First amount.
 *  @param x [in] Any number.
 *  @return b/x
 */
const Cmoney operator /( const Cmoney &b, int x)
{
	Cmoney a(b.Long()/x);
	return a;
}

/*----------------------------------------------------------------------------*/
/** @brief Compare 2 amounts.
 *  @param a [in] First amount.
 *  @param b [in] Second amount.
 *  @return false if the amounts are not equal.
 */
bool operator ==(const Cmoney &a, const Cmoney &b)
{
	return ((long)a ==(long)b) ? true:false;
}

/*----------------------------------------------------------------------------*/
/** @brief Compare 2 amounts.
 *  @param a [in] First amount.
 *  @param b [in] Second amount.
 *  @return true if the amounts are not equal.
 */
bool operator !=(const Cmoney &a, const Cmoney &b)
{
	return ((long)a ==(long)b) ? false:true;
}

/*----------------------------------------------------------------------------*/
/** @brief Subtract 2 amounts.
 *  @param a [in] Amount a.
 *  @param b [in] Amount b.
 *  @return a-b
 */
const Cmoney operator -(const Cmoney &a, const Cmoney &b)
{
	Cmoney c((long)a-(long)b);
	return c;
}

/*----------------------------------------------------------------------------*/
/** @brief Operator to add 2 amounts.
 *  @param a [in] First amount.
 *  @param b [in] Second amount.
 *  @return a+b
 */
const Cmoney operator +(const Cmoney &a, const Cmoney &b)
{
	Cmoney c((long)a+(long)b);
	return c;
}

/*----------------------------------------------------------------------------*/
/** @brief Convert money to a string.
 *  @param length [in] What length.
 *  @param flags [in] Flags to adjust the way of output.
 *  @param ape [in] Sign in front of amount, sometimes @ for unit-price.
 *  @return string output
 */
std::string Cmoney::to_string(int length, int flags, const std::string &ape) const
/* POST: money converted to string                                          */
/* FLAGS: MONEY_CENTS MONEY_LINE, MONEY_APE                                 */
{
	std::stringstream t;
	std::string result;
	int len = 0;
	if ( labs(m_cents) > 1000000L && length < 9 && !(flags
			& MONEY_CENTS))
	{
		t << static_cast<int> (m_cents / 100);
		result = t.str();
		len = result.length();
	}
	else if ((flags & MONEY_LINE) && !m_cents)
	{
		t << "-.--";
	}
	else
	{
		// Negative amounts.
		if (m_cents < 0)
		{
			t << "-";
		}
		int cents = abs(m_cents);
		t << (int)(cents / 100) << ".";
		cents =cents % 100;
		t << (int)(cents / 10) << (cents % 10);
	}
	// insert komma
	result = t.str();
	len =(int)result.length();
	if (flags & MONEY_APE)
	{
		result.insert(0, ape);
		len +=(int)ape.length();
	}
	if (flags & MONEY_LEADING_SPACE)
	{
		result.insert(0, " ");
	}
	if (len < length)
	{
		result.insert(0, std::string(length-len, ' '));
	}
	return result;
}

/*----------------------------------------------------------------------------*/
/** @brief Convert money to a double.
 *  @return double.
 */
Cmoney::operator double(void) const
{
	return (double)m_cents;
}

/*----------------------------------------------------------------------------*/
/** @brief Check if money is zero.
 *  @return OK on not zero.
 */
bool Cmoney::Zero() const
{
	return (m_cents) ? false:true;
}

/*----------------------------------------------------------------------------*/
/** @brief Convert money to a string in simple form.
 *  @return string.
 */
Cmoney::operator std::string() const
{
	return to_string(8, MONEY_CENTS);
}

/*----------------------------------------------------------------------------*/
/** @brief Convert money to a long.
 *  @return long.
 */
Cmoney::operator long(void) const
{
	return m_cents;
}

/*----------------------------------------------------------------------------*/
/** @brief Convert money to a long.
 *  @return long.
 */
long Cmoney::Long() const
{
	return m_cents;
}

/*----------------------------------------------------------------------------*/
void Cmoney::setLong(const std::string &value)
{
	m_cents = atol(value.c_str());
}

/*----------------------------------------------------------------------------*/
/** @brief Convert float to money.
 *  @return money itself
 */
Cmoney & Cmoney::operator =(double f)
{
	m_cents=static_cast<long>(f);
	return *this;
}

/*----------------------------------------------------------------------------*/
/** @brief Invert money.
 */
void Cmoney::Negative()
{
	m_cents=-m_cents;
}

/*----------------------------------------------------------------------------*/
/** @brief Get money from file handle.
 *  @param F [in] location to get money.
 */
void Cmoney::load( FILE *F)
{
	m_cents= fgetc(F)+(fgetc(F) << 8);
	m_cents= m_cents*100 + fgetc(F);
}

/*----------------------------------------------------------------------------*/
/** @brief Check if money is zero.
 *  @return OK on not zero.
 */
bool Cmoney::Flag() const
{
	return (m_cents) ? true:false;
}

/*----------------------------------------------------------------------------*/
/** @brief Print money representation in string.
 *  @return string of 10 characters.
 */
const char *Cmoney::Print(int length) const
{
	static std::string t;
	if (length <0)
	{
		length =8-2;
	}
	t = to_string(length, MONEY_CENTS);
	return t.c_str();
}

/*----------------------------------------------------------------------------*/
/** @brief Multiply integer with money.
 *  @return money*constant
 */
Cmoney operator *(int x, Cmoney &b)
{
	Cmoney c(b*x);
	return c;
}

/*----------------------------------------------------------------------------*/
/** @brief Compare 2 amounts.
 *  @param a [in] Money input a
 *  @param b [in] Money input b
 *  @return true for smaller or equal
 */
bool operator <=(const Cmoney &a, const Cmoney &b)
{
	return (a.Long() <= b.Long()) ? true:false;
}

/*----------------------------------------------------------------------------*/
/** @brief Compare 2 amounts.
 *  @param a [in] Money input a
 *  @param b [in] Money input b
 *  @return true for greater
 */
bool operator >(const Cmoney &a, const Cmoney &b)
{
	return (a.Long() >b.Long());
}

/*----------------------------------------------------------------------------*/
/** @brief Compare 2 amounts.
 *  @param a [in] Money input a
 *  @param b [in] Money input b
 *  @return true for greater or equal
 */
bool operator >=(const Cmoney &a, const Cmoney &b)
{
	return (a.Long() >=b.Long());
}

/*----------------------------------------------------------------------------*/
/** @brief Minus amount, e.g. return -amount.
 *  @param a [in] Money input a
 */
const Cmoney operator -(const Cmoney &a)
{
	return Cmoney(-a.m_cents);
}
