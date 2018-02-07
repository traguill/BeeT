#ifndef __BEETGUI_H__
#define __BEETGUI_H__

#include "Module.h"
#include "External\NodeEditor\Include\NodeEditor.h"
//Testing
#include "External/NodeEditor/Source/Shared/Interop.h"

class BeeTGui : public Module
{
public:
	BeeTGui(const char* name);
	~BeeTGui();

	bool Init();
	bool CleanUp();
	bool Update();

private:

	// Testing
	void CompleteNode();
	void ItemIsVisible();
	
	void TreeNode();
	void TreeNodeContent(int nodeId, const float padding);
	void TreeNodeContentInput(const float padding);
	void DrawTreeNode();

private:
	ax::NodeEditor::EditorContext* editorContext = nullptr;

	// Complex Node
	ax::rect HeaderRect;
	ax::rect ContentRect2;
	ax::rect NodeRect;

	// Tree Node
	ax::rect inputsRect;
	ax::rect contentRect;
	ax::rect outputsRect;

};
#endif

