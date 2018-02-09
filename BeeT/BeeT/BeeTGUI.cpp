#include "BeeTGui.h"
#include "Application.h"
#include "Log.h"

#include "ThirdParty/ImGui/imgui.h"

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
	MenuBar();
	return true;
}

void BeeTGui::MenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("This is a placeholder");
			ImGui::MenuItem("This is another placeholder");
			ImGui::MenuItem("This is the last placeholder");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Quit"))
		{
			g_app->RequestQuit();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
