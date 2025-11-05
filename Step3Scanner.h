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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"

#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */
//#define KWT_SIZE 10
#define MODE_ADDIT 0
//#define MODE_MULTI 2


#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 17

/* TO_DO: Define Token codes - Create your token classes */


enum TOKENS {
	ERR_T, MNID_T, INL_T, STR_T, LPR_T, RPR_T, LBR_T, RBR_T,
	KW_T, EOS_T, RTE_T, SEOF_T, CMT_T, OPR_T, ID_T, FLT_T, SEP_T,
	VAR_T, DOT_T // Add these if required
};

static const char* tokenStrTable[NUM_TOKENS] = {

	"ERR_T", "MNID_T", "INL_T", "STR_T", "LPR_T", "RPR_T", "LBR_T", "RBR_T",
	"KW_T", "EOS_T", "RTE_T", "SEOF_T", "CMT_T", "OPR_T", "ID_T", "FLT_T", "SEP_T"
};


/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD, OP_INC, OP_DEC } AriOperator; //Arithmetic operators: +, -, *, /,%,++,--
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT, OP_GE, OP_LE } RelOperator; // Relational operators : == , != , > , <, >= , <=
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator; //Logical operators: &&, ||, !
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;  //End-of-file markers: SEOF_0, SEOF_255

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	de_int codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	de_int intValue;				/* integer literal attribute (value) */
	de_int keywordIndex;			/* keyword index in the keyword table */
	de_int contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	de_real floatValue;				/* floating-point literal attribute (value) */
	de_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	de_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	de_byte flags;			/* Flags information */
	union {
		de_int intValue;				/* Integer value */
		de_real floatValue;			/* Float value */
		de_strg stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
//typedef struct Token {
//	de_int code;				/* token code */
//	TokenAttribute attribute;	/* token attribute */
//	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
//} Token;

typedef struct Token {
	int code;
	TokenAttribute attribute; // union with idLexeme, intValue, floatValue, etc.
} Token;

/* Scanner */
typedef struct ScannerData {
	int scanHistogram[NUM_TOKENS];
	Token* tokenStream;
	int tokenCount;
	// Add other fields as needed
} ScannerData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* TO_DO: Define lexeme FIXED classes */
/* EOF definitions */
#define EOS_CHR '\0'	// CH00
#define EOF_CHR 0xFF	// CH01
#define UND_CHR '_'		// CH02
#define AMP_CHR '&'		// CH03
#define QUT_CHR '\''	// CH04
#define HST_CHR '#'		// CH05
#define TAB_CHR '\t'	// CH06
#define SPC_CHR ' '		// CH07
#define NWL_CHR '\n'	// CH08
#define SCL_CHR ';'		// CH09
#define LPR_CHR '('		// CH10
#define RPR_CHR ')'		// CH11
#define LBR_CHR '{'		// CH12
#define RBR_CHR '}'		// CH13
#define DIG0_CHR '0'  // CH14
#define DIG1_CHR '1'  // CH15
#define DIG2_CHR '2'  // CH16
#define DIG3_CHR '3'  // CH17
#define DIG4_CHR '4'  // CH18
#define DIG5_CHR '5'  // CH19
#define DIG6_CHR '6'  // CH20
#define DIG7_CHR '7'  // CH21
#define DIG8_CHR '8'  // CH22
#define DIG9_CHR '9'  // CH23
#define A_CHR 'A'     // CH24
#define Z_CHR 'Z'     // CH25
#define a_CHR 'a'     // CH26
#define z_CHR 'z'     // CH27
#define ADD_CHR '+'   // CH28
#define SUB_CHR '-'   // CH29
#define MUL_CHR '*'   // CH30
#define DIV_CHR '/'   // CH31
#define MOD_CHR '%'   // CH32
#define EQ_CHR '='    // CH33
#define NE_CHR '!'    // CH34

#define OR_CHR '|'    // CH36
#define XOR_CHR '^'   // CH37
#define NOT_CHR '~'   // CH38
#define LT_CHR '<'    // CH39
#define GT_CHR '>'    // CH40
#define COMMA_CHR ','     // CH41
#define DOT_CHR '.'       // CH42
#define COLON_CHR ':'     // CH43
#define DQUOTE_CHR '\"'   // CH44
#define BSLASH_CHR '\\'   // CH45

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */


/* TO_DO: Error states and illegal state */
#define ESNR	8		/* Error state with no retract */
#define ESWR	9		/* Error state with retract */
#define FS		10		/* Illegal state */
// Additional error states
#define ESCINV       11  // Invalid character
#define ESLEXLONG    12  // Lexeme too long
#define ESSTRUNTERM  13  // Unterminated string literal
#define ESNUMFMT     14  // Invalid number format
#define ESUNEXP      15  // Unexpected symbol
#define ESUNRECOG    16  // Unrecognized token

 /* TO_DO: State transition table definition */
#define NUM_STATES 13  
#define CHAR_CLASSES 12

// Transition table: rows = states, columns = char classes

/* TO_DO: Transition table - type of states defined in separate table */

/* Character Classes:
   L(0), D(1), U(2), AMP(3), Q(4), SEOF(5), HASH(6), OP(7), SEP(8), WS(9), DOT(10), OTHER(11)
*/

static de_int transitionTable[NUM_STATES][CHAR_CLASSES] = {
	/*     L   D   U  AMP   Q  SEOF HASH  OP  SEP  WS  DOT  OTHER */
	/* S0 */ {  1,  2,  1,    3,  4, ESWR,   6,  7,   8,   0, 10, ESNR },
	/* S1 */ {  1,  1,  1,   12, 12,   12,  12, 12,  12,  12, 12,  12 }, // ID_T or KW_T
	/* S2 */ { ESWR, 2, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, 10, ESWR }, // INL_T or FLT_T
	/* S3 */ {  1,  1,  1,  ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR }, // MNID_T
	/* S4 */ {  4,  4,  4,    4,  5, ESWR,   4,   4,   4,   4,   4,   4 }, // STR_T start
	/* S5 */ { FS, FS, FS,   FS, FS, FS,   FS,  FS,  FS,  FS,  FS,  FS }, // STR_T accept
	/* S6 */ {  6,  6,  6,    6,  6, ESWR,  7,   6,   6,   6,   6,   6 }, // CMT_T start
	/* S7 */ { FS, FS, FS,   FS, FS, FS,  FS,  FS,  FS,  FS,  FS,  FS }, // CMT_T accept
	/* S8 */ { FS, FS, FS,   FS, FS, FS,  FS,  FS,  FS,  FS,  FS,  FS }, // SEP_T accept
	/* S9 */ { FS, FS, FS,   FS, FS, FS,  FS,  FS,  FS,  FS,  FS,  FS }, // EOS_T accept
	/* S10*/ { 11, 11, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR }, // FLT_T after DOT
	/* S11*/ { 11, 11, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR }, // FLT_T accept
	/* S12*/ { FS, FS, FS,   FS, FS, FS,  FS,  FS,  FS,  FS,  FS,  FS }  // ID_T accept
};

/* Define accepting states types */
#define EOS_CHR '\0'
#define EOF_CHR 0xFF
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */


/* TO_DO: Define list of acceptable states */
static de_int stateType[NUM_STATES] = {
	NOFS,  // S0
	NOFS,  // S1 ? not accepting
	FSWR,  // S2
	FSWR,  // S3
	NOFS,  // S4
	FSNR,  // S5
	NOFS,  // S6
	FSNR,  // S7
	FSNR,  // S8
	FSNR,  // S9
	NOFS,  // S10
	FSWR,  // S11
	FSWR   // S12 ? accepting for identifiers
};
/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
void printToken(Token t);
de_real parseFloatAttribute(de_strg lexeme);
de_int getSeparatorAttribute(de_strg lexeme);
de_int startScanner(BufferPointer psc_buf); /* Starts the scanner and returns a status code or result */
static de_int nextClass(de_char c); /* Maps a character to its character class index */
static de_int nextState(de_int currentState, de_char c); /* Computes the next state from the current state and input character */
de_void printScannerData(ScannerData scData);/* Prints scanner statistics (e.g., token histogram) */
Token tokenizer(de_void); /* Main tokenizer function that returns the next token */


/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/


/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(de_strg lexeme);

/* Declare accepting states functions */

//Token funcSL(de_strg lexeme);
Token funcIL(de_strg lexeme);
Token funcID(de_strg lexeme);
Token funcCMT(de_strg lexeme);
Token funcKEY(de_strg lexeme);
Token funcErr(de_strg lexeme);
Token funcEOS(de_strg lexeme);
Token funcFLT(de_strg lexeme);
Token funcMNID(de_strg lexeme);
Token funcSEP(de_strg lexeme);
Token funcSTR(de_strg lexeme);



/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 11

/* TO_DO: Define the list of keywords */
static de_strg keywordTable[KWT_SIZE] = {
	"data",		/* KW00 */
	"code",		/* KW01 */
	"int",		/* KW02 */
	"real",		/* KW03 */
	"string",	/* KW04 */
	"if",		/* KW05 */
	"then",		/* KW06 */
	"else",		/* KW07 */
	"while",	/* KW08 */
	"do",		/* KW09 */
	"return"	/* KW10 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT TAB_CHR  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	de_char indentationCharType;
	de_int indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;


/* finalStateTable should be defined in ONE .c file, so declare extern here */
extern PTR_ACCFUN finalStateTable[NUM_STATES];
/* Number of errors */
extern de_int numScannerErrors;

/* Scanner data */
extern ScannerData scData;


#endif
