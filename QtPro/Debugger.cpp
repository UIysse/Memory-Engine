#include <Windows.h>


LONG CALLBACK VectoredHandler2(
	_In_ PEXCEPTION_POINTERS ExceptionInfo
)
{
	exit(2);
}
bool goo()
{
	if (AddVectoredExceptionHandler(1, VectoredHandler2))
		return true;
	else
		return false;
}



