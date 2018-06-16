#include "BeeTGui.h"
#include "Application.h"
#include "Log.h"
#include "BeeTEditor.h"
#include "BTNodeTypes.h"
#include "FileDialog.h"
#include "OutputLog.h"
#include "BeeTDebugger.h"

#include "FileSystem.h" // Testing: To show the current directory path in MainMenuBar->File.
#include "Network.h"

#include "ThirdParty/ImGui/imgui.h"
#include "IconsFont.h"

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
	//ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowRounding, 0.0f);
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
	//ImGui::PopStyleVar(); // WindowRounding
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
			EditMenuBar();
			ImGui::EndMenu();
		}
		else if (g_app->network->settingsDirty)
			g_app->network->ResetSettings();

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
			if (ImGui::MenuItem(ICON_MODE_EDIT " Editor###mode_editor"))
			{
				mode = BeeTMode::BEET_EDITOR;
			}
			if (ImGui::MenuItem(ICON_BUG " Debug###mode_debug"))
			{
				mode = BeeTMode::BEET_DEBUGGER;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Show"))
		{
			ShowMenuBar();
			ImGui::EndMenu();
		}
		
		if (mode == BeeTMode::BEET_EDITOR)
		{
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_MODE_EDIT "EDIT").x - 20);
			ImGui::TextColored(ImVec4(0.0f, 0.75f, 0.2f, 1.0f), ICON_MODE_EDIT " EDIT");
		}
		else // Debug
		{
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_BUG " DEBUG").x - 20);
			if (g_app->network->IsListening())
				ImGui::TextColored(ImVec4(0.0f, 0.75f, 0.2f, 1.0f), ICON_BUG);
			else
				ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), ICON_BUG);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 0.75f, 0.2f, 1.0f), "DEBUG");
		}
		ImGui::EndMainMenuBar();
	}
}

void BeeTGui::FileMenuBar()
{
	if (ImGui::MenuItem(ICON_FILE " New##menubar_file_new"))
	{
		if (mode == BeeTMode::BEET_EDITOR)
		{
			fileDialog->ClearLastOpenedFileName();
			beetEditor->NewBehaviorTree();
		}
	}
	if (ImGui::MenuItem(ICON_OPEN " Open##menubar_file_open"))
	{
		if (mode == BeeTMode::BEET_EDITOR)
		{
			fileDialog->EnableWindow(FileDialogMode::OPEN_FILE);
			fileDialog->SetAcceptFunctionCallback(BeeTGui::LoadFile, this);
		}
	}
	if (ImGui::MenuItem(ICON_SAVE " Save##menubar_file_save"))
	{
		if (mode == BeeTMode::BEET_EDITOR)
		{
			fileDialog->EnableWindow(FileDialogMode::SAVE_FILE);
			fileDialog->SetAcceptFunctionCallback(BeeTGui::SaveFile, this);
		}
	}
	if (ImGui::MenuItem(ICON_BUG " Quit"))
	{
		g_app->RequestQuit();
	}
	ImGui::Text("Working directory:");
	ImGui::Separator();
	ImGui::Text(g_app->fileSystem->GetDirectory().data());
}

void BeeTGui::EditMenuBar()
{
	Network* nw = g_app->network;

	ImGui::Text("Settings");
	ImGui::Separator();
	// IP
	ImGui::PushItemWidth(100);
	ImGui::Text("Debug IP:"); ImGui::SameLine();
	char buf[250];
	strcpy_s(buf, nw->tmpIp.length() + 1, nw->tmpIp.data());
	if (ImGui::InputText("###debug_ip_input", buf, 250))
	{
		nw->tmpIp = buf;
		nw->settingsDirty = true;
	}
	// Port
	ImGui::Text("Debug port:"); ImGui::SameLine();
	if (ImGui::DragInt("###debug_port_input", &nw->tmpPort, 1, 0, 9999))
		nw->settingsDirty = true;
	ImGui::PopItemWidth();
	// Active
	if (nw->IsListening())
		ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), ICON_BUG);
	else
		ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), ICON_BUG);
	ImGui::SameLine();
	ImGui::Text("Debug active: ");
	ImGui::SameLine();
	if (ImGui::Checkbox("###debug_is_enabled", &nw->tmpListening))
		nw->settingsDirty = true;
	
	if (ImGui::Button("Apply Settings"))
	{
		nw->ApplySettings();
	}
}

void BeeTGui::OpenWindows()
{
	if (g_output->windowOpen)
		g_output->Draw();
}

void BeeTGui::ShowMenuBar()
{
	if (ImGui::MenuItem("Node ID", NULL, showNodeId))
	{
		showNodeId = !showNodeId;
	}
}

void BeeTGui::LoadFile(void* obj, const char* filename)
{
	((BeeTGui*)obj)->beetEditor->Load(filename);
}

void BeeTGui::SaveFile(void* obj, const char* filename)
{
	((BeeTGui*)obj)->beetEditor->Serialize(filename);
}
