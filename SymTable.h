#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <string>
#include <iostream>

using namespace std;



class ContentSection;
struct ContElem;
static int poslID=0;
struct FlinkElem {
	ContElem* PatchSpot;
	int flinkFlag;
	FlinkElem* next = 0;
public:
	FlinkElem(ContElem* cpatchspot, int ff) {
		PatchSpot = cpatchspot;
		flinkFlag = ff;
	}

};
struct SymTableElem {
	string symbol;
	char locality = 'l';
	int offset;
	int number;
	string sectionName;
	bool razresen = false;
	FlinkElem* flist, *llist;
	SymTableElem* next = 0;



public:
	SymTableElem(string sym, int coffset, string secName) {
		symbol = sym;
		offset = coffset; //DA LI DA NAPRAVIS OFFSET DA BUDE HEKSADECIMALNI
		number = ++poslID;
		sectionName = secName;
		flist = llist = 0;
	}
	void addFlink(ContElem* cp, int ci) {
		if (flist == 0) { flist = llist = new FlinkElem(cp, ci); }
		else {
			llist->next = new FlinkElem(cp, ci);
			llist = llist->next;
		}

	}

};
class SymTable{

public:
	

	
	
	
	SymTableElem *first, *last;
	SymTable();
	int PutSecSymbol(string pch);
	~SymTable();
	string SearchAndMark(string s,ContElem* p,int i);
	void PutSymbol(string s,ContElem* p,int i);
	void UpdateSymbol(string s);
	int PutUNDSymbol(string pch);
	int PutUNDSection();
	int PutABSSection();
};

#endif