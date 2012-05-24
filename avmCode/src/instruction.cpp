#include "instruction.h"
#include "error.h"
#include "cpu.h"

void breakD() { }

/*** main() for test purposes
int main()
{
	cout << "Test program for instruction.cpp" << endl << endl;
	cout << "Creating a vector with Instruction * ...";

	vector<Instruction *>	insVec;
	cout << "Done" << endl;

	Operand op;	op.code = 1;	op.val = 10;
	cout << "Putting some Instructions in the vector (subclasses of Instruction)...";
	insVec.push_back( new Instruction_assign(op,op,op) );
	insVec.push_back( new Instruction_jump(op,op,op) );
	insVec.push_back( new Instruction_nop(op,op,op) );
	cout << "Done" << endl;
	breakD();
	cout << "Retrieving Instruction info from vector..." << endl;
	int i,j;
	for (i=0; i<insVec.size(); i++)
	{
		cout << "ins #" << i << " ==> ";
		for (j=0; j<3; j++)
		{
			cout << "	op" << j << "	";
			cout << "code: "<< (insVec[i]->getOperands())[j].code;
			cout << " value: " << (insVec[i]->getOperands())[j].val << endl;
		}
		cout << endl;
	}
	return 0;
}
//***/

Instruction::Instruction() { }
Instruction::~Instruction() { }
Instruction::Instruction(Operand op1, Operand op2, Operand op3)
{
	opCode = UNDEF;
	op.resize(3);
	this->op[0] = (op1);
	this->op[1] = (op2);
	this->op[2] = (op3);
}

OpCode Instruction::getOpCode()			{	return opCode;	}
vector<Operand> Instruction::getOperands() 	{  	return op;  	}

Instruction_assign::Instruction_assign(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)	{	this->opCode = ASSIGN;	}
Instruction_add::Instruction_add(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)		{	this->opCode = ADD;	}
Instruction_sub::Instruction_sub(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)		{	this->opCode = SUB;	}
Instruction_mul::Instruction_mul(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)		{	this->opCode = MUL;	}
Instruction_div::Instruction_div(Operand op1, Operand op2, Operand op3)	: Instruction(op1, op2, op3)		{	this->opCode = DIV;	}
Instruction_mod::Instruction_mod(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)		{	this->opCode = MOD;	}
Instruction_quminus::Instruction_quminus(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)	{	this->opCode = QUMINUS;	}

Instruction_and::Instruction_and(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)		{	this->opCode = AND;	}
Instruction_or::Instruction_or(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)		{	this->opCode = OR;	}
Instruction_not::Instruction_not(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)		{	this->opCode = NOT;	}

Instruction_ifeq::Instruction_ifeq(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)		{	this->opCode = IF_EQ;		}
Instruction_ifnoteq::Instruction_ifnoteq(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)	{	this->opCode = IF_NOTEQ;	}
Instruction_iflesseq::Instruction_iflesseq(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)	{	this->opCode = IF_LESSEQ;	}
Instruction_ifgreatereq::Instruction_ifgreatereq(Operand op1,Operand op2,Operand op3):Instruction(op1,op2,op3)	{	this->opCode = IF_GREATEREQ;	}
Instruction_ifless::Instruction_ifless(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)	{	this->opCode = IF_LESS;		}
Instruction_ifgreater::Instruction_ifgreater(Operand op1, Operand op2, Operand op3):Instruction(op1,op2,op3)	{	this->opCode = IF_GREATER;	}
Instruction_jump::Instruction_jump(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)		{	this->opCode = JUMP;		}

Instruction_call::Instruction_call(Operand op1, Operand op2, Operand op3) : Instruction(op1, op2, op3)		{	this->opCode = CALL;		}
Instruction_param::Instruction_param(Operand op1,Operand op2, Operand op3) : Instruction(op1, op2, op3)		{	this->opCode = PARAM;		}
Instruction_return::Instruction_return(Operand op1,Operand op2,Operand op3): Instruction(op1, op2, op3)		{	this->opCode = RETURN;		}

Instruction_getretval::Instruction_getretval(Operand op1,Operand op2,Operand op3):Instruction(op1,op2,op3)	{	this->opCode = GETRETVAL;	}
Instruction_funcstart::Instruction_funcstart(Operand op1,Operand op2,Operand op3):Instruction(op1,op2,op3)	{	this->opCode = FUNCSTART;	}
Instruction_funcend::Instruction_funcend(Operand op1, Operand op2, Operand op3) : Instruction(op1,op2,op3)	{	this->opCode = FUNCEND;		}

