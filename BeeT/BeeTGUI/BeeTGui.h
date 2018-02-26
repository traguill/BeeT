#ifndef __BEETGUI_H__
#define __BEETGUI_H__

#include "Module.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"

enum BeeTMode {
	BEET_EDITOR,
	BEET_DEBUGGER
};

class BeeTEditor;
class BTNodeTypes;

class BeeTGui : public Module
{
public:
	BeeTGui(const char* name);
	~BeeTGui();

	bool Init();
	bool CleanUp();
	bool Update();

	ax::NodeEditor::EditorContext* GetNodeEditorContext()const;

private:
	void MenuBar();

public:
	BTNodeTypes* btNodeTypes = nullptr;

private:
	BeeTMode mode = BEET_EDITOR; // BeeTGui has two modes: Editor and Debugger. Only one of them can be enabled at the time

	ax::NodeEditor::EditorContext* editorContext = nullptr; // Context to handle the node graph editor

	// Modes
	BeeTEditor* beetEditor = nullptr;

};
#endif

