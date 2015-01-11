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

#include "render.h"

/* Initialize the variables */
static struct winsize windowSize;
static char renderHelp = 1;

void updateWindowSize() {
	/* Update the window size */
	ioctl(0, TIOCGWINSZ, &windowSize);
}

void toggleHelp() {
	/* Toggle help rendering */
	renderHelp = !renderHelp;
}

void render() {
	/* The entry pointer */
	const struct TODOEntry *entry = getTODOListFirst();

	/* The entry index */
	unsigned short int index = 1;

	/* The title & it's length */
	const char *title = getTODOListTitle();
	char titleLength = strlen(title);

	/* Iteration counter for the list title decorations */
	unsigned short int i;

	/* Clear the screen */
	CLEAR_SCREEN();

	/* Print the title */
	FGCOLOR(CYAN);
	for(i=0; i<windowSize.ws_col; i++) printf("=");
	printf("\n%*s\n", (windowSize.ws_col / 2) + (titleLength / 2), title);
	for(i=0; i<windowSize.ws_col; i++) printf("=");
	printf("\n\n");
	
	/* Iterate through the TODO list */
	while(entry != NULL) {
		if(entry->done) FGCOLOR(GREEN);
		else FGCOLOR(RED);
		printf("%3d: %s %s\n", index++, entry->done ? "✔" : "✘", entry->title);
		entry = entry->next;
	}

	/* Move the cursor to the bottom of the screen */
	printf("\033[%df\n", windowSize.ws_row - (renderHelp ? 7 : 2));
	
	if(renderHelp) {
		/* Print help */
		const char *commands[5][2] = {
			{"[n]", "Toggle entry [n]"},
			{"A [title]", "Add new entry"},
			{"D [n]", "Delete entry [n]"},
			{"H", "Toggle help"},
			{"Q", "Quit"}
		};

		for(i=0; i<5; i++) {
			FGCOLOR(YELLOW);
			printf("%*s%-13s", (windowSize.ws_col / 2) - 15, "", commands[i][0]);
			FGCOLOR(CYAN);
			printf("%s\n", commands[i][1]);
		}
	}

	FGCOLOR(WHITE);
	printf("\n> ");
}
