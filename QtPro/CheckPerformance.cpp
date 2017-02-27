#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include<ctime>

#include <windows.h>


void StartCounter(__int64& CounterStart, double &PCFreq)
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		std::cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double GetCounter(__int64& CounterStart, double &PCFreq)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

int modele()
{
	//StartCounter();
	Sleep(1000);
	//std::cout << GetCounter() << "\n";
	return 0;
}