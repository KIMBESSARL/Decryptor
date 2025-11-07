/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: Kingsly Mbekug / Jatin Bali
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                           @@    ”
# ECHO "    @@                           @@    ”
# ECHO "    @@      @@@@@@@@@@@@@        @@    ?"
# ECHO "    @@      @@           @@      @@    ?"
# ECHO "    @@      @@            @@     @@    ?"
# ECHO "    @@      @@             @@    @@    ?"
# ECHO "    @@      @@             @@    @@    ?"
# ECHO "    @@      @@            @@     @@    ?"
# ECHO "    @@      @@           @@      @@    ?"
# ECHO "    @@      @@@@@@@@@@@@@        @@    ?"
# ECHO "    @@                           @@    ”
# ECHO "    @@     D E C R Y P T E R     @@    ”
# ECHO "    @@      KINGSLY / JATIN      @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/
/*

/*
************************************************************
* File name: MainScanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main code for Scanner (A22)
* Function list: (...).
************************************************************
*/

/*
 *.............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: Preprocessor directives
 * The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define directive does not have any effect on other compiler projects
 * (Gcc, VSCode, Codeblocks, etc.).
 *.............................................................................
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

 /*check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/*
 * -------------------------------------------------------------
 *  Global vars and External vars
 * -------------------------------------------------------------
 */

 /* Global objects - variables (used in other codes as external) */
BufferPointer stringLiteralTable;	/* This buffer implements String Literal Table */
de_int errorNumber;				/* Run-time error number = 0 by default (ANSI) */

/* External objects */
extern de_int line; /* Source code line numbers - defined in scanner.c */
extern Token tokenizer(de_void);
/* External scanner functions and data */
extern de_int startScanner(BufferPointer pBuffer);
extern ScannerData scData;          /* Struct or global scanner data (defined in scanner.c) */
de_int tokenCounts[NUM_TOKENS] = { 0 };




/*
 * -------------------------------------------------------------
 *  Function declarations
 * -------------------------------------------------------------
 */
de_void printScannerError(de_strg fmt, ...);
de_void displayScanner(BufferPointer ptrBuffer);
de_long getScannerFilesize(de_strg fname);
de_void printToken(Token t);

/*
************************************************************
 *  Scanner Main function
 * Parameters:
 *   argc / argv = Parameters from command prompt
 * Return value:
 *	Success operation.
 ***********************************************************
 */


