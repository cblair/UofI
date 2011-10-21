%{
///////////////////////////////////////////////////////////////////////////////
//Class:        CS 445
//Semester:     Fall 2011
//Assignment:   Homework 2
//Author:       Colby Blair
//File name:    ecma.y
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <typeinfo>

//parse tree insert - just a debugger for now
#define _PTI(MSG) printf("%s\n", MSG);

extern int yylex();
extern int lineno;
extern int colno;
extern char yytext[];

//debugging
#ifdef YYDEBUG
extern int yydebug = 1;
#endif

// better error reporting
#define YYERROR_VERBOSE

// bison requires that you supply this function
int yyerror(const char *msg)
{
        printf("ERROR(PARSER): %s at line %d:%d. token: %s\n", 	
							msg, lineno,
							colno, yytext);
        return(1);
}

%}

%union {
        double dval;
        int varindex;
        int ival;
        char *sval;
}

%token EXIT
%token ALPHA_CHARACTER APOSTREPHE AS AMPER AMPER2 AMPEREQ ARRAY BOOLEAN BSLASH BREAK CAROT CAROTEQ CASE CATCH CLASS CONST CONTINUE CONTROL_ESCAPE_CHAR DEFAULT DELETE DIGIT DIVIDE DIVIDEEQ DO DOLLAR DOT ELSE EQ2 EQ3 EXCLAMATION EXPONENT_INDICATOR EXTENDS FALSE FINALLY FOR FUNCTION GT GT2 GT3 GTEQ IF IMPLEMENTS IMPORT IN INSTANCEOF INT INTERFACE INTERNAL IS LBRACE LOWER_ALPHA_CHAR LOWER_B LOWER_C LOWER_F LOWER_N LOWER_R LOWER_T LOWER_V LOWER_X LOWER_U LPAREN LSHIFT LSHIFTEQ LTEQ MINUS MINUSEQ NEW NON_DIGIT NOTEQ NOTEQ2 NULL_LIT NUMBER OBJECT PACKAGE PERCENT PERCENTEQ PLUS PLUS2 PLUSEQ PRIVATE PROTECTED PUBLIC QUOTE RBRACE RETURN RPAREN RSHIFT RSHIFTEQ SLASH STAR STRING SUPER SWITCH TIMESEQ TILDE THIS THROW TO TRUE TRY TYPEOF UINT UPPER_ALPHA_CHAR UNDERSCORE USE VAR VOID WHILE WITH ZERO
%token EACH GET SET NAMESPACE INCLUDE DYNAMIC FINAL NATIVE OVERRIDE PROTOTYPE STATIC 
%token PERIOD COMMA SEMI COLON LC RC LP RP LBRACK RBRACK EQ LT GT3EQ LTE GTE DLT DGT SUM DIF DIV MUL POW INCREMENT DECREMENT AMP DAMP PIPE PIPE2 OR AT QUESTION PIPEEQ
%token IDENT INTEGER SOURCE_CHAR HEX_PREFIX

%token ID INT_LIT NAME PUNC REAL_LIT STRING_LIT

%token HEX_PREFIX

%token NBSP SHORT USP ABSTRACT SP CHAR FF BYTE TAB TRANSIENT UNICODE_NONSPACING_MARK THROWS VT UNICODE_LETTER VOLATILE LONG SYNCHRONIZED UNICODE_COMBINING_MARK UNICODE_CONNECTOR_PUNCTUATION UNICODE_DIGIT UPPER_B EXPORT ENUM DOUBLE NOTEQQ DEBUGGER NOTEQQ2 HEXDIGIT ASTERISK GOTO NON_ZERO_DIGIT FLOAT PS LF LS CR APOSTROPHE MINUS2

%type <sval> THIS

%%

program:
	sourceElements EXIT		{return(0);}
	;


sourceCharacter:
	/* any unicode character */
	/* see section 6 */
	SOURCE_CHAR
	;

inputElementDiv:
	whiteSpace
	| lineTerminator
	| comment
	| token
	| divPunctuator
	;

inputElementRegExp:
	whiteSpace
	| lineTerminator
	| comment
	| token
	| regularExpressionLiteral
	;

