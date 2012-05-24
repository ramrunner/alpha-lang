%{
	#include "symHeader.h"
	#include "quad.h"
	#include "idgen.h"
	#include "stack.h"
	#include "finalCode.h"

	FILE *outFile;
	Table *curTable;
	extern FILE* yyin;
	Symbol *tmpSym;
	Constval tmpCV;
	Stack   *ifStack, *forStack, *whileStack;
	Stack   *loopStartStack, *loopEndStack;
	Stack	*funcStack;
%}

%union
{
	int intValue;
	double realValue;
	char * stringValue;
	struct expr* xval;
	struct symbol *sym;
}

%start program

%token	<stringValue>
	keyELSE keyIF keyWHILE keyFOR keyFUNCT keyRETURN
	keyBREAK keyCONTIN keyLOCAL keyTRUE keyFALSE keyNIL
	semLCURLBRACE semRCURLBRACE semLBRACE semRBRACE semDOT semLPAR
	semRPAR semCOLON semCOMMA sem2DDOT semDDOT opASSIGN opPLUS
	opMINUS opMULTI opDIV opMOD opEQ opNEQ opOR opAND opNOT
	opINC opDEC opLT opLTE opGT opGTE typeIDENTIFIER
	typeSTRING typeCOMMENT

%token	<intValue> constINT
%token	<realValue> constREAL

/*edo arxizo ta proseteristikotita/protereotita*/
%left opASSIGN
%left opOR
%left opAND
%nonassoc opEQ opNEQ
%nonassoc opLT opLTE opGT opGTE
%left opPLUS opMINUS
%left opMULTI opDIV opMOD
%right opNOT opINC opDEC UMINUS 
%left semDOT
%left semLBRACE semRBRACE
%left semLPAR semRPAR

%nonassoc IFd
%nonassoc keyELSE

%type <xval> expr term assignexpr lvalue primary consta assignORterm call tablemake funcdef tableitem
%type <xval> elist elistExt forstmt indexed indexedEXT indexelem
%type <intValue> op
%type <sym> funcdefEXT

%%

program : stmt program
	| /* empty */
	{	}
	;
	
stmt	: expr semCOLON { fprintf(outFile, "Line: %ld, Expression statement\n", yylineno); resetTmp();	}
	| ifstmt { resetTmp(); }  // ifstmt is quad ready
	| whilestmt { resetTmp(); }  //whilestmt is quad ready
	| forstmt { resetTmp(); }  //forstmt is quad ready
	| returnstmt { resetTmp();}  //returnstmt is quad ready
	| keyBREAK semCOLON 
	{ 
	   int n = pop(loopEndStack);
	   if (n==0)
	   {
	       yyerror("Break out of while/for loop\n");
	       exit(-1);
	   }
	   push(loopEndStack, nextQuadLine());
	   push(loopEndStack, n-1);
	   createQuad(JUMP, NULL, NULL, NULL);
	   resetTmp();
    }   //breakstmt is quad ready
	| keyCONTIN semCOLON 
	{
        Quad *tmpQuad;
        int n = pop(loopStartStack);
        if (n==0)
        {
            yyerror("Continue out of while/for loop\n");
            exit(-1);
        }    
        tmpQuad = createQuad(JUMP, NULL, NULL, NULL); 
        tmpQuad->label = n;
        push(loopStartStack, n);
        resetTmp();  
    }
	| block { resetTmp();	}  // block is quad ready kai kala
	| funcdef { resetTmp();  }  // funcdef is quad ready
	| semCOLON { resetTmp();  }  //empty stmt is quad ready , nai re standard!
	;

assignORterm	:assignexpr { $$ = $1;	 }
		|term	 {	$$ = $1;  }
		;
	
