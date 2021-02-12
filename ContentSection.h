#ifndef CONTENTSECTION_H_
#define CONTENTSECTION_H_





#include <string>
#include <map>
#include <regex>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <bitset>

class SymTable;
class RelocSection;
class AllSections;
using namespace std;
extern SymTable* ST;



struct ContElem {

	string content[5] = { "","","","","" };
	int pcRel= 0;
	int mem = 0;
	int beginLC;
	int memdestFlag = 2;
	ContElem* next = 0;
public:
	void AddByte(string cs, int ci) {
		content[ci] = cs;
	}
};
struct RelocElem {

	int offset;
	string type;
	int value;
	string symbol;
	RelocElem* next = 0;
	int flinkFlag = 0;
	char locality = 'g';
	string secNameReloc;
	int secInside = 0;
	ContElem* show;
	int additFlag = 0;
public:
	RelocElem(int coffset, string ctype, int cvalue,string csymbol,ContElem* cshow,int cflinkFlag,string cSecNameReloc) {
		symbol = csymbol;
		offset = coffset;
		type = ctype;
		value = cvalue;
		show = cshow;
		flinkFlag = cflinkFlag;
		secNameReloc = cSecNameReloc;
	}


};

class ContentSection
{

public:
	
	int LC=0;
	
	int SymPosition;
	ContElem* first, *last;
	RelocElem* relSecf,*relSecl;
	string name;
	int flinkFlag = 0;
	ContentSection(string cname);
	~ContentSection();
	void ConcatenateInstruction(string s);
	void ConcatenateJumpOperand(string s);
	void ConcatenateOperand(string s,int i);
	string CheckLitSym(string s,int i);
	void removeRelocElem(RelocElem* rel);

	void processMemDirective(string s);

	void ConcatenateMemOperand(string s,int help);

};

class AllSections {

public:

	struct SecElem {

		ContentSection* p;
		string SectionName;
		SecElem* next = 0;
	public:
		SecElem(ContentSection* cp, string cSectionName) {
			p = cp;
			SectionName = cSectionName;
		}

	};
	static SecElem* currentSection;
	SecElem *first, *last;
	AllSections() { first = last = 0; }
	~AllSections() {/*DOPISI DESTRUKTOR*/ }
	SecElem* AddNewSection(ContentSection* p1);
	void searchAllSections(string s);


};


#endif // !CONTENTSECTION_H_
