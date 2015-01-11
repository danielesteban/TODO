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
 *  \file main.c
 *
 *  Main file for the C90 TODO List
 */

/* Using Signals, Standard I/O & Strings */
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "data.h"
#include "lib.h"
#include "render.h"

/**
 *  \brief Signal handler for SIGWINCH
 *  \param sig The signal ID
 */
void sigwinchHandler(int sig) {
	/* Update the window size */
	updateWindowSize();

	/* Re-Render the GUI */
	if(sig) render();
}

/**
 *  \brief Signal handler for SIGINT
 *  \param sig The signal ID
 */
void sigintHandler(int sig) {
	/* Free the memory */
	freeTodoList();

	/* Reset the colors */
	printf("\033[0m");

	/* Reset the window title */
	printf("\033]0;\007");

	/* Clear the screen */
	CLEAR_SCREEN();

	if(sig) exit(0);
}

/**
 *  \brief The app entry point
 *
 *  \param argc The CLI arguments count
 *  \param argv The CLI arguments values
 *
 *  \return The application exit status code
 */
int main(int argc, char **argv) {
	/* The quit flag */
	char quit = 0;

	/* If we didn't get the expected parameters... */
	if(argc < 2 || argc > 3) {
		/* Print the usage and exit */
		printf("usage:\n%s filename [title]\n", argv[0]);
		return 1;
	}

	/* Load the TODO list */
	loadTODOList(argv[1], argc > 2 ? argv[2] : NULL);

	/* Set the window title */
	printf("\033]0;%s\007", getTODOListTitle());

	/* Set the default background color */
	BGCOLOR(BLACK);

	/* Get the initial window size */
	sigwinchHandler(0);

	/* Set the SIGWINCH handler */
	signal(SIGWINCH, sigwinchHandler);

	/* Set the SIGINT handler */
	signal(SIGINT, sigintHandler);

	/* Main loop */
	while(!quit) {
		/* The user input buffer */
		char userInput[MAX_BUFFER_SIZE];
		
		/* Render the GUI */
		render();

		/* Get the user input */
		fgets(userInput, sizeof(userInput), stdin);
		
		/* Remove the trailing '\n' character */
		userInput[strlen(userInput) - 1] = '\0';

		/* Process the user input */
		switch(userInput[0]) {
			case 'Q':
			case 'q':
				quit = 1;
			break;
			case 'A':
			case 'a':
				addNewEntry(userInput);
			break;
			case 'D':
			case 'd':
				userInput[0] = '0';
				deleteEntry(atoi(userInput));
			break;
			case 'H':
			case 'h':
			case '?':
				toggleHelp();
			break;
			default:
				toggleEntry(atoi(userInput));
			break;
		}
	}

	/* Reset the screen & Free the memory */
	sigintHandler(0);

	return 0;
}
