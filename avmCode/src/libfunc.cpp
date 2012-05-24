#include "libfunc.h"

void Lib::print(MemCell* mcp)
{
	ostringstream retStr;
	switch(mcp->type)
	{
		case CELL_NUM:
			{
				retStr<<mcp->data.numVal;
				break;
			}
		case CELL_STRING:
			{
				retStr<<*(mcp->data.strVal);
				break;
			}
		case CELL_BOOL:
			{
				retStr<<mcp->data.boolVal;
				break;
			}
		case CELL_TABLE:
			{
				
			}
		case CELL_USRFUNC:
			{
				retStr<<"User function "<<mcp->data.funcVal->address<<" : "<< mcp->data.funcVal->address<<" : "<<*(mcp->data.funcVal->id);
				break;
			}
		case CELL_LIBFUNC:
			{
				retStr<<"Library function "<<*(mcp->data.libFuncVal);
				break;
			}
		case CELL_NILL:
			{
				retStr<<"(NOUL!)";
				break;
			}
		case CELL_UNDEF:
			{
				retStr<<"This memCell has performed an illegal operation and will be terminated";
				break;
			}
			}
			}
	}
}
