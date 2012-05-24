/* 
*  Header file for quad manipulation
*  DsP, 10/04/05 
*/
#ifndef __QUAD_HEADER
#define __QUAD_HEADER
#include "symHeader.h"
#include "yak.tab.h"
#include "expr.h"

/*here is the enumeration for the different quad opCodes  TOTAL : 27 elements form 1-27*/
typedef enum opcodes { ASSIGN=1, ADD , SUB , MUL , DIV , MOD , QUMINUS , AND , OR , NOT \
			, IF_EQ, IF_NOTEQ , IF_LESSEQ, IF_GREATEREQ, IF_LESS, IF_GREATER, JUMP \
			, CALL , PARAM , RETURN , GETRETVAL , FUNCSTART , FUNCEND , TABLECREATE \
			, TABLEGETELEM , TABLESETELEM, NOP } Opcode;
/* **** PROSOXI: An alla3ei i seira twn opcodes isws na xriazetai na ginei allagi 
 * kai sto yak.y sto action tis paragwgis tou kanona expr: expr op assignORterm
 * kai sto finalCode.c sti quadToInstruction() .
 */
 
 extern char* opCodeString[];
 
/*declaration of the quad structure */
typedef struct quad 
{
	Opcode opcode;
	struct expr* arg1;
	struct expr* arg2;
	struct expr* arg3;
	int label;
	long line;
} Quad;

/*function declaration */
extern Quad* createQuad(Opcode, struct expr*,struct expr*, struct expr*);
extern int markQuad(Quad*);
extern Quad* getQuadByLine(long);
extern Quad* getQuadByLabel(int);
extern long nextQuadLine();
extern void printQuadTable(FILE *);
extern void printQuad(FILE *, Quad *);
extern int getQuadTable(Quad ***);
#endif
