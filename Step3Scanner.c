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
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif


#define MAX_LEXEME_LEN 256

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
de_int line;								/* Current line number of the source code */
extern de_int errorNumber;				/* Defined in platy_st.c - run-time error number */
static const char* tokenStr(de_int code);

/* globals (one definition) */
de_int numScannerErrors = 0;
ScannerData scData = { {0}, NULL, 0 };

/* forward declarations (if needed) */
Token(*finalStateTable[NUM_STATES])(de_strg lexeme) = {
	NULL,     // S0
	funcID,   // S1 ? if accepting identifiers here
	funcIL,   // S2
	funcMNID, // S3
	NULL,     // S4
	funcSTR,  // S5
	NULL,     // S6
	funcCMT,  // S7
	funcSEP,  // S8
	funcEOS,  // S9
	NULL,     // S10
	funcFLT,  // S11
	funcID    // S12 ? if accepting identifiers here
};


extern de_int stateType[NUM_STATES];
extern de_strg keywordTable[KWT_SIZE];

extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern de_int transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

//de_int startScanner(BufferPointer psc_buf) {
//	/* TO_DO: Start histogram */
//	for (de_int i=0; i<NUM_TOKENS;i++)
//		scData.scanHistogram[i] = 0;
//	/* Basic scanner initialization */
//	/* in case the buffer has been read previously  */
//	readerRecover(psc_buf);
//	readerClear(stringLiteralTable);
//	line = 1;
//	sourceBuffer = psc_buf;
//	return EXIT_SUCCESS; /*0*/
//}

static const char* tokenStr(de_int code) {
	static const char* tokenNames[] = {
		"ERR_T", "SEOF_T", "MNID_T", "KW_T", "CMT_T", "LPR_T",
		"RPR_T", "LBR_T", "RBR_T", "EOS_T", "NUM_T", "STR_T"
	};
	if (code < 0 || code >= NUM_TOKENS) return "UNKNOWN_T";
	return tokenNames[code];
}



/* startScanner: just initialize and verify buffer has content */
de_int startScanner(BufferPointer psc_buf) {
	for (de_int i = 0; i < NUM_TOKENS; i++)
		scData.scanHistogram[i] = 0;

	if (!psc_buf || readerGetPosWrte(psc_buf) <= 0) {
		printf("Decryptor: Empty program buffer - scanning canceled\n");
		return EXIT_FAILURE;
	}

	readerRecover(psc_buf);         // Reset reader position to start
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS;
}



/* Converts token code to readable string for printing statistics */
 
	






/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */


#define MAX_LEXEME_LEN 256  // keep as 256, but guard all writes carefully

Token tokenizer(void) {
	Token currentToken = { 0 };
	char lexeme[MAX_LEXEME_LEN] = { 0 };
	de_int lexLen = 0;
	de_char c;
	de_int state = 0, nextStateVal;

	// --- Skip whitespace and newlines ---
	while (1) {
		c = readerGetChar(sourceBuffer);
		if (c == SPC_CHR || c == TAB_CHR) continue;
		if (c == NWL_CHR) { line++; continue; }
		break;
	}

	// --- Handle EOF ---
	if (c == EOS_CHR || c == EOF_CHR) {
		currentToken.code = SEOF_T;
		currentToken.attribute.seofType = (c == EOS_CHR) ? SEOF_0 : SEOF_255;
		return currentToken;
	}

	// --- Handle single-character symbols ---
	switch (c) {
	case LPR_CHR: currentToken.code = LPR_T; return currentToken;
	case RPR_CHR: currentToken.code = RPR_T; return currentToken;
	case LBR_CHR: currentToken.code = LBR_T; return currentToken;
	case RBR_CHR: currentToken.code = RBR_T; return currentToken;
	case SCL_CHR: currentToken.code = EOS_T; return currentToken;
	}

	// --- Start DFA for multi-character tokens ---
	state = 0;

	while (1) {
		nextStateVal = nextState(state, c);

		if (stateType[nextStateVal] == NOFS) {
			if (lexLen < MAX_LEXEME_LEN - 1) {
				lexeme[lexLen++] = c;
			}
			else {
			//	printf("[DEBUG] Lexeme length exceeded limit (%d). Truncating.\n", MAX_LEXEME_LEN - 1);
				break;
			}
			state = nextStateVal;
			c = readerGetChar(sourceBuffer);
		}
		else {
			if (stateType[nextStateVal] == FSWR)
				readerRetract(sourceBuffer);

			if (stateType[nextStateVal] == FSNR && lexLen < MAX_LEXEME_LEN - 1)
				lexeme[lexLen++] = c;

			break;
		}
	}

	// Null-terminate lexeme safely
	lexeme[lexLen] = '\0';
	//printf("[DEBUG] Final state=%d, lexeme='%s'\n", nextStateVal, lexeme);

	// --- Call final state function safely ---
	if (finalStateTable[nextStateVal] != NULL) {
		currentToken = (*finalStateTable[nextStateVal])(lexeme);
	}
	else {
		//printf("[DEBUG] No function for final state %d, lexeme='%s'\n", nextStateVal, lexeme);
		currentToken.code = ERR_T;
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 1);
		currentToken.attribute.errLexeme[ERR_LEN - 1] = '\0';
	}

	//printf("[DEBUG] Returning token: %s, lexeme='%s'\n", tokenStrTable[currentToken.code], lexeme);
	return currentToken;
}




