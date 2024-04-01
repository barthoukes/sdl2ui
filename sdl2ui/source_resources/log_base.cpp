/*============================================================================*/
/**  @file       log_base.cpp
 **  @ingroup   zhongcan
 **  @brief		Log file module base class for many projects.
 **
 **  Log file module.
 **  This file contains the class for writing log entries into a log file.
 **  Also functions are provided to write different levels of log
 **  informations into xxx.log according to a configured log level.
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
#include "log_base.hpp"

#include <stdarg.h>

/*------------- Module options / compiler switches ---------------------------*/

/*------------- Includes -----------------------------------------------------*/

#include <string.h>
#include <assert.h>
#include <execinfo.h>
#include <signal.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include "timestamp.hpp"
#include <version_info.hpp>


/*------------- Local symbolic constants -------------------------------------*/

/** Default value of maximum size of new text to add since opening of log file.
 *   When this limit is reached, a backup of the log file is created and
 *   a new one started. This normally restricts the log file directory size
 *   to 10MB (2x5MB) at release version. The limit can also be configured
 *   in the configuration file by the entry "LogfileSize".
 */
#define DEFAULT_MAX_LOGFILE_SIZE 2000000

/// HEX value of a digit.
/// @param n [in] Digit to change in ascii.
#define hexDigit(n) \
	( ((n)>9) ? 'A'-10+(n):'0'+(n))

/// HEX value of 2 nibbles.
/// @param ptr [in] Where to put the number.
/// @param x [in] Number to convert.
#define hexByte(ptr,x) \
{ ptr[0]=hexDigit((unsigned char)((x)>>4)); ptr[1]=hexDigit((x)&0x0f); }

// First log instance.
ClogBase *logBase = NULL;

/*------------- Local macros -------------------------------------------------*/
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*------------- Local enum definitions ---------------------------------------*/

/*------------- Local structure definitions ----------------------------------*/

/*------------- Local functions prototypes -----------------------------------*/

/*------------- Global variables ---------------------------------------------*/

/*------------- Exported classes ---------------------------------------------*/

/*==============================================================================
 **              ClogBase
 **============================================================================*/
void handler( int sig)
{
	void *addresses[1024];
	char **strings;
	(void)sig;

	int size =backtrace(addresses, 1024);
	strings = backtrace_symbols(addresses, size);
	logBase->error("Stack frames: %d", size);
	for(int i = 0; i < size; i++)
	{
		logBase->error("%d: %X", i, (long)addresses[i]);
		logBase->error("%s", strings[i]);
	}
	free(strings);
	exit(-1);
}

/*==============================================================================
 **              ClogBase::ClogBase()
 **============================================================================*/
///
/// Constructor
///
/// @remarks    DEBUGGED
///                                 
/*============================================================================*/
ClogBase::ClogBase() :
	m_showErrors(true),
	m_disableLogFile(false),
	m_file_p(NULL),
	m_bytesInFile(0),
	m_maxFileSize(DEFAULT_MAX_LOGFILE_SIZE),
	m_logToDisplay(true),
	m_warningLevel(LOG_NOTHING),
	m_time(true),
	m_emptyLine(true)
{
	signal(SIGSEGV, handler);
	signal(SIGFPE, handler);
	signal(SIGABRT,handler);
}

/*==============================================================================
 **              ClogBase::~ClogBase()
 **============================================================================*/
///
/// @brief Destructor for Log files.
///
/// Destructor
///
/// @post       Class removed. Please also remove all references.
///               
/// @remarks    DEBUGGED
///                                 
/*============================================================================*/
ClogBase::~ClogBase()
{
	close();
	m_file_p = NULL;
	m_showErrors = false;
}

/*============================================================================*/

/*------------------- Exported functions -------------------------------------*/

/*==============================================================================
 **              ClogBase::open
 **============================================================================*/
