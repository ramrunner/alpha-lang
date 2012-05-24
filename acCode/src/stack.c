/* 
* general purpose Stack manipulation code
* kazaz , dsp 19/04/05
*/
#include "stack.h"

Stack* initStack()
{
	Stack* ptheStack;
	if((ptheStack = calloc(1,sizeof(Stack)))==NULL)	
	{
		fprintf(stderr,"Mallocation error : You'r not malloc enough\n");
		exit(-1);
	}
	else
	{
		ptheStack->down = NULL;
		ptheStack->up = NULL;
		ptheStack->data = 0;
		return ptheStack;
	}
}
void push(Stack* pstack,long item)
{
	while(pstack->up!=NULL)
		pstack=pstack->up;
	//printf("-push-> %l \n",item);
	pstack->data=item;
	pstack->up=initStack();
	pstack->up->down=pstack;
}

long pop(Stack* pstack)
{
	long temp;
	while(pstack->up!=NULL)
		pstack=pstack->up;
	//if (pstack->down == NULL)
	//{
//		fprintf(stderr, "Internal fatal stack error: Nothing to pop().\n");
//		exit(-1);
//	}
		
	temp=pstack->down->data;
	pstack->down->up=NULL;
	free(pstack);
	return temp;
}
	
