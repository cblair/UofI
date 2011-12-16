///////////////////////////////////////////////////////////////////////////////
//Class:	CS 445
//Semester:	Fall 2011
//Assignment:	Homework 6
//Author:	Colby Blair
//File name:	tac_x86_opcodes.h
///////////////////////////////////////////////////////////////////////////////

#ifndef _TAC_X86_OPCODES_H
#define _TAC_X86_OPCODES_H

enum OPCODE
{
	_AAA,
	_AAD,
	_AAM,
	_AAS,
	_ADC,
	_ADD,
	_AND,
	_CALL,
	_CBW,
	_CLC,
	_CLD,
	_CLI,
	_CMC,
	_CMP,
	_CMPSB,
	_CMPSW,
	_CWD,
	_DAA,
	_DAS,
	_DEC,
	_DIV,
	_ESC,
	_HLT,
	_IDIV,
	_IMUL,
	_IN,
	_INC,
	_INT,
	_INTO,
	_IRET,
	_Jxx,
	_JMP,
	_LAHF,
	_LDS,
	_LEA,
	_LES,
	_LOCK,
	_LODSB,
	_LODSW,
	_LOOP,
	_LOOPx,
	_MOV,
	_MOVSB,
	_MOVSW,
	_MUL,
	_NEG,
	_NOP,
	_NOT,
	_OR,
	_OUT,
	_POP,
	_POPF,
	_PUSH,
	_PUSHF,
	_RCL,
	_RCR,
	_REPxx,
	_RET,
	_RETN,
	_RETF,
	_ROL,
	_ROR,
	_SAHF,
	_SAL,
	_SAR,
	_SBB,
	_SCASB,
	_SCASW,
	_SHL,
	_SHR,
	_STC,
	_STD,
	_STI,
	_STOSB,
	_STOSW,
	_SUB,
	_TEST,
	_WAIT,
	_XCHG,
	_XLAT,
	_XOR,	
	};

#endif
