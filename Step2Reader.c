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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <ctype.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate (1)
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer readerCreate(de_int size, de_real factor) {
	// Defensive programming: validate size
	if (size <= 0) {
		return NULL;
	}
	
	// Allocate memory for the readerPointer
	BufferPointer readerPointer = calloc(1, sizeof(Buffer));
	if (readerPointer == NULL) {
		return NULL;
	}

	// Allocate memory for content
	de_strg content = malloc(size);
	if (content == NULL) {
		free(readerPointer); // Clean up previously allocated memory
		return NULL;
	}

	// Assign content to readerPointer
	readerPointer->content = content;

	// Initialize histogram (assuming it's an array or struct)
	memset(readerPointer->histogram, 0, sizeof(readerPointer->histogram));

	// Initialize errors (assuming it's a counter or array)
	readerPointer->numReaderErrors = 0;

	// Update properties
	readerPointer->size = size;
	readerPointer->factor = factor;

	// Initialize flags
	readerPointer->flags = (Flag){0};

	// Signal that the buffer was created successfully
	readerPointer->flags.isEmpty =TRUE;  // or 1 if de_boln is int


	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar (2)
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer const readerPointer, de_char ch) {
	de_strg tempReader = NULL;
	de_int newSize = 0;
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return NULL; // Defensive programming: invalid pointer
	}

	/* TO_DO: Test the inclusion of chars */
	if (readerPointer->position.wrte * (de_int)sizeof(de_char) < readerPointer->size) {
		/* TO_DO: Buffer not full: set flag */
		readerPointer->flags.isFull = FALSE;
	}
	else {
		/* TO_DO: Reset Full flag */
		readerPointer->flags.isFull = TRUE;
		/* TO_DO: Adjust the size to be duplicated */

		newSize = readerPointer->size * 2;
		tempReader = realloc(readerPointer->content, newSize);

		/* TO_DO: Defensive programming */

		if (tempReader == NULL) {
			return NULL; // Defensive programming: realloc failed
		}

		readerPointer->content = tempReader;
		readerPointer->size = newSize;
		readerPointer->flags.isMoved = TRUE;
		readerPointer->flags.isFull = FALSE; // Reset full flag after resize

	}
	/* TO_DO: Add the char */

	readerPointer->content[readerPointer->position.wrte] = ch;
	readerPointer->position.wrte++;

	/* TO_DO: Updates histogram */

		if (readerPointer->histogram != NULL) {
			readerPointer->histogram[(de_int)ch]++;
		}


	// Update flags
	readerPointer->flags.isEmpty = FALSE;

	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear(3)
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_boln readerClear(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */

	if (readerPointer == NULL || readerPointer->content == NULL) {
		return FALSE;
	}

	/* TO_DO: Adjust positions to zero */

	readerPointer->position.wrte = 0;
	readerPointer->position.read = 0;

	/* TO_DO: Adjust flags original */

	readerPointer->flags.isEmpty = TRUE;
	readerPointer->flags.isFull = FALSE;
	readerPointer->flags.isRead = FALSE;
	readerPointer->flags.isMoved = FALSE;

	return TRUE;
}

/*
***********************************************************
* Function name: readerFree(4)
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_boln readerFree(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if(readerPointer == NULL) {
		return FALSE;
	}
	/* Free memory (buffer/content) */

// Free content
	if (readerPointer->content != NULL) {
		free(readerPointer->content);
		readerPointer->content = NULL;
	}

	// Free the buffer itself
	free(readerPointer);

	return TRUE;


	//return FALSE;
}

/*
***********************************************************
* Function name: readerIsFull(5)
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_boln readerIsFull(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return FALSE;
	}
	/* TO_DO: Check flag if buffer is FUL */
	return readerPointer->flags.isFull;
	//return FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty(6)
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_boln readerIsEmpty(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
		if (readerPointer == NULL) {
			return FALSE;
		}

		/* TO_DO: Check flag if buffer is EMP */
		return readerPointer->flags.isEmpty;
	}	