whiteSpace:
	TAB
	| VT
	| FF
	| SP
	| NBSP
	| USP
	;

lineTerminator:
	| LF
	| CR
	| LS
	| PS
	;

comment:
	multiLineComment
	| singleLineComment
	;

multiLineComment:
	DIVIDE STAR multiLineCommentChars STAR DIVIDE
	;

multiLineCommentChars:
	multiLineNotAsterikChar multiLineCommentChars
	| ASTERISK postAsterikCommentChars
	;

postAsterikCommentChars:
	multiLineNotForwardSlashOrAsterikChar multiLineCommentChars
	| ASTERISK postAsterikCommentChars
	;

multiLineNotAsterikChar:
	sourceCharacter /* but not ASTERISK */
	;

multiLineNotForwardSlashOrAsterikChar:
	sourceCharacter /* but not FORWARD-SLASH or ASTERISK */
	;

singleLineComment:
	'//' singleLineCommentChars
	;	

singleLineCommentChars:
	singleLineCommentChar singleLineCommentChars
	;

singleLineCommentChar:
	sourceCharacter 
	;

token:
	reservedWord
	| identifier
	| punctuator
	;

reservedWord:
	keyword
	| futureReservedWord
	| nullLiteral
	| booleanLiteral
	;

keyword:
	BREAK | CASE | CATCH | CONTINUE | DEFAULT | DELETE | DO
	| ELSE | FINALLY | FOR | FUNCTION | IF | IN | INSTANCEOF
	| NEW | RETURN | SWITCH | THIS | THROW | TRY | TYPEOF
	| VAR | VOID | WHILE | WITH
	;

futureReservedWord:
	ABSTRACT | BOOLEAN | BYTE | CHAR | CLASS | CONST | DEBUGGER | DOUBLE
	| ENUM | EXPORT | EXTENDS | FINAL | FLOAT | GOTO | IMPLEMENTS | IMPORT
	| INT | INTERFACE | LONG | NATIVE | PACKAGE | PRIVATE | PROTECTED | PUBLIC
	| SHORT | STATIC | SUPER | SYNCHRONIZED | THROWS | TRANSIENT | VOLATILE
	;

type:
	/*made by Colby */
	BOOLEAN|STRING|OBJECT|INT|UINT|VOID|STAR|userDefinedType
	;

userDefinedType:
	identifier
	;

identifierBody:
	/*made by Colby */
	ID
	| ID COLON type
	| identifier DOT identifier
	;

identifier:
	/*changed by Colby */
	/*identifierName /* but not reservedWord */
	identifierBody
	;

identifierName:
	identifierStart
	| identifierName identifierPart
	;

identifierStart:
	unicodeLetter
	| DOLLAR
	| UNDERSCORE
	| unicodeEscapeSequence
	;

identifierPart:
	/* syntactic predicate to remove non-determinism upon alts 1 and 5 
	 * - always choose the identifierStart when possible
	 */
	unicodeCombiningMark
	| unicodeDigit
	| unicodeConnectorPunctuation
	| unicodeEscapeSequence
	;

unicodeLetter:
	/* any character in the unicode categories:
	   "uppercase letter (Lu)",
	   "lowercase letter (Li)",
	   "titlecase letter (Lt)",
	   "modifier letter (Lm)",
	   "other letter (lo)",
	   "letter number (NI)" 

	UNICODE_LETTER
	;
	*/
	ALPHA_CHARACTER
	;

unicodeCombiningMark:
	/* any character in the unicode categories:
		"non-spacing mark (Mn)"
		"combining spacing mark (Mc)"
	*/
	UNICODE_NONSPACING_MARK
	| UNICODE_COMBINING_MARK
	;

unicodeDigit:
	/* any character in the unicode category "decimal number (Nd)" */
	UNICODE_DIGIT
	;

unicodeConnectorPunctuation:
	/* any character in the unicode category "connector punctuation (Pc)" */
	UNICODE_CONNECTOR_PUNCTUATION
	;

unicodeEscapeSequence:
	LOWER_U hexDigit hexDigit hexDigit hexDigit
	;

hexDigit:
	/* explicitly enumerated in grammar */
	HEXDIGIT
	;

