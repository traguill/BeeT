#ifndef __OUTPUT_LOG_H__
#define __OUTPUT_LOG_H__

#include "ThirdParty/ImGui/imgui.h"

#include <vector>

class Outputlog
{
public:
	Outputlog();
	~Outputlog();

	void Draw();
	void Write(const char* data);

private:
	void ReevaluateFilter();

public:
	bool windowOpen = false;

private:
	ImGuiTextBuffer buf;
	ImGuiTextFilter filter;
	bool scroolToBottom;
	std::vector<int> lineOffsets;

	bool showInfo = true;
	bool showWarnings = true;
	bool showErrors = true;
};
extern Outputlog* g_output;
#endif