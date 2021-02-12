#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS



#include "ContentSection.h"
#include "SymTable.h"
#include "GlobalSymbolsTable.h"
#include "AllSections.h"
#include "EQU.h"
#include "InjectorFunctions.h"
extern map<string, string> hm; //TESTIRAJ 
int Binary16toInt(string binary);




ContentSection::ContentSection(string cname)
{
	name = cname;
	first = last = 0;
	relSecf = relSecl = 0;


}


ContentSection::~ContentSection()
{
	ContElem* temp = first;
	while (temp) {
		ContElem* old = temp;
		temp = temp->next;
		delete old;
	}

}


void  ContentSection::ConcatenateInstruction(string s) {

	if (first == 0) { first = last = new ContElem(); }
	else {
		last->next = new ContElem();
		last = last->next;
	}
	last->beginLC = LC;
	if (s == "byte" || s == "long" || s == "short") {
		if (s == "byte") {
			LC = LC + 1;
			last->mem = 1;
		}
		if (s == "long") {
			LC = LC + 4;
			last->mem = 4;
		}
		if (s == "short") {
			LC = LC + 2;
			last->mem = 2;
		}
		
	}
	else {
		last->content[0] = hm[s];
		LC++;
		if (s == "shr") {
			last->memdestFlag = 1;
		}
	}
}


void ContentSection::ConcatenateOperand(string s, int i) {
	if (!empty(s)) {
		if (regex_match(s, std::regex("\\$(.*)"))) {
			if (((i == 2) && (last->memdestFlag == 2)) || (i == 1 && (last->memdestFlag == 1))) {
				//DESTINACIJA NE SME BITI NEPOSREDNI OPERAND
				cout<< "DESTINACIJA NE SME BITI NEPOSREDNI OPERAND" << endl;
				exit(1);
			}
			char *cstr = &s[0];
			char *pch = strtok(cstr, "$");
			LC = LC + 1;
			string result = CheckLitSym(pch, i);
			last->content[(i == 1 ? 1 : 3)].append("00010000");
			last->content[(i == 1 ? 2 : 4)] = result;
			LC = LC + 2;
			return;
		}

		if (regex_match(s, std::regex("(%r)\\d"))) {//REGISTARSKO DIREKTNO

			char *cstr = &s[0];
			char *pch = strtok(cstr, "%r");
			stringstream sstream;
			sstream << bitset<4>(atoi(pch));
			string result = sstream.str();
			last->content[(i == 1 ? 1 : 3)].append("001").append(result).append("0");
			LC = LC + 1;
			return;
			
		}

		if (regex_match(s, std::regex("\\(%r(\\d)\\)"))) {//REGISTARSKO INDIREKTNO BEZ POMERAJA

			char *cstr = &s[0];
			char *pch = strtok(cstr, "%r");
			stringstream sstream;
			sstream << bitset<4>(atoi(pch));
			string result = sstream.str();
			last->content[(i == 1 ? 1 : 3)].append("010").append(result).append("0");
			LC = LC + 1;

			return;
		}

		if (regex_match(s, std::regex(".{1,}\\(%r(\\d)\\)"))) {//REGISTARSKO INDIREKTNO POMERAJ R_
			
			char *cstr = &s[0];
			char *pch = strtok(cstr, "(%r"); 
			char* pch2 = strtok(NULL, "(%r");
			if (atoi(pch2) == 7) {
				last->pcRel = 1;
			}
			LC = LC + 1;
			string indirect = CheckLitSym(pch, i);
			
			stringstream sstream;
			sstream << bitset<4>(atoi(pch2));
			
			string result = sstream.str();
			
			last->content[(i == 1 ? 1 : 3)].append("011").append(result).append("0");
			last->content[(i == 1 ? 2 : 4)].append(indirect);
			LC = LC + 2;
			return;

		}

		if (regex_match(s, std::regex(".{1,}\\(%pc\\)"))) {//REGISTARSKO INDIREKTNO POMERAJ PC
			last->pcRel = 1;
			char *cstr = &s[0];
			char *pch = strtok(cstr, "(%r");
			LC = LC + 1;
			string indirect = CheckLitSym(pch, i);
			pch = strtok(NULL, "(%r");
			stringstream sstream;
			sstream << bitset<4>(7);
			string result = sstream.str();
			last->content[(i == 1 ? 1 : 3)].append("011").append(result).append("0");
			last->content[(i == 1 ? 2 : 4)].append(indirect);
			LC = LC + 2;
			
			return;
		}

		if (regex_match(s, std::regex("(.*)"))) {
			LC = LC + 1;
			string result = CheckLitSym(s, i);
			last->content[(i == 1 ? 1 : 3)].append("100").append("1000").append("0");
			last->content[(i == 1 ? 2 : 4)].append(result);
			LC = LC + 2;
			return;
		}//MEMORIJSKO//MEMORIJSKO

	}

}



