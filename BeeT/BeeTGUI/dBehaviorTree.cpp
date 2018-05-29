#include "dBehaviorTree.h"
#include "dSample.h"
#include "ThirdParty/ImGui/imgui.h"

using namespace std;

dBehaviorTree::dBehaviorTree()
{
}

dBehaviorTree::dBehaviorTree(Data & data) : BehaviorTree(data)
{
}

dBehaviorTree::~dBehaviorTree()
{
}

void dBehaviorTree::AddSample(dSample * sample)
{
	// Quick insert
	double timestamp = sample->GetTimestamp();
	if (changes.empty() || changes.back()->GetTimestamp() <= timestamp)
	{
		changes.push_back(sample);
		return;
	}
	// For some reason sample is not older than the last one. Insert in the correct position
	for (int i = changes.size() - 1; i >= 0; i--)
	{
		if (changes[i]->GetTimestamp() <= timestamp)
		{
			changes.insert(changes.begin()+i, sample);
			break;
		}
	}
}

void dBehaviorTree::PrintSamples()
{
	for (auto sample : changes)
	{
		ImGui::Separator();
		ImGui::Text("Time: %d", sample->GetTimestamp());
		sample->Print();
	}
}
