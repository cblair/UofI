/*
 * AS3.y, from AS3.g3
 *
 * Copyright (c) 2005 Martin Schnabel
 * Copyright (c) 2006-2008 David Holroyd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.

// Originally derived from the ANTLRv2 ActionScript grammar by
// Martin Schnabel, included in the ASDT project,
//   http://www.asdt.org
//   http://sourceforge.net/projects/aseclipseplugin/
 */

%{
#include <stdio.h>
#include <stdarg.h>

#include "tree.h"

extern int yylex();
extern int lineno;
extern int colno;
extern char yytext[];
extern struct tree *YY_TREE;
char *YY_PRODRULE;

//debugging
#ifdef YYDEBUG
int yydebug = 1;
#endif

// better error reporting
#define YYERROR_VERBOSE

//tree_create_node macro
#define _TCN(PRODRULE) YY_PRODRULE = PRODRULE; YY_TREE = tree_create_node

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
	struct tree *t;
}

%token <t> PACKAGE /*		:	'package';*/
%token <t> PUBLIC /*		:	'public';*/
%token <t> PRIVATE /*		:	'private';*/
%token <t> PROTECTED /*	:	'protected';*/
%token <t> INTERNAL /*	:	'internal';*/
%token <t> OVERRIDE /*	:	'override';*/
%token <t> FUNCTION /*	:	'function';*/
%token <t> EXTENDS /*		:	'extends';*/
%token <t> IMPLEMENTS /*	:	'implements';*/
%token <t> VAR /*		:	'var';*/
%token <t> STATIC /*		:	'static';*/
%token <t> IF /*		:	'if';*/
%token <t> IMPORT /*		:	'import';*/
%token <t> FOR /*		:	'for';*/
%token <t> EACH /*		:	'each';*/
%token <t> IN /*		:	'in';*/
%token <t> WHILE /*		:	'while';*/
%token <t> DO /*		:	'do';*/
%token <t> SWITCH /*		:	'switch';*/
%token <t> CASE /*		:	'case';*/
%token <t> DEFAULT /*		:	'default';*/
%token <t> ELSE /*		:	'else';*/
%token <t> CONST /*		:	'const';*/
%token <t> CLASS /*		:	'class';*/
%token <t> INTERFACE /*	:	'interface';*/
%token <t> TRUE /*		:	'true';*/
%token <t> FALSE /*		:	'false';*/
%token <t> DYNAMIC /*		:	'dynamic';*/
%token <t> USE /*		:	'use';*/
%token <t> XML /*		:	'xml';*/
%token <t> NAMESPACE /*	:	'namespace';*/
%token <t> IS /*		:	'is';*/
%token <t> AS /*		:	'as';*/
%token <t> GET /*		:	'get';*/
%token <t> SET /*		:	'set';*/
%token <t> WITH /*		:	'with';*/
%token <t> RETURN /*		:	'return';*/
%token <t> CONTINUE /*	:	'continue';*/
%token <t> BREAK /*		:	'break';*/
%token <t> NULL_VAL /*		:	'null';*/
%token <t> NEW /*		:	'new';*/
%token <t> SUPER /*		:	'super';*/
%token <t> INSTANCEOF /*	:	'instanceof';*/
%token <t> DELETE /*		:	'delete';*/
%token <t> VOID /*		:	'void';*/
%token <t> TYPEOF /*		:	'typeof';*/
%token <t> TRY /*		:	'try';*/
%token <t> CATCH /*		:	'catch';*/
%token <t> FINALLY /*		:	'finally';*/
%token <t> UNDEFINED /*	:	'undefined';*/
%token <t> THROW /*		:	'throw';*/
%token <t> FINAL /*		:	'final';*/

