 /*
 * my_observer.cpp
 *
 *  Created on: 1 August 2022
 *      Author: mensfort
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2022
 ** Houkes Horeca Applications
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/

#include "taal_type.h"
#include "sdl_graphics.h"

/*----------------------------------------------------------------------------*/
void setLanguage(Taal language)
{
    Cgraphics::m_defaults.country = language;
}

/*----------------------------------------------------------------------------*/
Taal getLanguage()
{
    return Cgraphics::m_defaults.country;
}

/*----------------------------------------------------------------------------*/
/** Change to next language if LANG_NEXT else set to language
 * @param l [in|out] language result
 * @param language [in] New language
 */
void Language(Taal *l, Taal language)
{
    *l = (language == LANG_NEXT) ? ChangeNextLanguage( *l):language;
}

/*----------------------------------------------------------------------------*/
/** @brief Change to next allowed language
 *  @param language [in] Language to start
 *  @return next language enabled
 */
Taal ChangeNextLanguage( Taal language)
{
    Taal start =language;
    for ( int x = 0; x < (LANG_NEXT-1); x++)
    {
        language = (Taal) ((language + 1) % LANG_NEXT);
        if (   (language == LANG_INDO && Cgraphics::m_defaults.use_indonesian)
            || (language == LANG_SIMPLIFIED && Cgraphics::m_defaults.use_simplified)
            || (language == LANG_TRADITIONAL && Cgraphics::m_defaults.use_traditional)
            || (language == LANG_DUTCH && Cgraphics::m_defaults.use_dutch)
            || (language == LANG_ENGLISH  && Cgraphics::m_defaults.use_english)
            || (language == LANG_GERMAN && Cgraphics::m_defaults.use_german))
        {
            return language;
        }
    }
    return start;
}

/*----------------------------------------------------------------------------*/
/** @brief Check how many languages we support
 *  @param language [in] Language to start
 *  @return next language enabled
 */
int NumberLanguages()
{
    int retVal = 0;
    if (Cgraphics::m_defaults.use_indonesian)
    {
    	retVal++;
    }
    if (Cgraphics::m_defaults.use_simplified)
    {
    	retVal++;
    }
    if (Cgraphics::m_defaults.use_traditional)
    {
    	retVal++;
    }
    if (Cgraphics::m_defaults.use_dutch)
    {
    	retVal++;
    }
    if (Cgraphics::m_defaults.use_english)
    {
    	retVal++;
    }
    if (Cgraphics::m_defaults.use_german)
    {
    	retVal++;
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/** @brief Change to this european language
 *  @param language [in] Language
 *  @post European language copied in #EuroLang
 */
void ChangeLanguage(Taal language)
/* Change to next language if LANG_NEXT else set to language                */
{
    if ( language ==LANG_NEXT)
    {
        setLanguage( ChangeNextLanguage( getLanguage()));
    }
    else
    {
        setLanguage( language);
    }
    if (getLanguage() != LANG_SIMPLIFIED && getLanguage() !=LANG_TRADITIONAL)
    {
        EuroLang =getLanguage();
    }
}

bool isChinese()
{
    return (getLanguage()==LANG_TRADITIONAL || getLanguage()==LANG_SIMPLIFIED);
}
