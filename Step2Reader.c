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
    if (size <= 0) return NULL;

    BufferPointer readerPointer = calloc(1, sizeof(Buffer));
    if (!readerPointer) return NULL;

    readerPointer->content = malloc(size);
    if (!readerPointer->content) {
        free(readerPointer);
        return NULL;
    }

    memset(readerPointer->histogram, 0, sizeof(readerPointer->histogram));
    readerPointer->numReaderErrors = 0;
    readerPointer->size = size;
    readerPointer->factor = factor;
    readerPointer->flags.isEmpty = TRUE;
    readerPointer->flags.isFull = FALSE;
    readerPointer->flags.isMoved = FALSE;
    readerPointer->flags.isRead = FALSE;
    readerPointer->position.wrte = 0;
    readerPointer->position.read = 0;
    readerPointer->position.mark = 0;

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
    if (!readerPointer || !readerPointer->content) return NULL;

    if (readerPointer->position.wrte >= readerPointer->size) {
        de_int newSize = readerPointer->size * 2;
        de_strg temp = realloc(readerPointer->content, newSize);
        if (!temp) return NULL;
        readerPointer->content = temp;
        readerPointer->size = newSize;
        readerPointer->flags.isMoved = TRUE;
        readerPointer->flags.isFull = FALSE;
    }

    readerPointer->content[readerPointer->position.wrte++] = ch;
    readerPointer->flags.isEmpty = FALSE;
    if (readerPointer->position.wrte >= readerPointer->size) readerPointer->flags.isFull = TRUE;

    if (readerPointer->histogram != NULL && (unsigned char)ch < NCHAR) readerPointer->histogram[(de_int)ch]++;

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
    if (!readerPointer || !readerPointer->content) return FALSE;

    readerPointer->position.wrte = 0;
    readerPointer->position.read = 0;
    readerPointer->position.mark = 0;

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
    if (!readerPointer) return FALSE;
    if (readerPointer->content) free(readerPointer->content);
    free(readerPointer);
    return TRUE;
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
    if (!readerPointer) return FALSE;
    return readerPointer->flags.isFull;
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
    if (!readerPointer) return FALSE;
    return readerPointer->flags.isEmpty;
}

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
    if (!readerPointer || mark < 0 || mark >= readerPointer->size) return FALSE;
    readerPointer->position.mark = mark;
    return TRUE;
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
    if (!readerPointer || !readerPointer->content)
        return 0;

    de_int count = 0;
    for (de_int i = 0; i < readerPointer->position.wrte; ++i) {
        de_char ch = readerPointer->content[i];

        // Stop if null terminator is reached
        if (ch == '\0')
            break;

        // Keep structure (tabs/newlines) and printable characters
        if (isprint(ch) || ch == '\n' || ch == '\r' || ch == '\t')
            putchar(ch);
        else
            printf("\\x%02X", (unsigned char)ch);  // For debugging invisible chars

        count++;
    }

    return count;
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
    if (!readerPointer || !fileName) return 0;

    if (!readerPointer->content) {
        readerPointer->content = calloc(READER_DEFAULT_SIZE, sizeof(de_char));
        if (!readerPointer->content) return 0;
        readerPointer->size = READER_DEFAULT_SIZE;
        readerPointer->position.wrte = 0;
        readerPointer->flags.isEmpty = TRUE;
    }

    // Decrypt file content using vigenereMem
    de_strg decryptedContent = vigenereMem(fileName, STR_LANGNAME, DECYPHER);
    if (!decryptedContent) {
        fprintf(stderr, "Decryptor: Cannot allocate buffer - Use: buffer <input>.\nFilename: %s\n", fileName);
        return 0;
    }

    if (readerPointer->content) free(readerPointer->content);

    readerPointer->content = decryptedContent;
    readerPointer->size = (de_int)strlen(decryptedContent);

