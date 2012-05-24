#ifndef __MEM_HEADER
#define __MEM_HEADER

#include "general.h"
#include "instruction.h"
#include "cpu.h"
#include <cstdlib>
#include <cstdio>

typedef static struct userFunc
{
	unsigned int address,localsize;
	string * id;
} UserFunc;

class MEM
{
	public:
		static MEM* MEMInstance();	
		
		int 		getInstructionsNum()			{  return this->instructionMem.size();	}
		Instruction * 	getInstructionElem(int i)		{  return this->instructionMem[i];  	}
		string 		getStringElem(int i)			{  return this->stringMem[i];  		}
		double 		getNumElem(int i)			{  return this->numMem[i];   		}
		UserFunc* 	getUserFuncElem(int i)			{  return this->usrFuncMem[i];		}
//		{ 
//		      UserFunc funcyou;
//		      funcyou.address = this->usrFuncMem[i].address;
//		      funcyou.localsize = this->usrFuncMem[i].localsize;
//		      funcyou.id = this->usrFuncMem[i].id;
//		      UserFunc* funcyoup = &funcyou; 
//		      return funcyoup; 		
//		}
		string		getLibFuncElem( int i )			{  return this->libFuncMem[i];  	}

		bool	loadMemFromTextFile(ifstream&);
		bool	loadMemFromBinaryFile(FILE*);
		void	printMem();
		void	fixGlobalMem();
		int	formalArgNum(int pc);

	protected:
		MEM();

		bool		addToStringMem(string);
		bool		addToNumMem(double);
		bool		addToUsrFuncMem(UserFunc *);
		bool		addToLibFuncMem(string);
		bool		addToInstructionMem(Instruction *);
		Instruction* 	setProperInstruction(OpCode,Operand,Operand,Operand);

		vector<string> 			stringMem;
		vector<double> 			numMem;
		vector<UserFunc *> 		usrFuncMem;
		vector<string> 			libFuncMem;
		vector<Instruction *> 		instructionMem;	

		private:
			static MEM *pMEMInstance;
};

#endif