expr	:assignexpr  
	{	
		$$ = $1;
		fprintf(outFile, "-Line: %ld, Assign expression\n", yylineno);	 
	}
	|expr op assignORterm	
	{	
		if ( ($2 >= IF_EQ) && ($2 <= IF_GREATER) )
		{	// Edw 8a xriastoun jumps
			long offset = nextQuadLine();
			Expr* retExpr;
			Quad *tmpQuad;
			tmpSym = getTmpSym(curTable, yylineno);
			retExpr = createExpr(tmpSym, VAR_E, tmpCV, NULL);
			// z = x relop y
			// CODE: (offset)	IF (x relop y) goto offset+3
				tmpQuad = createQuad($2,$1,$3,NULL);
				tmpQuad->label = offset+3;
			// CODE: (offset+1)	z = FALSE
				tmpCV.bool = FALSE;
				createQuad(ASSIGN, createExpr(NULL, CBOOL_E, tmpCV, NULL), NULL, retExpr);
			// CODE: (offset+2)	JUMP offset+4
				tmpQuad = createQuad(JUMP, NULL, NULL, NULL);
				tmpQuad->label = offset+4;
			// CODE: (offset+3) z = TRUE
				tmpCV.bool = TRUE;
				createQuad(ASSIGN, createExpr(NULL, CBOOL_E, tmpCV, NULL), NULL, retExpr);
			// CODE: (offset+4) rest of code POSSIBLE BUG
			//	createQuad(NOP, NULL, NULL, NULL); //<--- No need
			$$ = retExpr;
		}
		else
		{
			//DsP -> 19/04/05
			Expr* retExpr;
			tmpSym = getTmpSym(curTable, yylineno);
			retExpr = createExpr(tmpSym, VAR_E, tmpCV, NULL);
			createQuad($2,$1,$3, retExpr);
			$$ = retExpr;
		}
		fprintf(outFile, "Line: %ld, expression op assignExpression\n", yylineno);	 
	}
	|term
	{	
		$$ = $1;
		fprintf(outFile, "-Line: %ld, Terminal\n", yylineno);	
	}
	;
	
op	:opPLUS	
	{
		$$ = ADD;
		fprintf(outFile, "Line: %ld, Operator +\n", yylineno);	 
	}
	|opMINUS  
	{
		$$ = SUB;
		fprintf(outFile, "Line: %ld, Operator -\n", yylineno);	 
	}
	|opMULTI  
	{
		$$ = MUL;
		fprintf(outFile, "Line: %ld, Operator *\n", yylineno);	 
	}
	|opDIV	 
	{	
		$$ = DIV;
		fprintf(outFile, "Line: %ld, Operator /\n", yylineno);	 
	}
	|opMOD	 
	{	
		$$ = MOD;
		fprintf(outFile, "Line: %ld, Operator %%\n", yylineno);  
	}
	|opGT	
	{	
		$$ = IF_GREATER;
		fprintf(outFile, "Line: %ld, Operator >\n", yylineno);	 
	}
	|opGTE	 
	{	
		$$ = IF_GREATEREQ;
		fprintf(outFile, "Line: %ld, Operator >=\n", yylineno);  
	}
	|opLT	
	{	
		$$ = IF_LESS;
		fprintf(outFile, "Line: %ld, Operator <\n", yylineno);	 
	}
	|opLTE	 
	{	
		$$ = IF_LESSEQ;
		fprintf(outFile, "Line: %ld, Operator <=\n", yylineno);  
	}
	|opEQ	
	{
		$$ = IF_EQ;
		fprintf(outFile, "Line: %ld, Operator ==\n", yylineno);  
	}
	|opNEQ	 
	{
		$$ = IF_NOTEQ;
		fprintf(outFile, "Line: %ld, Operator !=\n", yylineno);  
	}
	|opAND	 
	{	
		$$ = AND;
		fprintf(outFile, "Line: %ld, Operator &&\n", yylineno);  
	}
	|opOR	
	{	
		$$ = OR;
		fprintf(outFile, "Line: %ld, Operator ||\n", yylineno);  
	}
	;
	