Instruction_tablecreate::Instruction_tablecreate(Operand op1,Operand op2,Operand op3):Instruction(op1,op2,op3)	{	this->opCode = TABLECREATE;	}
Instruction_tablesetelem::Instruction_tablesetelem(Operand op1,Operand op2,Operand op3):Instruction(op1,op2,op3){	this->opCode = TABLESETELEM;	}
Instruction_tablegetelem::Instruction_tablegetelem(Operand op1,Operand op2,Operand op3):Instruction(op1,op2,op3){	this->opCode = TABLEGETELEM;	}

Instruction_nop::Instruction_nop(Operand,Operand,Operand) /* No need to call super.constructor */		{	this->opCode = NOP;		}

void Instruction_assign::execute(int i) // pairnei to arg3 kai to bazei sto arg1
{
	*cdeb << "Instruction_assign::execute()" << endl;

	int j;
	vector<MemCell *> op(0);
	for (j=0; j<this->op.size(); j++)
	{
		op[j] = operandToMemCell(this->op[j]); 
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}

	if (op[2]->type == CELL_TABLE)
	{
		// table_ref_counter--
		op[2]->data.tableVal->decRef();
	}

	*op[2] = *(memCellCopy(op[0]));

	for (j=0; j<this->op.size(); j++)
	{
		op[j] = operandToMemCell(this->op[j]); 
		*cdeb << "\t\top " << j+1 << " -> " << printMemCell(op[j]) << endl;
	}

	if (op[0]->type == CELL_TABLE)
	{
		// table_ref_counter++
		op[0]->data.tableVal->incRef();
	}
	CPU::CPUInstance()->pc++;
}

