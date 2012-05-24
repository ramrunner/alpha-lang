/* Auto to arxeio perilambanei to Main application */

#include "avm.h"
#include "mem.h"
#include "error.h"

static void breakC() { }

ostream *cdeb;

int main(int argc, char *argv[])
{
	int i;
	int debugState = 0;

	if (argc>3 && !strcmp(argv[3], "-g") )
	{
		debugState = 1;
		cdeb = &cout;
	}
	else
	{
		cdeb = new ofstream("/dev/null");
	}

	if(argc<3)
	{
		cerr<<"Usage: avm -t/-b program.text/program.abc"<<endl;
		exit(0);
	}
	else
	{
		*cdeb << "Loading " << argv[2] << endl;
		if(!strcmp(argv[1],"-t"))
		{
			ifstream *ifStr = new ifstream(argv[2]);
			MEM::MEMInstance()->loadMemFromTextFile(*ifStr);
		}
		else if(!strcmp(argv[1],"-b"))
		{
			FILE* afilep = fopen(argv[2], "r");;
			MEM::MEMInstance()->loadMemFromBinaryFile(afilep);
		}
		else
		{
			cout <<"Usage: avm -t/-b program.text/program.abc"<<endl;
			exit(0);
		}
	
	}

	MEM::MEMInstance()->fixGlobalMem();
	*cdeb << "Found " << CPU::CPUInstance()->memStackSize() << " global variables." << endl;
	*cdeb << "Found " << MEM::MEMInstance()->getInstructionsNum() << " instructions." << endl;
	*cdeb << endl;

	CPU::CPUInstance()->pc = 0;
	int totalExecIns = 0;
	try
	{
		string debugInp;
		while(CPU::CPUInstance()->pc != MEM::MEMInstance()->getInstructionsNum())
		{	
			(*cdeb) << "Instruction["<< CPU::CPUInstance()->pc << "].type = "<< MEM::MEMInstance()->getInstructionElem(CPU::CPUInstance()->pc)->getOpCode() << endl;
			MEM::MEMInstance()->getInstructionElem(CPU::CPUInstance()->pc)->execute();
			(*cdeb) << "Stack dump:" << endl;
			CPU::CPUInstance()->stackDump();
			*cdeb << endl;
			debugInp.clear();
			if (debugState>0)
				cin >> debugInp;
			totalExecIns++;
		}
	}
	catch(Error *e)	{	cerr << "Exception caught: " << e->getMsg() << endl;	return(-1);	}
	*cdeb << "Total number of executed instructions : " << totalExecIns << endl;

	return 0;
}