punctuator:
	LBRACE | RBRACE | LPAREN | RPAREN | LBRACK | RBRACK
	| DOT | SEMI | APOSTROPHE | LT | GT | LTEQ
	| GTEQ | EQ2 | NOTEQ | EQ3 | NOTEQ2
	| PLUS | MINUS | STAR | PERCENT | PLUS2 | MINUS2
	| LSHIFT | RSHIFT | GT3 | AMPER | PIPE | CAROT
	| EXCLAMATION | TILDE | AMPER2 | PIPE2 | QUESTION | COLON
	| EQ | PLUSEQ | MINUSEQ | TIMESEQ | PERCENTEQ | LSHIFTEQ
	| RSHIFTEQ | GT3EQ | AMPEREQ | PIPEEQ | CAROTEQ
	;

divPunctuator:
	DIVIDE
	| DIVIDEEQ
	;

literal:
	nullLiteral
	| booleanLiteral
	| numericLiteral
	| stringLiteral
	;

nullLiteral:
	NULL_LIT
	;

booleanLiteral:
	TRUE
	| FALSE
	;


numericLiteral:
	integerLiteral
	/*| decimalLiteral*/
	| hexIntegerLiteral
	;

decimalLiteral:
	decimalIntegerLiteral decimalIntegerLiteralTail
	| DOT decimalDigits 
	| DOT decimalDigits exponentPart
	;

decimalIntegerLiteralTail:
	/* this is necessary because of the way the grammar gets parsed */
	DOT 
	| DOT decimalDigits
	| DOT exponentPart
	| DOT decimalDigits exponentPart
	| exponentPart
	;

decimalIntegerLiteral:
	ZERO
	| NON_ZERO_DIGIT 
	| NON_ZERO_DIGIT decimalDigits
	;

decimalDigits:
	decimalDigit
	| decimalDigits decimalDigit
	;

decimalDigit:
	DIGIT /* grammar has each one explicitely listed */
	;

integerLiteral:
	DIGIT
	| DIGIT integerLiteral
	;

exponentIndicator:
	EXPONENT_INDICATOR /* grammar has both e and E listed */
	;

signedInteger:
	decimalDigits
	| PLUS decimalDigits
	| MINUS decimalDigits
	;

hexIntegerLiteral:
	HEX_PREFIX hexDigit
	| hexIntegerLiteral hexDigit
	;

stringLiteral:
	/* modified by Colby, pushing out to tokenizer
	QUOTE QUOTE
	QUOTE doubleStringCharacters QUOTE
	| APOSTROPHE APOSTROPHE
	| APOSTROPHE singleStringCharacters APOSTROPHE
	*/
	STRING_LIT
	;

doubleStringCharacters:
	doubleStringCharacter 
	| doubleStringCharacter doubleStringCharacters
	;

singleStringCharacters:
	singleStringCharacter
	| singleStringCharacter singleStringCharacters
	;

doubleStringCharacter:
	sourceCharacter /* but not double quote or backslash or line terminator */
	BSLASH escapeSequence
	;

singleStringCharacter:
	sourceCharacter /* but not single quote or backslash or line terminator */
	| BSLASH escapeSequence
	;

escapeSequence:
	characterEscapeSequence
	| ZERO /* [lookahead not a member of decimalDigit] */
	| hexEscapeSequence
	| unicodeEscapeSequence
	;

characterEscapeSequence:
	singleEscapeCharacter
	| nonEscapeCharacter
	;

singleEscapeCharacter:
	APOSTROPHE | QUOTE | BSLASH | LOWER_B | LOWER_F | LOWER_N | LOWER_R | LOWER_T | LOWER_V
	;

nonEscapeCharacter:
	sourceCharacter /* but not escapeCharacter or lineTerminator */
	;

escapeCharacter:
	singleEscapeCharacter
	| decimalDigit
	| LOWER_X
	| LOWER_U
	;

hexEscapeSequence:
	LOWER_X hexDigit hexDigit
	;

/* defined above
unicodeEscapeSequence:
	LOWER_U hexDigit hexDigit hexDigit hexDigit
	;*/

regularExpressionLiteral:
	SLASH regularExpressionBody SLASH regularExpressionFlags
	;

regularExpressionBody:
	regularExpressionFirstChar regularExpressionChars
	;

