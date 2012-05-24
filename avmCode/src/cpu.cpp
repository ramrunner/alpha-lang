#include "cpu.h"
#include "error.h"
#include "memCell.h"

/***
int main()
{
	*cdeb << "Test of cpu.cpp (includes error.cpp)" << endl;
	try
	{
		CPU *pc1 = CPU::CPUInstance();		*cdeb << "pc1 = " << hex << pc1 << endl;
		CPU *pc2 = CPU::CPUInstance(0);		*cdeb << "pc2 = " << hex << pc2 << endl;
		CPU *pc3 = CPU::CPUInstance(1);		*cdeb << "pc3 = " << hex << pc3 << endl;
		CPU *pc4 = CPU::CPUInstance(3);	// this should throw error
	}
	catch (Error* e)
	{
		*cdeb << e->getMsg() << endl;
		delete e;
	}
	return 0;
}
***/

static void breakE() { }

vector<CPU*> CPU::cpuInsVec;

void CPU::stackDump()
{
	int i;
	for (i=memStack.size()-1; i>=0; i--)
		*cdeb << printMemCell(memStack[i]) << endl;
}

CPU* CPU::CPUInstance(int i)
{
	if (i == cpuInsVec.size())
	{
		cpuInsVec.push_back(new CPU());
		return cpuInsVec[i];
	}
	else if (i>=0 && i<cpuInsVec.size())
		return cpuInsVec[i];
	else
		throw new Error("Error: from CPU::CPUInstance(int)  -> Invalid argument");
}

void CPU::memPush(MemCell *pM)
{
	breakE();
	if ( this->top <=  (signed int)this->memStack.size()-1 )
		this->memStack[this->top] = pM; 
	else
		this->memStack.push_back( pM );
	this->top++;
}

MemCell *CPU::memPop()  {  return this->memStack[--(this->top)];  }

bool CPU::addToMemStack(int n)
{
	MemCell *newCell;
	int i;
	bool ret = false;
	for (i=0; i<n; i++)
	{
		newCell = (MemCell *)malloc(sizeof(MemCell));
		newCell->type = CELL_UNDEF;
		this->memPush(newCell);
		ret = true;
	}
	return ret;
}

CPU* CPU::CPUInstance()
{
	if (cpuInsVec.size() == 0)
		cpuInsVec.push_back(new CPU());
	return cpuInsVec[cpuInsVec.size()-1];
}

int CPU::numOfInstances()  {  return cpuInsVec.size();  }

CPU::CPU() 
{ 
	this->pc 	= 1;	// Make the program counter to point to
				// the first line of the code
	this->top 	= 0;
	this->topsp 	= 0;

	this->retVal = (MemCell *)malloc(sizeof(MemCell));
	this->memStack = vector<MemCell *>(0);
}
