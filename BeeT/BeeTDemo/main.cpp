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
	BeeT::Init();

	//BeeT::LoadBehaviorTree("bt.txt");
	BeeT::Shutdown();

	system("pause");
	return 0;
}