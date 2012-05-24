#define __IN_FINALCODE_C
#include "finalCode.h"

static Instruction *finalCode;
static int insCounter;
static int insSize;

struct cIndex *ind;

void print_TextFile(FILE *outFile, FinalCode *code)
{
	
	//First print the magic number
	fprintf(outFile, "%lld\n", code->magicNumber);
	
	//Print the constant value tables
		//Strings
		{
			struct cIndex strings = code->constIndex[0];
			char **strArray = (char **)(strings.data);
			int total = strings.index;
			int i;
			fprintf(outFile, "%d", total);
			for (i=0;i<total;i++)
				fprintf(outFile, " %d %s\0", (int)strlen(strArray[i]), strArray[i]);
			fprintf(outFile, "\n");
		} 
		//Numbers
		{
			struct cIndex numbers = code->constIndex[1];
			double *numArray = (double *)(numbers.data);
			int total = numbers.index;
			int i;
			fprintf(outFile, "%d ", total);
			for (i=0; i<total; i++)
				fprintf(outFile, "%f ", numArray[i]);
			fprintf(outFile, "\n");
		}
		// Usr Functions
		{
			struct cIndex userFunctions = code->constIndex[2];
			usrFunc *usrFuncArray = (usrFunc *)(userFunctions.data);
			int total = userFunctions.index;
			int i;
			fprintf(outFile, "%d ", total);
			for (i=0; i<total; i++)
				fprintf(outFile, "%ld %d %s ", usrFuncArray[i].iaddress, usrFuncArray[i].localSize, usrFuncArray[i].id);
			fprintf(outFile, "\n");
		}
		// Lib Functions
		{
			struct cIndex libFunctions = code->constIndex[3];
			char **libFuncArray = (char **)(libFunctions.data);
			int total = libFunctions.index;
			int i;
			fprintf(outFile, "%d", total);
			for (i=0; i<total; i++)
				fprintf(outFile, " %d %s\0", (int)strlen(libFuncArray[i]), libFuncArray[i]);
			fprintf(outFile, "\n");
		}
		// Code
		{
			Instruction *codeArray = code->code;
			int total = code->codeSize;
			int i;
			
			fprintf(outFile, "%d\n", total);
			for (i=0; i<total; i++)
			{
				fprintf(outFile, "%d\t", codeArray[i].opCode);								// opCode
				fprintf(outFile, "%d %ld\t", codeArray[i].arg1.code, codeArray[i].arg1.val);	// code:val
				fprintf(outFile, "%d %ld\t", codeArray[i].arg2.code, codeArray[i].arg2.val);	// code:val
				fprintf(outFile, "%d %ld\t", codeArray[i].arg3.code, codeArray[i].arg3.val);	// code:val
				fprintf(outFile, "\n");
			}
		}
}

