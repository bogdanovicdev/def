#pragma once
#include <map>
#include <string>



using namespace std;

class ContentSection;
class EQU;
struct ContElem;
int processDirective(string line);

int processLabel(string line);

int processInstruction(string line);



map<string, string> createHASH_MAP();


int test_regex(string line);


int BackPatchCode();

void processMemOperand(string s,int help);

void addReloc(string s, ContElem* p, int i);

int SetUpAssembler();

void processOPCode(string line);

void processMemDirective(string line);

void BackPatchReloc();

void BackPatchPCRel();

void backPatchEQUSetup();

void backPatchEQUFinal();

void BackPatch32();

void processEQU(string line,int label);
void processOperand(string line, string line2);


void EQUPROCESSING();

void PROCESSINDIVIDUALEQU(EQU* equ);


void processJump(string line);


