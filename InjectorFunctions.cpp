
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS


#include "ContentSection.h"
#include "SymTable.h"
#include "GlobalSymbolsTable.h"
#include "AllSections.h"
#include "EQU.h"
#include "InjectorFunctions.h"
#include <regex>


extern GlobalSymbolsTable* gst;
extern int kraj;
extern ifstream inFile;
extern ofstream outFile;

int main(void) {
	
	inFile.open("C:\\Users\\Milijan\\Desktop\\demo.txt");
	outFile.open("C:\\Users\\Milijan\\Desktop\\demo2.txt");
	if (!inFile) {
		cerr << "unable to open file";
		exit(2);
	}
	for (std::string line; kraj==0;) {
		std::getline(inFile, line);
		test_regex(line);
	}
	ST->PutUNDSection();
	ST->PutABSSection();
	BackPatchReloc();
	BackPatchCode();
	BackPatch32();
	BackPatchPCRel();
	EQUPROCESSING();
	backPatchEQUFinal();

	/*outFile.open("C:\\Users\\Milijan\\Desktop\\demo2.txt"); //RELOKATIVNO?
	outFile << ST;*/
	SymTableElem* temp = ST->first;
	outFile << "SYMBOL TABLE" << endl;
	//outFile << "simbol " << " "<<"l/g " << "sekcija prip "<< "ofset " << "rednibroj " << endl;
	while (temp) {

		outFile << temp->symbol << " " << temp->locality << " " << temp->sectionName<<" "<<temp->offset << " "<<
			temp->number <<" " << endl;
		temp = temp->next;
	}
	AllSections::SecElem* temp2 = allsec->first;
	outFile << endl;
	outFile << endl;
	while (temp2) {
		outFile << temp2->SectionName <<" SEKCIJA"<< endl;
		ContElem* telem= temp2->p->first;
		while (telem) {
			outFile << HEXUP(telem->content[0]) << " ";
			outFile << HEXUP(telem->content[1]) << " ";
			outFile << HEXUP(telem->content[2]) << " ";
			outFile << HEXUP(telem->content[3]) << " ";
			outFile <<HEXUP(telem->content[4]) << endl;
			telem = telem->next;
		}
		outFile << endl;
		outFile << endl;
		RelocElem* rtelem = temp2->p->relSecf;
		outFile << "RELOC SEKCIJA " <<temp2->SectionName<< endl;
		while (rtelem) {
			outFile << rtelem->offset << " "
				<< rtelem->type << " "
				<< rtelem->value << " " <<rtelem->locality<<" "<< rtelem->symbol<<endl;
			rtelem = rtelem->next;
		}

		temp2 = temp2->next;
		outFile << endl;
	}
	outFile.close();

	
	//outFile.close()?
	return 0;

}



string SymTable::SearchAndMark(string s, ContElem* p,int i) {
	stringstream sstream;
	string val;
	SymTableElem* temp = first;
	
	while (temp) {

		if (temp->symbol == s) {
			if (temp->razresen){
				int result = temp->offset;

}
				sstream << bitset<16>(result);
				val = sstream.str();
				char *ret = &val[0];
				addReloc(s, p, i);
				return ret;
			}
			else {
				temp->addFlink(p, i);
				sstream << bitset<16>(0);
				val = sstream.str();
				char *ret = &val[0];
				addReloc(s, p, i);
				return ret;

			}

		}
		temp = temp->next;
	}
	PutSymbol(s, p, i);
	sstream << bitset<16>(0);
	val = sstream.str();
	char *ret = &val[0];
	return ret;
	

}


















map<string, string> hm = createHASH_MAP();
int emptys(string s) {
	return (s == "");
}


map<string,string> createHASH_MAP(){
	map<string, string> pom;
	pom["short"] = "0000000000000000";
	pom["word"] = "00000000000000000000000000000000";
	pom["byte"] = "00000000";
	pom["halt"] = "00000100";
	pom["iret"] = "00001100";
	pom["ret"] = "00010100";
	pom["int"] = "00011100";
	pom["call"] = "00100100";
	pom["jmp"] = "00101100";
	pom["jeq"] = "00110100";
	pom["jne"] = "00111100";
	pom["jgt"] = "01000100";
	pom["push"] = "01001100";
	pom["pop"] = "01010100";
	pom["xchg"] = "01011100";
	pom["mov"] = "01100100";
	pom["add"] = "01101100";
	pom["sub"] = "01110100";
	pom["mul"] = "01111100";
	pom["div"] = "10000100";
	pom["cmp"] = "10001100";
	pom["not"] = "10010100";
	pom["and"] = "10011100";
	pom["or"] = "10100100";
	pom["xor"] = "10101100";
	pom["test"] = "10110100";
	pom["shl"] = "10111100";
	pom["shr"] = "11000100";


	return pom;



}



