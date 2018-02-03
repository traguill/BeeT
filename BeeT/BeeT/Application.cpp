#include "Application.h"
#include "Log.h"
#include "Module.h"
#include "Window.h"
#include "Renderer.h"

using namespace std;

Application::Application()
{
	window = new Window("Window");
	renderer = new Renderer("Renderer");

	modules.push_back(window);
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