void print_BinaryFile(FILE *outFile, FinalCode *code)
{
	//First print the magic number
	fwrite(&(code->magicNumber),sizeof(long long),1,outFile);
	
	//Print the constant value tables
		//Strings
		{
			struct cIndex strings = code->constIndex[0];
			char **strArray = (char **)(strings.data);
			int total = strings.index;
			int i;
			//fprintf(outFile, "%d", total);
			fwrite(&total,sizeof(int),1,outFile);
			for (i=0;i<total;i++)
			{
				int aint = (int)strlen(strArray[i]);
				fwrite(&aint,sizeof(int),1,outFile);
				fwrite(strArray[i],sizeof(char),(strlen(strArray[i])),outFile);
				//fprintf(outFile, " %d %s\0", (int)strlen(strArray[i]), strArray[i]);
			}
			//fprintf(outFile, "\n");
		} 
		//Numbers
		{
			struct cIndex numbers = code->constIndex[1];
			double *numArray = (double *)(numbers.data);
			int total = numbers.index;
			int i;
			//fprintf(outFile, "%d ", total);
			fwrite(&total,sizeof(int),1,outFile);
			for (i=0; i<total; i++)
			{
				fwrite(&numArray[i],sizeof(double),1,outFile);
			//	fprintf(outFile, "%f ", numArray[i]);
			}
			//fprintf(outFile, "\n");
		}
		// Usr Functions
		{
			struct cIndex userFunctions = code->constIndex[2];
			usrFunc *usrFuncArray = (usrFunc *)(userFunctions.data);
			int total = userFunctions.index;
			int i;
			//fprintf(outFile, "%d ", total);
			fwrite(&total,sizeof(int),1,outFile);
			for (i=0; i<total; i++)
			{
			//	fprintf(outFile, "%ld %d %s ", usrFuncArray[i].iaddress, usrFuncArray[i].localSize, usrFuncArray[i].id);
				int xi;
				fwrite(&(usrFuncArray[i].iaddress),sizeof(long),1,outFile);
				fwrite(&(usrFuncArray[i].localSize),sizeof(int),1,outFile);
				xi = (int) (strlen(usrFuncArray[i].id));
				fwrite(&xi,sizeof(int),1,outFile);
				fwrite(&(usrFuncArray[i].id),sizeof(char),xi,outFile);
			}
			//fprintf(outFile, "\n");
		}
		// Lib Functions
		{
			struct cIndex libFunctions = code->constIndex[3];
			char **libFuncArray = (char **)(libFunctions.data);
			int total = libFunctions.index;
			int i;
			//fprintf(outFile, "%d", total);
			fwrite(&total,sizeof(int),1,outFile);
			for (i=0; i<total; i++)
			{
				int aint = (int)strlen(libFuncArray[i]);
				//fprintf(outFile, " %d %s\0", (int)strlen(libFuncArray[i]), libFuncArray[i]);
				fwrite(&aint,sizeof(int),1,outFile);
				fwrite(libFuncArray[i],sizeof(char),(strlen(libFuncArray[i])),outFile);
			}
			//fprintf(outFile, "\n");
		}
		// Code
		{
			Instruction *codeArray = code->code;
			int total = code->codeSize;
			int i;
			
			//fprintf(outFile, "%d\n", total);
			fwrite(&total,sizeof(int),1,outFile);
			for (i=0; i<total; i++)
			{
				//fprintf(outFile, "%d\t", codeArray[i].opCode);				// opCode
				fwrite(&(codeArray[i].opCode),sizeof(int),1,outFile);
				//fprintf(outFile, "%d %ld\t", codeArray[i].arg1.code, codeArray[i].arg1.val);	// code:val
				fwrite(&(codeArray[i].arg1.code),sizeof(int),1,outFile);
				fwrite(&(codeArray[i].arg1.val),sizeof(long),1,outFile);
				//fprintf(outFile, "%d %ld\t", codeArray[i].arg2.code, codeArray[i].arg2.val);	// code:val
				fwrite(&(codeArray[i].arg2.code),sizeof(int),1,outFile);
				fwrite(&(codeArray[i].arg2.val),sizeof(long),1,outFile);
				//fprintf(outFile, "%d %ld\t", codeArray[i].arg3.code, codeArray[i].arg3.val);	// code:val
				fwrite(&(codeArray[i].arg3.code),sizeof(int),1,outFile);
				fwrite(&(codeArray[i].arg3.val),sizeof(long),1,outFile);
				//fprintf(outFile, "\n");
			}
		}

}

void init_finalCode()
{
	int i;
	finalCode = calloc(INSTRUCTION_SIZE, sizeof(Instruction));
	insCounter = 0;
	insSize = INSTRUCTION_SIZE;
	
	assert(finalCode);
	
	ind = malloc(sizeof(struct cIndex)*4);
			// 0 -> string Index
			// 1 -> number Index
			// 2 -> usrFunc Index
			// 3 -> libFunc Index
	ind[0].data = malloc(sizeof(char *)*CINDEX_SIZE);
	ind[1].data = malloc(sizeof(double)*CINDEX_SIZE);
	ind[2].data = malloc(sizeof(usrFunc)*CINDEX_SIZE);
	ind[3].data = malloc(sizeof(char *)*CINDEX_SIZE);
	for (i=0;i<4;i++)
	{
		ind[i].index = 0;
		ind[i].maxData = CINDEX_SIZE;
	}
	
}

FinalCode *createFinalCode(Quad **quadTable, int totLine)
{
	int i;
	
	FinalCode *retFinalCode = malloc(sizeof(FinalCode));
	retFinalCode->magicNumber = 340200501;
	
	for (i=1; i<totLine; i++)
		insToFinalCode( quadToInstruction(quadTable[i]) );
	retFinalCode->code = finalCode;
	
	for (i=0; i<4; i++)
		retFinalCode->constIndex[i] = ind[i];
	retFinalCode->codeSize = insCounter;
	
	return retFinalCode;
}

static void insToFinalCode(Instruction ins)
{
	if (insCounter == insSize - 1)
	{
		finalCode = realloc(finalCode, sizeof(Instruction)*(insSize+INSTRUCTION_SIZE));
		insSize += INSTRUCTION_SIZE;
	}
	assert(finalCode);
	assert(insCounter <= insSize);
	finalCode[insCounter++] = ins;
}

