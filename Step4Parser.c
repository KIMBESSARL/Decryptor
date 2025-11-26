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
************************************************************
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

#ifndef PARSER_H_
#include "Step4Parser.h"
#endif

/* Parser data */
//extern ParserData psData; /* BNF statistics */
ParserData psData;      
de_int numParserErrors = 0; /* number of syntax errors reported by the parser */



/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TO_DO: This is the function to start the parser - check your program definition */

de_void startParser() {
	/* TO_DO: Initialize Parser data */
	de_int i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
//de_void matchToken(de_int tokenCode, de_int tokenAttribute) {
//	de_int matchFlag = 1;
//	switch (lookahead.code) {
//	case KW_T:
//		if (lookahead.attribute.codeType != tokenAttribute)
//			matchFlag = 0;
//	default:
//		if (lookahead.code != tokenCode)
//			matchFlag = 0;
//	}
//	if (matchFlag && lookahead.code == SEOF_T)
//		return;
//	if (matchFlag) {
//		lookahead = tokenizer();
//		if (lookahead.code == ERR_T) {
//			printError();
//			lookahead = tokenizer();
//			syntaxErrorNumber++;
//		}
//	}
//	else
//		syncErrorHandler(tokenCode);
//}

de_void matchToken(de_int tokenCode, de_int tokenAttribute) {
	de_int matchFlag = 1;

	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
		break;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
		break;
	}

	if (matchFlag && lookahead.code == SEOF_T)
		return;

	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else {
		syncErrorHandler(tokenCode);
	}
}


/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
de_void syncErrorHandler(de_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TO_DO: This is the function to error printing - adjust basically datatypes */
de_void printError() {
	extern de_int numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {CMT_T, MNID_T (main&), SEOF_T}.
 ***********************************************************
 */
//de_void program() {
//	psData.parsHistogram[BNF_program]++;
//	switch (lookahead.code) {
//	case CMT_T:
//		comment();
//	case KW_T:
//		matchToken(KW_T, KW_int);
//	case MNID_T:
//		if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 5) == 0) {
//			matchToken(MNID_T, NO_ATTR);
//			matchToken(LPR_T, NO_ATTR);
//			optParams();
//			matchToken(RPR_T, NO_ATTR);
//			matchToken(LBR_T, NO_ATTR);
//			dataSession();
//			codeSession();
//			matchToken(RBR_T, NO_ATTR);
//			break;
//		}
//		else {
//			printError();
//		}
//	case SEOF_T:
//		; // Empty
//		break;
//	default:
//		printError();
//	}
//	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
//}

de_void program() {

	psData.parsHistogram[BNF_program]++;

	// Skip initial comments
	while (lookahead.code == CMT_T) {
		comment();
	}

	// Parse ALL facts until EOF
	while (lookahead.code == KW_T) {
		fact();
	}

	// After finishing all facts, expect EOF
	if (lookahead.code != SEOF_T) {
		printError();
	}

	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}


de_void fact()
{
	enum KEYWORDS k = lookahead.attribute.codeType;

	// Valid fact-type keywords
	if (k != KW_artist &&
		k != KW_genre &&
		k != KW_location &&
		k != KW_award) {
		printError();
		return;
	}

	matchToken(KW_T, k);
	matchToken(LPR_T, NO_ATTR);

	optVarListDeclarations();

	matchToken(RPR_T, NO_ATTR);
	matchToken(DOT_T, NO_ATTR);

	psData.parsHistogram[BNF_dataSession]++;
}
/*
 ************************************************************
 * comment
 * BNF: comment
 * FIRST(<comment>)= {CMT_T}.
 ***********************************************************
 */
de_void comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}


/*
 ************************************************************
 * optParams
 * BNF: <optParams> -> <paramList> | e
 * FIRST(<optParams>) = { e, KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
de_void optParams() {
	psData.parsHistogram[BNF_optParams]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case KW_T:
		paramList();
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional param list parsed");
}

/*
 ************************************************************
 * paramList
 * BNF: <paramList> -> <opt_varlist_declarations>
 * FIRST(<paramList>) = { KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
de_void paramList() {
	psData.parsHistogram[BNF_optParams]++;
	switch (lookahead.attribute.codeType) {
	default:
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Param list parsed");
}

/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> data { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (KW_data)}.
 ***********************************************************
 */
//de_void dataSession() {
//	psData.parsHistogram[BNF_dataSession]++;
//	switch (lookahead.code) {
//	case CMT_T:
//		comment();
//	default:
//		matchToken(KW_T, KW_data);
//		matchToken(LBR_T, NO_ATTR);
//		optVarListDeclarations();
//		matchToken(RBR_T, NO_ATTR);
//		printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
//	}
//}

de_void dataSession() {
	psData.parsHistogram[BNF_dataSession]++;

	matchToken(KW_T, KW_artist);  // KW_T with attribute KW_data
	matchToken(LPR_T, NO_ATTR);
	matchToken(VAR_T, NO_ATTR);
	matchToken(RPR_T, NO_ATTR);
	matchToken(DOT_T, NO_ATTR);
	matchToken(KW_T, KW_genre);
	matchToken(LPR_T, NO_ATTR);
	optVarListDeclarations();
	matchToken(RPR_T, NO_ATTR);
	matchToken(DOT_T, NO_ATTR);
	matchToken(KW_T, KW_location);
	matchToken(LPR_T, NO_ATTR);
	optVarListDeclarations();
	matchToken(RPR_T, NO_ATTR);
	matchToken(DOT_T, NO_ATTR);
	
	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
}




/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
//de_void optVarListDeclarations() {
//	psData.parsHistogram[BNF_optVarListDeclarations]++;
//	switch (lookahead.code) {
//	default:
//		; // Empty
//	}
//	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
//}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> code { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (KW_code)}.
 ***********************************************************
 */
//de_void codeSession() {
//	psData.parsHistogram[BNF_codeSession]++;
//	switch (lookahead.code) {
//	case CMT_T:
//		comment();
//	default:
//		matchToken(KW_T, KW_code);
//		matchToken(LBR_T, NO_ATTR);
//		optionalStatements();
//		matchToken(RBR_T, NO_ATTR);
//		printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
//	}
//}

de_void codeSession() {
	psData.parsHistogram[BNF_codeSession]++;

	//matchToken(KW_T, KW_code);  // KW_T with attribute KW_code
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);

	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}




