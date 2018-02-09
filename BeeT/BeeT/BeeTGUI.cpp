#include "BeeTGui.h"
#include "Application.h"
#include "Log.h"
#include "BeeTEditor.h"

#include "ThirdParty/ImGui/imgui.h"

namespace ne = ax::NodeEditor;

BeeTGui::BeeTGui(const char* name) : Module(name)
{}

BeeTGui::~BeeTGui()
{}

bool BeeTGui::Init()
{
	editorContext = ne::CreateEditor();
	beetEditor = new BeeTEditor();

	g_app->AddModuleUpdate(this);
	return true;
}

bool BeeTGui::CleanUp()
{
	delete beetEditor;
	ne::DestroyEditor(editorContext);
	return true;
}

bool BeeTGui::Update()
{
	MenuBar();

	bool ret = false;
	switch (mode)
	{
	case BEET_EDITOR:
		ret = beetEditor->Update();
		break;
	case BEET_DEBUGGER:
		// TODO
		break;
	}

	return ret;
}

ax::NodeEditor::EditorContext * BeeTGui::GetNodeEditorContext() const
{
	return editorContext;
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
