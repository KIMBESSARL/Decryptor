/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@        @@@@@@@@@@@@@        @@    �"
# ECHO "    @@        @           @@       @@    �"
# ECHO "    @@        @            @@      @@    �"
# ECHO "    @@        @             @@     @@    �"
# ECHO "    @@        @             @@     @@    �"
# ECHO "    @@        @            @@      @@    �"
# ECHO "    @@        @           @@       @@    �"
# ECHO "    @@        @@@@@@@@@@@@@        @@    �"
# ECHO "    @@                             @@    ”
# ECHO "    @@        D E C O D E R        @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
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
void vigenereFile(const decoder_strg inputFileName, const decoder_strg outputFileName, const decoder_strg key, decoder_intg encode) {
	// TO_DO: Define the input and output files (ex: FILE* inputFile, FILE* outputFile
	// TO_DO: Use defensive programming (checking files)
	// TO_DO: Define local variables
	// TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
	// TO_DO: Close the files
}

// Function to perform the Vigenère cipher (encoding or decoding)
decoder_strg vigenereMem(const decoder_strg inputFileName, const decoder_strg key, decoder_intg encode) {
	// TO_DO define the return type and local variables
	decoder_strg output = NULL;
	// TO_DO: Check defensive programming
	// TO_DO: Use the logic to code/decode - consider the logic about visible chars only
	return output;
}

// Function to encode (cypher)
void cypher(const decoder_strg inputFileName, const decoder_strg outputFileName, const decoder_strg key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);
}

// Function to decode (decypher)
void decypher(const decoder_strg inputFileName, const decoder_strg outputFileName, const decoder_strg key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

// TO_DO: Get file size (util method)
decoder_intg getSizeOfFile(const decoder_strg filename) {
	decoder_intg size = 0;
    // TO_DO: Use the logic to get the size of the file
    return size;
}
