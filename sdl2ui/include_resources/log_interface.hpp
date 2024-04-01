/*============================================================================*/
/** @file       log_interface.hpp
**  @ingroup    include_files
**  @brief		Log file module interface class for many projects.
** 
**  Log file interface.
**  This file contains the class for writing log entries into a log file.
**  Also functions are provided to write different levels of log
**  informations into zhongcan.log according to a configured log level.
**
**  @author Bart Houkes
**
**  @par Classes:
**              ClogInterface
*/
/*------------------------------------------------------------------------------
**  Copyright (c) Bart Houkes, 2022
** 
**  Copyright notice:
**  This software is property of Bart Houkes
**  Unauthorized duplication and disclosure to third parties is forbidden.
**============================================================================*/

#pragma once

/*------------- Necessary include files --------------------------------*/
#include <memory>
#include <string>
#include <string.h>
#include "log_level_type.hpp"

#define Endl '\n'

/*------------- Enum definitions ---------------------------------------*/

/*------------- Structure definitions ----------------------------------*/

/*------------- Exported class declarations ----------------------------*/

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/*========================================================================
**              ClogInterface
**======================================================================*/
/** Log file class.
**  This class implements a log file with the possibility to write
**  log entries into the file.
*/
/*======================================================================*/
class ClogInterface
{
// Attributes
public:

// Attributes
private:

// Constructors
public:
    /** Constructor */
    ClogInterface( void) {}

    /** Default destructor */
	virtual ~ClogInterface() {}

	/// Write text
	virtual void warning( const char *logText, ...) =0;
	/// Errors
	virtual void error( const char *logText, ...) =0;
	/// writeHex.
	virtual void writeHex( const char *logText, const unsigned char *byteArray, int length, ...) =0;

	virtual void stackTrace() =0;

// Methods
public:
    /// Open the log file.
    /// The file is opened in the WTC.dll log file directory.
    /// If the log file already exits, new entries will be appended to it.
    /// @param fileName [in] I Name of the log file without path.
	/// @param logToDisplay [in] If comments go in this file.
	/// @param append [in] If file has to append.
	/// @param path [in] If the file should open in c:documents and settings...WTC
    /// @return BSS_OK on success, BSS_ERROR at failure.
    ///
    virtual bool open( const std::string &fileName, bool showOnDisplay,
    		           bool append, bool time, ElogLevel level) =0;

	/// Write text
	virtual ClogInterface& write( const char *logText, ...) =0;

    /// Close the log file.
    virtual void close( void) =0;
	/// Operator to add logging.
    virtual ClogInterface& operator<<(unsigned char by) =0;
	/// Operator to add logging.
    virtual ClogInterface& operator<<(short w) =0;
    /// Operator to add logging.
    virtual ClogInterface& operator<<(int l) =0;
    /// Operator to add logging.
    virtual ClogInterface& operator<<(unsigned int l) =0;
	/// Operator to add logging.
    virtual ClogInterface& operator<<(long l) =0;
	/// Operator to add logging.
    virtual ClogInterface& operator<<(unsigned long l) =0;
	/// Operator to add logging.
    virtual ClogInterface& operator<<(double f) =0;
	/// Operator to add logging.
    virtual ClogInterface& operator<<(char ch) =0;
	/// Operator to add logging.
    virtual ClogInterface& operator<<(char *ch) =0;
    /// Operator to add logging.
    virtual ClogInterface& operator<<(const char *ch) =0;
	/// Operator to add logging.
    virtual ClogInterface& operator<<(const std::string &ch) =0;
	/// Operator to add logging.
    virtual ClogInterface& operator<<(bool u) =0;
};

typedef std::shared_ptr<ClogInterface> ClogInterfacePtr;

/*------------- Exported function declarations -------------------------*/

/*========================================================================
**      end of file
**======================================================================*/
