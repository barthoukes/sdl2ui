/*============================================================================*/
/** @file       log_base.hpp
**  @ingroup    include_files
**  @brief		Log file module base class for many projects.
** 
**  Log file module.
**  This file contains the class for writing log entries into a log file.
**  Also functions are provided to write different levels of log
**  informations into zhongcan.log according to a configured log level.
**
**  @author Bart Houkes
**
**  @par Classes:
**              ClogBase
*/
/*------------------------------------------------------------------------------
**  Copyright (c) Bart Houkes, 2011
** 
**  Copyright notice:
**  This software is property of Bart Houkes
**  Unauthorized duplication and disclosure to third parties is forbidden.
**============================================================================*/

#pragma once

/*------------- Necessary include files --------------------------------*/
#include <mutex>
#include <string>
#include <memory>
#include <sstream>

#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "log_interface.hpp"

#define myMin(a,b)    	(((a) < (b)) ? (a) : (b))
#define myMax(a,b)    	(((a) > (b)) ? (a) : (b))
#define myLimit(a,mn,mx)(((a) > (mn)) ? myMin(mx,a) : (mn))
#define myMax3(a,b,c) 	(((a) > (b)) ? (myMax(a,c)) : (myMax(b,c)))

/*------------- Symbolic constants -------------------------------------*/
#define MAX_PATH 256
#define MAX_LOG_SIZE 4096

/*------------- Macros -------------------------------------------------*/

/// @brief Macro to make software more readable. eg. Log.write("hello %s", x)
#define Endl '\n'

/*------------- Enum definitions ---------------------------------------*/

/*------------- Structure definitions ----------------------------------*/

/*------------- Exported class declarations ----------------------------*/

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/*========================================================================
**              ClogBase
**======================================================================*/
/** Log file class.
**  This class implements a log file with the possibility to write
**  log entries into the file.
*/
/*======================================================================*/
class ClogBase : public ClogInterface
{
// Attributes
public:
	/** Show errors in a messagebox. */
	bool		m_showErrors;

	/** Disable writing to the log file. */
	bool		m_disableLogFile;

// Attributes
private:
    /** File pointer of log file.
    */
    FILE *      m_file_p;

    /** Counter of how many bytes are written into file after opening.
    */
    int			m_bytesInFile;

    /** Maximum size of log information to be written into file after opening.
    *   The file size can get bigger if there were already data in the
    *   log file before opening.
    */
    int			m_maxFileSize;

	/** Critical section */
    std::recursive_mutex  m_mutex;

    /** Name of the log file.
    */
    std::string m_szFileName; // was TCHAR

    /** Name of the old log file. It is the log file name extended with "old".
    */
    std::string m_szOldFileName;

	/** See if comments are allowed. */
	bool		m_logToDisplay;

    /** Enable warnings. */
	ElogLevel   m_warningLevel;

	/** Show the time. */
	bool		m_time;

// Constructors
public:
    /** Constructor.
    */
    ClogBase( void);
    
	/** Get the time. */
	//static void getTime( char *txt);

    /** Default destructor.
    *   The log file gets closed if it is still open.
    */
	virtual ~ClogBase( void);

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
    		           bool append, bool time, ElogLevel level);

    /// Close the log file.
    virtual void close( void);
	/// Dump header.
	virtual bool writeHeader( void);
	/// Write text
	virtual void write1( const std::string &logText);
	/// Write text
	virtual void error( const char *logText, ...);
	/// Write text
	virtual void warning( const char *logText, ...);
	/// Write text
	virtual void errorIf(  bool statement, const char *logText, ...);
	/// Write text
	virtual ClogBase &write( const char *logText, ...);
	/// enable all.
	virtual void enableAll()
	{ m_warningLevel =LOG_ALL; }
	/// writeHex.
	virtual void writeHex( const char *logText, const unsigned char *byteArray, int length, ...);
	/// Write text
	virtual void writeIf( bool statement, const char *logText, ...);
	/// Get additional dir name.
	virtual bool getDirName( char *dirName);
	/// Set max. file size.
	virtual bool SetMaxFileSize( int length );
	/// Operator to add logging.
	ClogInterface& operator<<(unsigned char by);
	/// Operator to add logging.
	ClogInterface& operator<<(short w);
    /// Operator to add logging.
    ClogInterface& operator<<(int l);
    /// Operator to add logging.
    ClogInterface& operator<<(unsigned int l);
	/// Operator to add logging.
	ClogInterface& operator<<(long l);
	/// Operator to add logging.
	ClogInterface& operator<<(unsigned long l);
	/// Operator to add logging.
	ClogInterface& operator<<(double f);
	/// Operator to add logging.
	ClogInterface& operator<<(char ch);
	/// Operator to add logging.
	ClogInterface& operator<<(char *ch);
    /// Operator to add logging.
    ClogInterface& operator<<(const char *ch);
	/// Operator to add logging.
	ClogInterface& operator<<(const std::string &input);
	/// Operator to add logging.
	ClogInterface& operator<<(bool u);
	/// Stop
	virtual void stop( const char *logText, ...);
	/// Stop if
	virtual void stopIf( bool statement, const char *logText, ...);

	virtual void stackTrace();

private:
	/// Write text
    virtual void writeTextInternal( const std::string &input);
	// Next line
	virtual void nextLineInternal();
	// Check maximum file length
	virtual void checkNextFile(int length);

private:
	std::stringstream m_composition;
	bool              m_emptyLine;
};

extern void handler(int sig);

typedef std::shared_ptr<ClogBase> ClogBasePtr;

#define ASSERT(a)  \
	if (!(a)) { handler(0); assert(NULL); }


/*------------- Exported function declarations -------------------------*/

/*========================================================================
**      end of file
**======================================================================*/
