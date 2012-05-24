#include "activationRecord.h"
#include "error.h"

static MemCell *newNumCell(double num);
static MemCell *newCell();

ActivationRecord::ActivationRecord() { ActivationRecord::ActivationRecord(0, 0); }
ActivationRecord::ActivationRecord(int localNO, int formalNO)
{
	this->localNO = localNO;
	this->formalNO = formalNO;
	this->state = false;
}

void ActivationRecord::deactivate()
{
	if (!this->state)
		throw new Error("ActivationRecord::deactivate(): Record is inactive");
	CPU *curCPU = CPU::CPUInstance();
	int i;
	for (i=0; i<this->localNO; i++)
		curCPU->memPop();		// pop the local var memCells
	int new_topsp = (int)curCPU->memPop()->data.numVal;
	int new_top = (int)curCPU->memPop()->data.numVal;
	int new_pc = (int)curCPU->memPop()->data.numVal;
	curCPU->memPop();			// pop the number of formal args
	for (i=0; i<this->formalNO; i++)
		 curCPU->memPop();		// pop the formal arguments var

	curCPU->top = new_top;
	curCPU->topsp = new_topsp;
	curCPU->pc = new_pc;
//	*cdeb << "setting new pc = " << curCPU->pc << endl;
	curCPU->memStack.resize(curCPU->top);
	this->state = false;

	delete this;				// When deactivated, it is no longer
						// needed...
}

void ActivationRecord::activate()
{
	if (this->state)
		throw new Error("ActivationRecord::activate(): Record in already active");
	CPU *curCPU = CPU::CPUInstance();
	int i;
	int saved_top = curCPU->top;
	int saved_topsp = curCPU->topsp;
	int saved_pc = this->retAddress;
	for (i=0;i<curCPU->paramVector.size();i++)
		curCPU->memPush( curCPU->paramVector[i] );
	for (i=curCPU->paramVector.size(); i<formalNO; i++)
		curCPU->memPush( newCell() );
		
	curCPU->paramVector.resize(0);

	curCPU->memPush( newNumCell(formalNO) );	//push ton ari8mo twn formal args
	curCPU->memPush( newNumCell(saved_pc) );
	curCPU->memPush( newNumCell(saved_top) );
	curCPU->memPush( newNumCell(saved_topsp) );

	curCPU->topsp = curCPU->top;			// TWRA to top deixnei ekei pou prepei na 
							// deixnei o topsp.

	for (i=0; i<this->localNO; i++)
		curCPU->memPush( newCell() );

	this->state = true;
}

static MemCell *newNumCell(double num)
{
	MemCell *ret = newCell();
	ret->type = CELL_NUM;
	ret->data.numVal = num;
	return ret;
}

static MemCell *newCell()
{
	MemCell *ret = (MemCell *)malloc(sizeof(MemCell));
	ret->type = CELL_UNDEF;
	return ret;
}