term	: semLPAR expr semRPAR	 
	{	
		$$=$2;
		fprintf(outFile, "Line: %ld, Terminal (expr)\n", yylineno);  
	}
	| opMINUS expr %prec UMINUS  
	{
		Expr* retExpr;
		tmpSym = getTmpSym(curTable, yylineno);
		retExpr = createExpr(tmpSym, VAR_E, tmpCV, NULL);
		createQuad(QUMINUS, $2, NULL, retExpr);
		$$ = retExpr;
		
		fprintf(outFile, "Line: %ld, Terminal -expr\n", yylineno);	 
	}
	| opNOT expr  
	{	
		//DsP -> 19/04/05
		Expr* retExpr;
		tmpSym = getTmpSym(curTable, yylineno);
		retExpr = createExpr(tmpSym, VAR_E, tmpCV, NULL);
		createQuad(NOT, $2, NULL, retExpr);
		$$ = retExpr;
		fprintf(outFile, "Line: %ld, Terminal !expr\n", yylineno);	 
	}
	| opINC lvalue	 
	{	
		//DsP -> 19/04/05
		Expr *retExpr, *tmpExpr;
		Expr *lval = emit_iftable(curTable,$2);
		tmpSym = getTmpSym(curTable, yylineno);
		retExpr = createExpr(tmpSym, VAR_E, tmpCV, NULL);
		tmpCV.num=1;
		tmpExpr = createExpr(NULL, CNUM_E,tmpCV, NULL);
		createQuad(ADD,lval,tmpExpr, retExpr);
		createQuad(ASSIGN,retExpr,NULL,lval);
		if ($2->type == TABLEITEM_E)
            createQuad(TABLESETELEM, $2, $2->index, retExpr);
		$$ = retExpr;
	} 
	| lvalue opINC	 
	{	
		//DsP -> 19/04/05
		Expr *retExpr, *tmpExpr;
		Expr *lval = emit_iftable(curTable,$1);
		tmpSym = getTmpSym(curTable, yylineno);
		retExpr = createExpr(tmpSym, VAR_E, tmpCV, NULL);
		tmpCV.num=1;
		tmpExpr = createExpr(NULL, CNUM_E,tmpCV, NULL);
		$$ = lval;
		createQuad(ADD,lval,tmpExpr, retExpr);
		createQuad(ASSIGN,retExpr,NULL,lval);
		if ($1->type == TABLEITEM_E)
            createQuad(TABLESETELEM, $1, $1->index, retExpr);
	}
	| opDEC lvalue	 
	{	
		//DsP -> 19/04/05
		Expr *retExpr, *tmpExpr;
		Expr *lval = emit_iftable(curTable,$2);
		tmpSym = getTmpSym(curTable, yylineno);
		retExpr = createExpr(tmpSym, VAR_E, tmpCV, NULL);
		tmpCV.num=1;
		tmpExpr = createExpr(NULL, CNUM_E,tmpCV, NULL);
		createQuad(SUB,lval,tmpExpr, retExpr);
		createQuad(ASSIGN,retExpr,NULL,lval);
		if ($2->type == TABLEITEM_E)
            createQuad(TABLESETELEM, $2, $2->index, retExpr);
   		$$ = retExpr;
	}
	| lvalue opDEC	 
	{	
		//DsP -> 19/04/05
		Expr *retExpr, *tmpExpr;
		Expr *lval = emit_iftable(curTable,$1);
		tmpSym = getTmpSym(curTable, yylineno);
		retExpr = createExpr(tmpSym, VAR_E, tmpCV, NULL);
		tmpCV.num=1;
		tmpExpr = createExpr(NULL, CNUM_E,tmpCV, NULL);
		$$ = lval;
		createQuad(SUB,lval,tmpExpr, retExpr);
		createQuad(ASSIGN,retExpr,NULL,lval);
		if ($1->type == TABLEITEM_E)
            createQuad(TABLESETELEM, $1, $1->index, retExpr);
	}
	| primary	
	{
		$$ = $1;
		fprintf(outFile, "-Line: %ld, Terminal primary\n", yylineno);	
	}
	;
	
assignexpr	: lvalue opASSIGN expr
			{  
                Expr *lval, *retExpr;
                if ($1->type == TABLEITEM_E)
                {
                    createQuad(TABLESETELEM, $1, $1->index, $3);
                }
                else
    				createQuad(ASSIGN, $3, NULL, $1);
                retExpr = createExpr(getTmpSym(curTable, yylineno), ASSIGN_E, tmpCV, NULL);
                createQuad(ASSIGN, $3, NULL, retExpr);
				$$ = retExpr;
			}
		;
		
primary	: lvalue 
	{	
		//DsP -> 19/04/05
		$$ = emit_iftable(curTable,$1);
	}
	| call	 
	{	
		$$=$1;
		fprintf( outFile, "Line: %ld, Primary: call\n", yylineno);	 
	}
	| tablemake  
	{	
		$$=$1;
		fprintf( outFile, "Line: %ld, Primary: tablemake\n", yylineno);  
	}
	| semLPAR funcdef semRPAR	
	{	
		$$=$2;
		fprintf( outFile, "Line: %ld, Primary: (Function definition)\n", yylineno);  
	}
	| consta  
	{
		$$ = $1;
		fprintf( outFile, "-Line: %ld, Primary: const\n", yylineno);  
	}
	;

