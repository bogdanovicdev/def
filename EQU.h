#pragma once
#ifndef _EQU_H_
#define _EQU_H_


#include <string>
#include <iostream>
#include <vector>
using namespace std;
struct EQUElem {
	string name;
	char sign;
	char locality='n';
	int value = 0;
	int symbolNum = -1;
	int secNum = -1;
	string section;
public:
	EQUElem(string cname, char csign) {
		name = cname;
		sign = csign;
	}

};


class EQU
{
	
public:
	EQU(string csymbolName);
	~EQU();
	std::vector<EQUElem*> v = {};
	string symbolName;
	int value;
	int sectionReloc = -1;
	int razresen = 0;
	string sectionRelocName;
	string sectiondef;
	


};




#endif

