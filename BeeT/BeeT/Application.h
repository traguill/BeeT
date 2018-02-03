#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <vector>

// Forward declaration of modules
class Module;
class Window;
class Renderer;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	bool CleanUp();

public:
	// Modules
	Window* window = nullptr;
	Renderer* renderer = nullptr;

private:
	std::vector<Module*> modules;

	std::vector<Module*> startModules;
	std::vector<Module*> preUpdateModules;
	std::vector<Module*> updateModules;
	std::vector<Module*> postUpdateModules;
};
extern Application* g_app;

#endif