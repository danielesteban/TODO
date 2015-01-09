/**
 *  C90 TODO List
 *  Copyright (C) 2015 Daniel Esteban Nombela <dani@dabuten.co>
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the author be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1.  The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software
 *      in a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *
 *  2.  Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *
 *  3.  This notice may not be removed or altered from any source distribution.
 */

/**
 *  \file main.c
 *
 *  Main file for the C90 TODO List
 */

/* Using CType, Standard I/O, Standard lib, Strings & IOCTL */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

/**
 *  \brief  The maximum buffer size
 *          (For file reading and user input)
 */
#define MAX_BUFFER_SIZE 511

/**
*   \brief  Clears the screen
*/
#define clearScreen() printf("\033[2J\033[3J\033[0f")

/**
 *  \brief The TODO entry struct
 */
struct TODOEntry {
	char *title;            /**< The entry title */
	char done;              /**< The entry status */
	struct TODOEntry *next; /**< A pointer to the next entry on the list */
};

/* The TODO list filename */
static char *TODOListFilename = NULL;

/* The TODO list title */
static char *TODOListTitle = NULL;

/* The TODO list pointers */
static struct TODOEntry *TODOListFirst = NULL;
static struct TODOEntry *TODOListLast = NULL;

/* The current window size */
static struct winsize windowSize;

/**
 *  \brief A really simple trimming function
 *
 *  \param str The string to be trimmed
 *
 *  \return A pointer to the trimmed string
 */
char *trim(char *str) {
	char *end;

	/* Trim leading space */
	while(isspace(*str)) str++;

	/* All spaces? */
	if(*str == 0) return str;

	/* Trim trailing space */
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) end--;

	/* Write new null terminator */
	*(end + 1) = 0;

	return str;
}

/**
 *  \brief Adds an entry
 *
 *  \param title The entry title
 *  \param done Whether the entry is done or not
 */
void addEntry(char *title, char done) {
	/* Allocate the memory */
	struct TODOEntry *entry = (struct TODOEntry*) malloc(sizeof(struct TODOEntry));
	
	/* Assign the entry data */
	entry->title = title;
	entry->done = done;
	entry->next = NULL;
	
	/* Push it to the list */
	if(TODOListFirst == NULL) TODOListFirst = entry;
	else TODOListLast->next = entry;
	TODOListLast = entry;
}

/**
 *  \brief Loads the TODO list from the hard disk
 *
 *  \param filename The TODO list filename
 *  \param title The TODO list title
 */
void loadTODOList(char *filename, char *title) {
	/* The reading buffers */
	char readChar;
	char readBuf[MAX_BUFFER_SIZE];
	unsigned short int readIndex = 0;
	char *trimmedBuf;
	char entryDone = -1;
	char *entryTitle;

	/* The file pointer */
	FILE *file;

	/* The lines counter */
	unsigned short int line = 0;

	/* Add extension to file name (if missing) */
	char *extension = strchr(filename, '.');
	TODOListFilename = (char *) malloc((sizeof(char) * strlen(filename)) + (extension == NULL ? 4 : 0) + 1);
	strcpy(TODOListFilename, filename);
	if(extension == NULL) strcpy(TODOListFilename + strlen(TODOListFilename), ".txt\0");

	/* Open the file */
	if((file = fopen(TODOListFilename, "r")) == NULL) {
		/* If we got no title, set it to the filename */
		if(title == NULL) {
			extension = strchr(TODOListFilename, '.');
			TODOListTitle = (char *) malloc((sizeof(char) * (extension - TODOListFilename)) + 1);
			strncpy(TODOListTitle, TODOListFilename, extension - TODOListFilename);
			TODOListTitle[0] = toupper(TODOListTitle[0]);
			TODOListTitle[extension - TODOListFilename] = '\0';
		} else {
			TODOListTitle = (char *) malloc((sizeof(char) * strlen(title)) + 1);
			strcpy(TODOListTitle, title);
		}
	} else {
		/* Read the file */
		readChar = fgetc(file);
		while(readChar != EOF) {
			if(line == 1) {
				/* Parse & trim the title line */
				if(readIndex < MAX_BUFFER_SIZE && readChar != '\n') readBuf[readIndex++] = readChar;
				else {
					readBuf[readIndex] = '\0';
					trimmedBuf = trim(readBuf);
					strcpy(TODOListTitle = (char *) malloc(sizeof(char) * (strlen(trimmedBuf) + 1)), trimmedBuf);
					readIndex = 0;
					line++;
				}
			} else if(line < 3) {
				/* Skip the title decoration lines */
				if(readChar == '\n') line++;
			} else if(entryDone == -1) {
				/* Parse the entry done flag */
				if(readIndex < MAX_BUFFER_SIZE && readChar != ' ') readBuf[readIndex++] = readChar;
				else {
					readBuf[readIndex] = '\0';
					entryDone = strcmp("✔", readBuf) == 0;
					readIndex = 0;
				}
			} else {
				/* Parse the entry title */
				if(readIndex < MAX_BUFFER_SIZE && readChar != '\n') readBuf[readIndex++] = readChar;
				else {
					readBuf[readIndex] = '\0';
					trimmedBuf = trim(readBuf);
					strcpy(entryTitle = (char *) malloc(sizeof(char) * (strlen(trimmedBuf) + 1)), trimmedBuf);
					addEntry(entryTitle, entryDone);
					readIndex = 0;
					entryDone = -1;
				}
			}
			readChar = fgetc(file);
		}

		/* Close the file */
		fclose(file);
	}
}

