/* 
 * Symbol table functions
 */

#include "symHeader.h"

List *createList()  
{
	List *retList = malloc(sizeof(List));  
	retList->data = NULL;
	retList->next = NULL;
	retList->prev = NULL;
	return retList;
}

Table *createTable()  
{
	Table *retTable = malloc(sizeof(Table));
	retTable->scope = -1;	// To kanw set me -1 wste an kapoios 3exasei
				// na to kanei set na bre8ei to la8os.
	retTable->totArg = 0;
	retTable->totLocalArg = 0;
	retTable->slist = NULL;
	retTable->root = NULL;
	retTable->vazeli = NULL;
	retTable->isFunc = 0; 
	return retTable;
}

Symbol *createSymbol(Symtype type, char *name, long lineDec)
{
	Symbol *retSymbol = malloc(sizeof(Symbol));
	retSymbol->type = type;
	retSymbol->name = name;
	retSymbol->lineDec = lineDec;
	retSymbol->iaddress = -1;
	if ( (type == USRFUNC) || (type == BLOCK) || (type==LIBFUNC) )
	{
		retSymbol->ptable = createTable();
		if (type == USRFUNC)
			retSymbol->ptable->isFunc = 2;
	}
	else
		retSymbol->ptable = NULL;
	return retSymbol;
}

static void insToList(List *plist, Symbol *psym)  // Auti i synartisi den xriazetai
{					 	 // ston teliko xristi tis "library"
						// gi'auto tin exw dilwsei STATIC
	if (plist->data == NULL)
	{
		plist->data = psym;
		return ;
	}
	
	while (plist->next != NULL)		
		plist = plist->next;
	plist->next = createList();
	plist->next->prev = plist;
	plist->next->data = psym;
}

void insToTable(Table *pt, Symbol *psym)  
{
	if (psym->ptable != NULL)
	{
		if (pt->scope<0)
		{
			fprintf(stderr, "ERROR within table while inserting symbol '%s' line: %ld.\n", psym->name, psym->lineDec);
			fprintf(stderr, "Table's scope not set properly.\nTerminating...\n");
			exit(-1);
		}
								// Ftiaxnei to scope pou yparxei
		psym->ptable->scope = pt->scope + 1;		// sto table tou symbol.
		psym->ptable->vazeli = pt;
		
		if ( (psym->type == USRFUNC) || (psym->type == LIBFUNC) )
		{
			psym->ptable->root = pt;
			while (psym->ptable->root->root != NULL)	
				psym->ptable->root = psym->ptable->root->root;
		}
		else if (psym->type == BLOCK)
			psym->ptable->root = pt;
		else	// An einai kati allo, tote yparxei kapoio la8os
		{	// giati tote to psym->ptable PREPEI na einai NULL
			fprintf(stderr, "ERROR with symbol '%s' line: %ld.\n", psym->name, psym->lineDec);
			fprintf(stderr, "Symbol type not set properly.\nTerminating...\n");
			exit(-1);
		}
		if (pt->isFunc == 2)
			psym->ptable->isFunc = 3;
	}
	if (pt->slist == NULL)
		pt->slist = createList();
	insToList(pt->slist, psym);
	psym->parent = pt;
	if ( (psym->type != BLOCK) && (psym->type != USRFUNC) && (psym->type != LIBFUNC) )
	{
		Table *tmp = pt;
		(pt->totLocalArg)++;
		while( (tmp->root != NULL) )// && (tmp->root->root != NULL))
		{
			if (tmp->isFunc > 0)
				break;
			tmp = tmp->root;
		}
		(tmp->totArg)++;
		psym->localOffset = tmp->totArg - 1;
	}
}

Searchtype lookupGlobal(Table *ptable, char *name, Symbol **retSym)
{
	while (ptable->root != NULL)
		ptable = ptable->root;
	return lookup(ptable, name, retSym);
}

Symbol *getTmpSym(Table *curTable, long yylineno)
{
	Symbol *retSym;
	char *newName = (char *)newTmp();
	int sRes = lookup(curTable, newName, &retSym);
	if ( sRes != NOTFOUND )
		return retSym;
	retSym = createSymbol(TMP, newName, yylineno);
	insToTable(curTable, retSym);
	return retSym;
}


Searchtype lookup(Table *ptable, char *name, Symbol **retSym)
{
	Boolean found;
	Symbol *tmpSymbol;
	List *tmpList;

	if (ptable->slist==NULL)
	{
		found = FALSE;
	}
	else
	{
		tmpList = ptable->slist;
		found = FALSE;

		for(;;)
		{
			tmpSymbol = tmpList->data;
			if ( (name == NULL) )
				return NOTFOUND;
			if (tmpSymbol->name != NULL)
				if (!strcmp(name, tmpSymbol->name))
				{	
					*retSym = tmpSymbol;
					found = TRUE;
					break;
				}
			if (tmpList->next != NULL)
				tmpList = tmpList->next;
			else
				break;
		}
	}

	if (ptable->root == NULL)
	{
		if (found == TRUE)
			return GLOBAL;
		else
			return NOTFOUND;
	}
	else
	{
		Searchtype retValue;
		if (found == TRUE)
			return LOCAL;
		else switch (retValue=lookup(ptable->root, name, retSym))
		{
			case LOCAL:			// To local kapoiou mikroterou scope
				return OTHER; break;	// einai OTHER gia auto to search
			default:
				return retValue; break;
		}
	}
}

char* createCopy(char* s,int len)
{
	char* retval = malloc( 2+sizeof(char)*len);
	strncpy(retval,s,len*sizeof(char));
	retval[len] = '\0';
	return retval;
}

