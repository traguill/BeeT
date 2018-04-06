#include "../BeeTLib/beet.h"

#ifdef _DEBUG
	#pragma comment(lib, "../build/demo/BeeTLibd.lib")
#else
	#pragma comment(lib, "../build/demo/BeeTLib.lib")
#endif // DEBUG

// Demo requirements
#include "Tests.h"
#include <iostream>

int main(int argc, char** argv)
{
	BEET_Init();

	RunTest(2);
	
	BEET_Shutdown();

	system("pause");
	return 0;
}