regularExpressionChars:
	/* [empty] */
	/*
	 * SPEC: regularExpressionChars regularExpressionChar
	 *   ->: regularExpressionChar regularExpressionChars
	 */ 
	regularExpressionChar regularExpressionChars
	;

regularExpressionFirstChar:
	nonTerminator /* but not * or \ or / */
	| backslashSequence
	;

regularExpressionChar:
	nonTerminator /* but not * or \ or / */
	| backslashSequence
	;

backslashSequence:
	BSLASH nonTerminator
	;

nonTerminator:
	sourceCharacter /* but not lineTerminator */
	;

regularExpressionFlags:
	regularExpressionFlags identifierPart
	;

/* A.2 Number Conversions */

stringNumericLiteral:
	strWhiteSpace
	| strNumericLiteral
	| strWhiteSpace strNumericLiteral
	| strNumericLiteral strWhiteSpace
	| strWhiteSpace strNumericLiteral strWhiteSpace
	;

strWhiteSpace:
	strWhiteSpaceChar
	| strWhiteSpaceChar strWhiteSpace
	;

strWhiteSpaceChar:
	TAB | SP | NBSP | FF | VT | CR | LF | LS | PS | USP
	;

strNumericLiteral:
	strDecimalLiteral
	| hexIntegerLiteral
	;

strDecimalLiteral:
	strUnsignedDecimalLiteral
	| PLUS strUnsignedDecimalLiteral
	| MINUS strUnsignedDecimalLiteral
	;

strUnsignedDecimalLiteral:
	'Infinity'
	| decimalDigits DOT
	| decimalDigits DOT decimalDigits 
	| decimalDigits DOT exponentPart
	| decimalDigits DOT decimalDigits exponentPart
	| DOT decimalDigits
	| DOT decimalDigits exponentPart
	| decimalDigits
	| decimalDigits exponentPart
	;

exponentPart:
	exponentIndicator signedInteger
	;


/* A.3 Expressions */

primaryExpression:
	'this'
	| identifier
	| literal
	| arrayLiteral
	| objectLiteral
	| LPAREN expression RPAREN
	;

arrayLiteral:
	LBRACK RBRACK
	| LBRACK elision RBRACK
	| LBRACK elementList RBRACK
	| LBRACK elementList COMMA RBRACK
	| LBRACK elementList COMMA elision RBRACK
	;

elementList:
	assignmentExpression
	| elision assignmentExpression
	| elementList COMMA assignmentExpression
	| elementList COMMA elision assignmentExpression
	;

elementListTail:
	COMMA assignmentExpression
	| COMMA elision assignmentExpression
	;

elision:
	COMMA
	| elision COMMA
	;

objectLiteral:
	LBRACE RBRACE
	| LBRACE propertyNameAndValueList RBRACE
	;

propertyNameAndValueList:
	propertyName COLON assignmentExpression
	| propertyNameAndValueList COMMA propertyName COLON assignmentExpression
	;

propertyNameAndValueListTail:
	COMMA propertyName COLON assignmentExpression
	;

propertyName:
	identifier
	| stringLiteral
	| numericLiteral
	;

memberExpression:
	primaryExpression
	| functionExpression
	| memberExpression LBRACK expression RBRACK
	| memberExpression DOT identifier
	| NEW memberExpression arguments
	;

memberExpressionTail:
	/* SPEC: not a part of formal grammar */
	LBRACK expression RBRACK
	| DOT identifier
	;

newExpression:
	memberExpression
	| NEW newExpression
	;

callExpression:
	memberExpression arguments
	| callExpression arguments
	| callExpression LBRACK expression RBRACK
	| callExpression DOT identifier
	;

callExpressionTail:
	arguments
	| LBRACK expression RBRACK
	| DOT identifier
	;

arguments:
	LPAREN RPAREN
	| LPAREN argumentList RPAREN 
	;

argumentList:
	assignmentExpression
	| argumentList COMMA assignmentExpression
	;

argumentListTail:
	COMMA assignmentExpression
	;

leftHandSideExpression:
	/*taken out by Colby
	newExpression
	| callExpression
	*/
	/*added by Colby*/
	/*identifier*/
	newExpression
	| callExpression
	;

