#ifndef GLOBALSYMBOLSTABLE_H_
#define GLOBALSYMBOLSTABLE_H_
#include <string>




using namespace std;

struct GlobSymTableElem {

	string symbol;
	GlobSymTableElem* next = 0;
public:
	GlobSymTableElem(string sym) :symbol(sym) {}

};
class GlobalSymbolsTable
{
public:


	GlobSymTableElem *first, *last;

	GlobalSymbolsTable();
	~GlobalSymbolsTable();


	


	void concatenate(char *s);
};

#endif // !GLOBALSYMBOLSTABLE_H_