#ifndef __INSTRUCTION_HEADER
#define __INSTRUCTION_HEADER

#include "general.h"
#include "memCell.h"
#include "operand.h"

typedef enum opCode
{
	UNDEF = -1, ASSIGN=1, ADD , SUB , MUL , DIV , MOD , QUMINUS , AND , OR , NOT \
	, IF_EQ, IF_NOTEQ , IF_LESSEQ, IF_GREATEREQ, IF_LESS, IF_GREATER, JUMP \
	, CALL , PARAM , RETURN , GETRETVAL , FUNCSTART , FUNCEND , TABLECREATE \
	, TABLEGETELEM , TABLESETELEM, NOP 
} OpCode;

class Instruction
{
	public:
		Instruction();              //Default constructor
		Instruction(Operand, Operand, Operand);
		~Instruction();

		vector<Operand> getOperands();

		virtual void execute(int=0) = 0;
		OpCode getOpCode();
	 	static	bool operandToLogic(MemCell*);
	protected:
		vector<Operand> op;
		OpCode opCode;

};

class Instruction_assign 	: public Instruction 
{ 
	public:
		Instruction_assign(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_add		: public Instruction
{
	public:
		Instruction_add(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_sub		: public Instruction 
{
	public:
		Instruction_sub(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_mul		: public Instruction 
{
	public:
		Instruction_mul(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_div		: public Instruction 
{ 
	public:
		Instruction_div(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_mod		: public Instruction 
{ 
	public:
		Instruction_mod(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_quminus	: public Instruction 
{ 
	public:
		Instruction_quminus(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_and		: public Instruction 
{ 
	public:
		Instruction_and(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_or		: public Instruction 
{ 
	public:
		Instruction_or(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_not		: public Instruction 
{ 
	public:
		Instruction_not(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_ifeq		: public Instruction 
{ 
	    public:
		Instruction_ifeq(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_ifnoteq	: public Instruction 
{ 
	public:
		Instruction_ifnoteq(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_iflesseq	: public Instruction 
{ 
	public:
		Instruction_iflesseq(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_ifgreatereq	: public Instruction 
{ 
	public:
		Instruction_ifgreatereq(Operand,Operand,Operand);
		void execute(int=0);
};

class Instruction_ifless	: public Instruction 
{ 
	public:
		Instruction_ifless(Operand,Operand,Operand);
		void execute(int=0);
};
class Instruction_ifgreater	: public Instruction 
{ 
	public:
	       	Instruction_ifgreater(Operand,Operand,Operand);  
		void execute(int=0);
};

class Instruction_jump		: public Instruction 
{ 
	public:
		Instruction_jump(Operand,Operand,Operand);
                void execute(int=0);                                                                                
};

class Instruction_call		: public Instruction 
{ 
	public:
		Instruction_call(Operand,Operand,Operand);
                void execute(int=0);                                                                                
};

class Instruction_param		: public Instruction 
{ 
	public:
		Instruction_param(Operand,Operand,Operand);
                void execute(int=0);                                                                                
};

class Instruction_return	: public Instruction 
{ 
	public:
		Instruction_return(Operand,Operand,Operand);
                void execute(int=0);                                                                                
};

class Instruction_getretval	: public Instruction 
{ 
	public:
		Instruction_getretval(Operand,Operand,Operand);
                void execute(int=0);                                                                                
};

class Instruction_funcstart	: public Instruction 
{ 
	public:
		Instruction_funcstart(Operand,Operand,Operand);
                void execute(int=0);                                                                                
};

class Instruction_funcend	: public Instruction 
{ 
	public:
		Instruction_funcend(Operand,Operand,Operand);
                void execute(int=0);                                                                                
};

class Instruction_tablecreate	: public Instruction 
{ 
	public:
		Instruction_tablecreate(Operand,Operand,Operand);
                void execute(int=0);                                                                                

};

class Instruction_tablegetelem	: public Instruction 
{ 
	public:
		Instruction_tablegetelem(Operand,Operand,Operand);
                void execute(int=0);                                                                                
};

class Instruction_tablesetelem	: public Instruction 
{ 
	public:
		Instruction_tablesetelem(Operand,Operand,Operand);
                void execute(int=0);                                                                                
};

class Instruction_nop		: public Instruction 
{ 
	public:
		Instruction_nop(Operand,Operand,Operand);
                void execute(int=0);                                                                                
};

#endif
