#include "expr.h"

Expr *emit_iftable(Table *curTable, Expr *e)
{
    Symbol *tmpSym;
    Expr *ret;
    Constval tmpCV;
    
    if (e->type != TABLEITEM_E)
        return e;
    tmpSym = getTmpSym(curTable, yylineno);
    ret = createExpr(tmpSym, VAR_E, tmpCV, NULL);
    createQuad(TABLEGETELEM, e, e->index, ret);
    return ret;
}

Expr *createExpr(Symbol *sym, Expr_t type, Constval cv, Expr *index)
{
	Expr *retValue = malloc(sizeof(Expr));
	retValue->sym = sym;
	retValue->type = type;
	retValue->cv = cv;
	retValue->index = index;
    retValue->next = NULL;
	return retValue;
}

char *printExpr(Expr* pExpr)
{
	if(pExpr==NULL)
	{
		return "null";
    }

	switch(pExpr->type)
	{
		case VAR_E: case TABLEITEM_E: case USRFUNC_E: case LIBFUNC_E: case NEWTABLE_E: case ASSIGN_E:
		{
			if(pExpr->sym == NULL) //why???
				assert(0);
            if (pExpr->sym->name != NULL)
    			return pExpr->sym->name;
            else
                return pExpr->cv.str;
		}
		case CNUM_E:
		{
			size_t len;
			char* gavros = calloc(1024,sizeof(char));
			snprintf(gavros,1000*sizeof(char),"%f",pExpr->cv.num);
			len = strlen(gavros);
			realloc(gavros,len*sizeof(char));
			return gavros;
		}
		case CBOOL_E:
		{
			if(pExpr->cv.bool==TRUE)
				return "true\0";
			else
				return "false\0";
		}
		case CSTRING_E:
		{
			return pExpr->cv.str;
		}
		case NIL_E:
		{
			return "NOUL\0";
		}
		default:
		{
			fprintf(stderr, "FATAL ERROR: pExpr->type = %d\n", pExpr->type);
			assert(0); //shouldn't reach here
		}
	}		
}

Expr_t convertType(Symtype stype)
{
    switch(stype)
    {
        case VAR : case ARG : return VAR_E;
        case USRFUNC : return USRFUNC_E;
        case LIBFUNC : return LIBFUNC_E;
        case BLOCK : case TMP : default: assert(0); // not supposed to reach here
    }
}
