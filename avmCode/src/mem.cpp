#include "mem.h"
#include "operand.h"

/*** test
int main()
{
	*cdeb << "Testing mem.cpp (along with instruction.cpp)" << endl;
	*cdeb << "Test singleton design pattern..." << endl;
	MEM *p1 = MEM::MEMInstance();
	MEM *p2 = p1->MEMInstance();
	MEM &ref1 = * MEM::MEMInstance();
	*cdeb << "The following should be the same" << endl;
	*cdeb << "p1  = "<< hex << p1 << endl;
	*cdeb << "p2  = "<< hex << p2 << endl;
	*cdeb << "ref = "<< hex << &ref1 << endl;
}
**/

static void breakP() { }

MEM* MEM::pMEMInstance=0;	// Arxika, den yparxei MEM instance -> initialize pointer

MEM* MEM::MEMInstance()
{
	if (pMEMInstance == 0)
		pMEMInstance = new MEM();
	return pMEMInstance;
}

MEM::MEM()	
{
	Operand op;
	this->instructionMem.push_back(new Instruction_nop(op, op, op) );
}

int MEM::formalArgNum(int pc)
{
		breakP();
	if (this->instructionMem[pc++]->getOpCode() != FUNCSTART)
		return -1;
	int argCount = 0, funcCount=1, i;
	bool count = true;	

	while(funcCount>0)
	{
		if (this->instructionMem[pc]->getOpCode()==FUNCSTART)
			funcCount++;
		if (this->instructionMem[pc]->getOpCode()==FUNCEND)
			funcCount--;
		if (funcCount>1)
			count = false;
		else
			count = true;
		vector<Operand> op= this->instructionMem[pc++]->getOperands();
		for (i=0; i<3; i++)
			if (count && op[i].code == 2 && op[i].val < argCount)
				argCount = op[i].val;
	}
	return argCount+1;
}

bool MEM::addToStringMem(string aString)
{
	stringMem.push_back(aString);
	return true;
}
bool MEM::addToNumMem(double aDouble)
{
	numMem.push_back(aDouble);
	return true;
}
bool MEM::addToUsrFuncMem(UserFunc *aUserFunc)
{
	usrFuncMem.push_back(aUserFunc);
	return true;
}
bool MEM::addToLibFuncMem(string aString)
{
	libFuncMem.push_back(aString);
	return true;
}
bool MEM::addToInstructionMem(Instruction* aInstruction)
{
	this->instructionMem.push_back(aInstruction);
	return true;
}
Instruction* MEM::setProperInstruction(OpCode aint,Operand op1,Operand op2,Operand op3)
{
	switch(aint)
	{
			case ASSIGN: return new Instruction_assign(op1,op2,op3);
			case ADD: return new Instruction_add(op1,op2,op3);
			case SUB: return new Instruction_sub(op1,op2,op3);
			case MUL: return new Instruction_mul(op1,op2,op3);
			case DIV: return new Instruction_div(op1,op2,op3);
			case MOD: return new Instruction_mod(op1,op2,op3);
			case QUMINUS: return new Instruction_quminus(op1,op2,op3);
			case AND: return new Instruction_and(op1,op2,op3);
			case OR: return new Instruction_or(op1,op2,op3);
			case NOT: return new Instruction_not(op1,op2,op3);
			case IF_EQ: return new Instruction_ifeq(op1,op2,op3);
			case IF_NOTEQ: return new Instruction_ifnoteq(op1,op2,op3);
			case IF_LESSEQ: return new Instruction_iflesseq(op1,op2,op3);
			case IF_GREATEREQ: return new Instruction_ifgreatereq(op1,op2,op3);
			case IF_LESS: return new Instruction_ifless(op1,op2,op3);
			case IF_GREATER: return new Instruction_ifgreater(op1,op2,op3);
			case JUMP: return new Instruction_jump(op1,op2,op3);
			case CALL: return new Instruction_call(op1,op2,op3);
			case PARAM: return new Instruction_param(op1,op2,op3);
			case RETURN: return new Instruction_return(op1,op2,op3);
			case GETRETVAL: return new Instruction_getretval(op1,op2,op3);
			case FUNCSTART: return new Instruction_funcstart(op1,op2,op3);
			case FUNCEND: return new Instruction_funcend(op1,op2,op3);
			case TABLECREATE: return new Instruction_tablecreate(op1,op2,op3);
			case TABLEGETELEM: return new Instruction_tablegetelem(op1,op2,op3);
			case TABLESETELEM: return new Instruction_tablesetelem(op1,op2,op3);
			case NOP: return new Instruction_nop(op1,op2,op3);
	}
	return (Instruction *)0;
}