int Binary16toInt(string binary) {
	int res = 0;
	for (int i = 0; i < 16; i++) {
		if (binary[i] == '1') {
			res += pow(2, 15 - i);
		}
	}

	return res;
}

int test_regex(string line)
{
	if (regex_match(line, std::regex("(.*)(\\.)(.*)"))) {
		processDirective(line);
		return 1;
	}
	processInstruction(line);
	return 1;
}



int SetUpAssembler() {
	return 1;
}

int processDirective(string line) {

	int lpresent = processLabel(line);
	if ((std::regex_match(line, std::regex("(.global)(.*)")))
		) {
		//PRAVI DRUGU ZA EXTERN
		char *cstr = &line[0];

		char *pch = strtok(cstr, " ,.");
		pch = strtok(NULL, " ,.");
		while (pch != NULL) {

			gst->concatenate(pch);
			pch = strtok(NULL, " ,.");

		}
		return 1;
	}
	else if (std::regex_match(line, std::regex("(.extern)(.*)"))) {
		char *cstr = &line[0];

		char *pch = strtok(cstr, " ,.");
		pch = strtok(NULL, " ,.");
		while (pch != NULL) {

			gst->concatenate(pch);
			ST->PutUNDSymbol(pch);
			pch = strtok(NULL, " ,.");

		}
		return 1;

	}
	else if (std::regex_match(line, std::regex("(.*)(.skip)(.*)"))) {
		char *cstr = &line[0];

		char *pch = strtok(cstr, " ,.");
		pch = strtok(NULL, " ,.");
		if (lpresent) {
			pch = strtok(NULL, " ,.");
		}
		allsec->currentSection->p->LC += stoi(pch);
		
		

	}
	else if (std::regex_match(line, std::regex("(.*)(.equ)(.*)"))) {
		processEQU(line, lpresent);
	}
	else if ((regex_match(line, std::regex("(.*)(.long)(.*)")))
		|| (regex_match(line, std::regex("(.*)(.byte)(.*)"))) ||
		(regex_match(line, std::regex("(.*)(.word)(.*)")))) {
		int help = 0;

		if (regex_match(line, std::regex("(.*)(.byte)(.*)"))) {
			help = 1;
		}
		if (regex_match(line, std::regex("(.*)(.word)(.*)"))) {
			help = 2;
		}
		char *cstr = &line[0];

		char *pch = strtok(cstr, " ,.");



		if (lpresent) {
			pch = strtok(NULL, " ,.");
		}
		processMemDirective(pch);

		pch = strtok(NULL, " ,.");
		processMemOperand(pch, help);

		return 1;

	}
	else if (regex_match(line, std::regex("(.end)"))) kraj= 1;
	else if (regex_match(line, std::regex("(.section)(.*)"))) {
		char *cstr = &line[0];
		char *pch = strtok(cstr, " ,.");
		pch = strtok(NULL, " ,.");
		cout << pch << "\n";
		//napravi novi content sekciju sa ovim simbolom ili proglasi za tekucu sekciju
		//neku koja je vec bila 
		allsec->searchAllSections(pch);
		AllSections::currentSection->p->SymPosition = ST->PutSecSymbol(pch);
		return 1;
	}
	else {
		cout << "ADDRESSING MISTAKE!" << endl;
		return 1;
	}
}

int processLabel(string line) {
	if (regex_match(line, std::regex("(.*)(:)(.*)"))) {
		char *cstr = &line[0];
		char *pch = strtok(cstr, " :");
		string help = pch;
		ST->UpdateSymbol(help);

		return 1;
	}
	else return 0;

}