%token <t> QUESTION /*		:	'?'	;*/
%token <t> LPAREN /*			:	'('	;*/
%token <t> RPAREN /*			:	')'	;*/
%token <t> LBRACK /*			:	'['	;*/
%token <t> RBRACK /*			:	']'	;*/
%token <t> LCURLY /*			:	'{'	;*/
%token <t> RCURLY /*			:	'}'	;*/
%token <t> COLON /*			:	':'	;*/
%token <t> DBL_COLON /*		:	'::'	;*/
%token <t> COMMA /*			:	','	;*/
%token <t> ASSIGN /*			:	'='	;*/
%token <t> EQUAL /*			:	'=='	;*/
%token <t> STRICT_EQUAL /*		:	'==='	;*/
%token <t> LNOT /*			:	'!'	;*/
%token <t> BNOT /*			:	'~'	;*/
%token <t> NOT_EQUAL /*		:	'!='	;*/
%token <t> STRICT_NOT_EQUAL /*	:	'!=='	;*/
%token <t> DIV /*			:	'/'	;*/
%token <t> DIV_ASSIGN /*		:	'/='	;*/
%token <t> PLUS /*			:	'+'	;*/
%token <t> PLUS_ASSIGN /*		:	'+='	;*/
%token <t> INC /*			:	'++'	;*/
%token <t> MINUS /*			:	'-'	;*/
%token <t> MINUS_ASSIGN /*		:	'-='	;*/
%token <t> DEC /*			:	'--'	;*/
%token <t> STAR /*			:	'*'	;*/
%token <t> STAR_ASSIGN /*		:	'*='	;*/
%token <t> MOD /*			:	'%'	;*/
%token <t> MOD_ASSIGN /*		:	'%='	;*/
%token <t> SR /*			:	'>>'	;*/
%token <t> SR_ASSIGN /*		:	'>>='	;*/
%token <t> BSR /*			:	'>>>'	;*/
%token <t> BSR_ASSIGN /*		:	'>>>='	;*/
%token <t> GE /*			:	'>='	;*/
%token <t> GT /*			:	'>'	;*/
%token <t> SL /*			:	'<<'	;*/
%token <t> SL_ASSIGN /*		:	'<<='	;*/
%token <t> LE /*			:	'<='	;*/
%token <t> LT /*			:	'<'	;*/
%token <t> BXOR /*			:	'^'	;*/
%token <t> BXOR_ASSIGN /*		:	'^='	;*/
%token <t> BOR /*			:	'|'	;*/
%token <t> BOR_ASSIGN /*		:	'|='	;*/
%token <t> LOR /*			:	'||'	;*/
%token <t> BAND /*			:	'&'	;*/
%token <t> BAND_ASSIGN /*		:	'&='	;*/
%token <t> LAND /*			:	'&&'	;*/
%token <t> LAND_ASSIGN /*		:	'&&='	;*/
%token <t> LOR_ASSIGN /*		:	'||='	;*/
%token <t> E4X_ATTRI /*		:	'@'	; */
%token <t> SEMI /*			:	';'	;*/
%token <t> DOT /*		:	'.'	;*/
%token <t> E4X_DESC /*	:	'..'	;*/
%token <t> REST /*		:	'...'	;*/
%token <t> AND INTRINSIC OR IDENT EOFX ENUMERABLE EXPLICIT FLOAT_LITERAL
%token <t> DECIMAL_LITERAL OCTAL_LITERAL STRING_LITERAL HEX_LITERAL 
%token <t> INCLUDE INCLUDE_DIRECTIVE

