#include "memCell.h"
#include "mem.h"
#include "cpu.h"
#include "error.h"

MemCell* memCellCopy(MemCell* mc)
{
	MemCell* retCellp = (MemCell *)malloc(sizeof(MemCell));
	if (retCellp == NULL)
		throw new Error("memCellCopy(): malloc() returned NULL");
	(*retCellp).type = (*mc).type;
	switch((*retCellp).type)
	{
			case CELL_NUM:
			{
				retCellp->data.numVal = mc->data.numVal;
				break;
			}
			case CELL_STRING:
			{
					string *tmp = new string(*(mc->data.strVal));
					retCellp->data.strVal = tmp ;
					break;
			}
			case CELL_LIBFUNC:
			{
					string *tmp = new string(*(mc->data.libFuncVal));
					retCellp->data.libFuncVal = tmp ;
					break;
			}
			case CELL_TABLE:
			{		
					Table *tmp = new Table( *(mc->data.tableVal) );	
					retCellp->data.tableVal = tmp;
					break;
			}
			default:
			{
				retCellp->data = mc->data;
				break;
			}
	}
	return retCellp;
}

MemCell* operandToMemCell(Operand op, int cpun)
{
    MemCell *retVal =(MemCell*) malloc(sizeof(MemCell));
	if (retVal == NULL)
		throw new Error("operandToMemCell(): malloc() returned null");
    switch(op.code)
    {
        case 0:
            {
                retVal->type = CELL_NUM;
                retVal->data.numVal = op.val;
                return retVal;
            }
        case 1:     
		{   
			retVal =  CPU::CPUInstance(cpun)->memStack[op.val];  
			if (retVal == NULL)
				throw new Error("operandToMemCell(): null memcell (global)");
			return retVal;
		}     // global var
        case 2:     
		{	// stack[ topsp - (registernum+1)- n + op.val ]
			// n = stack[topsp-( registernum+1) ]
			CPU *curCPU = CPU::CPUInstance(cpun);
			int n = (int)curCPU->memStack[curCPU->topsp - (REGISTERNUM+1)]->data.numVal;
//			*cdeb << "(formal)["<< curCPU->topsp -(REGISTERNUM+1)-n+op.val <<"]" << endl;
			retVal = curCPU->memStack[curCPU->topsp - (REGISTERNUM+1) - n + op.val];  
			if (retVal == NULL)
				throw new Error("operandToMemCell(): null memcell (formal)");
			return retVal;
		} // formal vars
        case 3:     
		{   
			retVal =  CPU::CPUInstance(cpun)->memStack[CPU::CPUInstance(cpun)->topsp+op.val]; 
			if (retVal==NULL)
				throw new Error("operandToMemCell(): null memcell (local)");
			return retVal;
		} //local vars
        case 4:     // number value
             {
                retVal->type = CELL_NUM;
                retVal->data.numVal = MEM::MEMInstance()->getNumElem(op.val);
                return retVal;
            }
        case 5:     // string value
            {
                retVal->type = CELL_STRING;
                retVal->data.strVal = new string(MEM::MEMInstance()->getStringElem(op.val));
                return retVal;
            }
        case 6:     // boolean value
            {
                retVal->type = CELL_BOOL;
                retVal->data.boolVal = (op.val == 0 ? false : true);
                return retVal;
            }
        case 7:     // nil
            {
                retVal->type = CELL_NILL;
                return retVal;
            }
        case 8:     // User function
            {
                retVal->type = CELL_USRFUNC;
                retVal->data.funcVal = MEM::MEMInstance()->getUserFuncElem(op.val);
                return retVal;
            }
        case 9:     // library function
            {
                retVal->type = CELL_LIBFUNC;
                retVal->data.libFuncVal = new string(MEM::MEMInstance()->getLibFuncElem(op.val));
                return retVal;
            }
        case 10:
            {
                throw new Error("Arigato-Return value register should not be appear in terminal koound");
                return 0;
            }
        default:
            {
                throw new Error("Sagionara-Japanese generic Error, penis not long enough");
                return 0;
            }
    }
    return 0;
}

string printMemCell(MemCell* memcellp, bool aftakia)
{
	int i;
	Table *tp;
	if (memcellp == NULL)
	{
		throw new Error("printMemCell(): null pointer argument");
	}
	ostringstream retStr;
	switch(memcellp->type)
	{
		case CELL_NUM:
			retStr<< memcellp->data.numVal;
			break;
		case CELL_STRING:
			if (aftakia)
				retStr << "\"" << *(memcellp->data.strVal) << "\"";
			else
				retStr << *(memcellp->data.strVal);
			break;
		case CELL_BOOL:
			if (memcellp->data.boolVal)
				retStr << "(true)";
			else
				retStr << "(false)";
			break;
		case CELL_USRFUNC:
			retStr << "USERFUNC:";
			retStr << "  name:" << *(memcellp->data.funcVal->id);
			retStr << "  addr:" << memcellp->data.funcVal->address;
			retStr << "  local_size:"<<memcellp->data.funcVal->localsize;
			break;
		case CELL_LIBFUNC:
			retStr<<"LIBFUNC:  name:"<< *(memcellp->data.libFuncVal); 
			break;
		case CELL_NILL:
			retStr<<"(null)"; 
			break;
		case CELL_TABLE:
			tp = memcellp->data.tableVal;
			retStr<<"[";
			for (i=0; i<tp->size(); i++)
			{
				retStr << " { " << printMemCell(tp->getKey(i)) << " : " << printMemCell(tp->getValue(i)) << " }";
				if (i != tp->size()-1)
					retStr << ",";
			}
			retStr << "]";
			break;
		case CELL_UNDEF:
			retStr<<"(undef)"; 
			break;
		default:
			retStr << "(empty)";
	}
	return retStr.str();
}

bool compareMemCell(MemCell* mcp1 , MemCell* mcp2)
{
	if(mcp1->type != mcp2->type)
		return false;
	else
	{
		switch(mcp1->type)
		{
			case CELL_NUM:
			{
				if(mcp1->data.numVal == mcp2->data.numVal)
					return true;
				else
					return false;
			}
			case CELL_STRING:
			{
				if( *(mcp1->data.strVal) == *(mcp2->data.strVal))
					return true;
				else
					return false;
			}
			case CELL_BOOL:
			{
				if(mcp1->data.boolVal == mcp2->data.boolVal)
					return true;
				else
					return false;
			}
			case CELL_TABLE:
			{
				if(*(mcp1->data.tableVal) == *(mcp2->data.tableVal))
					return true;
				else
					return false;
			}
			case CELL_USRFUNC:
			{
				if(mcp1->data.funcVal->address == mcp2->data.funcVal->address && mcp1->data.funcVal->localsize == mcp2->data.funcVal->localsize &&*( mcp1->data.funcVal->id) == *(mcp2->data.funcVal->id))
					return true;
				else
					return false;
			}
			case CELL_LIBFUNC:
			{
				if(*(mcp1->data.libFuncVal) == *(mcp2->data.libFuncVal))
					return true;
				else
					return false;
			}
			case CELL_NILL:
			{
				return true;
			}
			case CELL_UNDEF:
			{
				return true;
			}
			default:
			{
				throw new Error(";asjdnfgl;jndf");
			}
		}
	}
}
//JAVADOC : TABLE ITEMS
// 0 -> jumps               to val krataei to target tou jump
// 1 -> global offset       
// 2 -> formal offset
// 3 -> local offset
// 4 -> number *
// 5 -> string *
// 6 -> boolean *
// 7 -> nil
// 8 -> usrFunc *
// 9 -> libFunc *
// 10 -> retVal register
