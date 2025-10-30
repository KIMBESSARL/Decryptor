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
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

 /* TIP: Do not change pragmas, unless necessary .......................................*/
 /*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
 /*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

 /* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Constants about controls (not need to change) */
#define READER_ERROR		(-1)		/* General error message */
#define READER_TERMINATOR	'\0'		/* General EOF */

#define READER_DEFAULT_INCREMENT 15
#define MODE_MULTI 'm'

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE		INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE		100			/* default initial buffer reader capacity */
#define READER_DEFAULT_FACTOR	0.5f		/* default factor */
#define HISTOGRAM_SIZE 256			/* size of the histogram array */

/* TO_DO: Add your bit-masks constant definitions here */

/*
// BITS                             (    76543210)
#define READER_DEFAULT_FLAG 0x00 	// (0b00000000) = (0x00)_16 = (000)_10
// TO_DO: BIT 3: END = End of buffer flag
#define READER_SET_FLAG_END 0x08	// (0b00001000) = (0x08)_16 = (008)_10
// TO_DO: BIT 2: REL = Rellocation memory flag
#define READER_SET_FLAG_REL 0x04	// (0b00000100) = (0x04)_16 = (004)_10
// TO_DO: BIT 1: EMP = Buffer empty flag
#define READER_SET_FLAG_FUL 0x02	// (0b00000010) = (0x02)_16 = (002)_10
// TO_DO: BIT 0: FUL = Buffer full flag
#define READER_SET_FLAG_EMP 0x01	// (0b00000001) = (0x01)_16 = (001)_10
*/

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	de_int wrte;					/* the offset to the add chars (in chars) */
	de_int read;					/* the offset to the get a char position (in chars) */
	de_int mark;					/* the offset to the mark position (in chars) */
} Position;

/* Flags declaration */
typedef struct flag {
	de_boln isEmpty;					/* checks if there is no content */
	de_boln isFull;					/* the content is using all size */
	de_boln isRead;					/* all content was read */
	de_boln isMoved;					/* the content was moved in reallocation */
} Flag;

/* Buffer structure */
typedef struct bufferReader {
	de_strg		content;			/* pointer to the beginning of character array (character buffer) */
	de_int		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	de_real		factor;				/* factor for increase the buffer */
	Flag			flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;			/* Offset / position field */
	de_int		histogram[NCHAR];	/* Statistics of chars */
	de_int		numReaderErrors;	/* Number of errors from Reader */
	de_int		checkSum;			/* Sum of bytes(chars) */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
BufferPointer	readerCreate(de_int, de_real, de_int);
BufferPointer	readerAddChar(BufferPointer const, de_char);
de_boln		readerClear(BufferPointer const);
de_boln		readerFree(BufferPointer const);
de_boln		readerIsFull(BufferPointer const);
de_boln		readerIsEmpty(BufferPointer const);
de_boln		readerSetMark(BufferPointer const, de_int);
de_int		readerPrint(BufferPointer const);
de_int		readerLoad(BufferPointer const, de_strg);
de_boln		readerRecover(BufferPointer const);
de_boln		readerRetract(BufferPointer const);
de_boln		readerRestore(BufferPointer const);
de_int		readerChecksum(BufferPointer const);
/* Getters */
de_char		readerGetChar(BufferPointer const);
de_strg		readerGetContent(BufferPointer const, de_int);
de_int		readerGetPosRead(BufferPointer const);
de_int		readerGetPosWrte(BufferPointer const);
de_int		readerGetPosMark(BufferPointer const);
de_int		readerGetSize(BufferPointer const);
de_void		readerPrintFlags(BufferPointer const);
de_void		readerPrintStat(BufferPointer const);
de_int		readerNumErrors(BufferPointer const);
/* in Step2Reader.h */
de_char readerGetCharAt(BufferPointer const readerPointer, de_int index);

#endif