%type <t> compilationUnit
%type <t> as2CompilationUnit
%type <t> as3CompilationUnit
%type <t> importDefinitions
%type <t> as2Type
%type <t> packageDecl
%type <t> packageBlock
%type <t> packageBlockEntry
%type <t> importDefinition
%type <t> semi
%type <t> classDefinition
%type <t> as2ClassDefinition
%type <t> interfaceDefinition
%type <t> as2InterfaceDefinition
%type <t> classExtendsClause
%type <t> interfaceExtendsClause
%type <t> commaIdentifiers
%type <t> implementsClause
%type <t> typeBlock
%type <t> typeBlockEntry
%type <t> as2IncludeDirective
%type <t> includeDirective
%type <t> blockOrSemi
%type <t> optionalTypeExpression
%type <t> methodDefinition
%type <t> optionalAccessorRole
%type <t> accessorRole
%type <t> namespaceDefinition
%type <t> useNamespaceDirective
%type <t> commaVariableDeclarators
%type <t> variableDefinition
%type <t> varOrConst
%type <t> optionalVariableInitializer
%type <t> variableDeclarator
%type <t> declaration
%type <t> declarationTail
%type <t> variableInitializer
%type <t> commaParameterDeclaration
%type <t> parameterDeclarationList
%type <t> parameterDeclaration
%type <t> basicParameterDeclaration
%type <t> parameterDefault
%type <t> parameterRestDeclaration
%type <t> blockEntries
%type <t> block
%type <t> blockEntry
%type <t> condition
%type <t> statement
%type <t> statements
%type <t> superStatement
%type <t> declarationStatement
%type <t> expressionStatement
%type <t> ifStatement
%type <t> elseClause
%type <t> throwStatement
%type <t> defaultStatement
%type <t> tryStatement
%type <t> catchBlocks
%type <t> catchBlock
%type <t> finallyBlock
%type <t> returnStatement
%type <t> continueStatement
%type <t> breakStatement
%type <t> switchStatement
%type <t> caseStatements
%type <t> caseStatement
%type <t> optionalDefaultStatement
%type <t> switchStatementList
%type <t> forEachStatement
%type <t> forStatement
%type <t> traditionalForClause
%type <t> forInClause
%type <t> forInClauseDecl
%type <t> forInClauseTail
%type <t> forInit
%type <t> forCond
%type <t> forIter
%type <t> whileStatement
%type <t> doWhileStatement
%type <t> withStatement
%type <t> defaultXMLNamespaceStatement
%type <t> typeExpression
%type <t> identifier
%type <t> propertyIdentifier
%type <t> qualifier
%type <t> simpleQualifiedIdentifier
%type <t> expressionQualifiedIdentifier
%type <t> nonAttributeQualifiedIdentifier
%type <t> qualifiedIdentifier
%type <t> qualifiedIdent
%type <t> namespaceName
%type <t> reservedNamespace
%type <t> dotIdents
%type <t> identifierStar
%type <t> annotations
%type <t> annotation
%type <t> moreAnnotationParams
%type <t> annotationParamList
%type <t> annotationParam
%type <t> modifiers
%type <t> modifier
%type <t> arguments
%type <t> arrayLiteral
%type <t> elementList
%type <t> moreAssignmentExpressions
%type <t> nonemptyElementList
%type <t> element
%type <t> objectLiteral
%type <t> moreLiteralFields
%type <t> fieldList
%type <t> literalField
%type <t> fieldName
%type <t> expression
%type <t> expressionList
%type <t> assignmentExpression
%type <t> assignmentOperator
%type <t> conditionalExpression
%type <t> conditionalSubExpression
%type <t> logicalOrExpression
%type <t> logicalOrOperator
%type <t> logicalAndExpression
%type <t> logicalAndOperator
%type <t> bitwiseOrExpression
%type <t> bitwiseXorExpression
%type <t> bitwiseAndExpression
%type <t> equalityExpression
%type <t> equalityOperator
%type <t> relationalExpression
%type <t> relationalOperator
%type <t> shiftExpression
%type <t> shiftOperator
%type <t> additiveExpression
%type <t> additiveOperator
%type <t> multiplicativeExpression
%type <t> multiplicativeOperator
%type <t> unaryExpression
%type <t> unaryExpressionNotPlusMinus
%type <t> postfixExpression
%type <t> postfixExpression2
%type <t> e4xAttributeIdentifier
%type <t> primaryExpression
%type <t> constant
%type <t> number
%type <t> newExpression
%type <t> fullNewSubexpression
%type <t> brackets
%type <t> encapsulatedExpression
%type <t> functionSignature
%type <t> functionCommon
%type <t> functionExpression
%type <t> ident



%%


/**
 * this is the start rule for this parser
 */
