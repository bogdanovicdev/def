#ifndef ASSEMBLY_H_
#define ASSEMBLY_H_

#include "Assembly.h"
#include <vector>
#include "EQU.h"
#include <math.h>
#include <iostream>
#include "ContentSection.h"
#include "SymTable.h"
#include "GlobalSymbolsTable.h"
#include "AllSections.h"
#include "InjectorFunctions.h"
using namespace std;


 GlobalSymbolsTable* gst=new GlobalSymbolsTable();
 SymTable* ST=new SymTable();
 int kraj = 0;


map<string, char> equMap;
AllSections* allsec=new AllSections();
std::vector<EQU*> lista;

ifstream inFile;
ofstream outFile;
void BackPatch32();




#endif
	


