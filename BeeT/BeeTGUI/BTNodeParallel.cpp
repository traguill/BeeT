#include "BTNodeParallel.h"
#include "BTDecorator.h"

#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "ThirdParty/NodeEditor/Source/Shared/Interop.h"

namespace ne = ax::NodeEditor;

BTNodeParallel::BTNodeParallel(int id, int sourcePinId, int targetPinId, int targetPin2Id, BehaviorTree * bt, BTNode * parent)
	: BTNode(id, sourcePinId, targetPin2Id, 3, bt, parent)
{
	simplePin = new BTPin(targetPinId, ne::PinKind::Source, this, true);
}

BTNodeParallel::BTNodeParallel(BehaviorTree * bt, Data & data) : BTNode(bt, data)
{
}

BTNodeParallel::~BTNodeParallel()
{
}

void BTNodeParallel::InspectorInfo()
{
	ImGui::Text("hello im a parallel node");
}

void BTNodeParallel::PrepareToDraw()
{
	// Remove decorators now (if any)
	if (decoratorsToRemove.empty() == false)
	{
		for (auto dec : decoratorsToRemove)
		{
			dec->CleanUp();
			decorators.erase(std::find(decorators.begin(), decorators.end(), dec));
			delete dec;
		}
		decoratorsToRemove.clear();
	}
	// --------------------------------

	ne::BeginNode(id);
	ImGui::BeginVertical(id);

	// Input ---------------------------------
	ImGui::BeginHorizontal("inputs");
	ImGui::Spring(0, padding * 2);

	if (type->typeId != 0) // Check if is not root (type 0)
	{
		ImGui::Dummy(ImVec2(0, padding));
		ImGui::Spring(1, 0);
		inputsRect = ImGui_GetItemRect();

		ne::PushStyleVar(ne::StyleVar_PinArrowSize, 10.0f);
		ne::PushStyleVar(ne::StyleVar_PinArrowWidth, 10.0f);
		ne::PushStyleVar(ne::StyleVar_PinCorners, 12);
		ne::BeginPin(inputPin->id, ne::PinKind::Target);
		ne::PinPivotRect(to_imvec(inputsRect.top_left()), to_imvec(inputsRect.bottom_right()));
		ne::PinRect(to_imvec(inputsRect.top_left()), to_imvec(inputsRect.bottom_right()));
		ne::EndPin();
		ne::PopStyleVar(3);
	}
	else
		ImGui::Dummy(ImVec2(0, padding));

	ImGui::Spring(0, padding * 2);
	ImGui::EndHorizontal(); // 'inputs'
	// -----------------------------------------

	// Content ---------------------------------
	ImGui::BeginHorizontal("content_frame");
	ImGui::Spring(1, padding);

	ImGui::BeginVertical("contentBig", ImVec2(0.0f, 0.0f));

	for (auto dec : decorators)
	{
		dec->PrepareToDraw();
	}
	ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
	ImGui::Dummy(ImVec2(160, 0));

	ImGui::Spring(1);
	ImGui::Text("%s%s", type->icon.data(), name.data());
	ImGui::Spring(1);

	ImGui::EndVertical(); // 'content'

	contentRect = ImGui_GetItemRect();
	ImGui::EndVertical(); //content big
	ImGui::Spring(1, padding);
	ImGui::EndHorizontal(); // 'content_frame'
	// -------------------------------------------

	// Output ------------------------------------
	ImGui::BeginHorizontal("outputs");
	ImGui::Spring(0, padding * 2);


	ImGui::Dummy(ImVec2(0, padding));
	ImGui::Spring(1, 0);
	outputsRect = simpleOutputsRect = ImGui_GetItemRect();

	outputsRect.x += outputsRect.w * 0.25f;
	outputsRect.w *= 0.75f;

	simpleOutputsRect.w *= 0.2f;

	ne::PushStyleVar(ne::StyleVar_PinCorners, 3);
	
	ne::BeginPin(outputPin->id, ne::PinKind::Source);
	ne::PinPivotRect(to_imvec(outputsRect.top_left()), to_imvec(outputsRect.bottom_right()));
	ne::PinRect(to_imvec(outputsRect.top_left()), to_imvec(outputsRect.bottom_right()));
	ne::EndPin();

	ne::BeginPin(simplePin->id, ne::PinKind::Source); // TODO: Own output
	ne::PinPivotRect(to_imvec(simpleOutputsRect.top_left()), to_imvec(simpleOutputsRect.bottom_right()));
	ne::PinRect(to_imvec(simpleOutputsRect.top_left()), to_imvec(simpleOutputsRect.bottom_right()));
	ne::EndPin();

	ne::PopStyleVar();
	

	ImGui::Spring(0, padding * 2);
	ImGui::EndHorizontal(); // 'outputs'
	// -------------------------------------------

	ImGui::EndVertical(); // nodeId
	ne::EndNode();
}
