/* 
* General Stack manipulation header
* kazaz , dsp 19/04/05
*/
#ifndef __STACK_HEADER
#define __STACK_HEADER
#include "default.h"

typedef struct stack 
{
	long data;
	struct stack* down;
	struct stack* up;
} Stack;

extern Stack* initStack();
extern void push(Stack*,long);
extern long pop(Stack*);
#endif
