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
# ECHO "    @@      @@@@@@@@@@@@@        @@    �"
# ECHO "    @@      @@           @@      @@    �"
# ECHO "    @@      @@            @@     @@    �"
# ECHO "    @@      @@             @@    @@    �"
# ECHO "    @@      @@             @@    @@    �"
# ECHO "    @@      @@            @@     @@    �"
# ECHO "    @@      @@           @@      @@    �"
# ECHO "    @@      @@@@@@@@@@@@@        @@    �"
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
* Date: Sep 01 2025
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
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif


// Function to perform the Vigenère cipher (encoding or decoding)
void vigenereFile(const de_strg inputFileName, const de_strg outputFileName, const de_strg key, de_int encode) {
	// TO_DO: Use defensive programming (checking files)
	// TO_DO: Define local variables
	// TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
	// TO_DO: Close the files

     // Defensive programming: check for valid input
        if (inputFileName == NULL || outputFileName == NULL || key == NULL || strlen(key) == 0) {
            fprintf(stderr, "Error: Invalid input/output file name or key.\n");
            return;
        }

        // Open input file for reading
        FILE* inputFile = fopen(inputFileName, "r");
        if (!inputFile) {
            fprintf(stderr, "Error: Cannot open input file '%s': %s\n", inputFileName, strerror(errno));
           
            return;
        }

        // Open output file for writing
        FILE* outputFile = fopen(outputFileName, "w");
        if (!outputFile) {
            fprintf(stderr, "Error: Cannot open output file '%s': %s\n", outputFileName, strerror(errno));
            fclose(inputFile);
            return;
        }

        de_long keyLen = (de_long) strlen(key);
        de_long keyIndex = 0;
        de_int varHold;

        // Apply Vigenère cipher and write to output file
        while ((varHold = fgetc(inputFile)) != EOF) {
            if (varHold >= ASCII_START && varHold <= ASCII_END) {
                char keyChar = key[keyIndex % keyLen];
                de_int shift = keyChar % ASCII_RANGE;

                if (!encode) shift = ASCII_RANGE - shift;

                char newChar = ((varHold - ASCII_START + shift) % ASCII_RANGE) + ASCII_START;
                fputc(newChar, outputFile);
                keyIndex++;
            }
            else {
                fputc(varHold, outputFile); // Preserve non-visible characters
            }
        }


        fclose(inputFile);
        fclose(outputFile);

        // Display the output file content in the terminal
        FILE* displayFile = fopen(outputFileName, "r");
        if (!displayFile) {
            fprintf(stderr, "Error: Cannot open output file '%s' for display.\n", outputFileName);
            return;
        }

        printf("\n> type CODED / DECODED:\n\n");

        while ((varHold = fgetc(displayFile)) != EOF) {
            putchar(varHold);
        }

        fclose(displayFile);
    }


// Function to perform the Vigenère cipher (encoding or decoding)
de_strg vigenereMem(const de_strg inputFileName, const de_strg key, de_int encode) {
    // Defensive check for NULL inputFileName or key
    if (inputFileName == NULL || key == NULL || strlen(key) == 0) {
        fprintf(stderr, "Error: Invalid input file name or key.\n");
        return NULL;
    }

    // Open the input file
    FILE* file = fopen(inputFileName, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open input file '%s': %s\n", inputFileName, strerror(errno));
        return NULL;
    }

    // Determine file size
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    // Allocate memory for input text
   de_strg inputText = (de_strg)malloc(fileSize + 1);
    if (inputText == NULL) {
        fprintf(stderr, "Error: Memory allocation for input text failed.\n");
        fclose(file);
        return NULL;
    }

	// Read file content into inputText and display in the terminal
    fread(inputText, 1, fileSize, file);
    inputText[fileSize] = '\0'; // Null-terminate
    fclose(file);

    // Allocate memory for output string
    de_strg output = (de_strg)malloc(fileSize + 1);
    if (output == NULL) {
        fprintf(stderr, "Error: Memory allocation for output failed.\n");
        free(inputText);
        return NULL;
    }

    size_t keyLen = strlen(key);
    size_t keyIndex = 0;

    // Vigenère cipher logic for visible ASCII characters
    for (size_t i = 0; i < fileSize; ++i) {
        de_char ch = inputText[i];

        if (ch >= ASCII_START && ch <= ASCII_END) {
            de_char keyChar = key[keyIndex % keyLen];
            de_int shift = keyChar % ASCII_RANGE;

            if (!encode) {
                shift = ASCII_RANGE - shift;
            }

            output[i] = ((ch - ASCII_START + shift) % ASCII_RANGE) + ASCII_START;
            keyIndex++;
        }
        else {
            output[i] = ch; // Preserve non-visible characters
        }
    }

    output[fileSize] = '\0'; // Null-terminate output

    free(inputText); // Clean up inputText memory
    return output;
}


// Function to encode (cypher)
void cypher(const de_strg inputFileName, const de_strg outputFileName, const de_strg key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);
}

// Function to decode (decypher)
void decypher(const de_strg inputFileName, const de_strg outputFileName, const de_strg key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

// TO_DO: Get file size (util method)


// Function to get the size of a file in bytes
de_int getSizeOfFile(const de_strg filename) {
    de_int size = 0;

    // Open the file in binary read mode
    FILE* file = fopen(filename, "rb");

    // Defensive programming: check if the file opened successfully
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return -1; // Return -1 to indicate error
    }

    // Seek to the end of the file
    fseek(file, 0, SEEK_END); 

    // Get the current position in the file (which is the size)
    size = ftell(file);

    // Close the file
    fclose(file);

    return size;
}