int processInstruction(string line) {
	string hline = line;
	int label = processLabel(hline);
	char *cstr = &line[0];

	char* pch = strtok(cstr, " ,:");
	if (label) {
		pch = strtok(NULL, " .,:");
	}
	if (pch == NULL) {
		return 1;
	}
	processOPCode(pch);//POSTAVLJA SE NA 1 MEMDESTFLAG AKO JE SHR
	pch = strtok(NULL, " ,");
	if (pch == NULL) {
		return 1;
	}
	char* pch2 = strtok(NULL, " ,"); //RADI 3.6.2020
	if (pch2 == NULL) {
		if (regex_match(hline, std::regex("(.*)(push)(.*)")) ||
			regex_match(hline, std::regex("(.*)(pop)(.*)"))) {
			processOperand(pch, "");
			if (regex_match(hline, std::regex("(.*)(pop)(.*)"))) {
				allsec->currentSection->p->last->memdestFlag = 1;
			}
			else {
				allsec->currentSection->p->last->memdestFlag = 0;
			}
		}
		else {
			processJump(pch);

		}
	}
	else {
		processOperand(pch, pch2);

	}


	return 1;
}






void processOPCode(string line) {

	AllSections::currentSection->p->ConcatenateInstruction(line);

}



void processOperand(string line, string line2) {

	AllSections::currentSection->p->ConcatenateOperand(line,1);
	AllSections::currentSection->p->ConcatenateOperand(line2,2);
}

void processJump(string line) {
	AllSections::currentSection->p->ConcatenateJumpOperand(line);
}


void InjectorFunctions::addReloc(string line, ContElem* p, int i) {
	ContentSection* pom = AllSections::currentSection->p;
	
	if (pom->relSecf == 0) {
		pom->relSecf = pom->relSecl = new
			RelocElem(pom->LC, p->pcRel== 1 ? "R_386_PC32" : "R_386_32", pom->SymPosition, line,p,i,AllSections::currentSection->SectionName);


	}
	else {
		pom->relSecl->next = new
			RelocElem(pom->LC, p->pcRel == 1 ? "R_386_PC32" : "R_386_32", pom->SymPosition, line,p,i,AllSections::currentSection->SectionName);
		pom->relSecl = pom->relSecl->next;
	}
}


//EQU STRUKTURA










int BackPatchCode()
{
	SymTableElem* temp = ST->first;
	while (temp) {
		FlinkElem* ftemp = temp->flist;
		while (ftemp) {
			ContElem* t1 = ftemp->PatchSpot;
			string res;
			stringstream sstream;
			if (temp->locality == 'g') {
				sstream << bitset<16>(0);
			}
			else {
				
				sstream << bitset<16>(temp->offset);
				
			}
			res = sstream.str();
			t1->content[(ftemp->flinkFlag == 1) ? 2 : ((ftemp->flinkFlag == 2) ? 4 : 0)] = res;	
			ftemp = ftemp->next;
		}
		temp = temp->next;

	}
	return 1;
}

void BackPatchReloc() {

	GlobSymTableElem* temp = gst->first;

	while (temp) {

		SymTableElem* stemp = ST->first;
		while (stemp) {
			if (stemp->symbol == temp->symbol) {
				stemp->locality = 'g';
				
			}
			stemp = stemp->next;
		}
		temp = temp->next;
	}
	AllSections::SecElem* sectemp = allsec->first;
	while (sectemp) {
		RelocElem* elemtemp = sectemp->p->relSecf;
		while (elemtemp) {
			SymTableElem* symtemp = ST->first;
			while (symtemp) {
				if (symtemp->symbol == elemtemp->symbol) {
					if (elemtemp->locality == 'g') {
						elemtemp->value = symtemp->number;
					}
					SymTableElem* symtemp2 = ST->first;
					while (symtemp2) {

						if (symtemp2->symbol == symtemp->sectionName && symtemp->locality=='l') {
							elemtemp->value = symtemp2->number;
							elemtemp->locality = 'l';
							if (elemtemp->secNameReloc == symtemp->sectionName) elemtemp->secInside = 1;
						}
						symtemp2 = symtemp2->next;
					}
				}
				symtemp = symtemp->next;
			}
			elemtemp = elemtemp->next;
		}

		sectemp = sectemp->next;
	}

}

void InjectorFunctions::BackPatch32() {

	AllSections::SecElem* secTemp = allsec->first;
	while (secTemp) {
		RelocElem* relTemp = secTemp->p->relSecf;
		while (relTemp) {
			if (relTemp->type == "R_386_32" && relTemp->locality == 'g') {
				stringstream helpstream;
				helpstream << bitset<16>(0);
				string ret = helpstream.str();
				relTemp->show->content[relTemp->flinkFlag == 1 ? 2 : 0] = ret;
			}
			relTemp = relTemp->next;
		}
		secTemp = secTemp->next;
	}


}

