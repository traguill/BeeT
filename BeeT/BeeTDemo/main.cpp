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
	int num = 4;

	BeeT::TestClass object; 

	int num2 = object.AddFive(num);

	printf("%i\n", num2);

	system("pause");

	return 0;
}