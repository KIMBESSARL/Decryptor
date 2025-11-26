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
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Step2Reader.h"
#endif
#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

/* Global vars */
static Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern de_int		line;
extern Token			tokenizer(de_void);
extern de_strg		keywordTable[KWT_SIZE];
static de_int		syntaxErrorNumber = 0;

#define LANG_WRTE		"print&"
#define LANG_READ		"input&"
#define LANG_MAIN		"main&"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
//enum KEYWORDS {
//	NO_ATTR = -1,
//	KW_data,
//	KW_code,
//	KW_int,
//	KW_real,
//	KW_string,
//	KW_if,
//	KW_then,
//	KW_else,
//	KW_while,
//	KW_do,
//	KW_return
//};


enum KEYWORDS {
	NO_ATTR = -1,        // No special attribute

	/* FACT predicates */
	KW_artist = 0,
	KW_genre = 1,
	KW_location = 2,
	KW_award = 3,

	/* RULE predicates */
	KW_canadian_hiphop_artist = 4,
	KW_recommend_artist = 5,
	KW_top_artist = 6,

	/* CONSTANTS */
	KW_hiphop = 7,
	KW_grammy = 8,
	KW_juno = 9,
};




/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 23

/* Parser */
typedef struct parserData {
	de_int parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
//de_int numParserErrors;

/* Scanner data */
//ParserData psData;

extern ParserData psData;
extern de_int numParserErrors;

/* Function definitions */
de_void startParser();
de_void matchToken(de_int, de_int);
de_void matchToken(de_int expectedToken, de_int attr);

de_void syncErrorHandler(de_int);
de_void printError();
de_void printBNFData(ParserData psData);

/* List of BNF statements */

enum BNF_RULES {
	BNF_error,										/*  0: Error token */
	BNF_codeSession,								/*  1 */
	BNF_comment,									/*  2 */
	BNF_dataSession,								/*  3 */
	BNF_optVarListDeclarations,						/*  4 */
	BNF_optionalStatements,							/*  5 */
	BNF_outputStatement,							/*  6 */
	BNF_outputVariableList,							/*  7 */
	BNF_program,									/*  8 */
	BNF_statement,									/*  9 */
	BNF_statements,									/* 10 */
	BNF_statementsPrime,							/* 11 */
	BNF_optParams,									/* 12 */
	BNF_returnStatement,                  /* 0: Error token */             
	BNF_fact,                   /* 2: Single fact, e.g., artist(drake). */
	BNF_rule,                   /* 3: Single rule, e.g., canadian_hiphop_artist(X) :- ... */
	BNF_predicate,              /* 4: Predicate name, e.g., artist, genre */
	BNF_argumentList,           /* 5: Arguments of predicates, e.g., (drake), (X, hiphop) */
	BNF_argument,               /* 6: Single argument: variable or constant */
	BNF_variable,               /* 7: Variable, e.g., X */
	BNF_constant,               /* 8: Constant: drake, hiphop, toronto, grammy */
	BNF_body,                   /* 9: Body of a rule after :- */
	BNF_bodyPredicateList       /* 10: List of predicates in body separated by commas */
};


/* TO_DO: Define the list of keywords */
static de_strg BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",
	"BNF_codeSession",
	"BNF_comment",
	"BNF_dataSession",
	"BNF_optVarListDeclarations",
	"BNF_optionalStatements",
	"BNF_outputStatement",
	"BNF_outputVariableList",
	"BNF_program",
	"BNF_statement",
	"BNF_statements",
	"BNF_statementsPrime",
	"BNF_optParams",
	"BNF_returnStatement",
	"BNF_fact",
	"BNF_rule",
	"BNF_predicate",
	"BNF_argumentList",
	"BNF_argument",
	"BNF_variable",
	"BNF_constant",
	"BNF_body",
	"BNF_bodyPredicateList"
};

/* TO_DO: Place ALL non-terminal function declarations */
de_void codeSession();
de_void comment();
de_void dataSession();
de_void optVarListDeclarations();
de_void optionalStatements();
de_void outputStatement();
de_void outputVariableList();
de_void program();
de_void statement();
de_void statements();
de_void statementsPrime();
de_void optParams();
de_void paramList();
de_void argumentList();
de_void body();

de_void fact();                // Parses a single fact
de_void rule();                // Parses a single rule
//de_void predicate();           // Parses a predicate name
de_void argument();            // Parses a single argument (variable or constant)
//de_void bodyPredicateList();   // Parses multiple predicates in a rule body
//de_void match(de_int expectedToken);
#endif
