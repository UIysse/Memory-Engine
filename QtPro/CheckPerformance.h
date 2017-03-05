#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include<ctime>
#include <chrono>
#include <windows.h>
#include "MemRead.h"
#define CPPOUT fout

void StartCounter(__int64& CounterStart, double &PCFreq);
double GetCounter(__int64& CounterStart, double &PCFreq);
//RAII performance class
class GetPerf
{
	clock_t _start;
	std::string _name;
public:
	GetPerf(std::string &name) : _name (name), _start(clock())
	{

	}
	GetPerf(const char* name) : _name(name), _start(clock())
	{

	}
	~GetPerf() {
		auto _end = clock();
		CPPOUT << "Execution time : " << (_end - _start) / CLOCKS_PER_SEC << std::endl;
	}
};