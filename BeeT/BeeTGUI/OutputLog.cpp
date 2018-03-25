#include "OutputLog.h"

Outputlog::Outputlog()
{
}

Outputlog::~Outputlog()
{
	buf.clear();
}

void Outputlog::Draw()
{
	ImGui::Begin("Output##outputwindow", &windowOpen);

	if (ImGui::Button("Show..."))
	{
		ImGui::OpenPopup("##outputwin_showoptions");
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear"))
	{
		buf.clear();
		lineOffsets.clear();
	}

	if (ImGui::BeginPopup("##outputwin_showoptions"))
	{
		if (ImGui::MenuItem("Info", NULL, showInfo))
		{
			showInfo = !showInfo;
			ReevaluateFilter();
		}
		if (ImGui::MenuItem("Warnings", NULL, showWarnings))
		{
			showWarnings = !showWarnings;
			ReevaluateFilter();
		}
		if (ImGui::MenuItem("Errors", NULL, showErrors))
		{
			showErrors = !showErrors;
			ReevaluateFilter();
		}
		ImGui::EndPopup();
	}

	ImGui::Separator();
	
	const char* bufBegin = buf.begin();
	const char* line = bufBegin;
	for (int lineNum = 0; line != NULL; lineNum++)
	{
		const char* lineEnd = (lineNum < (int)lineOffsets.size()) ? bufBegin + lineOffsets[lineNum] : NULL;
		if (filter.PassFilter(line, lineEnd))
			ImGui::TextUnformatted(line, lineEnd);
		line = lineEnd && lineEnd[1] ? lineEnd + 1 : NULL;
	}

	if (scroolToBottom)
		ImGui::SetScrollHere(1.0f);
	scroolToBottom = false;
	ImGui::End();
}

void Outputlog::Write(const char * data)
{
	int oldSize = buf.size() - 1;
	buf.append(data);
	lineOffsets.push_back(oldSize);
	scroolToBottom = true;
}

void Outputlog::ReevaluateFilter()
{
	memset(&filter.InputBuf[0], '\0', 256);
	filter.Clear();

	char* pointer = &filter.InputBuf[0];
	bool modified = false;
	if (!showInfo)
	{
		memcpy(pointer, "-INFO,", 6);
		pointer += 6;
		modified = true;
	}

	if (!showWarnings)
	{
		memcpy(pointer, "-WARNING,", 9);
		pointer += 9;
		modified = true;
	}

	if (!showErrors)
	{
		memcpy(pointer, "-ERROR", 6);
		pointer += 6;
		modified = true;
	}
	if(modified)
		memcpy(filter.InputBuf[0] + pointer, "\0", 1);
	filter.Build();
}
