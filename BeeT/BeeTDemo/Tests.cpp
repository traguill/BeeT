#include "Tests.h"

// Test 2
#include "../BeeTLib/beet_std.h"

#include <iostream>

using namespace std;

void RunTest(int testId)
{
	switch (testId)
	{
	case 1:
		Test1();
		break;
	case 2:
		Test2();
		break;
	case 3:
		Test3();
		break;
	case 4:
		Test4();
		break;
	case 5:
		Test5();
		break;
	case 6:
		Test6();
		break;
	default:
		cout << "There is no test num " << testId << endl;
		break;
	}
}

// --------------------------------------------------------------------
// TEST 1
// --------------------------------------------------------------------
void Test1()
{
	printf("%i\n", BEET_BehaviorTreeCount());
	unsigned int uid = BEET_LoadBehaviorTreeFromFile("bt.json");
	printf("BT: %u\n", uid);
	printf("%i\n", BEET_BehaviorTreeCount());
}

// --------------------------------------------------------------------
// TEST 2
// --------------------------------------------------------------------
void Test2()
{
	dequeue* d = InitDequeue();
	if (d)
	{
		printf("Dequeue creation successful\n");

		// Test push_back
		for (int i = 0; i < 6; i++)
			dequeue_push_back(d, &i);
		int front = *(int*)dequeue_front(d);
		int back = *(int*)dequeue_back(d);
		printf("Push_back - Front %d\n", front);
		printf("Push_back - Back %d\n", back);

		// Test push_front
		int luckyNum = 7;
		for(int i = 0; i < 3; i++)
			dequeue_push_front(d, &luckyNum);

		front = *(int*)dequeue_front(d);
		back = *(int*)dequeue_back(d);
		printf("Push_front - Front %d\n", front);
		printf("Push_front - Back %d\n", back);

		luckyNum = 4;
		dequeue_push_front(d, &luckyNum);
		front = *(int*)dequeue_front(d);
		printf("Prev Front %d\n", front);

		// Test pop back
		for (int i = 0; i < 3; i++)
			dequeue_pop_back(d);
		back = *(int*)dequeue_back(d);
		printf("Pop back: %d\n", back);

		// Test pop front
		dequeue_pop_front(d);
		front = *(int*)dequeue_front(d);
		printf("Pop front %d\n", front);		

		DestroyDequeue(d);
		printf("Dequeue destroy successful\n");
	}
}

// --------------------------------------------------------------------
// TEST 3
// --------------------------------------------------------------------

NodeStatus Test3PatrolFunc(unsigned int btUid, const char * taskId)
{
	printf("BT(%u): %s - Patrolling the area...\n", btUid, taskId);
	return NS_SUCCESS;
}

NodeStatus Test3ShootFunc(unsigned int btUid, const char * taskId)
{
	printf("BT(%u): %s - The player! Shoot him!\n", btUid, taskId);
	return NS_SUCCESS;
}

NodeStatus Test3DieFunc(unsigned int btUid, const char * taskId)
{
	printf("BT(%u): %s - R.I.P\n", btUid, taskId);
	return NS_SUCCESS;
}

void Test3()
{
	unsigned int uid = BEET_LoadBehaviorTreeFromFile("bt.json");
	printf("Behavior Tree loaded with id %u\n", uid);
	
	int result = 0;
	result = BEET_SetTaskCallbackFunc(uid, "Patrol", Test3PatrolFunc);
	if (result)
		printf("Patrol callback function set correctly\n");
	else
		printf("Error while setting Patrol callback function\n");
	
	result = BEET_SetTaskCallbackFunc(uid, "Shoot", Test3ShootFunc);
	if (result)
		printf("Shoot callback function set correctly\n");
	else
		printf("Error while setting Shoot callback function\n");
	
	result = BEET_SetTaskCallbackFunc(uid, "Die", Test3DieFunc);
	if (result)
		printf("Die callback function set correctly\n");
	else
		printf("Error while setting Die callback function\n");

	printf("Running Behavior Tree:\n-----------------------------------------\n");
	BEET_ExecuteBehaviorTree(uid);
	printf("BehaviorTree end\n"); 
}

void Test4()
{
	unsigned int uid = BEET_LoadBehaviorTreeFromFile("blackboard.json");
	printf("Behavior Tree loaded with id %u\n", uid);

	bool mybool;
	int myint;
	float myfloat;
	string mystring;

	mybool = (bool)BEET_BBGetBool(uid, "mybool");
	printf("mybool = %i\n", mybool);

	myint = BEET_BBGetInt(uid, "myint");
	printf("myint = %i\n", myint);

	myfloat = BEET_BBGetFloat(uid, "myfloat");
	printf("myfloat = %f\n", myfloat);

	mystring = BEET_BBGetString(uid, "mystring");
	printf("mystring = %s\n", mystring.data());

	BEET_BBSetBool(uid, "mybool", BEET_FALSE);
	BEET_BBSetInt(uid, "myint", 4);
	BEET_BBSetFloat(uid, "myfloat", 123.456f);
	BEET_BBSetString(uid, "mystring", "Goodbye World");

	printf("----------------------------------\n");
	
	mybool = (bool)BEET_BBGetBool(uid, "mybool");
	printf("mybool = %i\n", mybool);

	myint = BEET_BBGetInt(uid, "myint");
	printf("myint = %i\n", myint);

	myfloat = BEET_BBGetFloat(uid, "myfloat");
	printf("myfloat = %f\n", myfloat);

	mystring = BEET_BBGetString(uid, "mystring");
	printf("mystring = %s\n", mystring.data());
}

NodeStatus Test5IsSeven(unsigned int btUid, const char * taskId)
{
	printf("%s\n", taskId);
	return NS_SUCCESS;
}

NodeStatus Test5Hello(unsigned int btUid, const char * taskId)
{
	printf("%s\n", taskId);
	return NS_SUCCESS;
}

void Test5()
{
	unsigned int uid = BEET_LoadBehaviorTreeFromFile("blackboard.json");
	printf("Behavior Tree loaded with id %u\n", uid);

	BEET_SetTaskCallbackFunc(uid, "IsSeven", Test5IsSeven);
	BEET_SetTaskCallbackFunc(uid, "Hello", Test5Hello);

	printf("Running Behavior Tree:\n-----------------------------------------\n");
	BEET_ExecuteBehaviorTree(uid);
	printf("BehaviorTree end\n");
}

void Test6()
{
	bool ret = BEET_InitDebugger(8080);
	
	if (ret)
		printf("BeeT Debugger initialized successfuly\n");
	else
		printf("BeeT Debugger failed at initializing\n");

	BEET_DebuggerSetActive(BEET_TRUE);

	unsigned int uid = BEET_LoadBehaviorTreeFromFile("blackboard.json");
	printf("Behavior Tree loaded with id %u\n", uid);

	BEET_SetTaskCallbackFunc(uid, "IsSeven", Test5IsSeven);
	BEET_SetTaskCallbackFunc(uid, "Hello", Test5Hello);

	while (true)
	{
		BEET_Tick();
	}
	
}
