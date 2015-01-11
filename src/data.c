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

#include "data.h"

/* Initialize the variables */
static char *TODOListFilename = NULL;
static char *TODOListTitle = NULL;
static struct TODOEntry *TODOListFirst = NULL;
static struct TODOEntry *TODOListLast = NULL;

void loadTODOList(const char *filename, const char *title) {
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

void deleteEntry(const unsigned short int entryIndex) {
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

void toggleEntry(const unsigned short int entryIndex) {
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

const char *getTODOListTitle() {
	return TODOListTitle;
}

const struct TODOEntry *getTODOListFirst() {
	return TODOListFirst;
}
