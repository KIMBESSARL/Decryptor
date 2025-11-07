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


Token(*finalStateTable[NUM_STATES])(de_strg lexeme) = {
	NULL,     // S0
	funcID,   // S1
	funcIL,   // S2
	funcMNID, // S3
	NULL,     // S4
	funcSTR,  // S5
	NULL,     // S6
	funcCMT,  // S7
	funcSEP,  // S8
	funcEOS,  // S9
	NULL,     // S10
	funcColonDash, // S11
	//funcFLT,  // S11
	funcID,   // S12
	funcColonDash, // S13
	funcCMT,       // S14
	funcColonDash  // S15
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





const char* tokenStr(de_int code) {
	if (code < 0 || code >= NUM_TOKENS) return "UNKNOWN_T";
	return tokenStrTable[code];
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

	// --- Handle comments starting with % ---
	if (c == '%') {
		lexLen = 0;
		lexeme[lexLen++] = c;
		while (1) {
			c = readerGetChar(sourceBuffer);
			if (c == NWL_CHR || c == EOS_CHR || c == EOF_CHR) break;
			if (lexLen < MAX_LEXEME_LEN - 1) lexeme[lexLen++] = c;
		}
		lexeme[lexLen] = '\0';
		currentToken.code = CMT_T;
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = '\0';
		return currentToken;
	}

	// --- Handle single-character symbols ---
	switch (c) {
	case LPR_CHR: currentToken.code = LPR_T; return currentToken;
	case RPR_CHR: currentToken.code = RPR_T; return currentToken;
	case LBR_CHR: currentToken.code = LBR_T; return currentToken;
	case RBR_CHR: currentToken.code = RBR_T; return currentToken;
	case DOT_CHR: currentToken.code = DOT_T; return currentToken;
	}

	// --- Handle multi-character tokens ---
	state = 0;
	lexLen = 0;

	while (1) {
		de_int charClassCode = charClass(c);
		nextStateVal = transitionTable[state][charClassCode];

		// Accumulate lexeme for non-accepting states
		if (stateType[nextStateVal] == NOFS) {
			if (lexLen < MAX_LEXEME_LEN - 1) lexeme[lexLen++] = c;
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

	// Null-terminate lexeme
	lexeme[lexLen] = '\0';

	// --- Handle special tokens ---
	if (strcmp(lexeme, ":-") == 0) {
		currentToken.code = COLON_DASH_T;
		strcpy(currentToken.attribute.idLexeme, ":-");
		return currentToken;
	}

	// --- Call final state function if defined ---
	if (finalStateTable[nextStateVal] != NULL) {
		currentToken = (*finalStateTable[nextStateVal])(lexeme);
	}
	else {
		currentToken.code = ERR_T;
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 1);
		currentToken.attribute.errLexeme[ERR_LEN - 1] = '\0';
	}

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
	// Print token name first
	printf("%-15s", tokenStrTable[token.code]);  // Adjust width if needed

	switch (token.code) {
		// Keywords, identifiers, variables, operators, colon-dash, comments
	case KW_T:
	case ID_T:
	case VAR_T:
	case OPR_T:
	case CMT_T:
		printf("\t%s\n", token.attribute.idLexeme);
		break;

		// Integer literals
	case INL_T:
		printf("\t%d\n", (int)token.attribute.intValue);
		break;

		// Float literals
	case FLT_T:
		printf("\t%.2f\n", token.attribute.floatValue);
		break;

		// String literals
	case STR_T:
		printf("\t%s\n", token.attribute.idLexeme);
		break;

		// Single-character symbols
	case LPR_T:
		printf("\t(\n");
		break;
	case RPR_T:
		printf("\t)\n");
		break;
	case LBR_T:
		printf("\t{\n");
		break;
	case RBR_T:
		printf("\t}\n");
		break;
	case DOT_T:
		printf("\t.\n");
		break;
	case COLON_DASH_T:
		printf("\t:-\n");
		break;

		// End of file
	case SEOF_T:
		printf("\t<EOF>\n");
		break;

		// Error tokens
	case ERR_T:
		printf("\t%s\n", token.attribute.errLexeme);
		break;

	default:
		printf("\n"); // unknown token
		break;
	}
}


 /* TO_DO: Adjust the function for ID */
// --- Keyword check helper ---
de_int isKeyword(const char* lexeme) {
	static const char* keywords[] = {
		"data", "code", "int", "real", "string", "if", "then", "else", "while", "do", "return", "mortal", "man"
	};
	size_t n = sizeof(keywords) / sizeof(keywords[0]);
	for (size_t i = 0; i < n; ++i)
		if (strcmp(lexeme, keywords[i]) == 0)
			return 1;
	return 0;
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

/* Handles identifiers, keywords, and variables */
Token funcID(de_strg lexeme) {
	Token t = { 0 };

	if (isupper((unsigned char)lexeme[0])) {
		t.code = VAR_T;    // Variable (starts with uppercase)
	}
	else {
		t.code = KW_T;     // Keyword/atom (starts with lowercase)
	}

	strncpy(t.attribute.idLexeme, lexeme, VID_LEN);
	t.attribute.idLexeme[VID_LEN] = '\0';
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



Token funcSTR(de_strg lexeme) {
	Token t = { 0 };
	t.code = STR_T;

	size_t len = strlen(lexeme);

	// Check for both single and double quotes
	if (len >= 2 && ((lexeme[0] == '"' && lexeme[len - 1] == '"') || (lexeme[0] == '\'' && lexeme[len - 1] == '\''))) {
		size_t contentLen = len - 2;  // exclude quotes
		if (contentLen >= VID_LEN) contentLen = VID_LEN - 1;

		// Store in token attribute
		strncpy(t.attribute.idLexeme, lexeme + 1, contentLen);
		t.attribute.idLexeme[contentLen] = '\0';

		// Store in string literal table
		for (size_t i = 1; i < len - 1; ++i) {
			readerAddChar(stringLiteralTable, lexeme[i]);
		}
		readerAddChar(stringLiteralTable, '\0'); // null-terminate
	}
	else {
		// No quotes — fallback
		strncpy(t.attribute.idLexeme, lexeme, VID_LEN - 1);
		t.attribute.idLexeme[VID_LEN - 1] = '\0';
		readerAddChar(stringLiteralTable, '\0'); // empty string
	}

	return t;
}



/* Handles comments starting with '%' until end of line */
Token funcCMT(de_strg lexeme) {
	Token t = { 0 };
	t.code = CMT_T;
	strncpy(t.attribute.idLexeme, lexeme, VID_LEN);
	t.attribute.idLexeme[VID_LEN] = '\0';
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
	if (c == SPC_CHR || c == TAB_CHR || c == '\n') return 9; // WS: whitespace
	if (c == DOT_CHR) return 10;            // DOT: .
	return 11;                              // OTHER
}


de_int charClass(de_char c) {
	if (isalpha(c)) {
		if (isupper(c)) return CC_VAR; // Variables start with uppercase
		return CC_L; // Lowercase letters
	}
	if (isdigit(c)) return CC_D;
	if (c == '_') return CC_U;
	if (c == '&') return CC_AMP; // Use actual ampersand character
	if (c == '"') return CC_Q;
	if (c == EOF_CHR || c == EOS_CHR) return CC_SEOF;
	if (c == '#') return CC_HASH;
	if (c == ':' || c == '-') return CC_COLONDASH; // For :- operator
	if (c == '%') return CC_COMMENT; // Prolog-style comment
	if (isOperator(c)) return CC_OP;
	if (isSeparator(c)) return CC_SEP;
	if (isspace(c)) return CC_WS;
	if (c == '.') return CC_DOT;
	if (c == '\'') return CC_SQUOTE;
	return CC_OTHER;
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


Token funcDot(de_strg lexeme) {
	Token t = { 0 };
	t.code = DOT_T;
	return t;
}

Token funcColonDash(char* lexeme) {
	Token t = { 0 };
	t.code = COLON_DASH_T;
	strcpy(t.attribute.idLexeme, ":-");
	return t;
}


Token funcVar(de_strg lexeme) {
	Token t = { 0 };
	t.code = VAR_T;
	strncpy(t.attribute.idLexeme, lexeme, VID_LEN);
	t.attribute.idLexeme[VID_LEN] = '\0';
	return t;
}


de_void printScannerData(de_void) {
	printf("Scanner configuration:\n");
	printf("States: %d\n", NUM_STATES);
	printf("Character Classes: %d\n", CHAR_CLASSES);
}
int isOperator(char c) {
	// Define what characters are considered operators
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == ':' || c == '<' || c == '>');
}

int isSeparator(char c) {
	// Define what characters are considered separators
	return (c == ',' || c == ';' || c == '(' || c == ')' || c == '[' || c == ']');
}


/* Handles '.' */
Token funcDOT(de_strg lexeme) {
	Token t = { 0 };
	t.code = DOT_T;
	strncpy(t.attribute.idLexeme, ".", VID_LEN);
	t.attribute.idLexeme[VID_LEN] = '\0';
	return t;
}

/* Stub for integer literals if referenced */
Token funcIL(de_strg lexeme) {
	Token t = { 0 };
	t.code = INL_T;
	t.attribute.intValue = atoi(lexeme);
	return t;
}