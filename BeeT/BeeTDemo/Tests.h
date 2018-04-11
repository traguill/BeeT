#ifndef __TESTS_H__
#define __TESTS_H__

#include "../BeeTLib/beet.h"

void RunTest(int testId);

NodeStatus TestCallbackFunc(int testId, unsigned int btUid, const char* taskId);

// TESTS ---------------------------------------------------

// Loads a BT file named 'bt.json'. 
// Prints the number of BTs loaded before and after. 
// Prints the BT loaded id.
void Test1();


// Tests all the dequeue functionality.
void Test2();

// Loads a BT file named 'testing.json'.
// Executes the BT to test the following nodes:
//		- Sequence - Task
void Test3();
NodeStatus Test3CallbackFunc(unsigned int btUid, const char* taskId);


#endif // !__TESTS_H__
