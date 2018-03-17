#include "../BeeTLib/beet.h"

#ifdef _DEBUG
	#pragma comment(lib, "../build/demo/BeeTLibd.lib")
#else
	#pragma comment(lib, "../build/demo/BeeTLib.lib")
#endif // DEBUG

// Demo requirements
#include <iostream>

int main(int argc, char** argv)
{
	BEET_Init();

	printf("%i\n", BEET_BehaviorTreeCount());
	int uid = BEET_LoadBehaviorTreeFromFile("bt.json");
	printf("BT: %i\n", uid);
	printf("%i\n", BEET_BehaviorTreeCount());
	
	BEET_Shutdown();

	system("pause");
	return 0;
}