void BackPatchPCRel() {
	int offPC = 0;
	AllSections::SecElem* secTemp = allsec->first;
	while (secTemp) {
		RelocElem* relTemp = secTemp->p->relSecf;
		while (relTemp) {
			if (relTemp->type == "R_386_PC32") {
				if (relTemp->flinkFlag == 1) {
					if (relTemp->show->content[4] == "") {
						offPC = 3;

					}
					else offPC = 5;

				}
				else {
					offPC = 2;
				}


				if (relTemp->locality == 'g') {
					int res=Binary16toInt(relTemp->show->content[relTemp->flinkFlag == 1 ? 2 : 4]);
					res = 0 - offPC;
					stringstream sstream;
					sstream << bitset<16>(res);
					string ret = sstream.str();
					relTemp->show->content[relTemp->flinkFlag == 1 ? 2 : 4] = ret;
				}

				if (relTemp->locality == 'l') {
					if (relTemp->secInside) {
						int res = Binary16toInt(relTemp->show->content[relTemp->flinkFlag == 1 ? 2 : 4]);
						res = res - relTemp->show->next->beginLC;
						stringstream sstream;
						sstream << bitset<16>(res);
						string ret = sstream.str();
						relTemp->show->content[relTemp->flinkFlag == 1 ? 2 : 4] = ret;
						
						//IZBRISI RELOKACIONI ZAPIS delete relTemp();
						secTemp->p->removeRelocElem(relTemp);

					}
					else {
						int res = Binary16toInt(relTemp->show->content[relTemp->flinkFlag == 1 ? 2 : 4]);
						res = res - relTemp->show->next->beginLC;
						stringstream sstream;
						sstream << bitset<16>(res);
						string ret = sstream.str();
						relTemp->show->content[relTemp->flinkFlag == 1 ? 2 : 4] = ret;
					}
					
				}

			}
			relTemp = relTemp->next;
		}
		
		secTemp = secTemp->next;
	}

		

}

void processMemDirective(string line) {
	AllSections::currentSection->p->processMemDirective(line);
}

void processMemOperand(string s,int help) {
	AllSections::currentSection->p->ConcatenateMemOperand(s,help);
}




void processEQU(string line, int label) {

	char *cstr = &line[0];


	char* pch = strtok(cstr, " ,:");
	if (label) {
		pch = strtok(NULL, " .,:");
	}
	pch = strtok(NULL, " ,.:");
	EQU* equ = new EQU(pch);
	cout << pch << endl;
	//EQUList.push_back(equ);

	pch = strtok(NULL, " ,.:");

	char *bstr = &pch[0];
	int first = 1;
	string helpliteral;

	while (*bstr) {

		if (first) {
			if (isdigit(*bstr)) {
				string digitliteral;

				while (isdigit(*bstr)) {
					digitliteral += *bstr++;
				}
				equ->value += stoi(digitliteral);
				first = 0;
			}
			else {
				string symliteral;
				while (*bstr && (*bstr != '+' && *bstr != '-')) {
					symliteral += *bstr++;
				}
				equ->v.push_back(new EQUElem(symliteral, '+'));
				first = 0;
			}
		}
		else {
			char sign = *bstr++;
			if (isdigit(*bstr)) {
				string digitliteral;

				while (isdigit(*bstr)) {
					digitliteral += *bstr++;
				}
				equ->value = (sign == '+') ? stoi(digitliteral) : -stoi(digitliteral);
			}
			else {
				string symliteral;
				while (*bstr && (*bstr != '+') && (*bstr != '-')) {
					symliteral += *bstr++;
				}
				equ->v.push_back(new EQUElem(symliteral, sign));
			}
		}
		//bstr++;
	}
	lista.push_back(equ);
	equ->sectiondef = allsec->currentSection->SectionName;
	for (EQUElem* n : equ->v) {
		cout << n->name << " " << n->sign << endl;
	}



}

