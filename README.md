# BeeT  <img height="64" src="https://i.imgur.com/qjj0X7c.png">

BeeT is a Behavior Tree library for C++. It contains a graphical application to easily create, edit and debug Behavior Trees.

Stage: ~~Pre-production~~ - ~~Vertical Slice~~ - ~~Alpha~~ - ~~[Beta]~~ - Gold

Last release: [BeeT](https://github.com/traguill/BeeT/releases/latest)

## How to use it

Include ```beet.h``` and ```BeeTLib.dll``` in your project.

First initialize the library calling:

```C
BEET_Init()
```

Next, we have to provide a function to be called every time a Task node is executed. It has the following format:

```C
NodeStatus MyFunc (int behaviorTreeId, const char* taskName);

// The task returns the state of the node after its execution. It can be:
NS_RUNNING,
NS_SUCCESS,
NS_FAILURE,
NS_SUSPEND

// We assign our callback function with
BEET_SetTaskCallbackFunc(MyFunc);
```

If we want to use the Debugger we have to initialize it at this point with:

```C
BEET_InitDebugger(portNumber); // portNumber must be the same open port the Editor has
```
To load a Behavior Tree we have two options, load it from a file or from memory:

```C
// From a file
int btID = BEET_LoadBehaviorTreeFromFile("myBTFile.json", BEET_TRUE);

// From memory
int btID = BEET_LoadBehaviorTree(myMemoryBuffer, memoryBufferSize, BEET_TRUE);

// Note: The last parameter in both functions indicates whether or not this tree will be considered for debugging. BEET_InitDebugger(int) needs to be called first.
```
An ```int``` is returned with the Behavior Tree identification.

In order to execute all the library logic we need to call ```BEET_Tick``` every frame passing the Delta Time as a parameter.

```C
BEET_Tick(deltaTime); // Updates all the library logic
```

Blackboard variables can be readed and written calling:

```C
// Read a Blackboard bool variable
bool value = BEET_GetBool(btID, "myBoolName");

// Set a Blackboard bool variable to false
bool success = BEET_SetBool(btID, "myBoolName", false);
```
To finish remember always to close the library with:

```C
BEET_Shutdown();
```

