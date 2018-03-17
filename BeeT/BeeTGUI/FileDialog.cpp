#include "FileDialog.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Application.h"
#include "FileSystem.h"

#include <vector>

using namespace std;

FileDialog::FileDialog()
{
	path = g_app->fileSystem->GetDirectory();
	format = ".json";
	newFilenameName = "*.json";
}

FileDialog::~FileDialog()
{
}

void FileDialog::Draw()
{
	if (show == false)
		return;

	ImGui::SetNextWindowFocus();
	//ImGui::ShowTestWindow();
	bool open = true;
	ImGui::Begin("File Dialog Title", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	ImGui::Text("%s", path.data());
	ImGui::Separator();
	ImGui::BeginChild("##filedialogwindow_listfiles", ImVec2(ImGui::GetWindowContentRegionWidth(), 300), false, ImGuiWindowFlags_HorizontalScrollbar);
	
	vector<string> files, filtered;
	g_app->fileSystem->EnumerateFiles("/", files);
	g_app->fileSystem->RemoveDirectoriesFromList(files);
	g_app->fileSystem->FilterFiles(files, filtered, format);
	bool isSelected = false;
	for (auto file : filtered)
	{
		static char tmp[sizeof(file)];
		sprintf_s(tmp, sizeof(file), " - %s", file.data());
		isSelected = file.compare(fileSelected) == 0 ? true : false;
		if (ImGui::Selectable(tmp, &isSelected))
		{
			switch (mode)
			{
			case OPEN_FILE:
				fileSelected = file;
				break;
			case SAVE_FILE:
				newFilenameName = file;
				break;
			}
		}
	}
		
	ImGui::EndChild();
	
	ImGui::Separator();
	
	ShowFileName(); // Specific to each mode
	
	ImGui::Text("Format: *%s", format.data()); 

	if (ImGui::Button("Accept##filedialogwindow_accept"))
	{
		if (object && acceptFunc)
		{
			switch (mode)
			{
			case OPEN_FILE:
				acceptFunc(object, fileSelected.data());
				lastOpenedFile = fileSelected;
				break;
			case SAVE_FILE:
				acceptFunc(object, newFilenameName.data());
				lastOpenedFile = newFilenameName;
				break;
			}
		}
		CleanUpBeforeClosingWindow();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel##filedialogwindow_cancel"))
	{
		CleanUpBeforeClosingWindow();
	}

	ImGui::End();
}

void FileDialog::SetFormat(const char * format)
{
	this->format = ".";
	this->format.append(format);
	show = true;
}

void FileDialog::EnableWindow(FileDialogMode mode)
{
	show = true;
	this->mode = mode;
	if (lastOpenedFile.length() != 0)
	{
		switch (mode)
		{
		case OPEN_FILE:
			fileSelected = lastOpenedFile;
			break;
		case SAVE_FILE:
			newFilenameName = lastOpenedFile;
			break;
		}
	}
}

void FileDialog::SetAcceptFunctionCallback(void(*fc)(void*, const char *), void* obj)
{
	acceptFunc = fc;
	object = obj;
}

void FileDialog::ClearLastOpenedFileName()
{
	lastOpenedFile = "";
}

void FileDialog::CleanUpBeforeClosingWindow()
{
	show = false;
	fileSelected = "";
	newFilenameName = "*.json";
}

void FileDialog::ShowFileName()
{
	if (mode == OPEN_FILE)
	{
		ImGui::Text("Name: %s", fileSelected.data());
	}
	else if (mode == SAVE_FILE)
	{
		char filename[_MAX_PATH];
		strcpy_s(filename, _MAX_PATH, newFilenameName.data());
		ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_AutoSelectAll;
		if (ImGui::InputText("###", filename, _MAX_PATH, inputFlags))
		{
			newFilenameName = filename;
		}
	}
}