lvalue	: typeIDENTIFIER
	{
		Expr_t tmpType;
		if (lookup(curTable, $1, &tmpSym) == NOTFOUND)
		{
			tmpType = VAR_E;
			tmpSym = createSymbol(VAR, $1, yylineno);
			insToTable(curTable, tmpSym);
			fprintf(outFile, "  --> Inserted to symbol table\n");
		}
		else
			tmpType = convertType(tmpSym->type);
		$$ = createExpr(tmpSym, tmpType, tmpCV, NULL);
	}
	| keyLOCAL typeIDENTIFIER
	{
		Expr_t tmpType;
		if (curTable->scope == 0)	// We are not in global scope
		{
			yyerror("Syntax error: Local decleration in global scope\n");
			exit(-1); // Se periptwsi pou i yyerror() den ekane exit()
		}
		if (lookup(curTable, $2, &tmpSym) != LOCAL)
		{
			tmpType = VAR_E;
			insToTable(curTable, createSymbol(VAR, $2, yylineno));
			fprintf(outFile, "  --> Inserted to symbol table\n");
		}
		else
			tmpType = convertType(tmpSym->type);
		$$ = createExpr(tmpSym, tmpType, tmpCV, NULL);
	}
	| sem2DDOT typeIDENTIFIER
	{
		if (lookupGlobal(curTable, $2, &tmpSym) == NOTFOUND)
		{
			yyerror("Global ID not found\n");
			exit(-1);
		}
		$$ = createExpr(tmpSym, convertType(tmpSym->type), tmpCV, NULL);
	}
	| tableitem
	{
        $$ = $1;
	}
	;

/* Tables here */

tableitem	: lvalue semDOT typeIDENTIFIER
		{
            //An to lvalue einai table, prepei na kanoume getelem
            Expr *retExpr;
            Expr *lval = emit_iftable(curTable,$1);
            tmpCV.str = $3;
            retExpr = createExpr( lval->sym, TABLEITEM_E, tmpCV, createExpr(NULL, CSTRING_E, tmpCV, NULL));
            $$ = retExpr;
		}
		| lvalue semLBRACE expr semRBRACE
		{  
            //An to lvalue einai table, prepei na kanoume getelem
            Expr *retExpr;
            Expr *lval = emit_iftable(curTable,$1);
            retExpr = createExpr( lval->sym, TABLEITEM_E, tmpCV, $3);
            $$ = retExpr;

        }
		;

/* End of tables here */
	

call	: lvalue semLPAR elist semRPAR
		{
            Expr* tmpExpr = $3;
            Expr* retExpr;
            Expr *lval = emit_iftable(curTable,$1);

            while(tmpExpr != NULL)
            {
                createQuad(PARAM, tmpExpr, NULL, NULL);
                tmpExpr = tmpExpr->next;
            }
			createQuad(CALL, lval , NULL, NULL);
			tmpSym = getTmpSym(curTable, yylineno);
            retExpr = createExpr(tmpSym, VAR_E, tmpCV, NULL);
            createQuad(GETRETVAL, NULL, NULL, retExpr);
            $$ = retExpr;
		}
		| call semLPAR elist semRPAR
		{
			Expr* tmpExpr = $3;
			Expr* retExpr;
			Expr* lval = emit_iftable(curTable, $1);
		
			while(tmpExpr != NULL)
			{
				createQuad(PARAM, tmpExpr, NULL, NULL);
				tmpExpr = tmpExpr->next;
			}
			createQuad(CALL, lval, NULL, NULL);
			tmpSym = getTmpSym(curTable, yylineno);
			retExpr = createExpr(tmpSym, VAR_E, tmpCV, NULL);
			createQuad(GETRETVAL, NULL, NULL, retExpr);
			$$ = retExpr;
		}
		;
	
elist	: expr elistExt  
		{

                $1->next = $2;
            $$ = $1;
		}
		| /* empty */
		{  $$ = NULL;  }
		;
	
elistExt	: semCOMMA expr elistExt
			{
                $2->next = $3;
                $$ = $2;
			}
			| /* empty */
			{  $$ = NULL;  }
			;
		
