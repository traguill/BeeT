---
layout: post
title:  "Get Started"
date:   2016-02-06 15:08:10 -0500
img: img/portfolio/cabin.png
modalID: get-started
category: Tutorial
---

# WORK IN PROGRESS

This is a quick guide of how to integrate **BeeT** into your project in a few minutes. It will aslo teach how to create a behavior tree with the editor.

First of all, **download the latest release** of BeeT.

If you want to skip the behavior tree creation go here.


### Create a simple tree

1- Open BeeT Editor.

2- Create a tree

3- Save it

### Integrate BeeT library


1- Include the `beet.h` header and link `BeeTLib` library to your project.

2- Initialize the library with `BEET_Init()` and before closing the application remember to call `BEET_Shutdown()`

3- (Optional) If you want to enable the debugger, call `BEET_InitDebugger` after the initialization. The call is the following:

```cpp
BEET_bool BEET_InitDebugger(const char* ip, int port);
```

You will need to pass the ip and the port of the computer where your application will connect to debug, not the computer that your application will run (or yes if you are using the same). The ip will be a const char like: "127.0.0.1".

4- Call `BEET_Tick(deltaTime)` in your main loop and send the deltaTime (time since last frame) as a parameter.

5- Set up the task callback function, the function that will be called once the tree reaches a leaf node (always will be a task node), with `BEET_SetTaskCallbackFunc(TaskCallbackFunc);`. The callback function must have this structure:

`NodeStatus TaskCallbackFunc(unsigned int btId, const char* taskId, NodeFunction func, const BBVar* extraData);`

**NodeStatus**: the status of the task after being executed. It can either be: `NS_SUCCESS`, `NS_FAILURE` or `NS_RUNNING`.

**btId**: the id of the tree that is running that task

**NodeFunction**: the function that is called for that task. It can be: `NF_ONINIT`, `NF_UPDATE` or `NF_ONFINISH`.

**BBVar**: pointer to a variable of the blackboard passed as extra data for that task. If no extra data has been passed this will be NULL.

6- To load a behavior tree you can either:

   - Load it from memory with `BEET_LoadBehaviorTree(const char* buffer, int size, BEET_bool debug);`
  
   - Load it from a file with `BEET_LoadBehaviorTreeFromFile(const char* filename, BEET_bool debug);`
   
The `debug` boolean will specify if this behavior tree is considered for debugging. Note: the debugger needs to be initialized beforehand in order to work.

7- Save the behavior tree id returned by the previous function to keep track of the tree. With that id you can change the blackboard variables later on with calls like:

` BEET_bool		BEET_BBGetBool(unsigned int btId, const char* varName);`

` BEET_bool		BEET_BBSetBool(unsigned int btId, const char* varName, BEET_bool value);`
