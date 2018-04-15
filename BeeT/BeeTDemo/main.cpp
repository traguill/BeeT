#include "../BeeTLib/beet.h"

#ifdef _DEBUG
	#pragma comment(lib, "../build/demo/BeeTLibd.lib")
#else
	#pragma comment(lib, "../build/demo/BeeTLib.lib")
#endif // DEBUG

// Demo requirements
#include "Tests.h"
#include <iostream>

int g_TestId = 0;

int main(int argc, char** argv)
{
	std::cout << "Enter a test number(1-3):" << std::endl;

	int n = 0;
	while (!(std::cin >> n))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input" << std::endl;
	}
	g_TestId = n;

	BEET_Init();
	RunTest(g_TestId);
	
	BEET_Shutdown();

	system("pause");
	return 0;
}