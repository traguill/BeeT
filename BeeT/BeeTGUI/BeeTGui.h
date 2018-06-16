#ifndef __BEETGUI_H__
#define __BEETGUI_H__

#include "Module.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include <vector>

enum BeeTMode {
	BEET_EDITOR,
	BEET_DEBUGGER
};

class BeeTEditor;
class BTNodeTypes;
class FileDialog;
class BeeTDebugger;

class BeeTGui : public Module
{
public:
	BeeTGui(const char* name);
	~BeeTGui();

	bool Init();
	bool CleanUp();
	bool Update();

	void SetCurrentEditorContext(int id);
	void SetCurrentEditorContext(ax::NodeEditor::EditorContext* context);
	ax::NodeEditor::EditorContext* GetNodeEditorContext()const;
	int CreateNodeEditorContext(); 

	static void LoadFile(void* obj, const char* filename);
	static void SaveFile(void* obj, const char* filename);

private:
	void MenuBar();
	void FileMenuBar();
	void EditMenuBar();
	void OpenWindows();
	void ShowMenuBar();

public:
	BTNodeTypes* btNodeTypes = nullptr;

	// Show Options
	bool showNodeId = false;

private:
	BeeTMode mode = BEET_EDITOR; // BeeTGui has two modes: Editor and Debugger. Only one of them can be enabled at the time

	std::vector<ax::NodeEditor::EditorContext*> editorContextList; 
	ax::NodeEditor::EditorContext* currentEditorContext = nullptr; // Context to handle the node graph editor
	FileDialog* fileDialog = nullptr;

	// Modes
	BeeTEditor* beetEditor = nullptr;
	BeeTDebugger* beetDebugger = nullptr;

};
#endif