void EQUPROCESSING() {
	int numprocessed = 0;
	int diff1 = 0;
	int diff2 = 0;
	while (1) {
		diff1 = diff2;
		for (EQU* equ : lista) {
			int nd = 0;
			if (equ->razresen == 0) {

				for (EQUElem* elem : equ->v) {
					int definedsymbol = 0;
					

					SymTableElem* temp = ST->first;
					while (temp) {
						if (temp->symbol == elem->name && temp->razresen) { //PROMENA AKO SE RADI O EKSTERNOM SIMBOLU
							definedsymbol = 1;
						}

						temp = temp->next;
					}

					if (definedsymbol == 1) {
						nd++;
					}
				}
				if (nd == (equ->v).size()) {
					PROCESSINDIVIDUALEQU(equ);
					//equ razresen vec ima
					numprocessed++;
					diff2++;

				}
			}
		}
		if (numprocessed == lista.size()) {
			break;
		}
		else if (diff1 == diff2) {
			cout << "KRUZNI EQU" << endl;
			exit(1);
		}

	}
}

void PROCESSINDIVIDUALEQU(EQU* equ) {

	map<string, int> map;
	string sectionNonZero;
	AllSections::SecElem* secTemp = allsec->first;
	while (secTemp) {
		map[secTemp->SectionName] = 0;
		secTemp = secTemp->next;
	}
	for (EQUElem* elem : equ->v) {
		



		//UPISI U HASH MAPU U ZAVISNOSTI OD ZNAKA
	
		SymTableElem* temp = ST->first;
		while (temp) {

			if (elem->name == temp->symbol) {
				if (elem->sign == '+') {
					map[temp->sectionName]++;


				}
				else if (elem->sign == '-') {
					map[temp->sectionName]--;
				}
				elem->locality = temp->locality;
				elem->value = temp->offset;
				elem->section = temp->sectionName;
				elem->symbolNum = temp->number;

			}

			temp = temp->next;
		}
	}
	int flag = 0;
	secTemp = allsec->first;
	while (secTemp) {
		if (map[secTemp->SectionName] != 0 && map[secTemp->SectionName] == 1 || map[secTemp->SectionName] == -1) {
			if (flag == 0) {
				flag = 1;
				sectionNonZero = secTemp->SectionName;
				equ->sectionRelocName = sectionNonZero;
			}
			else {
				//GRESKA
				cout << "EQU MISTAKE!";
				exit(1);
			}
		}

		secTemp = secTemp->next;
	}
	for (EQUElem* elem : equ->v) {
		if (elem->section != sectionNonZero) {
			equ->value += (elem->sign == '+') ? elem->value : -elem->value;
		}
		else {
			if (elem->locality == 'l') {
				SymTableElem* tst = ST->first;
				while (tst) {
					if (tst->symbol == elem->section) {
						equ->sectionReloc = tst->number;
						equ->value += (elem->sign == '+') ? elem->value : -elem->value;
					}
					tst = tst->next;
				}
			}
			else if (elem->locality == 'g') {
				equ->sectionReloc = elem->symbolNum;
				equ->value += (elem->sign == '+') ? elem->value : -elem->value;

			}
		}
	}
	if (equ->value < 0) equ->value = -equ->value;
	equ->razresen = 1;

	if (!ST->first) {
		ST->first = ST->last = new SymTableElem(equ->symbolName, -1,
			(equ->sectionReloc == -1) ? "ABS" : equ->sectionRelocName);
		ST->first->razresen = 1;
	}
	else {
		ST->last->next = new SymTableElem(equ->symbolName, equ->value,
			(equ->sectionReloc == -1) ? "ABS" : equ->sectionRelocName);
		ST->last = ST->last->next;

		ST->last->razresen = 1;
	}
}




