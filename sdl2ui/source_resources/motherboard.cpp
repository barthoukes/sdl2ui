/*============================================================================*/
/**  @file      motherboard.cpp
 **  @ingroup   resources
 **  @brief     Retrieve data from the motherboard.
 **
 **  Conversion of motherboard parameters.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **             Cmotherboard
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 15 dec 1994
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/
#include <stdio.h>
#include <sstream>
#include <string.h>
#include "disk.hpp"
#include "motherboard.hpp"

/*----------------------------------------------------------------------------*/
Cmotherboard::Cmotherboard()
{
}

Cmotherboard::~Cmotherboard()
{
}

std::string Cmotherboard::getMac()
{
    std::string retVal;

    // /sys/class/net/lo/address
    fileList files;
    Cdisk::getDirectories("/sys/class/net", files);
    for (std::string n : files)
    {
        if (n[0] == 'e')
        {
            retVal = "/sys/class/net/" + n + "/address";
            retVal = Cdisk::getFileData(retVal);
            break;
        }
    }
    return retVal;
}

std::string Cmotherboard::getModelName()
{
    std::string retVal;
    std::string cpuInfo = Cdisk::getFileData("/proc/cpuinfo");
    size_t idx = cpuInfo.find("model name");
    if (idx>0)
    {
        idx =cpuInfo.find(":", idx);
        idx++;
        idx++;
        const char *t = cpuInfo.c_str()+idx;
        while (*t>=' ')
        {
            retVal +=*t++;
        }
    }
    return retVal;

}
