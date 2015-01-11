/*
	C90 TODO List
	Copyright (C) 2015 Daniel Esteban Nombela <dani@dabuten.co>

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the author be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.

	3. This notice may not be removed or altered from any source distribution.
*/

/**
 *  \file data.h
 *
 *  Header file for the data manipulation functions & variables
 */

#ifndef _DATA_H_
#define _DATA_H_

/* Using CType, Standard lib, Standard I/O & Strings */
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib.h"

/**
 *  \brief The TODO entry struct
 */
struct TODOEntry {
	char *title;            /**< The entry title */
	char done;              /**< The entry status */
	struct TODOEntry *next; /**< A pointer to the next entry on the list */
};

/** The TODO list filename **/
static char *TODOListFilename;

/** The TODO list title **/
static char *TODOListTitle;

/** Pointer to the first entry in the TODO list **/
static struct TODOEntry *TODOListFirst;

/** Pointer to the last entry in the TODO list **/
static struct TODOEntry *TODOListLast;

/**
 *  \brief Loads the TODO list from the hard disk
 *
 *  \param filename The TODO list filename
 *  \param title The TODO list title
 */
void loadTODOList(const char *filename, const char *title);

/**
 *  \brief Saves the TODO list into the hard disk
 */
void saveTodoList();

/**
 *  \brief Frees the TODO list memory
 */
void freeTodoList();

/**
 *  \brief Adds a new entry parsing the user input
 *
 *  \param userInput The full user input (A[title])
 */
void addNewEntry(char *userInput);

/**
 *  \brief Adds an entry
 *
 *  \param title The entry title
 *  \param done Whether the entry is done or not
 */
void addEntry(char *title, char done);

/**
 *  \brief Deletes an entry
 *
 *  \param entryIndex The index of the entry to be deleted
 */
void deleteEntry(const unsigned short int entryIndex);

/**
 *  \brief Toggles an entry
 *
 *  \param entryIndex The index of the entry to be toggled
 */
void toggleEntry(const unsigned short int entryIndex);

/**
 *  \brief Returns the TODO list title
 *
 *  \return A pointer to the TODO list title
 */
const char *getTODOListTitle();

/**
 *  \brief Returns the first entry in the TODO
 *
 *  \return A pointer to the first entry in the TODO list
 */
const struct TODOEntry *getTODOListFirst();

#endif /* _DATA_H_ */
