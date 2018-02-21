#pragma once
#include <atomic>
class HoldPtr
{
public:
	std::atomic <bool> _bLogsWindow;
	std::atomic <bool> _bSearchWindow;
	HoldPtr() : _bLogsWindow(0), _bSearchWindow(0)
	{

	}
};