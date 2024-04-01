/*============================================================================*/
/**  @file      version_info.hpp
 **  @ingroup   source_zhongcan
 **  @brief		Version.
 **
 **  Version numbering. Still manual.
 **
 **  @author     mensfort
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 22 jan 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

/// Major version (SAP number).							
#define VERSION_MAJOR                      14
/// Minor version (weekly build number).						
#define VERSION_MINOR                      1
/// Increasing Build ID (number of times the PC software is built).		
#define VERSION_BUILD                      1
/// Date before build.
#define VERSION_DATE                       "2024-1-1"
/// Time before build.								
#define VERSION_TIME                       "16:35:01.08"

extern const char *version();