/* TO_DO: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *				KW_T(KW_while), MNID_T(print&), MNID_T(input&) }
 ***********************************************************
 */
de_void optionalStatements() {
	psData.parsHistogram[BNF_optionalStatements]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case MNID_T:
		if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) ||
			(strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0)) {
			statements();
			break;
		}
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *		KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
de_void statements() {
	psData.parsHistogram[BNF_statements]++;
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
de_void statementsPrime() {
	psData.parsHistogram[BNF_statementsPrime]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			statements();
			break;
		}
	default:
		; //empty string
	}
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if), KW_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
de_void statement() {
	psData.parsHistogram[BNF_statement]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case KW_T:
		switch (lookahead.attribute.codeType) {
		default:
			printError();
		}
		break;
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			outputStatement();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> print& (<output statementPrime>);
 * FIRST(<output statement>) = { MNID_T(print&) }
 ***********************************************************
 */
de_void outputStatement() {
	psData.parsHistogram[BNF_outputStatement]++;
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
de_void outputVariableList() {
	psData.parsHistogram[BNF_outputVariableList]++;
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

/*
 ************************************************************
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
/*
de_void printBNFData(ParserData psData) {
}
*/
de_void printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	de_int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}







de_void rule() {
	psData.parsHistogram[BNF_rule]++;
	matchToken(KW_T, lookahead.attribute.codeType);  // match rule predicate
	matchToken(LPR_T, NO_ATTR);
	argumentList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(COLON_DASH_T, NO_ATTR);
	body();
	matchToken(DOT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Rule parsed");
}
de_void argument() {
	if (lookahead.code == KW_T) { // constants
		matchToken(KW_T, lookahead.attribute.codeType);
	}
	else if (lookahead.code == VAR_T) { // variables
		matchToken(VAR_T, NO_ATTR);
	}
	else {
		printError();
	}
}

de_void argumentList() {
	// TODO: parse a list of arguments
	psData.parsHistogram[BNF_argumentList]++;
	// Example parsing logic
	switch (lookahead.code) {
	case IVID_T:
	case FVID_T:
	case SVID_T:
		matchToken(lookahead.code, NO_ATTR);
		// optionally handle multiple arguments
		break;
	default:
		; // epsilon
	}
}

de_void body() {
	psData.parsHistogram[BNF_body]++;
	// Example: parse statements or expressions
	optionalStatements();
}

de_void optVarListDeclarations() {
	psData.parsHistogram[BNF_optVarListDeclarations]++;

	if (lookahead.code == VAR_T) {
		matchToken(VAR_T, NO_ATTR);  // first variable/constant

		// handle optional additional variables separated by commas
		while (lookahead.code == COM_T) {
			matchToken(COM_T, NO_ATTR);
			if (lookahead.code == VAR_T)
				matchToken(VAR_T, NO_ATTR);
			else
				printError(); // expecting VAR_T after comma
		}
	}
	// else: empty list is allowed
}