///
/// @brief Find a register in the large database.
///
/// @param      fileName [in] file.
/// @param      showOnDisplay [in] Comments wanted.
/// @param      append [in] Write new file or open old one.
/// @param      time [in] Add time to each line.
/// @param      warningLevel [in] Only do the warnings or all.
///
/// @post       File opened in correct path.
///               
/// @return     TRUE on success.
///                 
/// @remarks    DEBUGGED
///               
/*============================================================================*/
bool ClogBase::open(const std::string &fileName, bool showOnDisplay, bool append,
		bool time, ElogLevel warningLevel)
{
	m_szFileName = fileName;
	m_szOldFileName = fileName + "_old";

	close();

	if (m_file_p != NULL)
	{
		return (true);
	}
	m_logToDisplay = showOnDisplay;
	m_warningLevel = warningLevel;
	m_time = time;
	m_file_p =fopen( m_szFileName.c_str(), append ? "at" : "wt");
	if (m_file_p == NULL)
	{
		return (false);
	}
    if (logBase == NULL)
    {
    	logBase = this;
    }

	fseek(m_file_p, -150, SEEK_END);
	m_bytesInFile = ftell(m_file_p);

	writeHeader();
	if (m_logToDisplay)
	{
	    *this << "==---------------------------------------------------==\n";
	    *this << "ClogBase::open  file opened for logging ptr=" << (long)m_file_p << Endl;
	}
	if (m_file_p)
		return true;
	return false;
}


/*==============================================================================
 **              ClogBase::writeHeader
 **============================================================================*/
///
/// Write the header.
///
/// @pre        File opened in correct path.
///               
/// @return     TRUE on success.
///                 
/// @remarks    DEBUGGED
///               
/*============================================================================*/
bool ClogBase::writeHeader(void)
{
	write( "*************************************************************\n"
	       "Start Logging version: %d.%d.%d\n"
	       "%s %s\n"
	       "*************************************************************",
		   VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_DATE, VERSION_TIME );

	// Please overload this function and add your own header.
	return true;
}

/*============================================================================*/
void ClogBase::stackTrace()
{
    void *addresses[1024];
    char **strings;

    int size =backtrace(addresses, 1024);
    strings = backtrace_symbols(addresses, size);
    error("Stack frames: %d", size);
    for(int i = 0; i < size; i++)
    {
        error("%d: %X", i, (long)addresses[i]);
        error("%s", strings[i]);
    }
    free(strings);
}

/*==============================================================================
 **              ClogBase::close
 **============================================================================*/
///
/// Close your logging.
///                 
/// @remarks    DEBUGGED
///                                 
/*=============================================================================*/
void ClogBase::close(void)
{
	m_mutex.lock();

	if (m_file_p != NULL)
	{
		write("ClogBase::close  ptr=%d\n", (unsigned long) m_file_p);
		if (m_logToDisplay)
		{
			write("ClogBase::close  ptr=%d\n", (unsigned long) m_file_p);
		}
		(void) fclose(m_file_p);
		m_file_p = NULL;
	}
	m_mutex.unlock();
}

/*==============================================================================
 **              ClogBase::write1
 **============================================================================*/
///
/// Write single string to logfile.
///
///  @param      logText [in] Handle to output file.
///
///  @post       formatted text added to logging.
///
///  @remarks    DEBUGGED
///                                 
/*============================================================================*/
void ClogBase::write1(const std::string &logText)
{
    m_mutex.lock();

	if (!m_disableLogFile && m_warningLevel !=LOG_NOTHING && m_file_p !=NULL)
	{
		nextLineInternal();
		writeTextInternal(logText);
		nextLineInternal();
	}

    m_mutex.unlock();
}

/*==============================================================================
 **              ClogBase::write
 **============================================================================*/
///
/// Write to logfile.
///
///  @param      logText [in] Handle to output file.
///
///  @post       formatted text added to logging.
///
///  @remarks    DEBUGGED
///                                 
/*============================================================================*/
ClogBase &ClogBase::write(const char *logText, ...)
{
    m_mutex.lock();

    if (!m_disableLogFile && m_warningLevel ==LOG_ALL && m_file_p != NULL)
	{
		nextLineInternal();
        va_list args;
        va_start(args, logText);
    	char  buffer[MAX_LOG_SIZE];
        vsnprintf(buffer, MAX_LOG_SIZE - 1, logText, args);
        writeTextInternal(buffer);
        va_end(args);
		nextLineInternal();
	}

    m_mutex.unlock();
    return *this;
}

/*==============================================================================
 **              ClogBase::writeHex
 **============================================================================*/