de_int main3Scanner(de_int argc, de_strg* argv) {
	stringLiteralTable = malloc(sizeof(Buffer));
	stringLiteralTable->content = malloc(INITIAL_SIZE);
	stringLiteralTable->size = INITIAL_SIZE;
	stringLiteralTable->position.wrte = 0;
	stringLiteralTable->flags.isEmpty = TRUE;


	BufferPointer sourceBuffer;		/* Pointer to input (source) buffer */
	FILE* fileHandler;				/* Input file handle */
	Token currentToken;				/* Token produced by the scanner */
	de_int loadSize = 0;			/* The size of the file loaded in the buffer */
	de_strg filename = NULL;        /* File name */
	de_int tokenCounts[NUM_TOKENS] = { 0 };  // Initialize all counts to 0


	/* Check for correct arrguments - source file name */
	if (argc <= 2) {
		/* __DATE__, __TIME__, __LINE__, __FILE__ are predefined preprocessor macros*/
		printScannerError("Date: %s  Time: %s", __DATE__, __TIME__);
		printScannerError("Runtime error at line %d in file %s", __LINE__, __FILE__);
		printScannerError("%s%s", argv[0], ": Missing source file name.");
		printScannerError("%s", "Usage: <Option=1> <SourceFile>");
		exit(EXIT_FAILURE);
	}

	/* Shows debug mode */
	printf("%s%d%s", "[Debug mode: ", DEBUG, "]\n");

	/* Create a source code input buffer - multiplicative mode */
	sourceBuffer = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_INCREMENT, MODE_MULTI);

	if (sourceBuffer == NULL) {
		printScannerError("%s%s", argv[1], ": Could not create source buffer");
		exit(EXIT_FAILURE);
	}


	/* Ensure proper arguments */
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Get filename */
	filename = argv[2];

	/* Create source buffer */
	sourceBuffer = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_INCREMENT, MODE_MULTI);
	if (!sourceBuffer) {
		fprintf(stderr, "Could not create source buffer\n");
		exit(EXIT_FAILURE);
	}

	/* Load file into buffer */
	loadSize = readerLoad(sourceBuffer, filename);
	if (loadSize == READER_ERROR) {
		fprintf(stderr, "%s: Error loading file %s\n", argv[0], filename);
		exit(EXIT_FAILURE);
	}

	/* Add SEOF marker */
	readerAddChar(sourceBuffer, READER_TERMINATOR);

	/* Display source buffer */
	if (loadSize > 0) {
		displayScanner(sourceBuffer);
	}

	/* Initialize scanner */
	if (startScanner(sourceBuffer) != EXIT_SUCCESS) {
		printf("Scanner initialization failed.\n");
		exit(EXIT_FAILURE);
	}

	/* Print header */
	printf("\nScanning source file...\n");
	printf("Token\t\tAttribute\n");
	printf("----------------------------------\n");

	/* Token statistics array */
	int errorCount = 0;
	Token t;

	/* Main tokenization loop */
	while (1) {
		t = tokenizer();
		if (t.code == SEOF_T) break;   // stop at end of file

		printToken(t);                 // print the token

		// Count the token occurrence
		if (t.code >= 0 && t.code < NUM_TOKENS)
			tokenCounts[t.code]++;

		// Count errors specifically
		if (t.code == ERR_T)
			errorCount++;
	}

	/* Print string literal table */
	printf("\nPrinting string table...\n");
	printf("----------------------------------\n");
	if (stringLiteralTable && stringLiteralTable->position.wrte > 0) {
		de_int i = 0;
		while (i < stringLiteralTable->position.wrte) {
			if (stringLiteralTable->content[i] != '\0')
				printf("%s\n", &stringLiteralTable->content[i]);

			while (i < stringLiteralTable->position.wrte && stringLiteralTable->content[i] != '\0')
				i++;
			i++; // Skip null terminator
		}
	}
	printf("----------------------------------\n");
	/* Print scanner errors */
	printf("Number of scanner errors: %d\n", errorCount);

	/* Print token statistics */
	printf("----------------------------------\n");
	printf("Token Statistics:\n");
	printf("----------------------------------\n");

	for (int i = 0; i < NUM_TOKENS; i++) {
		if (tokenCounts[i] > 0)
			printf("%-15s = %d\n", tokenStrTable[i], tokenCounts[i]);
	}

	printf("----------------------------------\n");


	return EXIT_SUCCESS;
}

/*
************************************************************
 *  Error printing function with variable number of arguments
 *  Params: Variable arguments, using formats from C language.
 *	 - Internal vars use list of arguments and types from stdarg.h
 *   - NOTE: The format is using signature from C Language
***********************************************************
*/

de_void printScannerError(de_strg fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	/* Move to new line */
	if (strchr(fmt, '\n') == NULL)
		fprintf(stderr, "\n");
}

/*
************************************************************
 * The function displays buffer contents
 * Param:
 *		- Scanner to be displayed.
 ***********************************************************
 */

de_void displayScanner(BufferPointer ptrBuffer) {
	printf("\nPrinting buffer parameters:\n\n");
	printf("The capacity of the buffer is:  %d\n", readerGetSize(ptrBuffer));
	printf("The current size of the buffer is:  %d\n", readerGetPosWrte(ptrBuffer));
	printf("\nPrinting buffer contents:\n\n");
	readerRecover(ptrBuffer);
	readerPrint(ptrBuffer);
}

/*
 ************************************************************
 * The function gets size of scanner file
 * Param:
 *	- Filename
 * Return:
 *	- Size of the file
 ***********************************************************
 */

de_long getScannerFilesize(de_strg fname) {
	FILE* fileInput;
	de_long fileLength;
	fileInput = fopen(fname, "r");
	if (fileInput == NULL) {
		printScannerError("%s%s", "Cannot open file: ", fname);
		return 0L;
	}
	fseek(fileInput, 0L, SEEK_END);
	fileLength = ftell(fileInput);
	fclose(fileInput);
	return fileLength;
}
