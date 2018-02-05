#include "BeeTGUI.h"
#include "Application.h"

namespace ne = ax::NodeEditor;

BeeTGUI::BeeTGUI(const char* name) : Module(name)
{}

BeeTGUI::~BeeTGUI()
{}

bool BeeTGUI::Init()
{
	editorContext = ne::CreateEditor();

	g_app->AddModuleUpdate(this);

	return true;
}

bool BeeTGUI::CleanUp()
{
	ne::DestroyEditor(editorContext);
	return true;
}

bool BeeTGUI::Update()
{
	ne::SetCurrentEditor(editorContext);
	ne::Begin("Test Editor");

	int uid = 1;

	ne::BeginNode(uid++);
	ImGui::Text("Node 1");
	ne::BeginPin(uid++, ne::PinKind::Target);
	ImGui::Text("In");
	ne::EndPin();
	ne::BeginPin(uid++, ne::PinKind::Source);
	ImGui::Text("Out");
	ne::EndPin();
	ne::EndNode();

	ne::End();
	return true;
}
