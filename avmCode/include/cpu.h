#ifndef __CPU_HEADER
#define __CPU_HEADER

#include "general.h"
#include "instruction.h"
#include "cpu.h"
#include "memCell.h"
#include "activationRecord.h"

#define REGISTERNUM 3

class CPU
{
	friend class Instruction;
	friend class ActivationRecord;
	friend MemCell* operandToMemCell(Operand,int);

	public:
			static CPU* CPUInstance(int i);
			static CPU* CPUInstance();
			static int numOfInstances();
			bool addToMemStack(int=1);

			void memPush(MemCell *);
			MemCell *memPop();

			long memStackSize()	{	return (long)this->memStack.size();		}
			void stackDump();

			int getRecordNum()	{	return this->actRecStack.size();		}
			ActivationRecord *getRecord()	{  return this->actRecStack[ this->actRecStack.size()-1 ];  }
			ActivationRecord *popRecord()	
				{
					ActivationRecord *retAct = this->actRecStack[ this->actRecStack.size()-1 ];  
					this->actRecStack.pop_back();  
					return retAct;
				}
			void putRecord(ActivationRecord *pAR) { actRecStack.push_back(pAR);  }
		
			int pc;
			MemCell *retVal;
			vector<MemCell*>		paramVector;
	protected:
			// The CPU registers
			int  top, topsp;

			vector<MemCell*>		memStack;	// The CPU's stack
			vector<ActivationRecord*>	actRecStack;	// The activation Record

			// We implement the singleton design pattern. The constructors are protected
			CPU();
	private:
			static vector<CPU*> cpuInsVec;
};


#endif
