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

#include "log_base.h"
#include "timestamp.h"
#include "version_info.h"

/*------------- Local symbolic constants -------------------------------------*/

/** Default value of maximum size of new text to add since opening of log file.
 *   When this limit is reached, a backup of the log file is created and
 *   a new one started. This normally restricts the log file directory size
 *   to 10MB (2x5MB) at release version. The limit can also be configured
 *   in the configuration file by the entry "LogfileSize".
 */
#define DEFAULT_MAX_LOGFILE_SIZE 10000000

/// HEX value of a digit.
/// @param n [in] Digit to change in ascii.
#define hexDigit(n) \
	( ((n)>9) ? 'A'-10+(n):'0'+(n))

/// HEX value of 2 nibbles.
/// @param ptr [in] Where to put the number.
/// @param x [in] Number to convert.
#define hexByte(ptr,x) \
{ ptr[0]=hexDigit((unsigned char)((x)>>4)); ptr[1]=hexDigit((x)&0x0f); }

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
	void *addresses[10];
	char **strings;
	(void)sig;

	int size =backtrace(addresses, 10);
	strings = backtrace_symbols(addresses, size);
	Log.error("Stack frames: %d", size);
	for(int i = 0; i < size; i++)
	{
		Log.error("%d: %X", i, (long)addresses[i]);
		Log.error("%s", strings[i]);
	}
	free(strings);
	Log.KillInstance();
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
	m_comments(false),
	m_warnings(false),
	m_time(true)
{
	pthread_mutex_init( &m_criticalSection, NULL);
	m_szFileName[0] = 0;
	m_szOldFileName[0] = 0;
	signal(SIGSEGV, handler);
	signal(SIGFPE, handler);
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
/// @param      comments [in] Comments wanted.
/// @param      append [in] Write new file or open old one.
/// @param      time [in] Add time to each line.
/// @param      warnings [in] Only do the warnings if true.
///
/// @post       File opened in correct path.
///               
/// @return     TRUE on success.
///                 
/// @remarks    DEBUGGED
///               
/*============================================================================*/
bool ClogBase::open(const char *fileName, bool comments, bool append,
		bool time, bool warnings)
{
	strcpy(m_szFileName, fileName);
	strcpy(m_szOldFileName, fileName);
	strcat(m_szOldFileName, "_old");

	close();
	strncpy(m_szFileName, fileName, MAX_PATH);
	m_szFileName[MAX_PATH] = 0;

	if (m_file_p != NULL)
	{
		return (true);
	}
	m_comments =comments;
	m_warnings =warnings;
	m_time = time;
	m_file_p =fopen( m_szFileName, append ? "at" : "wt");
	if (m_file_p == NULL)
	{
		return (false);
	}

	fseek(m_file_p, -150, SEEK_END);
	m_bytesInFile = ftell(m_file_p);

	writeHeader();
	if (m_comments)
	{
		write("==---------------------------------------------------==");
		write("ClogBase::open  file opened for logging ptr=%d",
				(unsigned long) (m_file_p));
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
	write( "*************************************************************" );
	write( "Start ALIVE version: %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
	write( "%s %s", VERSION_DATE, VERSION_TIME );
	write( "*************************************************************" );

	// Please overload this function and add your own header.
	return true;
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
	if (m_file_p != NULL)
	{
		if (m_comments)
		{
			write("ClogBase::close  ptr=%d\n", (unsigned long) m_file_p);
		}
		(void) fclose(m_file_p);
		m_file_p = NULL;
	}
	return;
}

/*==============================================================================
 **              ClogBase::write
 **============================================================================*/
///
/// Write to logfile.
///
/// @param      logText [in] Handle to output file.
///
/// @remarks    DEBUGGED
///                                 
/*============================================================================*/
void ClogBase::writeText( const char *logText)
{
	Ctimestamp sysTime;
	char line[MAX_LOG_SIZE];
	int len = (int) strlen(logText);
	bool noCr = false;
	char noCrStr[] = "[-CR]";
	int noCrStrLen = (int) strlen(noCrStr);
	bool noTime = false;
	char noTimeStr[] = "[-TIME]";
	int noTimeStrLen = (int) strlen(noTimeStr);
	static bool lastNoCr = false;

	if (len >= noCrStrLen && !memcmp(logText + len - noCrStrLen, noCrStr,
			noCrStrLen))
	{
		noCr = true;
		len -=noCrStrLen;
	}

	if (len >= noTimeStrLen && !memcmp(logText + len - noTimeStrLen,
			noTimeStr, noTimeStrLen))
	{
		noTime = true;
		len -=noTimeStrLen;
	}

	char f =logText[len];
	((char*)logText)[len]=0;
	if (m_comments)
	{
		puts( logText);
	}

	if (m_file_p == NULL)
	{
		((char*)logText)[len]=f;
		return;
	}

	if (lastNoCr || !m_time || noTime)
	{
		strncpy(line, logText, MAX_LOG_SIZE-1); line[MAX_LOG_SIZE-1] =0;
	}
	else
	{
		(void) sprintf(line, "%04d/%02d/%02d %02d:%02d:%02d.%03d: ",
				sysTime.getYear(), sysTime.getMonth(), sysTime.getDay(), sysTime.getHours(),
				sysTime.getMinutes(), sysTime.getSeconds(), sysTime.getMilliseconds());
		strncat( line, logText, 4000);
	}
	if (noCr)
	{
		line[strlen(line) - noCrStrLen] = 0;
	}
	if (noTime)
	{
		line[strlen(line) - noTimeStrLen] = 0;
	}

	if (!noCr)
	{
		strcat(line, "\n");
	}
	fwrite(line, 1, strlen(line), m_file_p);
	fflush(m_file_p);

	m_bytesInFile += (int) strlen(line);
	if (m_bytesInFile >= m_maxFileSize && noCr == false)
	{
		m_bytesInFile = 0;
		if (m_file_p)
		{
			(void) sprintf(line, "End of file.");
			fwrite(line, 1, strlen(line), m_file_p);
			fclose(m_file_p);
		}
		remove(m_szOldFileName);
		(void) rename(m_szFileName, m_szOldFileName);
		(void) remove(m_szFileName);
	    m_file_p =fopen( m_szFileName, "wt");
		if (m_file_p == NULL)
		{
			((char*)logText)[len]=f;
			return;
		}
	}
	lastNoCr = noCr;
	((char*)logText)[len]=f;
	return;
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
void ClogBase::write1(const char *logText)
{
	if (m_disableLogFile || m_warnings==false || m_file_p == NULL)
	{
		return;
	}
	pthread_mutex_lock( &m_criticalSection );

	strncpy(m_buffer,  logText, sizeof(m_buffer) - 1);

	writeText(m_buffer);
	pthread_mutex_unlock( &m_criticalSection );
}

#if 0
/*==============================================================================
 **              ClogBase::getTime
 **============================================================================*/
///
///  Get time to a string like a log-file.
///
///  @param      txt [in] Handle to output text.
///
///  @post       formatted text containing the time.
///
///  @remarks    DEBUGGED
///                                 
/*============================================================================*/
void ClogBase::getTime(char *txt)
{
	Ctimestamp sysTime;

	(void) sprintf(txt, "%04d/%02d/%02d %02d:%02d:%02d.%03d: %s",
			sysTime.getYear(), sysTime.getMonth(), sysTime.getDay(), sysTime.getHours(),
			sysTime.getMinutes(), sysTime.getSeconds(), sysTime.getMilliseconds(), txt);
}
#endif

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
void ClogBase::write(const char *logText, ...)
{
	if (m_disableLogFile || m_warnings==false || m_file_p == NULL)
	{
		return;
	}
	pthread_mutex_lock( &m_criticalSection );

	va_list args;
	va_start(args, logText);

	vsnprintf(m_buffer, sizeof(m_buffer) - 1, logText, args);

	writeText(m_buffer);
	pthread_mutex_unlock( &m_criticalSection );
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
	if (m_disableLogFile || m_warnings ==false || m_file_p == NULL)
	{
		return;
	}
	pthread_mutex_lock( &m_criticalSection );

	va_list args;
	va_start(args, length);

	vsnprintf(m_buffer, sizeof(m_buffer) - 1, logText, args);

	length = myMin(length, 512);
	char *ptr = m_buffer + strlen(m_buffer);
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
	writeText(m_buffer);
	pthread_mutex_unlock( &m_criticalSection );
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
	if (m_disableLogFile || m_file_p == NULL)
	{
		return;
	}
	if (statement)
	{
		pthread_mutex_lock( &m_criticalSection );

		va_list args;
		va_start(args, logText);

		vsnprintf(m_buffer, sizeof(m_buffer) - 1, logText, args);

		writeText(m_buffer);
		pthread_mutex_unlock( &m_criticalSection );
	}
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
	if (m_disableLogFile || m_file_p == NULL)
	{
		assert(NULL);
	}
	if (statement)
	{
		pthread_mutex_lock( &m_criticalSection );

		va_list args;
		va_start(args, logText);

		sprintf(m_buffer, "ERROR:  ");
		vsnprintf(m_buffer + 8, sizeof(m_buffer) - 1, logText, args);

		writeText(m_buffer);
		if (m_showErrors)
		{
			char errors[MAX_LOG_SIZE+16];
			sprintf( errors, "ERROR:  %s", logText);
			//Box( MessageBox(NULL, errors, "WTC", MB_OK);
		}
		pthread_mutex_unlock( &m_criticalSection );
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
	pthread_mutex_lock( &m_criticalSection );

	va_list args;
	va_start(args, logText);

	sprintf(m_buffer, "ERROR:  ");
	try
	{
		vsnprintf(m_buffer + 8, sizeof(m_buffer) - 1, logText, args);
	}
	catch(...)
	{
		sprintf( m_buffer+8, "Logging statement!!");
	}

	if (!m_disableLogFile && m_file_p != NULL)
	{
		writeText(m_buffer);
	}
	if (m_showErrors)
	{
		std::string errors;
		errors = m_buffer;
		//MessageBox(NULL, errors, _T("WTC"), MB_OK);
	}
	pthread_mutex_unlock( &m_criticalSection );
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
	if ( m_warnings ==false)
	{
		return;
	}
	pthread_mutex_lock( &m_criticalSection );

	va_list args;
	va_start(args, logText);

	sprintf(m_buffer, "WARNING:  ");
	vsnprintf(m_buffer + 8, sizeof(m_buffer) - 9, logText, args);

	if (!m_disableLogFile && m_file_p != NULL)
	{
		writeText(m_buffer);
	}
	pthread_mutex_unlock( &m_criticalSection );
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
		pthread_mutex_lock( &m_criticalSection );

		va_list args;
		va_start(args, logText);

		sprintf(m_buffer, "ERROR:  ");
		vsnprintf(m_buffer + 8, sizeof(m_buffer) - 9,
				logText, args);

		if (!m_disableLogFile && m_file_p != NULL)
		{
			writeText(m_buffer);
		}
		if (m_showErrors)
		{
			char errors[MAX_LOG_SIZE+16];
			sprintf( errors, "ERROR:  %s", logText);
			//MessageBox(NULL, errors, "WTC", MB_OK);
		}
		pthread_mutex_unlock( &m_criticalSection );
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
	pthread_mutex_lock( &m_criticalSection );

	va_list args;
	va_start(args, logText);

	sprintf(m_buffer, "ERROR:  ");
	vsnprintf(m_buffer + 8, sizeof(m_buffer) - 9, logText, args);

	if (!m_disableLogFile && m_file_p != NULL)
	{
		writeText(m_buffer);
	}
	if (m_showErrors)
	{
		char errors[MAX_LOG_SIZE+16];
		sprintf( errors, "ERROR:  %s", logText);
		//MessageBox(NULL, errors, "WTC", MB_OK);
	}
	pthread_mutex_unlock( &m_criticalSection );
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


bool ClogBase::getPathName(const char *fileName)
{
	bool retVal = true;
	char file1[MAX_LOG_SIZE], file2[MAX_LOG_SIZE], dirName[MAX_LOG_SIZE];
	strncpy(file1, fileName, MAX_LOG_SIZE-1);
	file2[0] = '_';
	strncpy(file2 + 1, fileName, MAX_LOG_SIZE-1);

	//getFileName( file1, file2);
	getDirName(dirName);
	// get common documents directory
	// (mostly C:\Documents and Settings\All Users\Documents)
	// try to create the LOGFILE directory for cases it doesn't exist
	strcpy( m_szFileName, LOG_FILE1 );
	strcpy( m_szOldFileName, LOG_FILE2 );
	return retVal;
}

/*------------- Local functions ----------------------------------------*/

/*========================================================================
 **      end of file
 **======================================================================*/
