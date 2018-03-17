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
	Init();

	printf("%i\n", BehaviorTreeCount());
	int uid = LoadBehaviorTreeFromFile("bt.txt");
	printf("BT: %i\n", uid);
	printf("%i\n", BehaviorTreeCount());

	
	Shutdown();

	system("pause");
	return 0;
}