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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

// Function to perform the Vigenère cipher (encoding or decoding)
void vigenereFile(const en_strg inputFileName, const en_strg outputFileName, const en_strg key, en_int encode) {
	// TO_DO: Use defensive programming (checking files)
	// TO_DO: Define local variables
	// TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
	// TO_DO: Close the files


    // Define the input and output files (ex: FILE* inputFile, FILE* outputFile
    FILE* inputFile = fopen(inputFileName, "r");
    // Open the output file for writing
    FILE* outputFile = fopen(outputFileName, "w");

    // Defensive programming: check if input file opened successfully
    if (!inputFile) {
        fprintf(stderr, "Error: Cannot open input file %s\n", inputFileName);
        return;
    }

    // Defensive programming: check if output file opened successfully
    if (!outputFile) {
        fprintf(stderr, "Error: Cannot open output file %s\n", outputFileName);
        fclose(inputFile); 
        return;
    }

    // Get the length of the key
    en_long keyLen = strlen(key);
    // Index to track position in the key
    en_long keyIndex = 0;
    en_int varHold; // Variable to hold each character read from the input file

    // Read each character from the input file until end-of-file
    while ((varHold = fgetc(inputFile)) != EOF) {
        // Check if the character is a letter
        if (isalpha(varHold)) {
            // Determine base character ('A' for uppercase, 'a' for lowercase)
            char base = isupper(varHold) ? 'A' : 'a';
            // Convert key character to uppercase and get its alphabetical index (0–25)
            char keyChar = toupper(key[keyIndex % keyLen]) - 'A';

            // If decoding, reverse the shift
            if (!encode) keyChar = 26 - keyChar;

            // Apply Vigenère cipher shift and wrap around alphabet
            char newChar = (varHold - base + keyChar) % 26 + base;
            // Write the encoded/decoded character to the output file
            fputc(newChar, outputFile);

            // Move to the next character in the key
            keyIndex++;
        }
        else {
            // Non-alphabetic characters are written unchanged
            fputc(varHold, outputFile);
        }
    }

    // Close both files
    fclose(inputFile);
    fclose(outputFile);
}


// Function to perform the Vigenère cipher (encoding or decoding)
en_strg vigenereMem(const en_strg inputFileName, const en_strg key, en_int encode) {
    // Defensive check for NULL inputFileName or key
    if (inputFileName == NULL || key == NULL || strlen(key) == 0) {
        fprintf(stderr, "Error: Invalid input file name or key.\n");
        return NULL;
    }

    // Open the input file
    FILE* file = fopen(inputFileName, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open input file '%s'.\n", inputFileName);
        return NULL;
    }

    // Determine file size
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    // Allocate memory for input text
    en_strg inputText = (en_strg)malloc(fileSize + 1);
    if (inputText == NULL) {
        fprintf(stderr, "Error: Memory allocation for input text failed.\n");
        fclose(file);
        return NULL;
    }

    // Read file content into inputText
    fread(inputText, 1, fileSize, file);
    inputText[fileSize] = '\0'; // Null-terminate
    fclose(file);

    // Allocate memory for output string
    en_strg output = (en_strg)malloc(fileSize + 1);
    if (output == NULL) {
        fprintf(stderr, "Error: Memory allocation for output failed.\n");
        free(inputText);
        return NULL;
    }

    size_t keyLen = strlen(key);
    size_t keyIndex = 0;

    // Vigenère cipher logic
    for (size_t i = 0; i < fileSize; ++i) {
        en_char ch = inputText[i];

        if (isalpha((unsigned char)ch)) {
            en_char base = isupper((unsigned char)ch) ? 'A' : 'a';
            en_char keyChar = toupper((unsigned char)key[keyIndex % keyLen]) - 'A';

            if (!encode) {
                keyChar = 26 - keyChar; // Reverse shift for decoding
            }

            output[i] = (ch - base + keyChar) % 26 + base;
            keyIndex++;
        }
        else {
            output[i] = ch; // Preserve non-alphabetic characters
        }
    }

    output[fileSize] = '\0'; // Null-terminate output

    free(inputText); // Clean up inputText memory
    return output;
}


// Function to encode (cypher)
void cypher(const en_strg inputFileName, const en_strg outputFileName, const en_strg key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);
}

// Function to decode (decypher)
void decypher(const en_strg inputFileName, const en_strg outputFileName, const en_strg key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

// TO_DO: Get file size (util method)


// Function to get the size of a file in bytes
en_int getSizeOfFile(const en_strg filename) {
    en_int size = 0;

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