void MEM::printMem()
{
/*	int i;
	//*cdeb <<"exo "<<stringMem.size()<<" Strings"<<endl;
	for(i=0;i<stringMem.size();i++)
	{
		//*cdeb<<i<<": "<<stringMem[i]<<endl;
	}
	//*cdeb <<"exo "<<numMem.size()<<" Nums"<<endl;
	for(i=0;i<numMem.size();i++)
	{
		//*cdeb<<i<<": "<<numMem[i]<<endl;
	}
	//*cdeb <<"exo "<<usrFuncMem.size()<<" UsrFuncs"<<endl;
	for(i=0;i<usrFuncMem.size();i++)
	{
		userFunc afunc;
		afunc = usrFuncMem[i];
		//*cdeb<<"\t USRFUNC:" <<afunc.id;
		//*cdeb<<"\t address:" << afunc.address;
		//*cdeb<<"\t localsize:" << afunc.localsize<<endl;
	}
	//*cdeb <<"exo "<<libFuncMem.size()<<" LibFuncs"<<endl;
	for(i=0;i<libFuncMem.size();i++)
	{
		//*cdeb<<i<<": "<<libFuncMem[i]<<endl;
	}
	//*cdeb <<"exo "<<instructionMem.size()<<" Instructions"<<endl;
	//*cdeb<<"****************************************************"<<endl;
	for(i=0;i<instructionMem.size();i++)
	{
		Instruction* aInstruction = instructionMem[i];
		vector<Operand> optable = aInstruction->getOperands();
		//*cdeb<<"\t Ins->" << aInstruction->getOpCode();
		//*cdeb<<"\t op1->" <<optable[0].code<<":"<<optable[0].val;
		//*cdeb<<"\t op2->" <<optable[1].code<<":"<<optable[1].val;
		//*cdeb<<"\t op3->" <<optable[2].code<<":"<<optable[2].val<<endl;
	}
	//*cdeb<<"****************************************************"<<endl;
*/
}

bool MEM::loadMemFromBinaryFile(FILE* afile)
{
        int aint;
        unsigned int total, i,j;
        long long magicNum = 340200501, anum;
        string astring;
        double adouble;
        long   along;
        fread(&anum,1*sizeof(long long),1,afile);
        if(anum != magicNum)
        {
                cerr<<"Magic Number Not Valid ..Terminating"<<endl;
                //*cdeb<<"number was " <<anum<<endl;
                return false;
        }
        else
        {
                //*cdeb<<"All ok . Magic Num found :"<<anum<<endl;
                fread(&total,1*sizeof(int),1,afile);
                //*cdeb<<"total strings:"<<total<<endl;
                for(i=0;i<total;i++)
                {
                        fread(&aint,sizeof(int),1,afile);
                        char* stringbuf =(char*) calloc(aint+1,sizeof(char));
                        fread(stringbuf,sizeof(char),aint,afile);
                        astring = stringbuf;
                        addToStringMem(astring);
                        astring.clear();
                        free(stringbuf);
                }
                fread(&total,1*sizeof(int),1,afile);
                //*cdeb<<"total doubles:"<<total<<endl;
                for(i=0;i<total;i++)
                {
                        fread(&adouble,1*sizeof(double),1,afile);
                        addToNumMem(adouble);
                        //*cdeb<<adouble<<endl;
                }
                fread(&total,1*sizeof(int),1,afile);
                //*cdeb<<"total userFuncs:"<<total<<endl;
                for(i=0;i<total;i++)
                {
                        int size;
                        UserFunc *aFunc = (UserFunc *)malloc(sizeof(UserFunc));
                        fread(&along,1*sizeof(long),1,afile);
                        aFunc->address = along;
	//		*cdeb << " addr: " << aFunc->address;
                        fread(&aint,1*sizeof(int),1,afile);
                        aFunc->localsize = aint;
	//		*cdeb << " local_sz: " << aFunc->localsize;
                        fread(&size,1*sizeof(int),1,afile);
			*cdeb << " name_size: " << size;
                        char* stringbuf =(char*) calloc(size+1,sizeof(char));
                        fread(stringbuf,sizeof(char),size,afile);
	//		*cdeb << " name_string: " << stringbuf;
                        astring = stringbuf;
                        aFunc->id = new string(astring);
                        addToUsrFuncMem(aFunc);
	//		*cdeb << " user func. name-> " << *(aFunc->id) << " or " << astring << "\t";
                        astring.clear();
                        free(stringbuf);
                }
                fread(&total,sizeof(int),1,afile);
                //*cdeb<<"total LibFuncs:"<<total<<endl;
                for(i=0;i<total;i++)
                {
                        int size;
                        fread(&size,sizeof(int),1,afile);
                        char* stringbuf =(char*) calloc(size+1,sizeof(char));
                        fread(stringbuf,sizeof(char),size,afile);
                        astring = stringbuf;
                        addToLibFuncMem(astring);
                        free(stringbuf);
                }
                fread(&total,sizeof(int),1,afile);
                //*cdeb<<"total instructions:"<<total<<endl;
                for(i=0;i<total;i++)
                {
			//breakP();
                        Operand op1,op2,op3;
                        OpCode opcode;
                        long along;
                        fread(&opcode,sizeof(int),1,afile);
                        fread(&aint,sizeof(int),1,afile);
                        op1.code=aint;
                        fread(&along,sizeof(long),1,afile);
                        op1.val=along;
                        fread(&aint,sizeof(int),1,afile);
                        op2.code = aint;
                        fread(&along,sizeof(long),1,afile);
                        op2.val = along;
                        fread(&aint,sizeof(int),1,afile);
                        op3.code = aint;
                        fread(&along,sizeof(long),1,afile);
                        op3.val = along;
                        addToInstructionMem( setProperInstruction(opcode,op1,op2,op3) );
                }
        }
        return true;
}
                                                                                                                                 

