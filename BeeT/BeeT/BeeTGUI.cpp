#include "BeeTGui.h"
#include "Application.h"
#include "Log.h"

BeeTGui::BeeTGui(const char* name) : Module(name)
{}

BeeTGui::~BeeTGui()
{}

bool BeeTGui::Init()
{
	g_app->AddModuleUpdate(this);
	return true;
}

bool BeeTGui::CleanUp()
{
	return true;
}

bool BeeTGui::Update()
{
	
	return true;
}