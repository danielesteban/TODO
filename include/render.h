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
 *  \file render.h
 *
 *  Header file for the rendering functions, macros & variables
 */

#ifndef _RENDER_H_
#define _RENDER_H_

/* Using Standard I/O, Strings & IOCTL */
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "data.h"

/**
*   \brief  Clears the screen
*/
#define CLEAR_SCREEN() printf("\033]50;ClearScrollback\a\033[2J\033[3J\033[0f")

/**
*   \brief  Sets the screen background color
*/
#define BGCOLOR(color) printf("\033[%d;1m", 40 + color)

/**
*   \brief  Sets the screen foreground color
*/
#define FGCOLOR(color) printf("\033[%d;1m", 30 + color)

/**
*   \brief  ANSI Color constants
*/
enum ANSI_COLORS {
	BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE
};

/** The current window size **/
static struct winsize windowSize;

/** The help rendering flag **/
static char renderHelp;

/**
 *  \brief Updates the window size
 */
void updateWindowSize();

/**
 *  \brief Toggles help rendering
 */
void toggleHelp();

/**
 *  \brief Renders the GUI
 */
void render();

#endif /* _RENDER_H_ */