/**
 *  \brief Saves the TODO list into the hard disk
 */
void saveTodoList() {
	/* The file pointer */
	FILE *file;
	
	/* The entry pointer */
	struct TODOEntry *entry = TODOListFirst;

	/* Title decorations length */
	const unsigned short int decorationsLength = 40;

	/* Iteration counter for the list title decorations */
	unsigned short int i;

	/* The title length */
	char titleLength = strlen(TODOListTitle);

	if(entry == NULL) return;

	/* Open the file */
	if((file = fopen(TODOListFilename, "w")) == NULL) {
		printf("ERROR writing to file: %s\n", TODOListFilename);
	} else {
		/* Print the title */
		for(i = 0; i<decorationsLength; i++) fputs("=", file);
		fprintf(file, "\n%*s\n", (decorationsLength / 2) + titleLength / 2, TODOListTitle);
		for(i = 0; i<decorationsLength; i++) fputs("=", file);
		fputs("\n", file);

		/* Iterate through the TODO list */
		while(entry != NULL) {
			fprintf(file, "%s %s\n", entry->done ? "✔" : "✘", entry->title);
			entry = entry->next;
		}

		/* Close the file */
		fclose(file);
	}
}

/**
 *  \brief Frees the TODO list memory
 */
void freeTodoList() {
	/* The entry pointers */
	struct TODOEntry *next = TODOListFirst;
	struct TODOEntry *entry;

	/* Iterate through the TODO list */
	while(next != NULL) {
		entry = next;
		next = entry->next;
		free(entry->title);
		free(entry);
	}

	TODOListFirst = TODOListLast = NULL;
	
	/* Free the filename & title */
	free(TODOListFilename);
	TODOListFilename = NULL;
	free(TODOListTitle);
	TODOListTitle = NULL;
}

/**
 *  \brief Adds a new entry parsing the user input
 *
 *  \param userInput The full user input (A[title])
 */
void addNewEntry(char *userInput) {
	/* Parse & trim entry title */
	char *trimmedInput = trim(userInput + 1);
	char *title = (char *) malloc((sizeof(char) * strlen(trimmedInput)) + 1);
	strcpy(title, trimmedInput);

	/* Add the entry */
	addEntry(title, 0);

	/* Save the TODO list */
	saveTodoList();
}

/**
 *  \brief Deletes an entry
 *
 *  \param entryIndex The index of the entry to be deleted
 */
void deleteEntry(unsigned short int entryIndex) {
	/* The entry pointers */
	struct TODOEntry *entry = TODOListFirst;
	struct TODOEntry *prev = NULL;

	/* The loop entry index */
	unsigned short int index = 1;

	/* Iterate through the TODO list */
	while(entry != NULL) {
		if(index++ == entryIndex) {
			/* Unlink the entry */
			if(prev == NULL) {
				if((TODOListFirst = entry->next) == NULL) TODOListLast = NULL;
			} else {
				if((prev->next = entry->next) == NULL) TODOListLast = prev;
			}

			/* Free the memory */
			free(entry->title);
			free(entry);
			break;
		}
		prev = entry;
		entry = entry->next;
	}

	/* Save the TODO list */
	saveTodoList();
}

/**
 *  \brief Toggles an entry
 *
 *  \param entryIndex The index of the entry to be toggled
 */
void toggleEntry(unsigned short int entryIndex) {
	/* The entry pointer */
	struct TODOEntry *entry = TODOListFirst;

	/* The loop entry index */
	unsigned short int index = 1;

	/* Iterate through the TODO list */
	while(entry != NULL) {
		if(index++ == entryIndex) {
			/* Toggle the entry status */
			entry->done = !entry->done;
			break;
		}
		entry = entry->next;
	}

	/* Save the TODO list */
	saveTodoList();
}

/**
 *  \brief Renders the GUI
 */
void render() {
	/* The entry pointer */
	struct TODOEntry *entry = TODOListFirst;

	/* The entry index */
	unsigned short int index = 1;

	/* The title length */
	char titleLength = strlen(TODOListTitle);

	/* Iteration counter for the list title decorations */
	unsigned short int i;

	/* Clear the screen */
	clearScreen();

	/* Print the title */
	for(i = 0; i<windowSize.ws_col; i++) printf("=");
	printf("\n%*s\n", (windowSize.ws_col / 2) + titleLength / 2, TODOListTitle);
	for(i = 0; i<windowSize.ws_col; i++) printf("=");
	printf("\n");
	
	/* Iterate through the TODO list */
	while(entry != NULL) {
		printf("%2d: %s %s\n", index++, entry->done ? "✔" : "✘", entry->title);
		entry = entry->next;
	}

	/* Move the cursor to the bottom of the screen */
	printf("\033[%df", windowSize.ws_row - 4);
	
	/* Print the menu */
	printf("[n] => Toggle entry [n]\nA[title] => Add new entry\nD[n] => Delete entry [n]\nQ => Quit\n");
}

/**
 *  \brief Signal handler for SIGWINCH
 *  \param sig The signal ID
 */
void sigwinchHandler(int sig) {
	/* Update the current window size */
	ioctl(0, TIOCGWINSZ, &windowSize);

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

	/* Clear the screen */
	clearScreen();

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
			default:
				toggleEntry(atoi(userInput));
			break;
		}
	}

	/* Free the memory */
	sigintHandler(0);

	return 0;
}
