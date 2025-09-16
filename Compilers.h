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
# ECHO "    @@        @@           @@      @@    �"
# ECHO "    @@        @@            @@     @@    �"
# ECHO "    @@        @@             @@    @@    �"
# ECHO "    @@        @@             @@    @@    �"
# ECHO "    @@        @@            @@     @@    �"
# ECHO "    @@        @@           @@      @@    �"
# ECHO "    @@        @@@@@@@@@@@@@        @@    �"
# ECHO "    @@                             @@    ”
# ECHO "    @@        D E C R Y P T E R    @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A1-A5.
* Date: Sep 01 2025
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/* TO_DO: Adjust your language (cannot be "Sofia") */
/* Language name */
#define STR_LANGNAME	"decoder"

/* Logical constants - adapt for your language */
#define DECODER_TRUE  1
#define DECODER_FALSE 0

#define INVALID NULL
#define EOF_CHAR '\0'

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/

/* TO_DO: Define your typedefs */
typedef char			decoder_char;
typedef char*			decoder_strg;
typedef int				decoder_intg;
typedef float			decoder_real;
typedef void			decoder_void;

typedef unsigned char	decoder_boln;
typedef unsigned char	decoder_byte;

typedef long			decoder_long;
typedef double			decoder_doub;

/*
------------------------------------------------------------
Programs:
1: Coder - invokes Main1Coder code
2: Reader - invokes Main2Reader code
3: Scanner - invokes Main3Scanner code
4: Parser - invokes Main4Parser code
5: Writer - invokes Main5Writer code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_CDR = '1', // Coder:	05pt
	PGM_RDR = '2', // Reader:	05pt
	PGM_SCN = '3', // Scanner:	15pt
	PGM_PSR = '4', // Parser:	15pt
	PGM_WRT = '5', // Writer:	10pt
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/
decoder_intg main1Coder	(decoder_intg argc, decoder_strg* argv);

decoder_void printLogo();
decoder_void errorPrint(decoder_strg fmt, ...);

#endif

/*
	  "args": [
		1,
		1,
		"README.txt",
		"CODED.txt"
	  ]
	  "args": [
		4,
		"CODED.txt"
	  ]
*/
