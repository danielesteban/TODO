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
 *  \file lib.h
 *
 *  Header file for the lib functions & macros
 */

#ifndef _LIB_H_
#define _LIB_H_

/* Using CType, Standard I/O & Strings */
#include <ctype.h>
#include <string.h>

/**
 *  \brief The maximum buffer size
 *         (For file reading and user input)
 */
#define MAX_BUFFER_SIZE 511

/**
 *  \brief A really simple trimming function
 *
 *  \param str The string to be trimmed
 *
 *  \return A pointer to the trimmed string
 */
char *trim(char *str);

#endif /* _LIB_H_ */
