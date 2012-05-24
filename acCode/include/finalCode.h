#ifndef __FINALCODE_HEADER
#define __FINALCODE_HEADER

#include "quad.h"
#include "expr.h"
#include "symHeader.h"

#define INSTRUCTION_SIZE  1024
#define CINDEX_SIZE  2048

typedef struct operand
{
	unsigned int code;
	unsigned long val;
} Operand;

typedef struct instuction
{
	unsigned int opCode;
	struct operand arg1, arg2, arg3;
} Instruction;

typedef struct cIndex
{
	void *data;
	int index;
	int maxData;
} CIndex;

typedef struct usrFunc
{
	long iaddress;
	int localSize;
	char *id;
} usrFunc;

typedef struct finalCode
{
	long long magicNumber;
	Instruction *code;
	int codeSize;
	struct cIndex constIndex[4];
} FinalCode;

extern FinalCode *createFinalCode(Quad **quadTable, int);
extern void init_finalCode();
extern void print_TextFile(FILE *, FinalCode *);
extern void print_BinaryFile(FILE *, FinalCode *);

	#ifdef __IN_FINALCODE_C
		static void insToFinalCode(Instruction);
		static Operand exprToOperand(Expr *);
		static Instruction quadToInstruction(Quad *);
	#endif

#endif
