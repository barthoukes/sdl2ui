/*
 * password.hpp
 *
 *  Created on: 3 march 2019
 *      Author: mensfort
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2019
 ** Houkes Horeca Applications
 **===========================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <string>

// @brief Class to create passwords
class Cpassword
{
public:
	Cpassword() {}
	virtual ~Cpassword() {}
	static bool isEncoded( const std::string &data);
	static std::string encode( const std::string &data);
	static std::string decode( const std::string &data);
};

/* End PASSWORD_H */
