#include "FileDialog.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Application.h"
#include "FileSystem.h"

#include <vector>

using namespace std;

FileDialog::FileDialog()
{
	path = g_app->fileSystem->GetDirectory();
	format = ".txt";
}

FileDialog::~FileDialog()
{
}

void FileDialog::Draw()
{
	
	ImGui::SetNextWindowFocus();
	//ImGui::ShowTestWindow();
	
	ImGui::Begin("File Dialog Title");

	ImGui::Text("%s", path.data());
	ImGui::Separator();
	ImGui::BeginChild("##filedialogwindow_listfiles", ImVec2(ImGui::GetWindowContentRegionWidth(), 300), false, ImGuiWindowFlags_HorizontalScrollbar);
	
	vector<string> files, filtered;
	g_app->fileSystem->EnumerateFiles("/", files);
	g_app->fileSystem->RemoveDirectoriesFromList(files);
	g_app->fileSystem->FilterFiles(files, filtered, format);
	for (auto file : filtered)
	{
		ImGui::Text("	* %s", file.data());
	}
		

	ImGui::EndChild();
	
	ImGui::Separator();
	//ImGui::Text("Name: "); ImGui::SameLine(); ImGui::InputText("##filedialogwindow_name", &b, 1);
	ImGui::Text("Format: *%s", format.data()); 

	ImGui::Button("Accept##filedialogwindow_accept");
	ImGui::SameLine();
	ImGui::Button("Cancel##filedialogwindow_cancel");

	ImGui::End();
}

void FileDialog::SetFormat(const char * format)
{
	this->format = ".";
	this->format.append(format);
}
