/*============================================================================*/
/**  @file		money.hpp
 *   @ingroup   zhongcan
 *   @brief     Money and easy money.
 *
 *   Classes to deal with money.
 *
 *   @author    Bart Houkes
 *============================================================================*/

#pragma once

#include <string>
#include <stdio.h>

/// If we want a horizontal line below the amount
#define MONEY_LINE  1
/// If we want in cents or euros
#define MONEY_CENTS 2
/// If we want an apetale in front of the amount
#define MONEY_APE   4
/// Width of normal amounts
#define MONEY_WIDTH 8

typedef int 	EmoneyFlags;
#define MONEY_SHOW_AFTER 		0		// No number or cursor visible before pressing.
#define MONEY_BAR     			1
#define MONEY_ASCII_ONLY		2
#define MONEY_KITCHEN 			2
#define MONEY_DEPOSIT			4
#define MONEY_CASH				8
#define MONEY_NON_CASH			16
#define MONEY_DIGITS(n)			((n)*MONEY_DIGITS_1)
#define MONEY_DIGITS_1			0x100
#define MONEY_DIGITS_2  		0x200
#define MONEY_DIGITS_3  		0x300
#define MONEY_DIGITS_4  		0x400
#define MONEY_DIGITS_5  		0x500
#define MONEY_DIGITS_6  		0x600
#define MONEY_DIGITS_7  		0x700
#define MONEY_DIGITS_8  		0x800
#define MONEY_DIGITS_9  		0x900
#define MONEY_DIGITS_10 		0xa00
#define MONEY_DIGITS_12 		0xc00
#define MONEY_DIGITS_13 		0xd00
#define MONEY_DIGITS_15 		0xf00
#define MONEY_DIGITS_16 		0x1000
#define MONEY_DIGITS_18 		0x1200
#define MONEY_DIGITS_22 		0x1600
#define MONEY_DIGITS_24 		0x1800
#define MONEY_DIGITS_30 		0x1e00
#define MONEY_DIGITS_20 		0x1400
#define MONEY_DIGITS_40 		0x2800
#define MONEY_DIGITS_60 		0x3c00
#define MONEY_DIGITS_255        0xFF00
#define MONEY_SHOW_0TO9         0x10000
#define MONEY_DRAW_GRAPHICS		0x20000
#define MONEY_BACKSPACE_KEY		0x40000
#define MONEY_NO_COMMA			0x80000
#define MONEY_SHOW_AT_FIRST  	0x100000	// Show cursor at first sight.
#define SHOW_AT_FIRST			0x200000
#define MONEY_LEADING_ZERO		0x400000
#define MONEY_NO_CLEAR_AMOUNT 	0x800000
#define MONEY_LEADING_SPACE     0x1000000

/// @brief Class to handle money.
class Cmoney
{
public:
	/// Constructor
	Cmoney(): m_cents(0) {}
	/// Constructor
	Cmoney(long g, int c) { m_cents=g*100+c; }
	/// Constructor
	explicit Cmoney(long c): m_cents(c) {}
	/// Constructor
	explicit Cmoney(int c): m_cents(c) {}
	/// Constructor
	explicit Cmoney(short c): m_cents(c) {}
	/// Constructor from string
	explicit Cmoney( const std::string &m);

	friend const Cmoney operator +(const Cmoney &a, const Cmoney &b);
	friend bool operator ==(const Cmoney &a, const Cmoney &b);
	friend bool operator !=(const Cmoney &a, const Cmoney &b);
	friend const Cmoney operator -(const Cmoney &a);
	friend const Cmoney operator -(const Cmoney &a, const Cmoney &b);
	friend bool operator <(const Cmoney &a, const Cmoney &b);
	friend bool operator <=(const Cmoney &a, const Cmoney &b);
	friend bool operator >=(const Cmoney &a, const Cmoney &b);
	friend const Cmoney operator *(int x, const Cmoney &b);
	friend const Cmoney operator *(const Cmoney &b, int x);
	friend const Cmoney operator /(const Cmoney &b, int x);
	friend double Result(double x, char sign, double y);

protected:
	long m_cents; ///< Real amounts always in cents.

public:
	Cmoney & operator +=(const Cmoney &x);
	Cmoney & operator *=(int x);
	Cmoney & operator *=(double d);
	Cmoney & operator -=(const Cmoney &v);
	Cmoney & operator /=(long x);
	Cmoney & operator =(double f);
	Cmoney & operator *=(const Cmoney &v);
    Cmoney & operator  =(int x);
    Cmoney & operator  =(long x);

    long Long() const;
	bool Zero() const;
	void load(FILE *F);
	bool Flag() const;
	void Negative();
	void setLong(const std::string &value);
	std::string to_string(int length, int flags, const std::string &ape="") const;
	operator double(void) const;
	operator std::string() const;
	const char *Print(int length=-1) const;
	operator long(void) const;
};

/** Price and price for half portion class */
class priceAndHalfPrice
{
public:
	priceAndHalfPrice()
	: valid(true)
	, full_price(0)
	, half_price(0) {}
	~priceAndHalfPrice() {}

	bool valid;
	Cmoney full_price;
	Cmoney half_price;
	void clear() { full_price = Cmoney(0); half_price = Cmoney(0); }
};
