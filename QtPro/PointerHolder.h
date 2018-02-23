#pragma once
#include <atomic>
class HoldPtr
{
public:
	std::atomic <bool> _bLogsWindow;
	std::atomic <bool> _bGlobalSearchInstance;
	HoldPtr() : _bLogsWindow(0), _bGlobalSearchInstance(0)
	{

	}
};