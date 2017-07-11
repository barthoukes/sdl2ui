/*============================================================================*/
/**  @file		money.h
 *   @ingroup   zhongcan
 *   @brief     Money and easy money.
 *
 *   Classes to deal with money.
 *
 *   @author    Bart Houkes
 *============================================================================*/
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
#include <string>
#include <stdio.h>
#include "glue_tinyxml.h"


#define MONEY_LINE  1
#define MONEY_CENTS 2
#define MONEY_APE   4

typedef int 	EmoneyFlags;
#define MONEY_SHOW_AFTER 		0		// No number or cursor visible before pressing.
#define MONEY_BAR     			1
#define MONEY_ASCII_ONLY		2
#define MONEY_KITCHEN 			2
#define MONEY_DEPOSIT			4
#define MONEY_CASH				8
#define MONEY_NON_CASH			16
#define MONEY_DIGITS_1			0x100
#define MONEY_DIGITS_2  		0x200
#define MONEY_DIGITS_3  		0x300
#define MONEY_DIGITS_4  		0x400
#define MONEY_DIGITS_5  		0x500
#define MONEY_DIGITS_6  		0x600
#define MONEY_DIGITS_7  		0x700
#define MONEY_DIGITS_8  		0x800
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
#define MONEY_DIGITS_255        0xFF00
#define MONEY_SHOW_0TO9         0x10000
#define MONEY_DRAW_GRAPHICS		0x20000
#define MONEY_BACKSPACE_KEY		0x40000
#define MONEY_NO_COMMA			0x80000
#define MONEY_SHOW_AT_FIRST  	0x100000	// Show cursor at first sight.
#define SHOW_AT_FIRST			0x200000
#define MONEY_LEADING_ZERO		0x400000
#define MONEY_NO_CLEAR_AMOUNT 	0x800000

/// @brief Class to handle money.
class Cmoney
{
public:
	Cmoney(long g, int c) { m_cents=g*100+c; }
	//Cmoney(double d);
	Cmoney(long c=0): m_cents(c) {}

private:
	long m_cents; ///< Real amounts always in cents.

public:
	void add( CGLUE_tinyXML &x, TiXmlElement *root, const char *name) const;
	void load(CGLUE_tinyXML &x, TiXmlElement *root, const char *name);
	Cmoney & operator +=(const Cmoney &x);
	Cmoney & operator *=(int x);
	Cmoney & operator *=(double d);
	Cmoney & operator -=(const Cmoney &v);
	Cmoney & operator /=(long x);
	Cmoney & operator =(double f);
	Cmoney & operator *=(const Cmoney &v);

	long Long() const;
	void load(FILE *F);
	int Flag() const;
	void Negative();
	void Round(void);
	operator double(void) const;
	operator std::string() const;
	void String(char *s, int length, int flags, const char *ape="") const;
	char *Print() const;
	operator long(void) const;
};

Cmoney operator +(const Cmoney &a, const Cmoney &b);
bool operator ==(const Cmoney &a, const Cmoney &b);
bool operator !=(const Cmoney &a, const Cmoney &b);
Cmoney operator -(const Cmoney &a, const Cmoney &b);
bool operator <(const Cmoney &a, const Cmoney &b);
bool operator <=(const Cmoney &a, const Cmoney &b);
bool operator >=(const Cmoney &a, const Cmoney &b);
Cmoney operator *(int x, const Cmoney &b);
Cmoney operator *(const Cmoney &b, int x);
Cmoney operator /(const Cmoney &b, int x);
double Result(double x, char sign, double y);

