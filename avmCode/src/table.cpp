#include "memCell.h"
Table::Table()	{	this->refCounter=1;	}

Table::Table(Table& orig)
{
	int i;
	for(i=0; i<orig.size(); i++)
		this->kvTable.push_back( pair<MemCell*,MemCell*>(memCellCopy(orig.kvTable[i].first), memCellCopy(orig.kvTable[i].second)) );
	this->refCounter = 1;
}

MemCell *Table::getValue(MemCell *key)
{
	int i;
	for (i=0; i<this->size(); i++)
		if ( compareMemCell(kvTable[i].first, key) )
			return kvTable[i].second;
	return (MemCell *) 0;
}

void Table::change(MemCell *key, MemCell *value)
{
	int i;
	for (i=0; i<this->size(); i++)
		if ( compareMemCell(kvTable[i].first, key) ) // to brikame! :)
		{
			kvTable[i].second =  value;
			return;
		}
	kvTable.push_back( pair<MemCell*,MemCell*>(key, value) );
}

bool operator==(Table& left, Table& right)
{
	if (left.size() != right.size())
		return false;
	int i;
	for (i=0; i<left.size(); i++)
	{
		if (! compareMemCell(left.kvTable[i].first, right.kvTable[i].first) )
			return false;
		if (! compareMemCell(left.kvTable[i].second, right.kvTable[i].second) )
			return false;
	}
	return true;
}