tablemake	: semLBRACE elist semRBRACE
			{
                Expr *retExpr, *eIter;
                int indexCounter = 0;
                tmpSym = getTmpSym(curTable, yylineno);
                retExpr = createExpr(tmpSym, NEWTABLE_E, tmpCV, NULL);
                createQuad(TABLECREATE, retExpr, NULL, NULL);
                for (eIter = $2; eIter != NULL; eIter = eIter->next)
                {
                    tmpCV.num = indexCounter++;
                    createQuad(TABLESETELEM, retExpr, createExpr(NULL, CNUM_E, tmpCV, NULL), eIter);
                }
                $$ = retExpr;
            }
            | semLBRACE indexed semRBRACE
			{  
                Expr *retExpr, *eIter;
                tmpSym = getTmpSym(curTable, yylineno);
                retExpr = createExpr(tmpSym, NEWTABLE_E, tmpCV, NULL);
                createQuad(TABLECREATE, retExpr, NULL, NULL);
                for (eIter = $2; eIter != NULL; eIter = eIter->next)
                {
                    createQuad(TABLESETELEM, retExpr, eIter->index, eIter->cv.exVal);
                }
                $$ = retExpr;
            }
            ;
		
indexed	:   indexelem indexedEXT
            {
                $1->next = $2;
                $$ = $1;
            }
	;

indexedEXT  : semCOMMA indexelem indexedEXT
            {
                $2->next = $3;
                $$ = $2;
            }
            | /* empty */
            {
                $$ = NULL;
            }
            ;
	
indexelem	: semLCURLBRACE expr semDDOT expr semRCURLBRACE
			{        //    expr1->key   expr2->value
                Expr *retExpr;
                tmpCV.exVal = $4;
                retExpr = createExpr(NULL, TABLEITEM_E, tmpCV, NULL);
                retExpr->index = $2;
                $$ = retExpr;
            }
		;
		
block	: semLCURLBRACE
	  	{
			Symbol *tmp = createSymbol(BLOCK, NULL, yylineno);
			insToTable(curTable, tmp);
			curTable=tmp->ptable;
			fprintf(outFile, "Line: %ld, Start of block\n", yylineno);
		}
	  blockEXT semRCURLBRACE
	  	{
			curTable=curTable->vazeli;
			fprintf(outFile, "Line: %ld, End of block\n", yylineno);
		}
	;

blockEXT	: stmt blockEXT
			{  fprintf(outFile, "Line: %ld, Block statement\n", yylineno);  }
		| /* empty */
		;
	
funcdef	: keyFUNCT funcdefEXT semLPAR idlist semRPAR 
				{
					// Frontizouma gia to jump prin to funcstart
					push(funcStack, nextQuadLine());
					createQuad(JUMP, NULL, NULL, NULL);
					push(funcStack, 1);
				    //for breaks/continues
				    push(loopStartStack,0); // we just entered a function , so we can't break (immediately)
			        push(loopEndStack,0); // same here
			        $2->iaddress = nextQuadLine();
				    //prepei na kanoume emit funcstart
					Expr *tmpExpr = createExpr($2, USRFUNC_E, tmpCV, NULL);
					createQuad(FUNCSTART, tmpExpr, NULL, NULL);
					$$ = tmpExpr;
				}
			block		
				{	//prepei na kanoume emit funcend
					Expr *tmpExpr = createExpr($2, USRFUNC_E, tmpCV, NULL);
					long funcEndAddr = nextQuadLine();
					int i,n = pop(funcStack);
					createQuad(FUNCEND, tmpExpr, NULL, NULL);
					for (i=0; i<n-1; i++)
						getQuadByLine( pop(funcStack) )->label = funcEndAddr;
					getQuadByLine( pop(funcStack) )->label = funcEndAddr+1;		// einai to jump print to
					curTable = curTable->vazeli;  					// funcstart
					$$ = tmpExpr;
				}
		;

funcdefEXT	: typeIDENTIFIER
			{  
				Searchtype stmp;
				Symbol* tmp;
				fprintf(outFile, "Line: %ld, Function definition: function %s(idlist) block\n", yylineno, $1);  		
				stmp = lookup(curTable, $1, &tmpSym);
				if ( (stmp==LOCAL) || (stmp==GLOBAL && curTable->scope==0) )
				{
					yyerror("Redefinition error\n");
					exit(-1);
				}
				tmp = createSymbol(USRFUNC, $1, yylineno);
				insToTable(curTable, tmp);
				fprintf(outFile,"  --> Inserted to symbol table\n");
				curTable=tmp->ptable;
				$$ = tmp;
			}
		| // empty
			{  
				Symbol* tmp;
				tmp = createSymbol(USRFUNC, (char *)generateID(), yylineno);
				insToTable(curTable, tmp);
				curTable=tmp->ptable;
				curTable->isFunc = 2;
				$$ = tmp;
			}
		;	
	