compilationUnit: as2CompilationUnit 	{/*YY_TREE = $1;*/ return(0);}
	| as3CompilationUnit		{/*YY_TREE = $1;*/ return(0);}
	;
as2CompilationUnit: importDefinitions as2Type
	;
importDefinitions: importDefinitions importDefinition
	| { $$ = NULL; }
	;
as2Type: annotations modifiers as2ClassDefinition
	| annotations modifiers as2InterfaceDefinition
	;
as3CompilationUnit: packageDecl	packageBlockEntries EOFX
	;
packageBlockEntries: packageBlockEntries packageBlockEntry
	|
	;
packageDecl: PACKAGE identifier packageBlock
	| PACKAGE packageBlock
	;
packageBlock
	:	LCURLY packageBlockEntries RCURLY
	;
packageBlockEntry: importDefinition
	|  annotations modifiers classDefinition
	|  annotations modifiers interfaceDefinition
	|  annotations modifiers variableDefinition
	|  annotations modifiers methodDefinition
	|  annotations modifiers namespaceDefinition
	|  annotations modifiers useNamespaceDirective
	|  SEMI
	;

importDefinition
	:	IMPORT identifierStar semi
	;

semi	:	SEMI
	;	

classDefinition: 
	/*
	CLASS ident classExtendsClause implementsClause typeBlock
	| CLASS ident classExtendsClause implementsClause typeBlock
	*/
	CLASS ident classExtendsClause typeBlock
	;

as2ClassDefinition
	:
		CLASS identifier
		classExtendsClause
		implementsClause
		typeBlock
	;

interfaceDefinition
	:	INTERFACE ident
		interfaceExtendsClause
		typeBlock
	;

as2InterfaceDefinition
	:	INTERFACE identifier
		interfaceExtendsClause
		typeBlock
	;

classExtendsClause:	
	{printf("TS402\n");}
	EXTENDS identifier
	| 
	{printf("TS406\n"); /*$$ = NULL;*/ }
	;
interfaceExtendsClause
	:	EXTENDS identifier commaIdentifiers
	| { $$ = NULL; }
	;
commaIdentifiers: COMMA identifier commaIdentifiers
	| { $$ = NULL; }
	;
implementsClause
	:	IMPLEMENTS identifier commaIdentifiers
	| { $$ = NULL; }
	;
typeBlock:	LCURLY typeBlockEntries	RCURLY
	;

typeBlockEntries: typeBlockEntry typeBlockEntries
	|
	;
typeBlockEntry:	annotations modifiers variableDefinition
	|	annotations modifiers methodDefinition
	|	importDefinition
	|	as2IncludeDirective
	;

as2IncludeDirective:	INCLUDE_DIRECTIVE
	STRING_LITERAL
	;

includeDirective:	INCLUDE STRING_LITERAL semi
	;

blockOrSemi:	block | semi ;
optionalTypeExpression: typeExpression
	| { $$ = NULL; }
	;
methodDefinition:	
	FUNCTION optionalAccessorRole ident parameterDeclarationList
		optionalTypeExpression blockOrSemi
	;

optionalAccessorRole:	accessorRole
	| { $$ = NULL; }
	;

accessorRole:	GET | SET
	;

namespaceDefinition:	NAMESPACE ident
	;

useNamespaceDirective:	USE NAMESPACE ident semi
	;

commaVariableDeclarators: COMMA variableDeclarator commaVariableDeclarators
	| { $$ = NULL; }
	;
variableDefinition:
	{printf("TS465\n");}
	varOrConst variableDeclarator commaVariableDeclarators semi
	;

varOrConst:	VAR | CONST
	;

optionalVariableInitializer: variableInitializer
	| { $$ = NULL; }
	;
variableDeclarator: 
	ident optionalTypeExpression optionalVariableInitializer
	| optionalTypeExpression optionalVariableInitializer
	;

declaration:	varOrConst variableDeclarator declarationTail
	;
declarationTail: commaVariableDeclarators
	;
variableInitializer:	ASSIGN assignmentExpression
	;
