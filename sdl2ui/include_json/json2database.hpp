/*============================================================================*/
/**  @file      json2database.hpp
 **  @ingroup   dialogs
 **  @brief		Json command for the database.
 **
 **  Send command to the database using json
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cjson2Database
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 23 may 2021
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/
#include <assert.h>

/*------------- Program includes ---------------------------------------------*/
#include <string>

#include "json_reader.hpp"

/*----------------------------------------------------------------------------*/
class Cjson2Database
{
public:
	Cjson2Database(const std::string &description);
	~Cjson2Database();
	void decode(const Json::Value &value);

private:
	Json::Features 	m_features;		///< Needed for parsing json.
	Json::Reader 	m_reader;		///< Needed for parsing json.
	Json::Value 	m_root;			///< Needed for parsing json.
	std::string     m_lastType;     ///< command received

};