consta	: constINT  
	{	
		tmpCV.num = (double) $1;
		$$ = createExpr(NULL, CNUM_E, tmpCV, NULL);
		fprintf(outFile, "-Line: %ld, Integer constant = %d\n", yylineno, $1);	 
	}
	| constREAL  
	{
		tmpCV.num = (double) $1;
		$$ = createExpr(NULL, CNUM_E, tmpCV, NULL);
		fprintf(outFile, "Line: %ld, Real constant = %lf\n", yylineno, $1);  
	}
	| typeSTRING  
	{
		tmpCV.str = $1;
		$$ = createExpr(NULL, CSTRING_E, tmpCV, NULL);
		fprintf(outFile, "Line: %ld, String constant = '%s'\n", yylineno, $1);	 
	}
	| keyNIL  
	{
		$$ = createExpr(NULL, NIL_E, tmpCV, NULL);
		fprintf(outFile, "Line: %ld, keyword 'nil'\n", yylineno);	
	}
	| keyTRUE	
	{
		tmpCV.bool = TRUE;
		$$ = createExpr(NULL, CBOOL_E, tmpCV, NULL);
		fprintf(outFile, "Line: %ld, keyword 'true'\n", yylineno);	 
	}
	| keyFALSE	 
	{
		tmpCV.bool = FALSE;
		$$ = createExpr(NULL, CBOOL_E, tmpCV, NULL);
		fprintf(outFile, "Line: %ld, keyword 'false'\n", yylineno);  
	}
	;
	
idlist	: typeIDENTIFIER
		{
			fprintf(outFile, "Line: %ld, idlist: ID: '%s'\n", yylineno, $1);
			if (lookup(curTable, $1, &tmpSym) == LOCAL)
			{
				yyerror("Error: ID already in idlist\n");
				exit(-1);
			}
			insToTable(curTable, createSymbol(ARG, $1, yylineno));
		}
	idlistEXT	{  fprintf(outFile, "Line: %ld, idlistEXT\n", yylineno);	 }
	|
	{
		insToTable(curTable, createSymbol(ARG, "0", yylineno));
	}
	;

idlistEXT	:semCOMMA typeIDENTIFIER idlistEXT
			{
				fprintf(outFile, "Line: %ld, idlistEXT: ID: '%s'\n", yylineno, $2);
				if (lookup(curTable, $2, &tmpSym) == LOCAL)
				{
					yyerror("Error: ID already in idlist\n");
					exit(-1);
				}
				insToTable(curTable, createSymbol(ARG, $2, yylineno) );
				fprintf(outFile,"  --> Inserted to symbol table\n");
			}
		| //empty
		;
	
ifstmt	:	ifONLYstmt { }
		|	ifelsestmt { }
		;
		
ifONLYstmt	: keyIF ifexpr stmt %prec IFd
	{
		Quad *tmpQuad = getQuadByLine(pop(ifStack));
		tmpQuad->label = nextQuadLine();	
	}
	;
	
ifexpr	:semLPAR expr semRPAR
	{
		//Change by DsP 20/4/05
		Expr *tmpExpr;
		tmpCV.bool = TRUE;
		tmpExpr = createExpr(NULL, CBOOL_E, tmpCV, NULL);
		push(ifStack, nextQuadLine());
		createQuad(IF_NOTEQ, $2, tmpExpr, NULL);
	}	
	;

ifelsestmt	:keyIF ifexpr thenstmt stmt 
				{
					Quad *tmpQuad = getQuadByLine(pop(ifStack));
					tmpQuad->label = nextQuadLine();
				}
			;

thenstmt	:stmt keyELSE 
				{
					//kazaz
					Quad *tmpQuad = getQuadByLine(pop(ifStack));
					tmpQuad->label = nextQuadLine()+1;	 //dld sti grammi akribws META to goto (bl. apo katw)
					push(ifStack, nextQuadLine());	//dld i grammi pou 8a exei goto end-of-else-block
					createQuad(JUMP, NULL, NULL, NULL); 
				}
			;	 
	