bool MEM::loadMemFromTextFile(ifstream& aStream)
{
	int aint;
	unsigned int magicNum = 340200501,total, anum, i,j;
	string astring;
	double adouble;
	aStream>> anum;
	if(anum != magicNum)
	{
		cerr<<"Magic Number Not Valid ..Terminating"<<endl;
		return false;
	}
	else
	{
		//*cdeb<<"All ok . Magic Num found :"<<anum<<endl;
		aStream>>total;
		//*cdeb<<"total strings:"<<total<<endl;
		for(i=0;i<total;i++)
		{
			char mourouna;
			aStream>>aint;
			aStream.get(mourouna);
			for(j=0;j<aint;j++)
			{
				aStream.get(mourouna);
				astring += mourouna;
			}	
			addToStringMem(astring);
			astring.clear();
		}
		aStream>>total;
		//*cdeb<<"total doubles:"<<total<<endl;
		for(i=0;i<total;i++)
		{
			aStream>>adouble;	
			addToNumMem(adouble);
		}
		aStream>>total;
		//*cdeb<<"total userFuncs:"<<total<<endl;
		for(i=0;i<total;i++)
		{
			userFunc *aFunc = (UserFunc *)malloc(sizeof(UserFunc));
			aStream>>anum;	
			aFunc->address = anum;
			aStream>>anum;	
			aFunc->localsize = anum;
			aStream>>astring;
			aFunc->id = new string(astring);
			addToUsrFuncMem(aFunc);
		}
		aStream>>total;
		//*cdeb<<"total LibFuncs:"<<total<<endl;
		for(i=0;i<total;i++)
		{
			aStream>>aint;
			aStream>>astring;	
			addToLibFuncMem(astring);
		}
		aStream>>total;
		//*cdeb<<"total instructions:"<<total<<endl;
		for(i=0;i<total;i++)
		{
			Operand op1,op2,op3;
			OpCode opcode;
			long along;
			aStream>>aint;
			opcode = (OpCode)aint;
			aStream>>aint;
			op1.code=aint;
			aStream>>along;
			op1.val=along;
			aStream>>aint;
			op2.code = aint;
			aStream>>along;
			op2.val = along;
			aStream>>aint;
			op3.code = aint;
			aStream>>along;
			op3.val = along;
			addToInstructionMem(setProperInstruction(opcode,op1,op2,op3));
		}
	}
	return true;
}

void MEM::fixGlobalMem()
{
	vector<Operand> tmpOp;
	Instruction *tmpIns;
	int k,j,i = this->instructionMem.size();
	for(j=1;j<i;j++)
	{
		tmpIns = instructionMem[j];
		tmpOp = tmpIns->getOperands();
	
		for(k=0;k<3;k++)
		{
			if(tmpOp[k].code == 0  && tmpOp[k].val==0)
				continue;
			else
			if(tmpOp[k].code == 1 && CPU::CPUInstance()->memStackSize() <= tmpOp[k].val)
			{
				CPU::CPUInstance()->addToMemStack(tmpOp[k].val - CPU::CPUInstance()->memStackSize()+1);
			}
		}	
	}
}