/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

de_int nextState(de_int state, de_char c) {
	de_int col;
	de_int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

void printToken(Token token) {
	printf("%-10s", tokenStrTable[token.code]);
	switch (token.code) {
	case KW_T:
	case MNID_T:
	case ID_T:
		printf("\t\t%s\n", token.attribute.idLexeme);
		break;
	case INL_T:
		printf("\t\t%d\n", (int)token.attribute.intValue);
		break;
	case FLT_T:
		printf("\t\t%.2f\n", token.attribute.floatValue);
		break;
	case STR_T:
		printf("\t\t%s\n", token.attribute.idLexeme);
		break;
	case SEOF_T:
		printf("\t0\n");
		break;
	default:
		printf("\n");
	}
}



/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for ID */
// --- Keyword check helper ---
de_int isKeyword(const char* lexeme) {
	static const char* keywords[] = {
		"if", "else", "while", "for", "int", "float", "return", "print"
	};
	size_t n = sizeof(keywords) / sizeof(keywords[0]);
	for (size_t i = 0; i < n; ++i)
		if (strcmp(lexeme, keywords[i]) == 0)
			return 1;
	return 0;
}


// --- ID (identifiers or keywords) ---
Token funcID(de_strg lexeme) {
	Token t = { 0 };
	//printf("[DEBUG] funcID() called with '%s'\n", lexeme);

	if (strcmp(lexeme, "main") == 0) {
		t.code = MNID_T;
	}
	else if (isKeyword(lexeme)) {
		t.code = KW_T;
	}
	else {
		t.code = ID_T;
	}

	// Copy safely into the attribute
	strncpy(t.attribute.idLexeme, lexeme, VID_LEN);
	t.attribute.idLexeme[VID_LEN] = '\0';

	return t;
}

// --- Integer literal ---
Token funcIL(de_strg lexeme) {
	Token t = { 0 };
	//printf("[DEBUG] funcIL() called with '%s'\n", lexeme);

	t.code = INL_T;
	t.attribute.intValue = atoi(lexeme);
	return t;
}

// --- Float literal ---
Token funcFLT(de_strg lexeme) {
	Token t = { 0 };
	//printf("[DEBUG] funcFLT() called with '%s'\n", lexeme);

	t.code = FLT_T;
	t.attribute.floatValue = (float)atof(lexeme);
	return t;
}

// --- Machine identifier ---
Token funcMNID(de_strg lexeme) {
	Token t = { 0 };
	//printf("[DEBUG] funcMNID() called with '%s'\n", lexeme);

	t.code = MNID_T;
	strncpy(t.attribute.idLexeme, lexeme, VID_LEN);
	t.attribute.idLexeme[VID_LEN] = '\0';

	return t;
}


// --- Separator ---
Token funcSEP(de_strg lexeme) {
	Token t = { 0 };
	//printf("[DEBUG] funcSEP() called with '%s'\n", lexeme);

	t.code = SEP_T;
	strncpy(t.attribute.idLexeme, lexeme, VID_LEN);
	t.attribute.idLexeme[VID_LEN] = '\0';

	return t;
}

// --- End of statement ---
Token funcEOS(de_strg lexeme) {
	Token t = { 0 };
	//printf("[DEBUG] funcEOS() called with '%s'\n", lexeme);

	t.code = EOS_T;
	return t;
}



// funcSTR: handles string literals like "Hello world"
//Token funcSTR(de_strg lexeme) {
//	Token t = { 0 };
//	//printf("[DEBUG] funcSTR() called with '%s'\n", lexeme);
//
//	t.code = STR_T;
//
//	// Remove quotes if present
//	size_t len = strlen(lexeme);
//	if (len >= 2 && lexeme[0] == '"' && lexeme[len - 1] == '"') {
//		// Copy content without the quotes
//		size_t contentLen = len - 2;
//		if (contentLen >= MAX_LEXEME_LEN - 1)
//			contentLen = MAX_LEXEME_LEN - 1;
//
//		strncpy(t.attribute.idLexeme, lexeme + 1, contentLen);
//		t.attribute.idLexeme[contentLen] = '\0';
//	}
//	else {
//		// Copy entire lexeme safely if quotes missing
//		strncpy(t.attribute.idLexeme, lexeme, VID_LEN);
//		t.attribute.idLexeme[VID_LEN] = '\0';
//
//	}
//
//	return t;
//}



Token funcSTR(de_strg lexeme) {
	Token t = { 0 };
	t.code = STR_T;

	size_t len = strlen(lexeme);
	if (len >= 2 && lexeme[0] == '"' && lexeme[len - 1] == '"') {
		// Copy content without the quotes
		size_t contentLen = len - 2;
		if (contentLen > VID_LEN)
			contentLen = VID_LEN;

		strncpy(t.attribute.idLexeme, lexeme + 1, contentLen);
		t.attribute.idLexeme[contentLen] = '\0';
	}
	else {
		// Copy entire lexeme safely if quotes missing
		strncpy(t.attribute.idLexeme, lexeme, VID_LEN);
		t.attribute.idLexeme[VID_LEN] = '\0';
	}

	return t;
}


/* funcCMT: comment -> store preview in small id buffer or full in strLexeme if you prefer */
Token funcCMT(de_strg lexeme) {
	Token t = { 0 };
	t.code = CMT_T;
	/* store a preview in idLexeme (VID_LEN); if you want full comment, use strLexeme */
	size_t copyLen = strlen(lexeme);
	if (copyLen > VID_LEN) copyLen = VID_LEN;
	memcpy(t.attribute.idLexeme, lexeme, copyLen);
	t.attribute.idLexeme[copyLen] = '\0';
	return t;
}


/*
 ************************************************************
 * Acceptance State Function IL
 *		Function responsible to identify IL (integer literals).
 * - It is necessary respect the limit (ex: 2-byte integer in C).
 * - In the case of larger lexemes, error shoul be returned.
 * - Only first ERR_LEN characters are accepted and eventually,
 *   additional three dots (...) should be put in the output.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for IL */
// funcIL: handles integer literals

/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table
 *   (stringLiteralTable). You need to include the literals in
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for SL */


/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ***********************************************************
 */
 /* TO_DO: Adjust the function for IL */




/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
/* TO_DO: Use your column configuration */

/* Adjust the logic to return next column in TT */
/*    [A-z],[0-9],    _,    &,   \', SEOF,    #, other
	   L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */

de_int nextClass(de_char c) {
	if (isalpha(c)) return 0;               // L: letter
	if (isdigit(c)) return 1;               // D: digit
	if (c == UND_CHR) return 2;             // U: underscore
	if (c == AMP_CHR) return 3;             // AMP: &
	if (c == QUT_CHR) return 4;             // Q: quote "
	if (c == EOS_CHR || c == EOF_CHR) return 5; // SEOF
	if (c == HST_CHR) return 6;             // HASH: #
	if (strchr("+-*/=%", c)) return 7;      // OP: operators
	if (strchr(",:", c)) return 8;          // SEP: separators
	if (c == SPC_CHR || c == TAB_CHR) return 9; // WS: whitespace
	if (c == DOT_CHR) return 10;            // DOT: .
	return 11;                              // OTHER
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */

Token funcKEY(de_strg lexeme) {
	Token currentToken = { 0 };
	de_int kwindex = -1, j = 0;
	de_int len = (de_int)strlen(lexeme);
	///lexeme[len - 1] = EOS_CHR;

	for (de_int j = 0; j < KWT_SIZE; j++) {
		if (!strcmp(lexeme, &keywordTable[j][0])) {
			kwindex = j;
			break;
		}
	}

	if (kwindex != -1) {
		currentToken.code = KW_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken = funcErr(lexeme);
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(de_strg lexeme) {
	Token currentToken = { 0 };
	de_int i = 0, len = (de_int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == NWL_CHR)
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */

 /* printScannerData: prints histogram */
void printScannerData(ScannerData scDataLocal) {
	printf("Statistics:\n");
	printf("----------------------------------\n");
	for (int cont = 0; cont < NUM_TOKENS; cont++) {
		if (scDataLocal.scanHistogram[cont] > 0) {
			printf("Token[%s]=%d\n", tokenStrTable[cont], scDataLocal.scanHistogram[cont]);
		}
	}
	printf("----------------------------------\n");
}




/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/




// Example definition for parseFloatAttribute
de_real parseFloatAttribute(de_strg lexeme) {
	return (de_real)atof(lexeme); // Explicit cast to de_real
}

// Example definition for getSeparatorAttribute
de_int getSeparatorAttribute(de_strg lexeme) {
	if (strcmp(lexeme, ";") == 0) return SCL_CHR;
	if (strcmp(lexeme, ",") == 0) return COMMA_CHR;
	// Add more cases as needed
	return -1; // Unknown separator
}
