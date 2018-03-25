#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <vector>

// Application lifecycle steps
enum AppStep
{
	APP_START,
	APP_LOOP,
	APP_END,
	APP_QUIT
};

// Forward declaration of modules
class Module;
class FileSystem;
class Window;
class Input;
class Renderer;
class BeeTGui;
class Network;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	bool CleanUp();
	bool Update();

	// Add modules in steps. These methods doesn't check duplicates
	void AddModuleStart(Module* moduleToAdd);
	void AddModulePreUpdate(Module* moduleToAdd);
	void AddModuleUpdate(Module* moduleToAdd);
	void AddModulePostUpdate(Module* moduleToAdd);

	// Quick way to exit the application
	void RequestQuit();

public:
	// Modules
	FileSystem* fileSystem = nullptr;
	Window* window = nullptr;
	Input* input = nullptr;
	Renderer* renderer = nullptr;
	BeeTGui* beetGui = nullptr;
	Network* network = nullptr;

private:
	std::vector<Module*> modules;

	std::vector<Module*> startModules;
	std::vector<Module*> preUpdateModules;
	std::vector<Module*> updateModules;
	std::vector<Module*> postUpdateModules;

	bool quitRequested = false;
};
extern Application* g_app;

#endif