whilestmt	: keyWHILE
            {
                push(whileStack, nextQuadLine());     //krataei to quad pou ypologizei to expr
                                    //whileStack -> L1
            }
            semLPAR expr                                                // CODE: L1 :: expr 
            {
                Expr *tmpExpr;
                push(whileStack, nextQuadLine());         // krataei to quad tou condition
                                    // whileStack ->  L1 L2
		        tmpCV.bool = TRUE;
		        tmpExpr = createExpr(NULL, CBOOL_E, tmpCV, NULL);
		        createQuad(IF_NOTEQ, $4, tmpExpr, NULL);                  // CODE: L2 :: cond
            }
            semRPAR stmt                                                  // CODE: stmt
			{
			     int i,n;
			     Quad *tmpQuad=createQuad(JUMP, NULL, NULL, NULL);       // CODE: L3 :: JUMP-> L2
			     long saveS = pop(whileStack);
			                     // whileStack -> L1
			     tmpQuad->label = pop(whileStack);       // jump -> L1
			                     // whileStack -> (empty)
			     getQuadByLine(saveS)->label = nextQuadLine();       // L2->label = rest of code
                n= - pop(loopEndStack) - 1;
                for (i=0;i<n; i++)
                {
                    getQuadByLine(pop(loopEndStack))->label = nextQuadLine();
                }
                pop(loopStartStack);

            }
            ;
		
forstmt     : keyFOR semLPAR elist semCOLON
            {
                push(loopStartStack, nextQuadLine());   // Krataei to jump address otan doume continue
                push(loopEndStack, -1);                 // Krataei to jump otan doume break. MUST be backpatched

                push (forStack, nextQuadLine());        // L2:: Grammi ypologismou tou expr
                    //forStack:  | L2
            }
            expr semCOLON   
            {
                push(forStack, nextQuadLine());         // L3:: Grammi pou kanei to check tou condition
                    //forStack:  | L2 L3
                Expr *tmpExpr;
                tmpCV.bool = TRUE;
                tmpExpr = createExpr(NULL, CBOOL_E, tmpCV, NULL);
                createQuad(IF_NOTEQ, $6 , tmpExpr, NULL);       // if expr != TRUE -> JUMP
                push(forStack, nextQuadLine());         // L4:: Grammi pou kanei jump sto stmt (L7)
                    //forStack:  | L2 L3 L4
                createQuad(JUMP, NULL, NULL, NULL);
                push(forStack, nextQuadLine());         // L5:: Grammi pou exei to elist2
                    //forStack:  | L2 L3 L4 L5
            }
            elist semRPAR    
            {
                push(forStack, nextQuadLine());         // L6:: Grammi pou kanei jump ston ypologismo tou expr+cond (L2)
                    //forStack:  | L2 L3 L4 L5 L6
                createQuad(JUMP, NULL, NULL, NULL);     // jump L2
                push(forStack, nextQuadLine());         // L7:: Grammi pou exei to stmt
                    //forStack:  | L2 L3 L4 L5 L6 L7
            }
            stmt              
            {
                long int L[10];
                int i,n;
                push(forStack, nextQuadLine());         // L8:: Grammi pou kanei jump sto elist2 (L5)
                    //forStack:  | L2 L3 L4 L5 L6 L7 L8
                createQuad(JUMP, NULL, NULL, NULL);
                push(forStack, nextQuadLine());         // L9:: Grammi pou exei to rest-of-code
                    //forStack:  | L2 L3 L4 L5 L6 L7 L8 L9
                //Arxizei to backpatching! :-)
                L[0] = L[1] = 0;
                for (i=9;i>=2;i--)
                    L[i] = pop(forStack);
                getQuadByLine(L[3])->label = L[9];
                getQuadByLine(L[4])->label = L[7];
                getQuadByLine(L[6])->label = L[2];
                getQuadByLine(L[8])->label = L[5];
                
                // Edw kanoume backpatch to loopEndStack wste na doulevei kala
                //	i break;
                n= - pop(loopEndStack) - 1;
                for (i=0;i<n; i++)
                {
                    getQuadByLine(pop(loopEndStack))->label = nextQuadLine();
                }
                pop(loopStartStack);
            }
            ;
		
returnstmt	: keyRETURN	semCOLON
			{
		long i = pop(funcStack);
		if (i==0)
		{
			yyerror("return out of function definition");
			exit(-1);
		}
			    // for breaks/continues
                pop(loopStartStack);
                pop(loopEndStack);
                createQuad(RETURN, NULL, NULL, NULL);
		push(funcStack, nextQuadLine() );
		push(funcStack, i+1);
		createQuad(JUMP, NULL, NULL, NULL);
            }
	       	| keyRETURN expr semCOLON
			{
		long i = pop(funcStack);
		if (i==0)
		{
			yyerror("return out of function definition");
			exit(-1);
		}
			    // for breaks/continues
                pop(loopStartStack);
                pop(loopEndStack);
			  createQuad(RETURN, $2, NULL, NULL);
		push(funcStack, nextQuadLine() );
		push(funcStack, i+1 );
		createQuad(JUMP, NULL, NULL, NULL);
            }
		;