postfixExpression:
	leftHandSideExpression
	| leftHandSideExpression PLUS2
	| leftHandSideExpression MINUS2
	;

unaryExpression:
	/*reduce/reduce issues
	postfixExpression
	| DELETE unaryExpression
	|VOID unaryExpression*/
	TYPEOF unaryExpression
	| PLUS2 unaryExpression
	| MINUS2 unaryExpression
	| PLUS unaryExpression
	| MINUS unaryExpression
	| TILDE unaryExpression
	| EXCLAMATION unaryExpression
	;

multiplicativeExpression:
	unaryExpression
	| multiplicativeExpression ASTERISK unaryExpression
	| multiplicativeExpression DIVIDE unaryExpression
	| multiplicativeExpression PERCENT unaryExpression
	;

multiplicativeExpressionTail:
	ASTERISK unaryExpression
	| DIVIDE unaryExpression
	| PERCENT unaryExpression
	;

additiveExpression:
	multiplicativeExpression
	| additiveExpression PLUS multiplicativeExpression
	| additiveExpression MINUS multiplicativeExpression
	;

additiveExpressionTail:
	PLUS multiplicativeExpression
	| MINUS multiplicativeExpression
	;

shiftExpression:
	additiveExpression
	| shiftExpression LSHIFT additiveExpression
	| shiftExpression RSHIFT additiveExpression
	| shiftExpression GT3 additiveExpression
	;

shiftExpressionTail:
	LSHIFT additiveExpression
	| RSHIFT additiveExpression
	| GT3 additiveExpression
	;

relationalExpression:
	shiftExpression
	| relationalExpression LT shiftExpression
	| relationalExpression GT shiftExpression
	| relationalExpression LTEQ shiftExpression
	| relationalExpression GTEQ shiftExpression
	| relationalExpression INSTANCEOF shiftExpression
	| relationalExpression IN shiftExpression
	;

relationalExpressionTail:
	LT shiftExpression
	| GT shiftExpression
	| LTEQ shiftExpression
	| GTEQ shiftExpression
	| INSTANCEOF shiftExpression
	| IN shiftExpression
	;

relationalExpressionNolnBody:
	/*reduce/reduce issues*/
	LT|GT|LTEQ|GTEQ|INSTANCEOF
	;

relationalExpressionNoln:
	shiftExpression
	| relationalExpression shiftExpression
	/*| relationalExpression relationalExpressionNolnBody shiftExpression*/
	;

equalityExpression:
	relationalExpression
	| relationalExpression equalityExpressionTail
	;

equalityExpressionTailPrefix:
	EQ2|NOTEQ|EQ3|NOTEQ2
	;

equalityExpressionTail:
	equalityExpressionTailPrefix relationalExpression
	;

equalityExpressionNolnBody:
	EQ2|NOTEQQ|EQ3|NOTEQQ2 
	;

equalityExpressionNoln:
	relationalExpressionNoln
	| equalityExpressionNoln equalityExpressionNolnBody relationalExpressionNoln
	;

equalityExpressionNolnTail:
	equalityExpressionTailPrefix relationalExpressionNoln
	;

bitwiseAndExpression:
	equalityExpression
	| bitwiseAndExpression AMPER equalityExpression
	;

bitwiseAndExpressionTail:
	AMPER equalityExpression
	;

bitwiseAndExpressionNoln:
	equalityExpressionNoln
	| bitwiseAndExpressionNoln AMPER equalityExpressionNoln
	;

bitwiseAndExpressionNolnTail:
	AMPER equalityExpressionNoln
	;

bitwiseXorExpression:
	bitwiseAndExpression
	| bitwiseXorExpression CAROT bitwiseAndExpression
	;

bitwiseXorExpressionTail:
	CAROT bitwiseAndExpression
	;

bitwiseXorExpressionNoln:
	bitwiseAndExpressionNoln
	| bitwiseXorExpressionNoln CAROT bitwiseAndExpressionNoln
	;

bitwiseXorExpressionNolnTail:
	CAROT bitwiseAndExpressionNoln
	;

bitwiseOrExpression:
	bitwiseXorExpression
	| bitwiseOrExpression PIPE bitwiseXorExpression
	;

