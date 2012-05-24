#ifndef __MEMCELL_HEADER
#define __MEMCELL_HEADER

#include "general.h"
#include "operand.h"
#include <utility>

class Table
{
	public:
		Table();	// default constructor
		Table(Table &);	// copy constructor

		struct memCell*			getValue(struct memCell *key);
		struct memCell*			getValue(int i)			{	return this->kvTable[i].second;		}
		struct memCell*			getKey(int i)			{	return this->kvTable[i].first;	 	}
		long 				size()				{	return this->kvTable.size();		}

		void				change(struct memCell *,struct memCell *);
		void				incRef()			{	this->refCounter++;			}
		void				decRef()	// automatic garbage collection		
							{	
								if ( --(this->refCounter) )
									delete this;
							}

		friend bool			operator==(Table& left, Table& right);


	private:
		vector< pair<struct memCell*, struct memCell*> > kvTable;
		int refCounter;
};

typedef enum memCellType
{
	CELL_NUM, CELL_STRING, CELL_BOOL, CELL_TABLE, CELL_USRFUNC,
	CELL_LIBFUNC, CELL_NILL, CELL_UNDEF
} MemCellType;

typedef struct memCell
{
	MemCellType type;
	union malakoUnion
	{
		double	numVal;
		string *  strVal;
		bool boolVal;
		struct userFunc* funcVal;
		string *  libFuncVal;
		Table * tableVal;
	} data;
} MemCell;

MemCell* memCellCopy(MemCell*);
MemCell* operandToMemCell(struct operand, int=0);
string printMemCell(MemCell*,bool AftakiaStaStrings=true);
bool compareMemCell(MemCell*, MemCell*);

#endif