///
/// Write to logfile.
///
///  @param      logText   [in] Text to print first.
///  @param		 byteArray [in] Array of bytes.
///  @param		 length    [in] Length for array.
///
///  @post       formatted text added to logging.
///
///  @remarks    DEBUGGED
///                                 
/*============================================================================*/
void ClogBase::writeHex(const char *logText, const unsigned char *byteArray, int length, ...)
{
    m_mutex.lock();

	if (m_disableLogFile || m_warningLevel !=LOG_ALL || m_file_p == NULL)
	{
	    m_mutex.unlock();
		return;
	}
	va_list args;
	va_start(args, length);

	char  buffer[MAX_LOG_SIZE];
	vsnprintf(buffer, MAX_LOG_SIZE - 1, logText, args);
	va_end(args);

	length = myMin(length, 512);
	char *ptr = buffer + strlen(buffer);
	while (length)
	{
		hexByte( ptr, *byteArray);
		byteArray++;
		ptr[2] = ' ';
		ptr += 3;
		length--;
	}
	ptr[0] = 0;
	ptr[1] = 0;
	nextLineInternal();
	writeTextInternal(buffer);
	nextLineInternal();

    m_mutex.unlock();
}

/*==============================================================================
 **              ClogBase::writeIf
 **============================================================================*/
///
///  Write to logfile in case of issue.
///
///  @param      logText [in] Handle to output file.
///  @param		 statement [in] Condition.
///
///  @post       formatted text added to logging.
///
///  @remarks    DEBUGGED
///                                 
/*============================================================================*/
void ClogBase::writeIf(bool statement, const char *logText, ...)
{
    m_mutex.lock();

	if (!m_disableLogFile && m_file_p != NULL)
	{
		if (statement)
		{
			va_list args;
			va_start(args, logText);

			char buffer[MAX_LOG_SIZE];
			vsnprintf(buffer, MAX_LOG_SIZE - 1, logText, args);
			writeTextInternal(buffer);
			va_end(args);
		}
	}

	m_mutex.unlock();
}

/*==============================================================================
 **              ClogBase::stopIf
 **============================================================================*/
///
///  Write to logfile. Assert in case of problems.
///
///  @param		 statement [in] Condition.
///  @param      logText [in] Handle to output file.
///
///  @post       formatted text added to logging.
///
///  @remarks    DEBUGGED
///                                 
/*============================================================================*/
void ClogBase::stopIf(bool statement, const char *logText, ...)
{
	if (statement)
	{
	    m_mutex.lock();

		va_list args;
		va_start(args, logText);

		char buffer[MAX_LOG_SIZE];
		try
		{
			sprintf(buffer, "ERROR:  ");
			vsnprintf(buffer + 8, MAX_LOG_SIZE - 1 - 8, logText, args);
		}
		catch(...)
		{
			sprintf( buffer+8, "Logging statement!!");
		}
		if (!m_disableLogFile)
		{
			nextLineInternal();
			writeTextInternal(buffer);
			nextLineInternal();
		}
		va_end(args);

	    m_mutex.unlock();
		assert(NULL);
	}
}

/*==============================================================================
 **              ClogBase::error
 **============================================================================*/
///
/// @brief Write ERROR to logfile.
///
/// @param      logText [in] Handle to output file.
///
/// @remarks    DEBUGGED
///                                 
/*============================================================================*/
void ClogBase::error(const char *logText, ...)
{
    m_mutex.lock();

	va_list args;
	va_start(args, logText);

	char buffer[MAX_LOG_SIZE];
	try
	{
		sprintf(buffer, "ERROR:  ");
		vsnprintf(buffer + 8, MAX_LOG_SIZE - 1 - 8, logText, args);
	}
	catch(...)
	{
		sprintf( buffer+8, "Logging statement!!");
	}
	if (!m_disableLogFile)
	{
		nextLineInternal();
		writeTextInternal(buffer);
		nextLineInternal();
	}
	va_end(args);

    m_mutex.unlock();
}

/*==============================================================================
 **              ClogBase::warning
 **============================================================================*/
///
/// @brief Write WARNING to logfile.
///
/// @param      logText [in] Handle to output file.
///
/// @remarks    DEBUGGED
///
/*============================================================================*/
void ClogBase::warning(const char *logText, ...)
{
    m_mutex.lock();

	if ( m_warningLevel ==LOG_NOTHING)
	{
		return;
	}
	va_list args;
	va_start(args, logText);

	char buffer[MAX_LOG_SIZE];
	try
	{
	    sprintf(buffer, "WARNING:  ");
	    vsnprintf(buffer + 8, sizeof(buffer) - 9, logText, args);
	}
	catch(...)
	{
		sprintf( buffer+8, "Logging statement!!");
	}
	if (!m_disableLogFile)
	{
		nextLineInternal();
		writeTextInternal(buffer);
		nextLineInternal();
	}
	va_end(args);

	m_mutex.unlock();
}