commaParameterDeclaration: COMMA parameterDeclaration commaParameterDeclaration
	| { $$ = NULL; }
	;
parameterDeclarationList:	LPAREN RPAREN
	|	LPAREN parameterDeclaration commaParameterDeclaration RPAREN
	;

parameterDeclaration:	basicParameterDeclaration | parameterRestDeclaration
	;

basicParameterDeclaration:
		ident optionalTypeExpression parameterDefault
	|	CONST ident optionalTypeExpression parameterDefault
	|	ident optionalTypeExpression
	|	CONST ident optionalTypeExpression
	;

parameterDefault:	ASSIGN assignmentExpression
	;

parameterRestDeclaration:	REST ident
	| REST
	;

blockEntries: blockEntry blockEntries
	| { $$ = NULL; }
	;

block:	LCURLY blockEntries RCURLY
	;

blockEntry: statement
	;

condition:	LPAREN expression RPAREN
	;

statement
	:	superStatement
	|	block
	|	declarationStatement
	|	expressionStatement
	|	ifStatement
	|	forEachStatement
	|	forStatement
	|	whileStatement
	|	doWhileStatement
	|	withStatement
	|	switchStatement
	|	breakStatement
	|	continueStatement
	|	returnStatement
	|	throwStatement
	|	tryStatement
	|	defaultXMLNamespaceStatement
	|	SEMI
	;

superStatement:	SUPER arguments semi
	;

declarationStatement:	declaration semi
	;

expressionStatement:	expressionList semi
	;
	
ifStatement:
		IF condition statement elseClause
	|	IF condition statement
	;

elseClause:	ELSE statement
	;

throwStatement:	THROW expression semi
	;

tryStatement:
	TRY block finallyBlock
	| TRY block catchBlocks finallyBlock
	| TRY block catchBlocks
	;

catchBlocks: catchBlock catchBlocks |  { $$ = NULL; } ;

catchBlock:	CATCH LPAREN ident optionalTypeExpression RPAREN block
	;

finallyBlock:	FINALLY block
	;

returnStatement:	RETURN expression semi
	| RETURN semi
	;
		
continueStatement: CONTINUE semi
	;

breakStatement:	BREAK semi
	;

switchStatement: SWITCH condition switchBlock
	;

switchBlock: LCURLY caseStatements RCURLY
	;

caseStatements: caseStatements caseStatement |  { $$ = NULL; } ;

caseStatement
	:	CASE expression COLON switchStatementList
	;
	
optionalDefaultStatement: defaultStatement | {$$ = NULL; } ;

defaultStatement
	:	DEFAULT COLON switchStatementList
	;

switchStatementList:	statements optionalDefaultStatement
	;

statements : statement statements | { $$ = NULL };

forEachStatement: FOR EACH LPAREN forInClause RPAREN statement
	;

forStatement: FOR LPAREN forInClause RPAREN statement
	| FOR LPAREN	traditionalForClause RPAREN statement
	;

traditionalForClause:	forInit SEMI forCond SEMI forIter
	;

forInClause:	forInClauseDecl IN forInClauseTail
	;

forInClauseDecl:	declaration
	;


forInClauseTail
	:	expressionList
	;

forInit	: declaration
	| expressionList
	| { $$ = NULL; }
	;

forCond :	expressionList
	| { $$ = NULL; }
	;

forIter :	expressionList
	| { $$ = NULL; }
	;

whileStatement
	:	WHILE condition statement
	;

doWhileStatement
	:	DO statement WHILE condition semi
	;

withStatement
	:	WITH condition statement
	;

defaultXMLNamespaceStatement
	:	DEFAULT XML NAMESPACE ASSIGN expression semi
	;

typeExpression
	:	COLON identifier
	|	COLON VOID
	|	COLON STAR
	;	

identifier:	qualifiedIdent
	;

propertyIdentifier:	STAR
	|	ident
	;

qualifier:	propertyIdentifier | reservedNamespace
	;

simpleQualifiedIdentifier
	:	propertyIdentifier
	|	qualifier DBL_COLON propertyIdentifier
	|	qualifier DBL_COLON brackets
	;

