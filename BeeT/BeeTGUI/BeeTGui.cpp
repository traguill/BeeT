#include "BeeTGui.h"
#include "Application.h"
#include "Log.h"
#include "BeeTEditor.h"
#include "BTNodeTypes.h"

#include "FileSystem.h" // Testing: To show the current directory path in MainMenuBar->File.

#include "ThirdParty/ImGui/imgui.h"

namespace ne = ax::NodeEditor;

BeeTGui::BeeTGui(const char* name) : Module(name)
{}

BeeTGui::~BeeTGui()
{}

bool BeeTGui::Init()
{
	editorContext = ne::CreateEditor();

	// Init node types file
	btNodeTypes = new BTNodeTypes();
	btNodeTypes->Init();
	
	beetEditor = new BeeTEditor();
	beetEditor->Init();

	g_app->AddModuleUpdate(this);
	return true;
}

bool BeeTGui::CleanUp()
{
	delete btNodeTypes;
	beetEditor->CleanUp();
	delete beetEditor;
	ne::DestroyEditor(editorContext);
	return true;
}

bool BeeTGui::Update()
{
	MenuBar();
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowRounding, 0.0f);
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
	ImGui::PopStyleVar(); // WindowRounding
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
			ImGui::MenuItem(g_app->fileSystem->GetDirectory().data());
			if (ImGui::MenuItem("Save##menubar_file_save"))
			{
				if (mode == BeeTMode::BEET_EDITOR)
				{
					beetEditor->Serialize();
				}
			}
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