bitwiseOrExpressionTail:
	PIPE bitwiseXorExpression
	;

bitwiseOrExpressionNoln:
	bitwiseXorExpressionNoln
	| bitwiseOrExpressionNoln PIPE bitwiseXorExpressionNoln
	;

bitwiseOrExpressionNolnTail:
	PIPE bitwiseOrExpressionNolnTail
	;

logicalAndExpression:
	bitwiseOrExpression
	| logicalAndExpressionNoln AMPER2 bitwiseOrExpression
	;

logicalAndExpressionNoln:
	bitwiseOrExpressionNoln
	| logicalAndExpressionNoln AMPER2 bitwiseOrExpressionNoln
	;

logicalAndExpressionNolnTail:
	AMPER2 bitwiseOrExpressionNoln
	;

logicalOrExpression:
	logicalAndExpression
	| logicalOrExpression PIPE2 logicalAndExpression
	;

logicalOrExpressionTail:
	PIPE2 logicalAndExpression
	;

logicalOrExpressionNoln:
	logicalAndExpressionNoln
	| logicalOrExpressionNoln PIPE2 logicalAndExpressionNoln
	;

logicalOrExpressionNolnTail:
	PIPE2 logicalAndExpressionNoln
	;

conditionalExpression:
	logicalOrExpression
	| logicalOrExpression QUESTION assignmentExpression COLON assignmentExpression
	;

conditionalExpressionNoln:
	logicalOrExpressionNoln
	| logicalOrExpressionNoln QUESTION assignmentExpressionNoln COLON assignmentExpressionNoln
	;

assignmentExpression:
	conditionalExpression
	| leftHandSideExpression assignmentOperator assignmentExpression
	;

assignmentExpressionNoln:
	conditionalExpressionNoln
	| leftHandSideExpression assignmentOperator assignmentExpressionNoln
	;

assignmentOperator:
	/* note that in the grammar these are listed out explicitely */
	EQ | TIMESEQ | DIVIDEEQ | PERCENTEQ | PLUSEQ | MINUSEQ | LSHIFTEQ | RSHIFTEQ
	| GT3EQ | AMPEREQ | CAROTEQ | PIPEEQ
	;

expression:
	/*
	assignmentExpression
	| expression COMMA assignmentExpression
	*/
	identifier
	| literal
	| identifier assignmentOperator expression;
	;

expressionTail:
	COMMA assignmentExpression
	;

expressionNoln:
	assignmentExpressionNoln
	| expressionNoln COMMA assignmentExpressionNoln
	;

expressionNolnTail:
	COMMA assignmentExpressionNoln
	;

/* A.4 Statements */

statement:
	block
	| variableStatement	
	| expressionStatement
	| functionDeclaration
	| ifStatement
	| iterationStatement
	| continueStatement
	| breakStatement
	| returnStatement
	| withStatement
	| labelledStatement
	| throwStatement
	| tryStatement
	/*classStatement - 3 shift/reduces:*/
	| classStatement
	| importStatement
	| superStatement /*TODO: hack, should be in class constructor only*/	
	| packageStatement
	/*
	| emptyStatement
	/* reduce reduce: | switchStatement*/
	;

block:
	LBRACE RBRACE
	| LBRACE statementList RBRACE
	;

statementList:
	statement
	| statementList statement
	;

variableStatement:
	/*modified by Colby */
	/*VAR variableDeclarationList SEMI*/
	variablePrefix variableDeclarationList SEMI
	;

variablePrefix:
	VAR
	| CONST
	| STATIC
	| CONST STATIC
	| STATIC CONST
	;

variableDeclarationList:
	variableDeclaration
	| variableDeclarationList COMMA variableDeclaration
	;

variableDeclarationTail:
	COMMA variableDeclaration
	;

variableDeclarationListNoln:
	variableDeclarationNoln
	| variableDeclarationListNoln COMMA variableDeclarationNoln
	;

variableDeclarationListNolnTail:
	COMMA variableDeclarationNoln
	;

variableDeclaration:
	identifier
	| assignmentExpression
	;

variableDeclarationNoln:
	identifier
	| identifier initialiserNoln
	;

initialiserNoln:
	EQ assignmentExpressionNoln
	;