void backPatchEQUSetup() {


	for (EQU* equ : lista) {
		map<string, int> map;
		string sectionNonZero;
		AllSections::SecElem* secTemp = allsec->first;
		while (secTemp) {
			map[secTemp->SectionName] = 0;
			secTemp = secTemp->next;
		}
		for (EQUElem* elem : equ->v) {
			string equsym;



			//UPISI U HASH MAPU U ZAVISNOSTI OD ZNAKA
			equsym += elem->name;
			SymTableElem* temp = ST->first;
			while (temp) {

				if (equsym == temp->symbol) {
					if (elem->sign == '+') {
						map[temp->sectionName]++;


					}
					else if (elem->sign == '-') {
						map[temp->sectionName]--;
					}
					elem->locality = temp->locality;
					elem->value = temp->offset;
					elem->section = temp->sectionName;
					elem->symbolNum = temp->number;

				}

				temp = temp->next;
			}
		}
		int flag = 0;
		secTemp = allsec->first;
		while (secTemp) {
			if (map[secTemp->SectionName] != 0 && map[secTemp->SectionName] == 1 || map[secTemp->SectionName] == -1) {
				if (flag == 0) {
					flag = 1;
					sectionNonZero = secTemp->SectionName;
					equ->sectionRelocName = sectionNonZero;
				}
				else {
					//GRESKA
					cout << "EQU MISTAKE!";
					exit(1);
				}
			}

			secTemp = secTemp->next;
		}
		for (EQUElem* elem : equ->v) {
			if (elem->section != sectionNonZero) {
				equ->value += (elem->sign == '+') ? elem->value : -elem->value;
			}
			else {
				if (elem->locality == 'l') {
					SymTableElem* tst = ST->first;
					while (tst) {
						if (tst->symbol == elem->section) {
							equ->sectionReloc = tst->number;
						}
						tst = tst->next;
					}
				}
				else if (elem->locality == 'g') {
					equ->sectionReloc = elem->symbolNum;

				}
			}
		}
		if (equ->value < 0) equ->value = -equ->value;
		equ->razresen = 1;
	}


}//NE KORISTI SE METODA, PROMENJEN NACIN OBRADE


void backPatchEQUFinal() {
	AllSections::SecElem* sectemp = allsec->first;
	while (sectemp) {
		RelocElem* elemtemp = sectemp->p->relSecf;
		while (elemtemp) {

			for (EQU* equ : lista) {
				if (equ->symbolName == elemtemp->symbol && elemtemp->additFlag == 0) {
					stringstream sstream;
					sstream << bitset<16>(equ->value);
					string ret = sstream.str();
					elemtemp->show->content[elemtemp->flinkFlag == 1 ? 2 : 4] = ret;
					if (equ->sectionReloc != -1) {
						ContentSection* pom = sectemp->p;
						if (pom->relSecf == 0) {
							pom->relSecf = pom->relSecl = new
								RelocElem(elemtemp->offset, elemtemp->show->pcRel == 1 ? "R_386_PC32" : "R_386_32", equ->sectionReloc,
									equ->symbolName, elemtemp->show, elemtemp->flinkFlag, equ->sectionRelocName);
							pom->relSecl->additFlag = 1;



						}
						else {
							pom->relSecl->next = new
								RelocElem(elemtemp->offset, elemtemp->show->pcRel == 1 ? "R_386_PC32" : "R_386_32", equ->sectionReloc,
									equ->symbolName, elemtemp->show, elemtemp->flinkFlag, equ->sectionRelocName);
							pom->relSecl = pom->relSecl->next;
							pom->relSecl->additFlag = 1;
						}
					}
				}
			}
			elemtemp = elemtemp->next;
		}
		sectemp = sectemp->next;
	}
}


string ENDIANBUFF(string test) {
	for (int i = 0; i < 8; i++) {
		char temp;
		temp = test[i];
		test[i] = test[i + 8];
		test[i + 8] = temp;
	}
	string ret = test;
	return ret;
}

int Binary16toI(string binary) {
	int res = 0;
	for (int i = 0; i < 16; i++) {
		if (binary[i] == '1') {
			res += pow(2, 15 - i);
		}
	}

	return res;
}
int Binary8toInt(string binary) {
	int res = 0;
	for (int i = 0; i < 8; i++) {
		if (binary[i] == '1') {
			res += pow(2, 7 - i);
		}
	}

	return res;
}
string HEXUP(string s) {
	string temp;
	string res;
	string ret;
	int i = 0;
	int duzina = s.length();
	if (!emptys(s)) {
		if (s.length() == 16) {
			temp = ENDIANBUFF(s);
			i = Binary16toI(temp);
		}
		else if (s.length() == 8) {
			i = Binary8toInt(s);
		}
		stringstream ss;
		
		if (duzina == 8 && i < 16) {
			ret += "0";
		}
		if ((duzina==16) && (i>255) && (i<4095)) {
			ret +="0";
		}
		if ((duzina == 16) && (i > 16) && (i < 256)) {
			ret += "00";
		}
		if ((duzina == 16) && (i < 16)) {
			ret += "000";
		}
		ss << hex << i;
		res = ss.str();
		ret += res;
		return ret;
	}
	return "";
}






#endif