expressionQualifiedIdentifier
	:	encapsulatedExpression DBL_COLON propertyIdentifier
	|	encapsulatedExpression DBL_COLON brackets
	;

nonAttributeQualifiedIdentifier
	:	simpleQualifiedIdentifier
	|	expressionQualifiedIdentifier
	;

qualifiedIdentifier
	:	e4xAttributeIdentifier
	|	nonAttributeQualifiedIdentifier
	;

qualifiedIdent
	:	namespaceName DBL_COLON ident 
	|	ident 
	;

namespaceName:	IDENT | reservedNamespace
	;

reservedNamespace
	:	PUBLIC
	|	PRIVATE
	|	PROTECTED
	|	INTERNAL
	;

dotIdents : DOT ident dotIdents |  { $$ = NULL; } ;

identifierStar
	:	ident dotIdents DOT STAR
	|	ident dotIdents
	;

annotations: annotation annotations 
	| includeDirective annotations
	| { $$ = NULL; }
	;

annotation:	LBRACK ident annotationParamList RBRACK
	|	LBRACK ident RBRACK
	;

moreAnnotationParams: COMMA annotationParam moreAnnotationParams
	| { $$ = NULL; }
	;

annotationParamList: LPAREN annotationParam moreAnnotationParams RPAREN
	| LPAREN RPAREN
	;

annotationParam:
		ident ASSIGN constant
	|	constant
	|	ident
	;

modifiers:	modifier modifiers
	| { $$ = NULL; }
	;

modifier
	:	namespaceName
	|	STATIC
	|	FINAL
	|	ENUMERABLE
	|	EXPLICIT
	|	OVERRIDE
	|	DYNAMIC
	|	INTRINSIC
	;

arguments:	LPAREN expressionList RPAREN
	|	LPAREN RPAREN
	;

arrayLiteral:	LBRACK elementList RBRACK
	| 	LBRACK  RBRACK
	;
		
elementList:	COMMA
	|	nonemptyElementList
	;

moreAssignmentExpressions:
	 COMMA assignmentExpression moreAssignmentExpressions
	| { $$ = NULL; }
	;

nonemptyElementList
	:	assignmentExpression moreAssignmentExpressions
	;

element	:	assignmentExpression
	;


objectLiteral:	LCURLY fieldList RCURLY
	| 	LCURLY  RCURLY
	;
	
moreLiteralFields: COMMA literalField moreLiteralFields
	| COMMA
	|  { $$ = NULL; } ;

fieldList:	literalField moreLiteralFields
	;
	
literalField : 	fieldName COLON element
	;
	
fieldName:	ident
	|	number
	;

expression:	assignmentExpression
	;

expressionList:	assignmentExpression moreAssignmentExpressions
	;

assignmentExpression
	: conditionalExpression assignmentOperator assignmentExpression
	| conditionalExpression
	;

assignmentOperator
	:	ASSIGN
	| 	STAR_ASSIGN
	|	DIV_ASSIGN
	|	MOD_ASSIGN
	|	PLUS_ASSIGN
	|	MINUS_ASSIGN
	|	SL_ASSIGN
	|	SR_ASSIGN
	|	BSR_ASSIGN
	|	BAND_ASSIGN
	|	BXOR_ASSIGN
	|	BOR_ASSIGN
	|	LAND_ASSIGN
	|	LOR_ASSIGN
	;

conditionalExpression
	:	logicalOrExpression
	|	logicalOrExpression QUESTION conditionalSubExpression
	;

conditionalSubExpression
	:	assignmentExpression COLON assignmentExpression
	;

logicalOrExpression
	:	logicalAndExpression
	|	logicalAndExpression logicalOrOperator logicalOrExpression
	;

logicalOrOperator:	LOR | OR
	;

logicalAndExpression
	:	bitwiseOrExpression
	|	bitwiseOrExpression logicalAndOperator logicalAndExpression
	;

logicalAndOperator
	:	LAND | AND
	;