void ContentSection::ConcatenateJumpOperand(string s) {
	
	if (regex_match(s, std::regex("\\*%r\\d"))) {//REGISTARSKO DIREKTNO

		char *cstr = &s[0];
		char *pch = strtok(cstr, "*%r");
		stringstream sstream;
		sstream << bitset<4>(atoi(pch));
		string result = sstream.str();
		last->content[1].append("001").append(result).append("0");
		LC = LC + 1;
		return;
	}

	if (regex_match(s, std::regex("\\*\\(%r(\\d)\\)"))) {//REGISTARSKO INDIREKTNO BEZ POMERAJA

		char *cstr = &s[0];
		char *pch = strtok(cstr, "*%r(");
		stringstream sstream;
		sstream << bitset<4>(atoi(pch));
		string result = sstream.str();
		last->content[1].append("010").append(result).append("0");
		LC = LC + 1;
		return;
	}

	if (regex_match(s, std::regex("\\*.{1,}\\(%r(\\d)\\)"))) {//REGISTARSKO INDIREKTNO POMERAJ R_

		char *cstr = &s[0];
		char *pch = strtok(cstr, "*(%r");
		char* pch2 = strtok(NULL, "(%r)");
		if (atoi(pch2) == 7) {
			last->pcRel = 1;
		}
		LC = LC + 1;
		string indirect = CheckLitSym(pch,1);
		stringstream sstream;
		sstream << bitset<4>(atoi(pch2));
		string result = sstream.str();
		last->content[1].append("011").append(result).append("0");
		last->content[2].append(indirect);
		LC = LC + 2;

		return;
	}

	if (regex_match(s, std::regex("\\*.{1,}\\(%pc\\)"))) {//REGISTARSKO INDIREKTNO POMERAJ PC
		last->pcRel = 1;
		char *cstr = &s[0];
		char *pch = strtok(cstr, "(*%r");
		LC = LC + 1;
		string indirect = CheckLitSym(pch,1);
		pch = strtok(NULL, "(*%r");
		stringstream sstream;
		sstream << bitset<4>(7);
		string result = sstream.str();
		last->content[1].append("011").append(result).append("0");
		last->content[2].append(indirect);
		LC = LC + 2;
		return;
	}

	if (regex_match(s, std::regex("\\*(.*)"))) {
		char *cstr = &s[0];
		char *pch = strtok(cstr, "*");
		LC = LC + 1;
		string mem= CheckLitSym(pch,1);
		last->content[1].append("100").append("1000").append("0");
		last->content[2].append(mem);
		LC = LC + 2;
		return;
	}

	if (regex_match(s, std::regex("(.*)"))) {
		LC = LC + 1;
		string result = CheckLitSym(s,1);
		last->content[1].append("000").append("1000").append("0");
		last->content[2].append(result);
		LC = LC + 2;
		return;
	}
}




string ContentSection::CheckLitSym(string s,int i) {
	
	if (isdigit(s[0])) {

		stringstream sstream;
		char *cstr = &s[0];
		sstream << bitset<16>(atoi(cstr));
		string result = sstream.str();
		return result; //ISTESTIRAJ


	}
	else {

		string symvalue= ST->SearchAndMark(s,last,i);


		return symvalue;
	}

}

void ContentSection::processMemDirective(string s) {
	ConcatenateInstruction(s);
	
}
void ContentSection::ConcatenateMemOperand(string s,int help) {
	string result = CheckLitSym(s, 0);
	
	if (help == 1) {
		int rez = Binary16toInt(result);
		stringstream sstream;
		sstream << bitset<8>(rez);
		string value = sstream.str();
		last->content[0] = value;
	}
	if (help == 2) {
		int rez = Binary16toInt(result);
		stringstream sstream;
		sstream << bitset<16>(rez);
		string value = sstream.str();
		last->content[0] = value;
	}
}


void ContentSection::removeRelocElem(RelocElem* rel) {

	RelocElem* temp1 = relSecf;
	RelocElem* onebefore = 0;
	while (temp1) {

		if (rel == temp1) {
			if (!onebefore) {
				relSecf = temp1->next;
				if (relSecf == 0) {
					relSecl = 0;
				}
				RelocElem *old = temp1;
				temp1->next = 0;

			}
			else {
				RelocElem* old = temp1;
				onebefore->next = temp1->next;
				temp1->next = 0;
				delete old;
			}
		}
		onebefore = temp1;
		temp1 = temp1->next;
	}

}

AllSections::SecElem* AllSections::AddNewSection(ContentSection *p1) {

	if (first == 0) {

		first = last =new SecElem(p1, p1->name);


	}
	else {
		last->next = new SecElem(p1, p1->name);
		last = last->next;

	}
	return last;
}
void AllSections::searchAllSections(string s) {

	SecElem* temp = first;
	while (temp) {

		if (temp->SectionName == s) {
			currentSection = temp;
			return;
		}
		temp = temp->next;
	}
	ContentSection* cs = new ContentSection(s);
	currentSection = AddNewSection(cs);

}






int empty(string s) {
	return (s == "");
}

#endif