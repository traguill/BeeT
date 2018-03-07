#include "FileDialog.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Application.h"
#include "FileSystem.h"

#include <vector>

using namespace std;

FileDialog::FileDialog()
{
	path = g_app->fileSystem->GetDirectory();
}

FileDialog::~FileDialog()
{
}

void FileDialog::Draw()
{
	
	ImGui::SetNextWindowFocus();
	//ImGui::ShowTestWindow();
	
	ImGui::Begin("File Dialog Title");

	ImGui::Text("%s ##filedialogwindow_filename", path.data());
	
	ImGui::BeginChild("##filedialogwindow_listfiles", ImVec2(ImGui::GetWindowContentRegionWidth(), 300), false, ImGuiWindowFlags_HorizontalScrollbar);
	
	vector<string> files;
	g_app->fileSystem->EnumerateFiles("/", files);
	for (auto file : files)
	{
		if (g_app->fileSystem->IsDirectory(file.data()))
			ImGui::Text("'-> %s", file.data());
		else
			ImGui::Text("* %s", file.data());
	}
		

	ImGui::EndChild();
	

	//ImGui::Text("Name: "); ImGui::SameLine(); ImGui::InputText("##filedialogwindow_name", &b, 1);
	ImGui::Text("Format: *.txt"); 

	ImGui::Button("Accept##filedialogwindow_accept");
	ImGui::SameLine();
	ImGui::Button("Cancel##filedialogwindow_cancel");

	ImGui::End();
}