expressionStatement:
	/* [lookahead not a member of {{, function}} */
	expression SEMI
	;

ifStatement:
	IF LPAREN expression RPAREN statement ELSE statement
	| IF LPAREN expression RPAREN statement
	;

iterationStatementForBody:
		/*(expressionNoln)? SEMI (expression)? SEMI (expression)?*/
		SEMI SEMI
		| SEMI SEMI expression
		| SEMI expression SEMI
		| SEMI expression SEMI expression
		| expressionNoln SEMI SEMI
		| expressionNoln SEMI SEMI expression
		| expressionNoln SEMI expression SEMI
		| expressionNoln SEMI expression SEMI expression
		/*| 'var' variableDeclarationListNoln SEMI (expression)? SEMI (expression)?*/
		| VAR variableDeclarationListNoln SEMI SEMI
		| VAR variableDeclarationListNoln SEMI SEMI expression
		| VAR variableDeclarationListNoln SEMI expression SEMI
		| VAR variableDeclarationListNoln SEMI expression SEMI expression
		| leftHandSideExpression IN expression
		| VAR variableDeclarationNoln IN expression
	;


iterationStatement:
	DO statement WHILE LPAREN expression RPAREN SEMI
	| WHILE LPAREN expression RPAREN statement
	| FOR LPAREN iterationStatementForBody RPAREN statement
	;

continueStatement:
	CONTINUE SEMI
	| CONTINUE identifier SEMI
	;

breakStatement:
	BREAK SEMI
	| BREAK identifier SEMI
	;

returnStatement:
	RETURN SEMI
	| RETURN expression SEMI
	;

withStatement:
	WITH LPAREN expression RPAREN statement
	;

switchStatement:
	SWITCH LPAREN expression RPAREN caseBlock
	;

caseBlock:
	LBRACE RBRACE
	| LBRACE caseClauses RBRACE
	| LBRACE defaultClause RBRACE
	| LBRACE defaultClause caseClauses RBRACE
	| LBRACE caseClauses defaultClause RBRACE
	| LBRACE caseClauses defaultClause caseClauses RBRACE
	;

caseClauses:
	caseClause
	| caseClauses caseClause
	;

caseClause:
	CASE expression COLON
	CASE expression COLON statementList
	;

defaultClause:
	DEFAULT COLON
	| DEFAULT COLON statementList
	;

labelledStatement:
	identifier COLON statement
	;

throwStatement:
	THROW expression SEMI
	;

tryStatement:
	TRY block catch_
	| TRY block finally_
	| TRY block catch_ finally_
	;

packageStatement:
	/*made by Colby */
	PACKAGE identifier packageBody
	;

packageBody:
	LBRACE RBRACE
	| LBRACE packageList RBRACE
	;

packageList:
	packageListElement
	| packageListElement packageList

packageListElement:
	importStatement
	| classStatement
	| PUBLIC classStatement
	/*
	| classVariableOrMethodList 
	*/
	;

classStatement:
	CLASS identifier classBody
	| CLASS identifier EXTENDS identifier classBody
	;

classVariableOrMethod:
	variableStatement
	/*| functionDeclaration*/
	| memberExpression
	| PUBLIC classVariableOrMethod
	| PRIVATE classVariableOrMethod
	;

classVariableOrMethodList:
	classVariableOrMethod
	| classVariableOrMethod classVariableOrMethodList
	;	

classBody:
	LBRACE RBRACE
	| LBRACE classVariableOrMethodList RBRACE
	;

importStatement:
	IMPORT identifier SEMI
	;

superStatement:
	SUPER LPAREN RPAREN SEMI
	| SUPER LPAREN formalParameterList RPAREN SEMI
	;

catch_:
	CATCH LPAREN identifier RPAREN block
	;

finally_:
	FINALLY block
	;

/* A.5 Functions and Programs */

functionDeclaration:
	/*'function' identifier LPAREN (formalParameterList)? LBRACE functionBody RBRACE*/
	FUNCTION identifier functionHead functionBody
	| FUNCTION identifier functionHead COLON identifier functionBody
	;

functionExpression:
	/*'function' (identifier)? LPAREN (formalParameterList)? LBRACE functionBody RBRACE*/
	functionDeclaration
	| FUNCTION functionHead functionBody
	;

