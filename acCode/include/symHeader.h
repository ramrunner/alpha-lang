/* 
 * Symbol table header file
 */
#ifndef __SYM_HEADER
#define __SYM_HEADER

#include "default.h"
#include "idgen.h"

typedef enum boolean { FALSE=0, TRUE } Boolean;
typedef enum symtype { VAR, ARG, USRFUNC, LIBFUNC, BLOCK, TMP } Symtype;
typedef enum searchtype { NOTFOUND=0, LOCAL, OTHER, GLOBAL } Searchtype;

typedef struct list
{
	struct symbol *data;
	struct list *next;
	struct list *prev;
} List;

typedef struct symbol
{
	enum symtype type;
	char *name;
	long lineDec;
	long iaddress;
	int localOffset;
	struct table *ptable;
	struct table *parent;
} Symbol;

typedef struct table
{
	int isFunc;
	int scope;
	int totArg;
	int totLocalArg;
	struct list *slist;
	struct table *root;	// to root deixnei to epomeno NOMIMO table embeleias
	struct table *vazeli;	// to vazeli deixnei sto table pou dimiourgise
				// to THIS. H ypar3i tou boleuei ton parser.
} Table;

extern List *createList();		// Kanei malloc(sizeof(List))
					// kai setarei ola se NULL
					
extern Table *createTable();		// Kanei malloc(sizeof(Table));
					// kai ta setarei ola se NULL i' 0

extern Symbol *createSymbol(Symtype, char *, long);	
	/* Kanei malloc(Symbol) kai kanei 'set' to symtype, name kai lineDec. 
	 * An einai USRFUNC i' BLOCK tote kanei set tin ptable=createTable()
	 */

extern void insToTable(Table *, Symbol *);	// Bazei mesa sto table to symbol afou
						// frontisei na kanei createList() kai
						// na ftia3ei tous pointers tis listas
		// --> Episis, an to Symbol exei setarismeno to ptable ( != NULL)
		// --> frontizei na ftia3ei to scope tou kai to root tou.

extern Searchtype lookup(Table *, char *, Symbol**);	// Kanei lookup to name me tin seira:
							// LOCAL, OTHER, GLOBAL

extern Searchtype lookupGlobal(Table *, char *,Symbol**);// Kanei lookup to name MONO sto 
							// global scope tou Table

extern Symbol *getTmpSym(Table *, long);

extern void printTable(FILE *, Table *, int);

extern char *yytext;
extern long yylineno;
extern int yylex();
extern int yyerror(char*);

#endif