/// @brief Total amount to show/print.
class account_money
{
public:
	account_money() {}
	~account_money() {}
public:
	Cmoney		restaurant_total_high;	///< Specific amount to print/display.
	Cmoney		restaurant_total_low;	///< Specific amount to print/display.
	Cmoney		takeaway_total_high;	///< Specific amount to print/display.
	Cmoney		takeaway_total_low;		///< Specific amount to print/display.
	Cmoney		delivery_total_high;	///< Specific amount to print/display.
	Cmoney		delivery_total_low;		///< Specific amount to print/display.
	Cmoney		restaurant_charity_total;	///< Total charity
	Cmoney		takeaway_charity_total; ///< Total charity
	Cmoney		restaurant_tax_high;	///< Specific amount to print/display.
	Cmoney		restaurant_tax_low;		///< Specific amount to print/display.
	Cmoney		takeaway_tax_high;		///< Specific amount to print/display.
	Cmoney		takeaway_tax_low;		///< Specific amount to print/display.
	Cmoney		delivery_tax_high;		///< Specific amount to print/display.
	Cmoney		delivery_tax_low;		///< Specific amount to print/display.
	Cmoney		restaurant_total_tax_free; ///< Specific amount for tax-free.
	Cmoney		takeaway_total_tax_free; ///< Specific amount for tax-free.
	Cmoney		delivery_total_tax_free; ///< Specific amount for tax-free.

	/** @brief Add amounts to account-money.
	 *  @param x [in] What we want to add.
	 *  @post  my money increased by x.
	 */
	account_money & operator +=(const account_money &x)
	{
		restaurant_total_high +=x.restaurant_total_high;
		restaurant_total_low  +=x.restaurant_total_low;
		restaurant_tax_high   +=x.restaurant_tax_high;
		restaurant_tax_low    +=x.restaurant_tax_low;
		takeaway_tax_high     +=x.takeaway_tax_high;
		takeaway_tax_low      +=x.takeaway_tax_low;
		takeaway_total_high   +=x.takeaway_total_high;
		takeaway_total_low    +=x.takeaway_total_low;
		restaurant_charity_total+=x.restaurant_charity_total;
		takeaway_charity_total  +=x.takeaway_charity_total;
		delivery_total_high   +=x.delivery_total_high;
		delivery_total_low    +=x.delivery_total_low;
		delivery_tax_high     +=x.delivery_tax_high;
		delivery_tax_low      +=x.delivery_tax_low;
		restaurant_total_tax_free +=x.restaurant_total_tax_free;
		takeaway_total_tax_free +=x.takeaway_total_tax_free;
		delivery_total_tax_free +=x.delivery_total_tax_free;
		return *this;
	}

public:
	std::string getTotal() const;
	std::string getTotalLow() const;
	std::string getTotalHigh() const;
	std::string getTotalTaxFree() const;
	std::string getTaxLow() const;
	std::string getTaxHigh() const;
	std::string getTotalCharity() const;
};

/// @brief Total amount to show/print.
class payment_money
{
public:
	payment_money() {}
	~payment_money() {}
	Cmoney all() { return cash+pin+credit_card+visa+pay_pal+master_card+american_express+send_bill+renminbi+return_amount; }

public:
	Cmoney		cash;				///< Specific amount to print/display.
	Cmoney		pin; 				///< Specific amount to print/display.
	Cmoney		credit_card;		///< Specific amount to print/display.
	Cmoney		visa;				///< Specific amount to print/display.
	Cmoney		pay_pal; 			///< Specific amount to print/display.
	Cmoney		master_card;		///< Specific amount to print/display.
	Cmoney		american_express;	///< Specific amount to print/display.
	Cmoney		send_bill;			///< Specific amount to print/display.
	Cmoney		renminbi;			///< Specific amount to print/display.
	Cmoney		return_amount;		///< Specific amount to print/display.
	/** @brief Add amounts to account-money.
	 *  @param x [in] What we want to add.
	 *  @post  my money increased by x.
	 */
	payment_money & operator +=(const payment_money &x)
	{
		cash +=x.cash;
		pin +=x.pin;
		credit_card +=x.credit_card;
		visa +=x.visa;
		pay_pal +=x.pay_pal;
		master_card +=x.master_card;
		american_express +=x.american_express;
		send_bill +=x.send_bill;
		renminbi +=x.renminbi;
		return_amount +=x.return_amount;
		return *this;
	}
	int count() const
	{
		int retVal =(cash.Long()!=0) + (pin.Long()!=0) +
				    (credit_card.Long()!=0) +(visa.Long()!=0) +
				    (pay_pal.Long()!=0) +(master_card.Long()!=0) +
				    (american_express.Long()!=0) +(send_bill.Long()!=0) +
				    (renminbi.Long()!=0) + (return_amount.Long()!=0);
		return retVal;
	}
};

account_money operator +(const account_money &a, const account_money &b);

