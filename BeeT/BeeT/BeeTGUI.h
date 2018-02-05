#ifndef __BEETGUI_H__
#define __BEETGUI_H__

#include "Module.h"
#include "External\NodeEditor\Include\NodeEditor.h"

class BeeTGUI : public Module
{
public:
	BeeTGUI(const char* name);
	~BeeTGUI();

	bool Init();
	bool CleanUp();
	bool Update();

private:
	ax::NodeEditor::EditorContext* editorContext = nullptr;

};
#endif