bitwiseOrExpression
	:	bitwiseXorExpression
	|	bitwiseXorExpression BOR bitwiseOrExpression
	;

bitwiseXorExpression
	:	bitwiseAndExpression
	|	bitwiseAndExpression BXOR bitwiseXorExpression
	;

bitwiseAndExpression
	:	equalityExpression
	|	equalityExpression BAND bitwiseAndExpression
	;

equalityExpression
	:	relationalExpression
	|	relationalExpression equalityOperator equalityExpression
	;

equalityOperator
	:	STRICT_EQUAL | STRICT_NOT_EQUAL | NOT_EQUAL | EQUAL
	;
	
relationalExpression
	:	shiftExpression
	|	shiftExpression relationalOperator relationalExpression
	;

relationalOperator
	:	IN
	|	LT | GT | LE | GE | IS | AS | INSTANCEOF
	;

shiftExpression
	:	additiveExpression
	|	additiveExpression shiftOperator shiftExpression
	;

shiftOperator:	SL | SR | BSR
	;

additiveExpression
	:	multiplicativeExpression
	|	multiplicativeExpression additiveOperator additiveExpression
	;

additiveOperator
	:	PLUS | MINUS
	;

multiplicativeExpression
	:	unaryExpression
	|	unaryExpression	multiplicativeOperator multiplicativeExpression
	;

multiplicativeOperator:	STAR
	|	DIV
	|	MOD
	;

unaryExpression
	:	INC unaryExpression
	|	DEC unaryExpression
	|	MINUS unaryExpression
	|	PLUS unaryExpression
	|	unaryExpressionNotPlusMinus
	;

unaryExpressionNotPlusMinus
	:	DELETE postfixExpression
	|	VOID unaryExpression
	|	TYPEOF unaryExpression
	|	LNOT unaryExpression
	|	BNOT unaryExpression
	|	postfixExpression
	;

postfixExpression: postfixExpression2 
	|  postfixExpression2 INC
	|  postfixExpression2 DEC
	;
postfixExpression2
	:	primaryExpression
	|	postfixExpression LBRACK expression RBRACK
	|	postfixExpression E4X_DESC qualifiedIdentifier
	|	postfixExpression DOT LPAREN expression RPAREN
	|	postfixExpression DOT e4xAttributeIdentifier
	|	postfixExpression DOT STAR
	|	postfixExpression arguments
 	;

e4xAttributeIdentifier
	:	E4X_ATTRI qualifiedIdent
	|	E4X_ATTRI STAR
	|	E4X_ATTRI LBRACK expression RBRACK
	;

primaryExpression
	:	UNDEFINED
	|	constant
	|	arrayLiteral
	|	objectLiteral
	|	functionExpression
	|	newExpression
	|	encapsulatedExpression
	|	e4xAttributeIdentifier
	|	qualifiedIdent
	;


/*
propOrIdent:	
		DOT qualifiedIdent
	;
*/

constant:
		number
	|	STRING_LITERAL
	|	TRUE
	|	FALSE
	|	NULL_VAL
	;

number	:	HEX_LITERAL
	|	DECIMAL_LITERAL
	|	OCTAL_LITERAL
	|	FLOAT_LITERAL
	;

newExpression
	:	NEW fullNewSubexpression arguments
	|	NEW fullNewSubexpression
	;

fullNewSubexpression: primaryExpression
	| fullNewSubexpression DOT qualifiedIdent
	| fullNewSubexpression brackets
	;

brackets
	:	LBRACK expressionList RBRACK
	;

encapsulatedExpression:	LPAREN assignmentExpression RPAREN
	;

functionSignature
	:	parameterDeclarationList optionalTypeExpression
	;

functionCommon
	:	functionSignature block
	;

functionExpression
	:	FUNCTION IDENT functionCommon
	|	FUNCTION functionCommon
	;


ident
	:	IDENT 		{ _TCN("ident",1, $1); }
	|	USE
	|	XML
	|	DYNAMIC
	|	NAMESPACE
	|	IS
	|	AS
	|	GET
	|	SET
	;

