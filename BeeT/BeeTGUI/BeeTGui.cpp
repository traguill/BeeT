#include "BeeTGui.h"
#include "Application.h"
#include "Log.h"
#include "BeeTEditor.h"
#include "BTNodeTypes.h"
#include "FileDialog.h"
#include "OutputLog.h"
#include "BeeTDebugger.h"

#include "FileSystem.h" // Testing: To show the current directory path in MainMenuBar->File.

#include "ThirdParty/ImGui/imgui.h"

namespace ne = ax::NodeEditor;

BeeTGui::BeeTGui(const char* name) : Module(name)
{}

BeeTGui::~BeeTGui()
{}

bool BeeTGui::Init()
{
	currentEditorContext = ne::CreateEditor();
	ne::SetCurrentEditor(currentEditorContext);
	editorContextList.push_back(currentEditorContext);

	// Init node types file
	btNodeTypes = new BTNodeTypes();
	btNodeTypes->Init();
	
	beetEditor = new BeeTEditor();
	beetEditor->Init();

	beetDebugger = new BeeTDebugger();
	beetDebugger->Init();

	fileDialog = new FileDialog();
	g_app->AddModuleUpdate(this);
	return true;
}

bool BeeTGui::CleanUp()
{
	beetDebugger->CleanUp();
	delete beetDebugger;
	delete fileDialog;
	delete btNodeTypes;
	beetEditor->CleanUp();
	delete beetEditor;
	for (auto editorContext : editorContextList)
	{
		ne::DestroyEditor(editorContext);
	}
	return true;
}

bool BeeTGui::Update()
{
	MenuBar();
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowRounding, 0.0f);
	OpenWindows(); // Draw currently open windows
	fileDialog->Draw();
	bool ret = false;
	switch (mode)
	{
	case BEET_EDITOR:
		ret = beetEditor->Update();
		break;
	case BEET_DEBUGGER:
		ret = beetDebugger->Update();
		break;
	}
	ImGui::PopStyleVar(); // WindowRounding
	return ret;
}

void BeeTGui::SetCurrentEditorContext(int id)
{
	if (id < editorContextList.size())
	{
		currentEditorContext = editorContextList[id];
		ne::SetCurrentEditor(currentEditorContext);
	}
}

void BeeTGui::SetCurrentEditorContext(ax::NodeEditor::EditorContext * context)
{
	assert(context);
	currentEditorContext = context;
	ne::SetCurrentEditor(currentEditorContext);
}

ax::NodeEditor::EditorContext * BeeTGui::GetNodeEditorContext() const
{
	return currentEditorContext;
}

int BeeTGui::CreateNodeEditorContext()
{
	editorContextList.push_back(ne::CreateEditor());

	return editorContextList.size() - 1;
}

void BeeTGui::MenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			FileMenuBar();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("This is a placeholder");
			ImGui::MenuItem("This is another placeholder");
			ImGui::MenuItem("This is the last placeholder");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Output", NULL, g_output->windowOpen))
			{
				g_output->windowOpen = !g_output->windowOpen;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Mode"))
		{
			if (ImGui::MenuItem("Editor###mode_editor"))
			{
				mode = BeeTMode::BEET_EDITOR;
			}
			if (ImGui::MenuItem("Debug###mode_debug"))
			{
				mode = BeeTMode::BEET_DEBUGGER;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void BeeTGui::FileMenuBar()
{
	ImGui::Text(g_app->fileSystem->GetDirectory().data());
	ImGui::Separator();
	if (ImGui::MenuItem("New##menubar_file_new"))
	{
		if (mode == BeeTMode::BEET_EDITOR)
		{
			fileDialog->ClearLastOpenedFileName();
			beetEditor->NewBehaviorTree();
		}
	}
	if (ImGui::MenuItem("Open##menubar_file_open"))
	{
		if (mode == BeeTMode::BEET_EDITOR)
		{
			fileDialog->EnableWindow(FileDialogMode::OPEN_FILE);
			fileDialog->SetAcceptFunctionCallback(BeeTGui::LoadFile, this);
		}
	}
	if (ImGui::MenuItem("Save##menubar_file_save"))
	{
		if (mode == BeeTMode::BEET_EDITOR)
		{
			fileDialog->EnableWindow(FileDialogMode::SAVE_FILE);
			fileDialog->SetAcceptFunctionCallback(BeeTGui::SaveFile, this);
		}
	}
	if (ImGui::MenuItem("Quit"))
	{
		g_app->RequestQuit();
	}
}

void BeeTGui::OpenWindows()
{
	if (g_output->windowOpen)
		g_output->Draw();
}

void BeeTGui::LoadFile(void* obj, const char* filename)
{
	((BeeTGui*)obj)->beetEditor->Load(filename);
}

void BeeTGui::SaveFile(void* obj, const char* filename)
{
	((BeeTGui*)obj)->beetEditor->Serialize(filename);
}
