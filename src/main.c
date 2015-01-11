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

/* Using CType, Signals, Standard I/O & Strings */
#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "data.h"
#include "lib.h"
#include "render.h"

/**
 *  \brief At exit handler
 */
void atExit() {
	/* Free the memory */
	freeTodoList();

	/* Reset the window title */
	printf("\033]0;\007");

	/* Reset the colors */
	printf("\033[0m");

	/* Clear the screen */
	CLEAR_SCREEN();
}

/**
 *  \brief Signals handler
 *  \param sig The signal ID
 */
void signalsHandler(int sig) {
	switch(sig) {
		case SIGWINCH:
			/* Update the window size */
			updateWindowSize();

			/* Re-Render the GUI */
			render();
		break;
		case SIGINT:
			/* Gracefully exit the program */
			exit(0);
		break;
	}
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
	/* The sigaction for signal handling */
	struct sigaction sa;

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

	/* Set the initial window size */
	updateWindowSize();

	/* Set the window title */
	printf("\033]0;%s\007", getTODOListTitle());

	/* Set the default background color */
	BGCOLOR(BLACK);

	/* Set the signals handler */
	sa.sa_handler = signalsHandler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);

	/* Set the SIGWINCH handler */
	if(sigaction(SIGWINCH, &sa, NULL) == -1) {
		printf("ERROR setting the SIGWINCH handler\n");
		exit(1);
	}

	/* Set the SIGINT handler */
	if(sigaction(SIGINT, &sa, NULL) == -1) {
		printf("ERROR setting the SIGINT handler\n");
		exit(1);
	}

	/* Set the atExit handler */
	atexit(atExit);

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
		switch(toupper(userInput[0])) {
			case 'Q':
				quit = 1;
			break;
			case 'A':
				addNewEntry(userInput);
			break;
			case 'D':
				userInput[0] = '0';
				deleteEntry(atoi(userInput));
			break;
			case 'H':
				toggleHelp();
			break;
			default:
				toggleEntry(atoi(userInput));
			break;
		}
	}

	return 0;
}