%%
int	 yyerror(char *s) 
{
	fprintf(stderr, "Line: %ld,	 %s\n",yylineno, s);
	exit(-1);
	return 1;
}

void fixLibs(void)
{
	int i;
	char *libs[]={"print","input","tableindices","tablelength","tablecopy","totalarguments","typeof","strtonum","argument","sqrt","cos","sin"};
	for(i=0;i<12;i++)
	{
		insToTable(curTable,createSymbol(LIBFUNC,libs[i],0));
	}
//	curTable->totArg -= 12;
}
	

int main(int argc, char *argv[]) 
{
	Quad **quadTable;
	int totQuadLines,isBinary=1;
	FinalCode *finalCode;

    FILE *outputFile = stdout;
    FILE *inputFile = stdin;

	curTable = createTable();
	curTable->isFunc = 1;	//dilwnei oti to table einai global
	curTable->scope = 0;	// Arxizoume apo GLOBAL scope
	outFile = fopen("/dev/null", "w");
	tmpSym = malloc(sizeof(Symbol));
	
	ifStack = initStack();
	forStack = initStack();
    whileStack = initStack();
    
    loopStartStack = initStack();
    loopEndStack = initStack();

	funcStack = initStack();  
  
	push(funcStack, 0);
    push(loopStartStack, 0);
    push(loopEndStack, 0);

    textcolor(1, 1, 0);
    fprintf(stdout, "\nAlpha Compiler v1.0\n");
    fprintf(stdout, "\tby DsP & AsCii...\n\n");
    textcolor(4, 6, 0);

    switch(argc)
    {
        case 0:  //vasili ton ksyneis?? ;) pos tha ginei afto?
            fprintf(stderr, "Panic. Fatal Error: argc=0\n");
            exit(-1);
        case 1:
            printf("Using standard IO streams.\n");
		textcolor(0,7,0);
            printf("Enter code:\n");
            break;
        case 2: //./ac input_filename
            printf("Using \"%s\" for input.\n", argv[1]);
            if ( (inputFile=fopen(argv[1], "r")) == NULL )
            {
                perror("fopen()");
                exit(errno);
            }
            break;
	case 3: // ./ac input -b/-t
	    perror("Usage: ac <input_file> < -b/-t > <output_file> \n");
	    perror("-b for binary output , -t for text");
	    exit(-1);
	    break;
        case 4:
	    if(strcmp(argv[2],"-b") == 0)
	    {
		printf("binary output selected. \n");
	    }
	    else if(strcmp(argv[2],"-t") !=0)
	    {
	        perror("Usage: ac <input_file> < -b/-t > <output_file> \n");
     	        perror("-b for binary output , -t for text");
		exit(-1);
	    }
	    else
	    {
	    	printf("text output selected. \n");
		isBinary=0;
	    }
            printf("Using \"%s\" for input.\n", argv[1]);
            printf("Using \"%s\" for output.\n", argv[3]);
            if ( ((inputFile=fopen(argv[1], "r")) == NULL) || ( (outputFile=fopen(argv[3], "w")) ==NULL) )
            {
                perror("fopen()");
                exit(errno);
            }
            break;
        default:
		textcolor(0,2,0);
            printf("Invalid arguments.\n");
            printf("\nUsage:\n");
            printf("%s [ input_file ].\n", argv[0]);
            exit(0);
    }
		textcolor(0,7,0);
        yyin = inputFile;

	fixLibs();
	
	yyparse();

	printf("\n\n");
	printQuadTable(stdout);
	printf("\n\n");
	
	init_finalCode();
	totQuadLines = getQuadTable(&quadTable);
	finalCode = createFinalCode(quadTable, totQuadLines);
	
	if(isBinary == 0 )
		print_TextFile(outputFile, finalCode);
	else
	print_BinaryFile(outputFile, finalCode);
	
	for (;;)
	{
		if (curTable->root == NULL)
			break;
		curTable	= curTable->root;
	}

	return 0;
}

void textcolor(int attr, int fg, int bg){	char command[13];	/* Command is the control command to the terminal */	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);	printf("%s", command);}

void breakP() { }
