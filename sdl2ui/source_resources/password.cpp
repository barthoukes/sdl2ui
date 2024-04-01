/*============================================================================*/
/**  @file      password.cpp
 **  @ingroup   zhongcan
 **  @brief		Encode or decode password
 **
 **  Decode or encode password for zhongcan
 **
 **  @author Bart
 **
 **  @par Project:
 **  Security Systems: Zhongcan
 **
 **  @par Classes:
 **              ClogBase
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

/*------------- Standard includes --------------------------------------------*/

/*------------- Module options / compiler switches ---------------------------*/

/*------------- Includes -----------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "password.hpp"


bool Cpassword::isEncoded( const std::string &data)
{
	if (data.length() < 20)
	{
		return false;
	}
	std::string str = decode(data);
	return !str.empty();
}

//echo whatever | openssl enc -aes-128-cbc -a -salt -pass pass:zhongguo > /tmp/2.txt
std::string Cpassword::encode( const std::string &data)
{
	std::string s="echo " + data + " | openssl enc -aes-128-cbc -a -salt -pass pass:zhongguo > /tmp/2.txt";
	system( s.c_str() );
	FILE *F =fopen("/tmp/2.txt", "r");
	if (!F)
	{
		exit(-1);
	}
	char str[256];
	(void)fgets( str, 255, F);
	fclose(F);
	int n=strlen(str);
	while (n>0 && str[n-1]=='\n')
	{
		str[--n]=0;
	}
	return str;
}

//cat /tmp/2.txt | openssl enc -aes-128-cbc -a -d -salt -pass pass:zhongguo > /tmp/3.txt
std::string Cpassword::decode( const std::string &data)
{
	std::string s="echo " + data + " | openssl enc -aes-128-cbc -a -d -salt -pass pass:zhongguo > /tmp/2.txt";
	(void)system( s.c_str() );
	FILE *F =fopen("/tmp/2.txt", "r");
	if (!F)
	{
		exit(-1);
	}
	char str[256];
	(void)fgets( str, 255, F);
	fclose(F);
	int n=strlen(str);
	while (n>0 && str[n-1]=='\n')
	{
		str[--n]=0;
	}
	return str;
}
