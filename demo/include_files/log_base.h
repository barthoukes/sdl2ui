/*============================================================================*/
/** @file       log_base.h
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
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <string>
#include "singleton.h"

#define myMin(a,b)    	(((a) < (b)) ? (a) : (b))
#define myMax(a,b)    	(((a) > (b)) ? (a) : (b))
#define myLimit(a,mn,mx)(((a) > (mn)) ? myMin(mx,a) : (mn))
#define myMax3(a,b,c) 	(((a) > (b)) ? (myMax(a,c)) : (myMax(b,c)))

/*------------- Symbolic constants -------------------------------------*/
#define MAX_PATH 256
#define MAX_LOG_SIZE 4096

/*------------- Macros -------------------------------------------------*/

/// @brief Macro to make software more readable. eg. Log.write("hello %s", x)
#define Log  (*ClogBase::Instance())

/*------------- Enum definitions ---------------------------------------*/

/*------------- Structure definitions ----------------------------------*/

/*------------- Exported class declarations ----------------------------*/

/*========================================================================
**              ClogBase
**======================================================================*/
/** Log file class.
**  This class implements a log file with the possibility to write
**  log entries into the file.
*/
/*======================================================================*/
class ClogBase : public Tsingleton<ClogBase>
{
	friend class Tsingleton<ClogBase>;
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
	pthread_mutex_t m_criticalSection;

    /** Name of the log file.
    */
    char        m_szFileName[MAX_PATH+1]; // was TCHAR

    /** Name of the old log file. It is the log file name extended with "old".
    */
    char        m_szOldFileName[MAX_PATH+1];

	/** See if comments are allowed. */
	bool		m_comments;

    /** Enable warnings. */
	bool 		m_warnings;

	/** Buffer for temporary text. */
	char		m_buffer[MAX_LOG_SIZE];

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
	/// @param comments [in] If comments go in this file.
	/// @param append [in] If file has to append.
	/// @param path [in] If the file should open in c:documents and settings...WTC
    /// @return BSS_OK on success, BSS_ERROR at failure.
    ///
    virtual bool open( const char *fileName, bool comments,
    		           bool append, bool time =true, bool warnings=false);

    /// Close the log file.
    virtual void close( void);
	/// Dump header.
	virtual bool writeHeader( void);
	/// Write text
	virtual void write1( const char *logText);
	/// Write text
	virtual void error( const char *logText, ...);
	/// Write text
	virtual void warning( const char *logText, ...);
	/// Write text
	virtual void errorIf(  bool statement, const char *logText, ...);
	/// Write text
	virtual void write( const char *logText, ...);
	/// enable all.
	virtual void enableAll()
	{ m_warnings =true; }
	/// writeHex.
	virtual void writeHex( const char *logText, const unsigned char *byteArray, int length, ...);
	/// Write text
	virtual void writeIf( bool statement, const char *logText, ...);
	/// Get the path to the log-file.
	virtual bool getPathName( const char *fileName);
	/// Get file names.
	virtual bool getFileName( char *fileName, char *fileName2) 
	{ 
		*fileName =0; *fileName2 =0; return true;
	}
	/// Get additional dir name.
	virtual bool getDirName( char *dirName)
	{
		*dirName =0; return true;
	}
	/// Set max. file size.
	virtual bool SetMaxFileSize( int length )
	{
		m_maxFileSize =length;
		return true;
    }
	/// Operator to add logging.
	ClogBase& operator<<(unsigned char by)
	{
		write( "%02X[-CR]", by);
		return *this;
	}
	/// Operator to add logging.
	ClogBase& operator<<(short w)
	{
		write( "%d[-CR]", w);
		return *this;
	}
	/// Operator to add logging.
	ClogBase& operator<<(long l)
	{
		write( "%ld[-CR]", l);
		return *this;
	}
	/// Operator to add logging.
	ClogBase& operator<<(unsigned long l)
	{
		write( "%ul[-CR]", l);
		return *this;
	}
	/// Operator to add logging.
	ClogBase& operator<<(double f)
	{
		write( "%f[-CR]", f);
		return *this;
	}
	/// Operator to add logging.
	//ClogBase& operator<<( int64_t dwdw)
	//{
	//	write( "%08X%08X[-CR]",(size_t)(dwdw >> 32),(size_t)(dwdw & 0xFFFFFFFF));
	//	return *this;
	//}
	/// Operator to add logging.
	//ClogBase& operator<<( uint64_t dwdw)
	//{
	//	write( "%08X%08X[-CR]",(size_t)(dwdw >> 32),(size_t)(dwdw & 0xFFFFFFFF));
	//	return *this;
	//}
	/// Operator to add logging.
	ClogBase& operator<<(char ch)
	{
		char s[8];
		s[0]=ch;
		s[1]=0;
		s[2]=0;
		if ( ch=='\n')
		{
			write("");
		}
		else
		{
			s[0]=ch; s[1]='['; s[2]='-'; s[3]='C'; s[4]='R'; s[5]=']'; s[6]=0;
			write1( s );
		}
		return *this;
	}
	/// Operator to add logging.
	ClogBase& operator<<(char *ch)
	{
		write( "%s[-CR]", ch);
		return *this;
	}
	/// Operator to add logging.
	ClogBase& operator<<(const std::string &ch)
	{
		write( "%s[-CR]", ch.c_str() );
		return *this;
	}
	/// Operator to add logging.
	ClogBase& operator<<(unsigned u)
	{
		write( "%u[-CR]", u);
		return *this;
	}
	/// Operator to add logging.
	ClogBase& operator<<(bool u)
	{
		write( u?"TRUE[-CR]":"FALSE[-CR]");
		return *this;
	}
	/// Stop
	virtual void stop( const char *logText, ...);
	/// Stop if
	virtual void stopIf( bool statement, const char *logText, ...);

private:
	/// Write text
    virtual void writeText( const char *logText);

};

extern void handler(int sig);

#define ASSERT(a)  \
	if (!(a)) { handler(0); assert(NULL); }


/*------------- Exported function declarations -------------------------*/

/*========================================================================
**      end of file
**======================================================================*/
