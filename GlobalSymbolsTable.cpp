#include "ContentSection.h"
#include "SymTable.h"
#include "GlobalSymbolsTable.h"
#include "AllSections.h"
#include "EQU.h"
#include "InjectorFunctions.h"


GlobalSymbolsTable::GlobalSymbolsTable()
{
	first = last = 0;
}


GlobalSymbolsTable::~GlobalSymbolsTable()
{
}

void GlobalSymbolsTable::concatenate(char * s) {
	if (first == 0) {
		first = last = new GlobSymTableElem(s);
	}
	else {
		last->next = new GlobSymTableElem(s);
		last = last->next;
	}
}