static Operand exprToOperand(Expr *pExpr)
{
	Operand retOp;
	retOp.code = -1;
	retOp.val = -1;	
	breakP();
	if (pExpr == NULL)
	{
		retOp.code = 0;
		retOp.val = 0;
		return retOp;
	}
	
	switch (pExpr->type)
	{
		case VAR_E: case NEWTABLE_E: case TABLEITEM_E: case ASSIGN_E:
		{
			if (pExpr->sym == NULL)
			{
				fprintf(stderr, "Fatal Internal Error\n");
				exit(-1);
			}
			if (pExpr->sym->type == ARG)
				retOp.code = 2;
			else
			{
				Table *tmp = pExpr->sym->parent;
				assert(tmp);
				retOp.code = 1;
				while(tmp->root != NULL)
				{
					tmp = tmp->root;
					if (tmp->slist->data->type == ARG)
					{
						retOp.code = 3;
						break;
					}	
				}
			}
//			else if (pExpr->sym->parent->scope == 0)
//				retOp.code = 1;
//			else
//				retOp.code = 3;
			retOp.val = pExpr->sym->localOffset;
			return retOp;
			break;
		}
		case USRFUNC_E:
		{
			int i;
			usrFunc newUsr;
			retOp.code = 8;
			for (i=0;i<ind[2].index;i++)
				if ( !strcmp( ((usrFunc *)(ind[2].data))[i].id, pExpr->sym->name) )
				{
					retOp.val = i;
					return retOp;
				}
			newUsr.iaddress = pExpr->sym->iaddress;
			List *tmpList = pExpr->sym->ptable->slist;
			while (tmpList->next != NULL)
				tmpList = tmpList->next;
			if (tmpList->data->type != BLOCK)
				assert(0);
			newUsr.localSize = tmpList->data->ptable->totArg;
			newUsr.id = pExpr->sym->name;
//			printf("usrfunc %s , sz %d , iadd %ld\n", newUsr.id, newUsr.localSize, newUsr.iaddress);
			((usrFunc *)ind[2].data)[ind[2].index] = newUsr;
			retOp.val = (ind[2].index)++;
			return retOp;
			break;
		}
		case LIBFUNC_E:
		{
			int i;
			retOp.code = 9;
			for (i=0; i<ind[3].index; i++)
				if ( !strcmp( ((char **)ind[3].data)[i], pExpr->sym->name) )
				{
					retOp.val = i;
					return retOp;
				}
			((char **)ind[3].data)[ind[3].index] = pExpr->sym->name;
			retOp.val = (ind[3].index)++;
			return retOp;
			break;
		}
		case CSTRING_E:
		{
			int i;
			retOp.code = 5;
			for (i=0; i<ind[0].index; i++)
				if ( !strcmp( ((char **)ind[0].data)[i], pExpr->cv.str) )
				{
					retOp.val = i;
					return retOp;
				}
			((char **)ind[0].data)[ind[0].index] = pExpr->cv.str;
			retOp.val = (ind[0].index)++;
			return retOp;
			break;
		}
		case CBOOL_E:
		{
			retOp.code = 6;
			if (pExpr->cv.bool == TRUE)
				retOp.val = 1;
			else
				retOp.val = 0;
			return retOp;
			break;
		}
		case CNUM_E:
		{
			int i;
			retOp.code = 4;
			for (i=0; i<ind[1].index; i++)
				if ( ((double *)ind[1].data)[i] == pExpr->cv.num )
				{
					retOp.val = i;
					return retOp;
				}
			((double *)ind[1].data)[ind[1].index] = pExpr->cv.num;
			retOp.val = (ind[1].index)++;
			return retOp;			
			break;
		}
		default:
		{
			fprintf(stderr, "ERROR: pExpr->type = %d\n", pExpr->type);
			assert(0);
		}
	}
}

static Instruction quadToInstruction(Quad *pQuad)
{
	Instruction retIns;
	retIns.opCode = pQuad->opcode;
	retIns.arg1 = exprToOperand(pQuad->arg1);
	retIns.arg2 = exprToOperand(pQuad->arg2);
	if ( (pQuad->opcode >= IF_EQ) && (pQuad->opcode <= JUMP) )
	{
		retIns.arg3.code = 0;
		retIns.arg3.val = pQuad->label;
	}	
	else
		retIns.arg3 = exprToOperand(pQuad->arg3);
	return retIns;	
}
