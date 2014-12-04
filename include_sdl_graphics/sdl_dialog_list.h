/*============================================================================*/
/**  @file       sdl_dialog_list.h
 **  @ingroup    zhongcan_sdl
 **  @brief		 Keep any list of dialogs.
 **
 **  We have 2 standard lists, one for message boxes, another for our world.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CdialogList
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 17 nov 2013
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

#ifndef SDL_DIALOG_LIST_H_
#define SDL_DIALOG_LIST_H_

#include <string>
#include <vector>
#include "my_thread.h"

/** @brief Basic interface for dialog. */
class CdialogBase
{
public:
	/** @brief Constructor. */
	CdialogBase() {}
	/** @brief Destructor. */
	virtual ~CdialogBase() {}
	virtual std::string getName()=0;
};

/** @brief List of dialogs available.
 */
class CdialogList: public CmyLock
{
public:
	CdialogList();
	virtual ~CdialogList();

public:
	void addDialog( CdialogBase		*interface);
	void removeDialog( CdialogBase	*interface);
	CdialogBase *firstDialog();
	CdialogBase *lastDialog();
	CdialogBase *nextDialog( CdialogBase *current);
	CdialogBase *previousDialog( CdialogBase *current);
	void clear();

private:
	std::vector<CdialogBase*> m_dialogs;///< All dialogs available.
	CdialogBase *m_interface; ///< Interface dialog.
};

/// Main system
extern CdialogList g_myWorld;
/// Message boxes.
extern CdialogList g_messageBox;

#endif /* SDL_DIALOG_LIST_H_ */
