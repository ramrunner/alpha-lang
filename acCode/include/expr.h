#ifndef __EXPR_HEADER
#define __EXPR_HEADER

#include "symHeader.h"
#include "quad.h"

typedef union constval
{
	double      num;
	char        *str;
	Boolean     bool;
	struct expr *exVal; //xrisimopoieitai stin indexelem/tablemake
} Constval;

typedef enum expr_t
	{
		VAR_E,  TABLEITEM_E,\
		USRFUNC_E,  LIBFUNC_E,\
		ARITH_E,  BOOL_E,  ASSIGN_E,  NEWTABLE_E,\
		CNUM_E,  CBOOL_E,  CSTRING_E,\
		NIL_E \
	} Expr_t;

typedef struct expr
{
	Symbol *sym;
	Expr_t type;
	Constval cv;
	struct expr *index;
	struct expr *next;
} Expr;

//typedef struct expr* ExprPTR;

extern Expr *emit_iftable(Table *, Expr *);
extern Expr *createExpr(Symbol*, Expr_t, Constval, Expr*);
extern char *printExpr(Expr*);
extern Expr_t convertType(Symtype);


#endif
