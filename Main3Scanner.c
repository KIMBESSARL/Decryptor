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
    // Initialize string literal table
    stringLiteralTable = malloc(sizeof(Buffer));
    stringLiteralTable->content = malloc(INITIAL_SIZE);
    stringLiteralTable->size = INITIAL_SIZE;
    stringLiteralTable->position.wrte = 0;
    stringLiteralTable->flags.isEmpty = TRUE;

    BufferPointer sourceBuffer;
    //Token currentToken;
    de_int loadSize = 0;
    de_strg filename = NULL;
    de_int tokenCounts[NUM_TOKENS] = { 0 };
    de_int errorCount = 0;

    /* Check for correct arguments */
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <option> <source file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("[Debug mode: %d]\n", DEBUG);

    /* Create source buffer */
    sourceBuffer = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_INCREMENT, MODE_MULTI);
    if (!sourceBuffer) {
        fprintf(stderr, "Could not create source buffer\n");
        exit(EXIT_FAILURE);
    }

    /* Load source file */
    filename = argv[2];
    loadSize = readerLoad(sourceBuffer, filename);
    if (loadSize == READER_ERROR) {
        fprintf(stderr, "Error loading file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    /* Add SEOF marker */
    readerAddChar(sourceBuffer, READER_TERMINATOR);

    /* Display source buffer */
    if (loadSize > 0)
        displayScanner(sourceBuffer);

    /* Initialize scanner */
    if (startScanner(sourceBuffer) != EXIT_SUCCESS) {
        printf("Scanner initialization failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("\nScanning source file...\n");
    printf("Token\t\tAttribute\n");
    printf("----------------------------------\n");

    /* --- Main tokenization loop --- */
    int additions = 0;

    while (1) {
        Token t = tokenizer();

        // Detect upcoming EOF before printing it
        if ((t.code == SEOF_T || t.code == EOS_T) && additions < 1) {
            // Append new content directly into the existing buffer
            de_int oldWrite = sourceBuffer->position.wrte;

            const char* newLine = "";
               /* "% the new comment line in my input file\n"
                "% the third line\n"
                "% the fourth line\n"
                "man(X)\n"
                "% Facts: Artists and their attributes\n"
                "artist(drake).\n"
                "artist(nav).\n"
                "artist(toBi).\n"
                "artist(maestro_fresh_wes).\n";*/

            for (int i = 0; newLine[i] != '\0'; i++)
                readerAddChar(sourceBuffer, newLine[i]);

            // Add terminator again
            readerAddChar(sourceBuffer, READER_TERMINATOR);

            // Start reading from newly added section
            sourceBuffer->position.read = oldWrite;

            additions++;
            continue; // go read from appended text without printing EOF
        }

        // If it's *not* EOF, print normally
        if (t.code != SEOF_T && t.code != EOS_T)
            printToken(t);

        tokenCounts[t.code]++; // <-- update stats
        if (t.code == ERR_T)
            errorCount++;

        // Stop only after the *final* EOF
        if ((t.code == SEOF_T || t.code == EOS_T) && additions >= 1)
            break;
    }



    /* --- Print string literal table --- */
    printf("\nPrinting string table...\n");
    printf("----------------------------------\n");
    if (stringLiteralTable && stringLiteralTable->position.wrte > 0) {
        de_int i = 0;
        while (i < stringLiteralTable->position.wrte) {
            if (stringLiteralTable->content[i] != '\0')
                printf("%s\n", &stringLiteralTable->content[i]);

            while (i < stringLiteralTable->position.wrte && stringLiteralTable->content[i] != '\0')
                i++;
            i++;
        }
    }
    printf("----------------------------------\n");

    /* --- Print summary --- */
    printf("Number of scanner errors: %d\n", errorCount);
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
