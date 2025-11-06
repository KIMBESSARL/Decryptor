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
extern de_void printScannerData(ScannerData scData);



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



	/* Get filename */
	filename = argv[2];

	/* Open file */
	fileHandler = fopen(filename, "r");
	if (fileHandler == NULL) {
		printScannerError("%s%s%s", argv[0], ": Cannot open file: ", filename);
		exit(EXIT_FAILURE);
	}

	/* Load file contents into buffer */
	loadSize = readerLoad(sourceBuffer, filename);
	if (loadSize == READER_ERROR) {
		printScannerError("%s%s", argv[0], ": Error loading file");
		exit(EXIT_FAILURE);
	}
	/* Close file */
	fclose(fileHandler);


	/* Find the size of the file */
	if (loadSize == READER_ERROR) {
		printf("The input file %s %s\n", argv[2], "is not completely loaded.");
		printf("Input file size: %ld\n", getScannerFilesize(argv[2]));
	}

	/* Compact and display the source buffer and add SEOF to input program buffer */
	if ((loadSize != READER_ERROR) && (loadSize != 0)) {
		if (readerAddChar(sourceBuffer, READER_TERMINATOR)) {
			displayScanner(sourceBuffer);
		}
	}


	if (argc < 2) {
		fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
		return EXIT_FAILURE;
	}
	printf("\n");
	printf("\nScanning source file...\n");
	printf("Token\t\tAttribute\n");
	printf("----------------------------------\n");

	/* 1?? Create source buffer */
	sourceBuffer = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_INCREMENT, MODE_MULTI);
	if (!sourceBuffer) {
		fprintf(stderr, "Could not create source buffer\n");
		exit(EXIT_FAILURE);
	}
	/* 2?? Load source file */
	if (!readerLoad(sourceBuffer, argv[2])) {
		fprintf(stderr, "Could not load source file\n");
		exit(EXIT_FAILURE);
	}

	/* 3?? Add SEOF marker and reset reader */
	readerAddChar(sourceBuffer, '\0');
	//readerRecover(sourceBuffer);

	if (startScanner(sourceBuffer) != EXIT_SUCCESS) {
		printf("Scanner initialization failed.\n");
		exit(EXIT_FAILURE);
	}


/* Loop to print the Statistic*/
/* Load your source file once */
readerLoad(sourceBuffer, filename);
/* Start scanning */
while (1) {
	Token t = tokenizer();

	/* --- print token info safely --- */
	switch (t.code) {
	case KW_T:
	case MNID_T:
	case ID_T:
	case STR_T:
		/* only print lexeme if the field exists */
		printf("%-20s %s\n", tokenStrTable[t.code], t.attribute.idLexeme);
		break;

	case ERR_T:
		printf("%-20s %s\n", tokenStrTable[t.code], t.attribute.errLexeme);
		break;

	case SEOF_T:
		/* some compilers complain if seofType doesn’t exist — you can remove it if needed */
		printf("%-20s\n", tokenStrTable[t.code]);
		break;

	default:
		printf("%-20s\n", tokenStrTable[t.code]);
		break;
	}

	/* --- count tokens --- */
	if (t.code >= 0 && t.code < NUM_TOKENS)
		tokenCounts[t.code]++;

	/* --- stop at end-of-file --- */
	if (t.code == SEOF_T)
		break;
}

/* After scanning is complete */
printf("\nPrinting string table...\n");
printf("----------------------------------\n");

if (stringLiteralTable && stringLiteralTable->position.wrte > 0) {
	de_int i = 0;
	while (i < stringLiteralTable->position.wrte) {
		if (stringLiteralTable->content[i] != '\0') {
			printf("%s\n", &stringLiteralTable->content[i]);
		}

		// Move to next string
		while (i < stringLiteralTable->position.wrte && stringLiteralTable->content[i] != '\0') {
			i++;
		}
		i++; // Skip the null terminator
	}
}

printf("----------------------------------\n");

/* Print the number of error*/
int errorCount = 0;
Token t = tokenizer();
if (t.code == ERR_T) {
	errorCount++;
}
printf("Number of scanner errors: %d\n", errorCount);// end of the error count print

/* Print function of */
printf("----------------------------------\n");
printf("Statistics:\n");
printf("----------------------------------\n");
for (int i = 0; i < NUM_TOKENS; i++) {
	if (tokenCounts[i] > 0)
		printf("Token[%s]=%d\n", tokenStrTable[i], tokenCounts[i]);
}
printf("----------------------------------\n");

	return (EXIT_SUCCESS);
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
