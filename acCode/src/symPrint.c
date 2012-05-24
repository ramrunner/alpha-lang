#include "symHeader.h"

static void _printSpace(FILE *,int);

void printTable(FILE* out, Table* pt, int n)
{
	List* curList;
	Symbol* curSym;
	
	if (pt==NULL)
		return;
	curList = pt->slist;
	for (;;)
	{
		if (curList == NULL)
			return;
		curSym = curList->data;
		_printSpace(out, n);
		switch(curSym->type)
		{
			case USRFUNC:
				fprintf(out, "USR function ID: '%s' line: %ld\n", curSym->name, curSym->lineDec);
				printTable(out, curSym->ptable, n+1);
				break;
			case LIBFUNC:
				fprintf(out, "LIB function ID: '%s' line: %ld\n", curSym->name, curSym->lineDec);
				printTable(out, curSym->ptable, n+1);
				break;
			case BLOCK:
				fprintf(out, "Start of block at line: %ld\n", curSym->lineDec);
				printTable(out, curSym->ptable, n+1);
				break;
			case VAR:
				fprintf(out, "VAR ID: '%s' line: %ld\n", curSym->name, curSym->lineDec);
				break;
			case ARG:
				fprintf(out, "ARG ID: '%s' line: %ld\n", curSym->name, curSym->lineDec); 
				break;
			default:
				assert(0);
		}	
		curList = curList->next;
	} 	
}

static void _printSpace(FILE* out, int n)
{
	int i;
	for (i=0;i<n;i++)
		fprintf(out,"  ");
	return ;
}
