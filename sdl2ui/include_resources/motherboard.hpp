/*============================================================================*/
/**  @file		motherboard.hpp
 *   @ingroup   zhongcan
 *   @brief     Motherboard extract data.
 *
 *   Classes to deal with motherboard.
 *
 *   @author    Bart Houkes
 *============================================================================*/

#pragma once

class Cmotherboard
{
public:
    Cmotherboard();
    virtual ~Cmotherboard();

    std::string getMac();
    std::string getModelName();
};