/*==============================================================================
 **              ClogBase::errorIf
 **============================================================================*/
///
/// Write to logfile in case of statement is true. Error shown in case of problems.
///
///  @param		 statement [in] Condition.
///  @param      logText [in] Handle to output file.
///
///  @post       formatted text added to logging. 
///
///  @remarks    DEBUGGED
///                                 
/*============================================================================*/
void ClogBase::errorIf(bool statement, const char *logText, ...)
{
	if (statement)
	{
	    m_mutex.lock();

		va_list args;
		va_start(args, logText);

		char buffer[MAX_LOG_SIZE];
		try
		{
		    sprintf(buffer, "ERROR:  ");
		    vsnprintf(buffer + 8, sizeof(buffer) - 9, logText, args);
		}
		catch(...)
		{
			sprintf( buffer+8, "Logging statement!!");
		}
		if (!m_disableLogFile)
		{
			nextLineInternal();
			writeTextInternal(buffer);
			nextLineInternal();
		}
		va_end(args);

	    m_mutex.unlock();
	}
}

/*==============================================================================
 **              ClogBase::stop
 **============================================================================*/
///
/// @brief Write ERROR to logfile.
///
/// @param      logText [in] Handle to output file.
///
/// @remarks    DEBUGGED
///                                 
/*============================================================================*/
void ClogBase::stop(const char *logText, ...)
{
    m_mutex.lock();

	va_list args;
	va_start(args, logText);

	char buffer[MAX_LOG_SIZE];
	try
	{
	    sprintf(buffer, "ERROR:  ");
	    vsnprintf(buffer + 8, sizeof(buffer) - 9, logText, args);
	}
	catch(...)
	{
		sprintf( buffer+8, "Logging statement!!");
	}
	if (!m_disableLogFile)
	{
		nextLineInternal();
		writeTextInternal(buffer);
		nextLineInternal();
	}
	va_end(args);

    m_mutex.unlock();
	assert(0);
}

/*==============================================================================
 **              ClogBase::getPathName
 **============================================================================*/
///
/// Find path to your documents and settings.
///
/// @param      fileName	[in] file 1.
///
/// @pre        Windows 2000 or above.
///
/// @post       C:\\Documents and Settings\\All Users\\Documents calculated.
///               
/// @return     C:\\Documents and Settings\\All Users\\Documents
///                 
/// @remarks    DEBUGGED
///
/*============================================================================*/
#define LOG_FILE1   	 "/home/mensfort/zhongcan/log.txt"
#define LOG_FILE2   	 "/home/mensfort/zhongcan/log.old"

/// Get additional dir name.
bool ClogBase::getDirName( char *dirName)
{
    m_mutex.lock();

    *dirName =0;

    m_mutex.unlock();
    return true;
}

/// Set max. file size.
bool ClogBase::SetMaxFileSize( int length )
{
    m_mutex.lock();

    m_maxFileSize =length;

    m_mutex.unlock();
    return true;
}

// Operator to add logging.
ClogInterface& ClogBase::operator<<(unsigned char by)
{
    m_mutex.lock();

    if (by == '\n')
    {
    	nextLineInternal();
    }
    else
    {
    	m_composition << by;
        m_emptyLine = false;
    }

    m_mutex.unlock();
    return *this;
}
/// Operator to add logging.
ClogInterface& ClogBase::operator<<(short w)
{
    m_mutex.lock();

    m_composition << w;
    m_emptyLine = false;

    m_mutex.unlock();
    return *this;
}

/// Operator to add logging.
ClogInterface& ClogBase::operator<<(long l)
{
    m_mutex.lock();

    m_composition << l;
    m_emptyLine = false;

    m_mutex.unlock();
    return *this;
}

/// Operator to add logging.
ClogInterface& ClogBase::operator<<(unsigned long l)
{
    m_mutex.lock();

    m_composition << l;
    m_emptyLine = false;

    m_mutex.unlock();
    return *this;
}

