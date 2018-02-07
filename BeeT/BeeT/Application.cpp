#include "Application.h"
#include "Log.h"
#include "Module.h"
#include "Window.h"
#include "Input.h"
#include "Renderer.h"
#include "BeeTGui.h"

using namespace std;

Application::Application()
{
	window = new Window("Window");
	input = new Input("Input");
	beetGui = new BeeTGui("BeeTGui");
	renderer = new Renderer("Renderer");

	modules.push_back(window);
	modules.push_back(input);
	modules.push_back(beetGui);
	modules.push_back(renderer);
}

Application::~Application()
{
	for (vector<Module*>::reverse_iterator itModule = modules.rbegin(); itModule != modules.rend(); ++itModule)
	{
		delete *itModule;
	}
}

bool Application::Init()
{
	bool ret = false;
	for (vector<Module*>::iterator itModule = modules.begin(); itModule != modules.end(); ++itModule) 
	{
		ret = (*itModule)->Init();
		if (ret == false)
			break;
	}
	return ret;
}

bool Application::CleanUp()
{
	bool ret = false;
	for (vector<Module*>::reverse_iterator itModule = modules.rbegin(); itModule != modules.rend(); ++itModule)
	{
		ret = (*itModule)->CleanUp();
		if (ret == false)
		{
			LOGE("Module %s failed during CleanUp step", (*itModule)->GetName());
		}
	}
	return true;
}

bool Application::Update()
{
	// PreUpdate
	for (vector<Module*>::iterator itModule = preUpdateModules.begin(); itModule != preUpdateModules.end(); ++itModule)
	{
		if ((*itModule)->PreUpdate() == false)
			return false;
	}

	// Update
	for (vector<Module*>::iterator itModule = updateModules.begin(); itModule != updateModules.end(); ++itModule)
	{
		if ((*itModule)->Update() == false)
			return false;
	}
	
	// PostUpdate
	for (vector<Module*>::iterator itModule = postUpdateModules.begin(); itModule != postUpdateModules.end(); ++itModule)
	{
		if ((*itModule)->PostUpdate() == false)
			return false;
	}

	return true;
}

void Application::AddModuleStart(Module * moduleToAdd)
{
	startModules.push_back(moduleToAdd);
}

void Application::AddModulePreUpdate(Module * moduleToAdd)
{
	preUpdateModules.push_back(moduleToAdd);
}

void Application::AddModuleUpdate(Module * moduleToAdd)
{
	updateModules.push_back(moduleToAdd);
}

void Application::AddModulePostUpdate(Module * moduleToAdd)
{
	postUpdateModules.push_back(moduleToAdd);
}