void Instruction_sub::execute(int i) 
{
	*cdeb << "Instruction_sub::execute()" << endl;
	int j;
	vector<MemCell *> op(0);
	for (j=0; j<this->op.size(); j++)
	{
		op[j] = operandToMemCell(this->op[j]); 
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	
	if(op[0]->type == CELL_NUM && op[1]->type == CELL_NUM)
	{
		op[2]->type = CELL_NUM;
		op[2]->data.numVal = op[0]->data.numVal - op[1]->data.numVal;
	}
	else
		throw new Error("Instruction_sub::execute(): Operand not number");
	CPU::CPUInstance()->pc++;
}
void Instruction_add::execute(int i) 
{
	*cdeb << "Instruction_add::execute()" << endl;
	int j;
	vector<MemCell *> op(3);
	for (j=0; j<this->op.size(); j++)
	{
		op[j] = operandToMemCell(this->op[j]); 
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	if(op[0]->type == CELL_NUM && op[1]->type == CELL_NUM)
	{
		op[2]->type = CELL_NUM;
		op[2]->data.numVal = op[0]->data.numVal + op[1]->data.numVal;
	}
	else
		throw new Error("Instruction_add::execute(): Operand not number");
	CPU::CPUInstance()->pc++;
}
void Instruction_mul::execute(int i) 
{ 
	*cdeb << "Instruction_mul::execute()" << endl;
	int j;
	vector<MemCell *> op(0);
	for (j=0; j<this->op.size(); j++)
	{
		op[j] = operandToMemCell(this->op[j]); 
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	
	if(op[0]->type == CELL_NUM && op[1]->type == CELL_NUM)
	{
		op[2]->type = CELL_NUM;
		op[2]->data.numVal = op[0]->data.numVal * op[1]->data.numVal;
	}
	else
		throw new Error("Instruction_mul::execute(): Operand not number");
	CPU::CPUInstance()->pc++;
}
void Instruction_div::execute(int i) 
{ 
	*cdeb << "Instruction_div::execute()" << endl;
	int j;
	vector<MemCell *> op(0);
	for (j=0; j<this->op.size(); j++)
	{
		op[j] = operandToMemCell(this->op[j]); 
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	
	if(op[0]->type == CELL_NUM && op[1]->type == CELL_NUM)
	{
		op[2]->type = CELL_NUM;
		op[2]->data.numVal = op[0]->data.numVal / op[1]->data.numVal;
	}
	else
		throw new Error("Instruction_div::execute(): Operand not number");
	CPU::CPUInstance()->pc++;
}
void Instruction_mod::execute(int i) 
{ 
	*cdeb << "Instruction_mod::execute()" << endl;
	int j;
	vector<MemCell *> op(0);
	for (j=0; j<this->op.size(); j++)
	{
		op[j] = operandToMemCell(this->op[j]); 
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	
	if(op[0]->type == CELL_NUM && op[1]->type == CELL_NUM)
	{
		op[2]->type = CELL_NUM;
		op[2]->data.numVal = (int)op[0]->data.numVal % (int)op[1]->data.numVal;
	}
	else
		throw new Error("Intruction_mod::execute(): Operand not number");
	CPU::CPUInstance()->pc++;
}
void Instruction_quminus::execute(int i) 
{ 
	*cdeb << "Instruction_qminus::execute()" << endl;
	int j;
	vector<MemCell *> op(0);
	for (j=0; j<this->op.size(); j++)
	{
		op[j] = operandToMemCell(this->op[j]); 
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	
	if(op[0]->type == CELL_NUM)
	{
		op[2]->type = CELL_NUM;
		op[2]->data.numVal = 0 - op[0]->data.numVal;
	}
	else
		throw new Error("Instruction_qminus::execute(): Operand not number");
	CPU::CPUInstance()->pc++;
}

void Instruction_and::execute(int i) 
{
	bool temp1,temp2;
	*cdeb<<"Instruction_and::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	//a long series of boring bullshit....
	temp1 = Instruction::operandToLogic(op[0]);
	temp2 = Instruction::operandToLogic(op[1]);
	op[2]->type = CELL_BOOL;
	op[2]->data.boolVal = temp1 && temp2;
	CPU::CPUInstance()->pc++;
 
}
void Instruction_or::execute(int i) 
{ 
	bool temp1,temp2;
	*cdeb<<"Instruction_or::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	//a long series of boring bullshit....
	temp1 = Instruction::operandToLogic(op[0]);
	temp2 = Instruction::operandToLogic(op[1]);
	op[2]->type = CELL_BOOL;
	op[2]->data.boolVal = temp1 || temp2;
	CPU::CPUInstance()->pc++;
}
void Instruction_not::execute(int i) 
{ 
	bool temp1;
	*cdeb<<"Instruction_not::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	//a slightly less long series of boring bullshit....
	temp1 = Instruction::operandToLogic(op[0]);
	op[2]->type = CELL_BOOL;
	op[2]->data.boolVal = !temp1;
	CPU::CPUInstance()->pc++;
}
void Instruction_ifeq::execute(int i) 
{
	bool temp1,temp2;
	*cdeb<<"Instruction_ifeq::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	if(op[0]->type == CELL_BOOL || op[1]->type == CELL_BOOL)
	{
		temp1 = operandToLogic(op[0]);
		temp2 = operandToLogic(op[1]);
		if(temp1&&temp2)
			CPU::CPUInstance()->pc =(int) op[2]->data.numVal;
		else
			CPU::CPUInstance()->pc++;
	}
	else if(compareMemCell(op[0],op[1]))
		CPU::CPUInstance()->pc = (int)op[2]->data.numVal;
	else
			CPU::CPUInstance()->pc++;
	
} 
void Instruction_ifnoteq::execute(int i) 
{ 
	bool temp1,temp2;
	*cdeb<<"Instruction_ifnoteq::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	if(op[0]->type == CELL_BOOL || op[1]->type == CELL_BOOL)
	{
		temp1 = operandToLogic(op[0]);
		temp2 = operandToLogic(op[1]);
		if(temp1&&temp2)
			CPU::CPUInstance()->pc++;
		else
			CPU::CPUInstance()->pc =(int) op[2]->data.numVal;
	}
	else if(!compareMemCell(op[0],op[1]))
		CPU::CPUInstance()->pc =(int) op[2]->data.numVal;
	else
			CPU::CPUInstance()->pc++;
}
void Instruction_iflesseq::execute(int i) 
{ 
	*cdeb<<"Instruction_iflesseq::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	if(op[0]->type == CELL_NUM && op[1]->type == CELL_NUM)
	{
		if(op[0]->data.numVal <= op[1]->data.numVal)
			CPU::CPUInstance()->pc = (int)op[2]->data.numVal;
		else
			CPU::CPUInstance()->pc++;
	}
	else 
		throw new Error("Instruction_iflesseq::execute() Operand not number"); 
}
void Instruction_ifgreatereq::execute(int i) 
{ 
	*cdeb<<"Instruction_ifgreatereq::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	if(op[0]->type == CELL_NUM && op[1]->type == CELL_NUM)
	{
		if(op[0]->data.numVal >= op[1]->data.numVal)
			CPU::CPUInstance()->pc = (int)op[2]->data.numVal;
		else
			CPU::CPUInstance()->pc++;
	}
	else 
		throw new Error("Instruction_ifgreatereq::execute(): Operand not number"); 
}
void Instruction_ifless::execute(int i) 
{ 
	*cdeb<<"Instruction_ifless::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	if(op[0]->type == CELL_NUM && op[1]->type == CELL_NUM)
	{
		if(op[0]->data.numVal < op[1]->data.numVal)
			CPU::CPUInstance()->pc = (int)op[2]->data.numVal;
		else
			CPU::CPUInstance()->pc++;
	}
	else 
		throw new Error("Instruction_ifless::execute(): Operand not number"); 
}
void Instruction_ifgreater::execute(int i) 
{ 
	*cdeb<<"Instruction_ifgreater::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	if(op[0]->type == CELL_NUM && op[1]->type == CELL_NUM)
	{
		if(op[0]->data.numVal > op[1]->data.numVal)
			CPU::CPUInstance()->pc = (int)op[2]->data.numVal;
		else
			CPU::CPUInstance()->pc++;
	}
	else 
		throw new Error("Instruction_ifgreater::execute(): Operand not number"); 
}
void Instruction_jump::execute(int i) 
{ 
	*cdeb<<"Instruction_jump::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	if (op[2]->data.numVal > MEM::MEMInstance()->getInstructionsNum() || op[2]->data.numVal<=0)
		throw new Error("Instruction_jump::execute(): Invalid jump address");
	else if (op[2]->data.numVal == MEM::MEMInstance()->getInstructionsNum())
		throw new Error("Instruction_jump::execute(): Jump to end of code. Terminate.");
	CPU::CPUInstance()->pc = (int)op[2]->data.numVal;
}

void Instruction_call::execute(int i) 
{ 
	*cdeb<<"Instruction_call::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top" << j+1 <<" -> "<< printMemCell(op[j]) << endl;
	}
	switch(op[0]->type)
	{
		case CELL_USRFUNC:
		{	
			// Briskoume to max meta3y: ari8mo formal arguments pou xrisimopoiei i function kai
				// ari8mo params prin to call.
			int formalArgNeeded = max( CPU::CPUInstance()->paramVector.size() , MEM::MEMInstance()->formalArgNum(op[0]->data.funcVal->address) );
			ActivationRecord *actRec = new ActivationRecord( op[0]->data.funcVal->localsize,  formalArgNeeded );
			actRec->setRetAddress( CPU::CPUInstance()->pc+1 );
			CPU::CPUInstance()->putRecord(actRec);
			CPU::CPUInstance()->pc = op[0]->data.funcVal->address;
			break;
		}
		case CELL_LIBFUNC:
		{
			if ( *(op[0]->data.libFuncVal) == string("print") )
			{
				int i;
				for (i=0; i<CPU::CPUInstance()->paramVector.size(); i++ )
					cout << printMemCell( CPU::CPUInstance()->paramVector[i], false );
				cout << endl;
				CPU::CPUInstance()->paramVector.resize(0);
				CPU::CPUInstance()->pc++;
			}
			else
				throw new Error("Instruction_call::Execute() _LIBFUNC_ : Not yet implemented");
			break;
		}
		default:
			throw new Error("Instruction_call::Execute(): Cannot call non-function object");
	}
}
void Instruction_param::execute(int i) 
{
	*cdeb<<"Instruction_param::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	CPU::CPUInstance()->paramVector.push_back( op[0] );
	CPU::CPUInstance()->pc++;
}

void Instruction_return::execute(int i) 
{ 
	*cdeb<<"Instruction_return::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	*(CPU::CPUInstance()->retVal) = *op[0];
	CPU::CPUInstance()->pc++;
}

void Instruction_getretval::execute(int i) 
{ 
	*cdeb<<"Instruction_getretval::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	*op[2] = *(CPU::CPUInstance()->retVal);
	for (j=0; j<this->op.size(); j++)
		*cdeb << "\t\top" <<j+1 <<" -> " << printMemCell(op[j])<<endl;
	CPU::CPUInstance()->pc++;
}
void Instruction_funcstart::execute(int i) 
{ 
	*cdeb << "Instruction_funcstart::execute()" << endl;
	CPU::CPUInstance()->getRecord()->activate();
//	*cdeb << "\tFunction's activation record activated" << endl;
//	*cdeb << "\t\t locals: " << CPU::CPUInstance()->getRecord()->getLocal() << endl;
//	*cdeb << "\t\t formals: " << CPU::CPUInstance()->getRecord()->getFormal() << endl;
	CPU::CPUInstance()->pc++;
}
void Instruction_funcend::execute(int i) 
{ 
	*cdeb << "Instruction_funcend::execute()" << endl;
	CPU::CPUInstance()->popRecord()->deactivate();
//	*cdeb << "\tFunction activation record DEactivated" << endl;
		// deactivate() takes care of the pc
}

void Instruction_tablecreate::execute(int i) 
{ 
	*cdeb<<"Instruction_tablecreate::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	if (op[0]->type == CELL_TABLE)		// if already a table	
		op[0]->data.tableVal->decRef();	// reference_counter--;
	op[0]->type = CELL_TABLE;
	op[0]->data.tableVal = new Table();
	CPU::CPUInstance()->pc++;
}
void Instruction_tablesetelem::execute(int i) 
{ //	op0 [ op1 ] = op2 
	*cdeb<<"Instruction_setelem::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	if (op[0]->type != CELL_TABLE)
		throw new Error("Instruction_setelem::execute(): Item not table");
	MemCell *inItem =op[0]->data.tableVal->getValue(op[1]);			
	if (inItem != NULL && inItem->type == CELL_TABLE)	// if ( op0[ op1 ] == TABLE
		inItem->data.tableVal->decRef();		// op0[op1] --> ref_counter--
	if (op[2]->type == CELL_TABLE)				// if op2 == TABLE
		op[2]->data.tableVal->incRef();			// op2-> ref_counter++
	op[0]->data.tableVal->change(op[1], op[2]);
	CPU::CPUInstance()->pc++;
}
void Instruction_tablegetelem::execute(int i) 
{ //	op2 = op0[ op1 ]
	*cdeb<<"Instruction_getelem::execute()"<<endl;
	int j;
	vector<MemCell *> op(0);
	for(j=0; j<this->op.size();j++)
	{
		op[j] = operandToMemCell(this->op[j]);
		*cdeb << "\top[ "<<this->op[j].code<<":"<<this->op[j].val<<"]" << j+1 << " -> " << printMemCell(op[j]) << endl;
	}
	if (op[0]->type != CELL_TABLE)
		throw new Error("Instruction_getelem::execute(): Item not table");
	if (op[2]->type == CELL_TABLE)
		op[2]->data.tableVal->decRef();
	MemCell *retCell = op[0]->data.tableVal->getValue( op[1] );
	if (retCell == NULL)
		throw new Error("Instruction_getelem::execute(): Index not found");
	if (retCell->type == CELL_TABLE)
		retCell->data.tableVal->incRef();
	*(op[2]) = *retCell;
	CPU::CPUInstance()->pc++;
}

void Instruction_nop::execute(int i) 
{ 
	*cdeb << "Instruction_nop::execute()" << endl;
	CPU::CPUInstance()->pc++;
}

bool Instruction::operandToLogic(MemCell* mcp)
{
	switch(mcp->type)
	{
		case CELL_NUM:
		{
			if(mcp->data.numVal == 0)
				return false;
			else
				return true;
		}
		case CELL_STRING:
		{
			if( mcp->data.strVal->size() == 0 )
				return false;
			else
				return true;
		}
		case CELL_BOOL:
		{
			if(mcp->data.boolVal == true)
				return true;
			else
				return false;
		}
		case CELL_TABLE:
		{
			return true;
		}
		case CELL_USRFUNC:
		{
			return true;
		}
		case CELL_LIBFUNC:
		{
			return true;
		}
		case CELL_NILL:
		{
			return false;
		}
		case CELL_UNDEF:
		{
			return false;
		}
	}
	
}