/// Operator to add logging.
ClogInterface& ClogBase::operator<<(int l)
{
    m_mutex.lock();

    m_composition << l;
    m_emptyLine = false;

    m_mutex.unlock();
    return *this;
}

/// Operator to add logging.
ClogInterface& ClogBase::operator<<(unsigned int l)
{
    m_mutex.lock();

    m_composition << l;
    m_emptyLine = false;

    m_mutex.unlock();
    return *this;
}

/// Operator to add logging.
ClogInterface& ClogBase::operator<<(double f)
{
    m_mutex.lock();

    m_composition << f;
    m_emptyLine = false;

    m_mutex.unlock();
    return *this;
}

/// Operator to add logging.
ClogInterface& ClogBase::operator<<(char ch)
{
    m_mutex.lock();

    if (ch ==Endl || !ch)
    {
        nextLineInternal();
    }
    else
    {
        m_composition << ch;
        m_emptyLine = false;
    }

    m_mutex.unlock();
    return *this;
}

/// Operator to add logging.
ClogInterface& ClogBase::operator<<(char *ch)
{
    m_mutex.lock();

    writeTextInternal(std::string(ch));

    m_mutex.unlock();
    return *this;
}

/// Operator to add logging.
ClogInterface& ClogBase::operator<<(const char *ch)
{
    m_mutex.lock();

    writeTextInternal(std::string(ch));

    m_mutex.unlock();
    return *this;
}

/// Operator to add logging.
ClogInterface& ClogBase::operator<<(const std::string &input)
{
    m_mutex.lock();

    writeTextInternal(input);

    m_mutex.unlock();
    return *this;
}

/// Operator to add logging.
ClogInterface& ClogBase::operator<<(bool u)
{
    m_mutex.lock();

    m_composition << u;
    m_emptyLine = false;

    m_mutex.unlock();
    return *this;
}

/*------------- Local functions ----------------------------------------*/

/*==============================================================================
 **              ClogBase::write
 **============================================================================*/
///
/// Write to logfile. Not locked, only use internal.
///
/// @param      logText [in] Handle to output file.
///
/// @remarks    DEBUGGED
///
/*============================================================================*/
void ClogBase::writeTextInternal( const std::string &input)
{
    std::string output;

    if (m_logToDisplay)
	{
		puts( input.c_str());
	}

    std::string word;
    int index =0;
    for (;;)
    {
    	std::string::size_type loc =input.find( '\n', index);

    	if ( loc ==std::string::npos || loc<=0)
    	{
    		// Add remainder of string.
    		word.assign( input, index, input.length()-index);
    	    if (word.length() > 0)
    	    {
                m_composition << word;
                m_emptyLine = false;
    	    }
    		break;
    	}
    	word.assign( input, index, (int)loc-index);
        if (word.length() > 0)
        {
            m_composition << word;
            m_emptyLine = false;
            nextLineInternal();
        }
    	index =(int)loc+1;
    }
}

/*----------------------------------------------------------------------------*/
void ClogBase::nextLineInternal()
{
	if (!m_emptyLine)
	{
        if (m_logToDisplay)
    	{
    		puts( m_composition.str().c_str());
    	}
        if (m_file_p)
        {
        	std::string output = Ctimestamp().getSysTime() + m_composition.str() + "\n";
        	int length = (int)output.length();
        	checkNextFile(length);

        	if (m_file_p)
        	{
        		fwrite(output.c_str(), 1, length, m_file_p);
        		fflush(m_file_p);
        	}
        }
	    m_composition = std::stringstream();
	    m_emptyLine = true;
	}
}

/*----------------------------------------------------------------------------*/
void ClogBase::checkNextFile(int length)
{
	m_bytesInFile += length;
	if (m_bytesInFile >= m_maxFileSize)
	{
		m_bytesInFile = 0;
		if (m_file_p)
		{
		    std::string output = "End of file.\n";
			fwrite(output.c_str(), 1, (int)output.length(), m_file_p);
			fclose(m_file_p);
		}
		remove(m_szOldFileName.c_str());
		(void) rename(m_szFileName.c_str(), m_szOldFileName.c_str());
		(void) remove(m_szFileName.c_str());
	    m_file_p =fopen( m_szFileName.c_str(), "wt");
	}
}

/*========================================================================
 **      end of file
 **======================================================================*/