//    // Ensure null termination within buffer size
//if (readerPointer->size >= readerPointer->size_allocated)
//    readerPointer->content[readerPointer->size - 1] = '\0';
//else
//    readerPointer->content[readerPointer->size] = '\0';
    readerPointer->position.wrte = readerPointer->size;
    readerPointer->position.read = 0;
    readerPointer->position.mark = 0;
    readerPointer->flags.isEmpty = (readerPointer->size == 0);
    readerPointer->flags.isFull = FALSE;
    readerPointer->flags.isRead = FALSE;
    readerPointer->flags.isMoved = TRUE;

    memset(readerPointer->histogram, 0, sizeof(readerPointer->histogram));
    for (de_int i = 0; i < readerPointer->size; ++i) {
        de_char ch = readerPointer->content[i];
        if ((unsigned char)ch < NCHAR) readerPointer->histogram[(de_int)ch]++;
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
    if (!readerPointer) return FALSE;
    readerPointer->position.read = 0;
    readerPointer->position.mark = 0;
    return TRUE;
}

/*
***********************************************************
* Function name: readerRetract(11)
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
    if (!readerPointer || !readerPointer->content) return FALSE;
    if (readerPointer->position.read > 0) {
        readerPointer->position.read--;
        return TRUE;
    }
    return FALSE;
}

/*
***********************************************************
* Function name: readerRestore(12)
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
    if (!readerPointer || !readerPointer->content) return FALSE;
    readerPointer->position.read = readerPointer->position.mark;
    return TRUE;
}

/*
***********************************************************
* Function name: readerGetChar(13)
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
    if (!readerPointer || !readerPointer->content) return CHARSEOF;
    if (readerPointer->position.read >= readerPointer->position.wrte) return CHARSEOF;
    de_char ch = readerPointer->content[readerPointer->position.read++];
    readerPointer->flags.isRead = TRUE;
    return ch;
}

/*
***********************************************************
* Function name: readerGetContent(14)
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
    if (!readerPointer || !readerPointer->content) return NULL;
    if (pos < 0 || pos >= readerPointer->position.wrte) return NULL;
    return &readerPointer->content[pos];
}

/*
***********************************************************
* Function name: readerGetPosRead(15)
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
    if (!readerPointer) return -1;
    return readerPointer->position.read;
}
/*
***********************************************************
* Function name: readerGetPosWrte(16)
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
    if (!readerPointer) return -1;
    return readerPointer->position.wrte;
}

/*
***********************************************************
* Function name: readerGetPosMark(17)
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
    if (!readerPointer) return -1;
    return readerPointer->position.mark;
}

/*
***********************************************************
* Function name: readerGetSize(18)
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
*/

de_int readerGetSize(BufferPointer const readerPointer) {
    if (!readerPointer) return -1;
    return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetFlags(19)
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

de_void readerPrintFlags(BufferPointer const readerPointer) {
    if (!readerPointer) return;
    printf("Flag.isEmpty = %d\n", readerPointer->flags.isEmpty);
    printf("Flag.isFull  = %d\n", readerPointer->flags.isFull);
    printf("Flag.isMoved = %d\n", readerPointer->flags.isMoved);
    printf("Flag.isRead  = %d\n", readerPointer->flags.isRead);
}

/*
***********************************************************
* Function name: readerShowStat(20)
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
    if (!readerPointer || !readerPointer->histogram) return;
    for (de_int i = 0; i < NCHAR; ++i) {
        if (readerPointer->histogram[i] > 0) {
            printf("B[%c]=%d, ", (de_char)i, readerPointer->histogram[i]);
        }
    }
    printf("\n");
}

/*
***********************************************************
* Function name: readerNumErrors(21)
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
    if (!readerPointer) return -1;
    return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerChecksum(22)
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
    if (!readerPointer || !readerPointer->content) return -1;
    de_int checksum = 0;
    for (de_int i = 0; i < readerPointer->position.wrte; ++i)
        checksum += (de_int)readerPointer->content[i];
    return checksum;
}