functionHead:
	LPAREN RPAREN
	| LPAREN formalParameterList RPAREN
	;

functionBody:
	LBRACE RBRACE
	| LBRACE sourceElements RBRACE
	;

formalParameterList:
	identifier
	| formalParameterList COMMA identifier
	;

formalParameterListTail:
	COMMA identifier
	;

sourceElements:
	sourceElement
	| sourceElements sourceElement
	;

sourceElement:
	statement {/* moving to statement/*| functionDeclaration*/}
	;

/* A.6 URI character classes */

uri:
	/*(uriCharacters)?*/
	uriCharacters
	;

uriCharacters:
	uriCharacter
	| uriCharacter uriCharacters
	;

uriCharacter:
	uriReserved
	| uriUnescaped
	| uriEscaped
	;

uriReserved:
	SEMI | SLASH | QUESTION | COLON | AT | AMPER | EQ | PLUS | DOLLAR | COMMA
	;

uriUnescaped:
	uriAlpha
	| decimalDigit
	| uriMark
	;

uriEscaped:
	PERCENT hexDigit hexDigit
	;

uriAlpha:
	ALPHA_CHARACTER /* consists of a-zA-Z */
	;

uriMark:
	MINUS | UNDERSCORE | DOT | EXCLAMATION | TILDE | ASTERISK | APOSTROPHE | LPAREN | RPAREN
	;

/* A.7 Regular Exrpessions */

patter:
	disjunction
	;

disjunction:
	alternative
	| alternative PIPE disjunction
	;

alternative:
	term
	;

term:
	assertion
	| atom
	| atom quantifier
	;

assertion:
	CAROT
	| DOLLAR
	| LOWER_B /* double check this - looks like a space in the manual? */
	| UPPER_B /* double check this - looks like a space in the manual? */
	;

quantifier:
	quantifierPrefix
	| quantifierPrefix QUESTION
	;

quantifierPrefix:
	ASTERISK
	| PLUS
	| QUESTION
	| LBRACE decimalDigits RBRACE
	| LBRACE decimalDigits COMMA RBRACE
	| LBRACE decimalDigits COMMA decimalDigits RBRACE
	;

atomBodyPrefix:
	COLON|EQ|EXCLAMATION
	;

atomBody:
	disjunction
	| atomBodyPrefix disjunction
	;

atom:
	patternCharacter
	| DOT
	| BSLASH atomEscape
	| characterClass
	| LPAREN atomBody RPAREN
	;

patternCharacter:
	sourceCharacter 
	;

atomEscape:
	decimalEscape
	| characterEscape
	| characterClassEscape
	;

characterEscape:
	controlEscape
	| LOWER_C controlLetter
	| hexEscapeSequence
	| unicodeEscapeSequence
	| identityEscape
	;

controlEscape:
	CONTROL_ESCAPE_CHAR /* one of: fnrtv */
	;

controlLetter:
	/* one of: a-z A-Z */
	LOWER_ALPHA_CHAR
	| UPPER_ALPHA_CHAR
	;

identityEscape:
	sourceCharacter /* but not identifierPart */
	;

characterClassEscape:
	'd' | 'D' | 's' | 'S' | 'w' | 'W'
	;

decimalEscape:
	decimalIntegerLiteral /* lookahead not a member of decimalDigit */
	;

characterClass:
	/* [ [lookahead not a member of {^}] ClassRanges ] 
	| [ ^ classRanges ]
	*/
	;
	
classRanges:
	/* empty */
	| nonemptyClassRanges
	;

nonemptyClassRanges:
	classAtom
	| classAtom nonemptyClassRangesNoDash
	| classAtom MINUS classAtom classRanges
	;

nonemptyClassRangesNoDash:
	classAtom
	| classAtomNoDash nonemptyClassRangesNoDash
	| classAtomNoDash MINUS classAtom classRanges
	;

classAtom:
	MINUS
	| classAtomNoDash
	;

classAtomNoDash:
	sourceCharacter /* but not one of: \ ] - */
	| BSLASH classEscape
	;

classEscape:
	decimalEscape
	| LOWER_B
	| characterEscape
	| characterClassEscape
	;


%%
