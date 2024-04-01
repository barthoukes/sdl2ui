/*============================================================================*/
/** @file       font_and_size_type.hpp
**  @ingroup    include_files
**  @brief      Type for fonts and size.
**
**
**  @author Bart Houkes
*/
/*------------------------------------------------------------------------------
**  Copyright (c) Bart Houkes, 2011
**
**  Copyright notice:
**  This software is property of Bart Houkes
**  Unauthorized duplication and disclosure to third parties is forbidden.
**============================================================================*/

#pragma once

#include <memory>
#include <string>

typedef struct
{
    std::string fontName; ///< Font name
    int         fontSize; ///< Number of pixels
} fontAndSize;

typedef struct
{
    fontAndSize    local; ///< Normal A-Z
    fontAndSize  chinese; ///< Chinese font and size
} fontsAndSizes;
