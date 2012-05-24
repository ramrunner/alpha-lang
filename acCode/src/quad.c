/* 
*  Source File for quad manipulation
*  DsP , 10/04/05
*/
#include "quad.h"

/*the table of quads , each indice is a line*/
static Quad** QuadTable;
static unsigned int  label=1;
static unsigned long line=1,qtsize=1024;

char *opCodeString[]= { "err", "ASSIGN\t", "ADD\t\t", "SUB\t\t", "MUL\t\t", "DIV\t\t", "MOD\t\t", "QUMINUS\t", "AND\t\t"
                        , "OR\t", "NOT\t\t", "IF_EQ\t", "IF_NOTEQ\t", "IF_LESSEQ\t", "IF_GREATEREQ\t"
                        , "IF_LESS\t", "IF_GREATER\t", "JUMP\t", "CALL\t", "PARAM\t", "RETURN\t"
                        , "GETRETVAL\t", "FUNCSTART\t", "FUNCEND\t", "TBLCREATE\t"
                        , "TBLGETELEM\t", "TBLSETELEM\t", "NOP\t\t" };

int getQuadTable(Quad ***pppQuad)
{
	(*pppQuad) = QuadTable;
	return line;
}

void printQuadTable(FILE *out)
{
	int i;
	for (i=1;i<line;i++)
		printQuad(out, QuadTable[i]);
	textcolor(0,7,0);
}

void printQuad(FILE *out, Quad *pQuad)
{
	textcolor(3, 7, 0);
    fprintf(out, "(line)%ld \t", pQuad->line);
    textcolor(1, 2, 0);
	fprintf(out, "%s      ",opCodeString[pQuad->opcode]);
	fprintf(out, "%s\t\t",printExpr(pQuad->arg1));
	fprintf(out, "%s\t\t",printExpr(pQuad->arg2));
	fprintf(out, "%s\t\t",printExpr(pQuad->arg3));
	textcolor(0, 4, 0);
	fprintf(out, "(labe)%d   \n",pQuad->label);
}

static int createQuadTable()
{
	if((QuadTable =(Quad**) calloc(qtsize,sizeof(Quad*))) == NULL)
	{
		perror("Could Not Create the Quad Table");
		exit(-1);
	}
	else
	{
		QuadTable[0]=NULL; /*in order to match line increment*/
		return 1;
	}
}

static int addToQuadTable(Quad* pQuad)
{
	if(line==qtsize-2) /* WE NEED MEMMORY YUMMY YUMMY! */
	{
		if(realloc(QuadTable,2*(qtsize*sizeof(Quad*))) == NULL)
		{
			perror("Could not reallocate space for Quad Table");
			exit(-1);
		}
		else
		{
			qtsize*=2;
			QuadTable[line] = pQuad;
			return(line);
		}
	}
	else
	{
		QuadTable[line] = pQuad;
		return(line);
	}
}
		
	
Quad* createQuad(Opcode opcode, struct expr* arg1, struct expr* arg2, struct expr* arg3)
{
	Quad* pQuad;
	if(line==1)
		createQuadTable();
	if((pQuad = malloc(sizeof(Quad))) == NULL)
	{
		perror("Quad Allocation Error");
		exit(-1);
	}
	else
	{
		pQuad->opcode = opcode;
		pQuad->arg1 = arg1;
		pQuad->arg2 = arg2;
		pQuad->arg3 = arg3;
		pQuad->label = 0;
		pQuad->line = line;
		addToQuadTable(pQuad);
		line++; /*every time a quad is born , a line is increased*/
		return pQuad;
	}
}

/*this Function gives a unique label to each quad*/
int markQuad(Quad * quad)
{
	if(quad->label != 0)
	{
		perror("Quad requested has already been assigned to a label");
		assert(0);
	}
	else
	{
		quad->label = label;
		return(label++); /*same as createQuad , the label is increased but AFTER it is returned*/
	}	
}

Quad* getQuadByLine(long qline)
{
    if (qline >= line)
        assert(0);
	return QuadTable[qline];
}
Quad* getQuadByLabel(int qlabel)
{
	int i;
	for(i=1;i<line;i++)
	{
		if(QuadTable[i]->label == qlabel)
			return QuadTable[i];
	}
	/*shouldn't reach this if quad exists*/
	perror("Quad not found");
	exit(-1);
		
}
long nextQuadLine()
{
	return(line);
}