//	return FALSE;
//}

/*
***********************************************************
* Function name: readerSetMark(7)
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_boln readerSetMark(BufferPointer const readerPointer, de_int mark) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || mark < 0 || mark >= readerPointer->size) {
		return FALSE;
	}
	/* TO_DO: Adjust mark */

	readerPointer->position.mark = mark;
	return TRUE;
	//return FALSE;
}


/*
***********************************************************
* Function name: readerPrint(8)
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_int readerPrint(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming (including invalid chars) */
	de_int count = 0;
	// Loop through the buffer content up to the write position
	for (de_int i = 0; i < readerPointer->position.wrte; ++i) {
		de_char ch = readerPointer->content[i];

		// Skip invalid characters (e.g., CHARSEOF or non-printable)
		if (ch == CHARSEOF || ch < 0 || ch > 127) {
			continue;
		}
	/* TO_DO: Print the buffer content */
		putchar(ch);  // Print character to stdout
		count++;
	}

	return count;
	//return 0;
}

/*
***********************************************************
* Function name: readerLoad(9)
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_int readerLoad(BufferPointer const readerPointer, de_strg fileName) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || fileName == NULL) {
		return 0;
	}
	/* TO_DO: Loads the file */
	de_strg loadedContent = vigenereMem(fileName, STR_LANGNAME, DECYPHER);
	if (loadedContent == NULL) {
		return 0; // Failed to load or decrypt
	}
	/* TO_DO: Creates the string calling vigenereMem(fileName, STR_LANGNAME, DECYPHER) */
	// Free existing content if needed
	if (readerPointer->content != NULL) {
		free(readerPointer->content);
	}

	// Assign new content
	readerPointer->content = loadedContent;

	// Update size and position
	size_t length = strlen(loadedContent);
	if (length > INT_MAX) {
		// Handle overflow case (e.g., reject the file or truncate)
		return 0;
	}
	readerPointer->size = (de_int)length;
	readerPointer->position.wrte = readerPointer->size;
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;

	// Update flags
	readerPointer->flags.isEmpty = (readerPointer->size == 0) ? TRUE : FALSE;
	readerPointer->flags.isFull = FALSE;
	readerPointer->flags.isRead = FALSE;
	readerPointer->flags.isMoved = FALSE;

	// Update histogram
	memset(readerPointer->histogram, 0, sizeof(readerPointer->histogram));
	for (de_int i = 0; i < readerPointer->size; ++i) {
		de_char ch = readerPointer->content[i];
		if (ch >= 0 && ch < NCHAR) {
			readerPointer->histogram[(de_int)ch]++;
		}
	}

	return readerPointer->size;
}


/*
***********************************************************
* Function name: readerRecover(10)
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_boln readerRecover(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return FALSE;
	}
	/* TO_DO: Adjust read and mark to zero */
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;

	return TRUE;
//	return FALSE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_boln readerRetract(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Retract (return 1 pos read) */
	return FALSE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_boln readerRestore(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Restore read to mark */
	return TRUE;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_char readerGetChar(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Returns size in the read position and updates read */
	return '\0';
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_strg readerGetContent(BufferPointer const readerPointer, de_int pos) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return content (string) */
	return NULL;
}

/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_int readerGetPosRead(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return read */
	return 0;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_int readerGetPosWrte(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return wrte */
	return 0;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_int readerGetPosMark(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return mark */
	return 0;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/de_int readerGetSize(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return size */
	return 0;
}

/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
de_void readerPrintFlags(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return;
	/* TO_DO: Return flags */
}
#else
#define bGetFlags(readerPointer) ((readerPointer)?(readerPointer->flags):(RT_FAIL_1))
#endif

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_void readerPrintStat(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Print statistics */
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
de_int readerNumErrors(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return the number of errors */
	return 0;
}

/*
***********************************************************
* Function name: readerChecksum
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

de_int readerChecksum(BufferPointer readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return the checksum (given by the content) */
	return 0;
}
