#include "dBehaviorTree.h"
#include "dSample.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Log.h"
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
		if(sampleSelected == -1)
			sample->Effect();
		return;
	}
	// For some reason sample is not older than the last one. Insert in the correct position
	for (int i = changes.size() - 1; i >= 0; i--)
	{
		if (changes[i]->GetTimestamp() <= timestamp)
		{
			changes.insert(changes.begin()+i, sample);
			if (sampleSelected == -1)
				sample->Effect();
			break;
		}
	}
}

void dBehaviorTree::PrintSamples()
{
	int counter = changes.size() - 1;
	for (vector<dSample*>::reverse_iterator it = changes.rbegin(); it != changes.rend(); it++)
	{
		ImGui::BeginGroup();
		ImGui::Separator();
		ImGui::Text("Time: %.2f", (*it)->GetTimestamp());
		(*it)->Print();
		ImGui::EndGroup();
		if (ImGui::IsItemClicked())
		{
			ApplySampleEffect(counter);
		}
		counter--;
		//if (counter <= changes.size() - 10)
			//break;
	}
}

void dBehaviorTree::ApplySampleEffect(int newSample)
{
	if (newSample == sampleSelected)
		return;

	int sSample = sampleSelected == -1 ? changes.size() - 1 : sampleSelected;
	int nSample = newSample == -1 ? changes.size() - 1 : newSample;
	// Revert changes
	if (nSample < sSample)
	{
		for (int i = sSample; i > nSample; i--)
			changes[i]->CounterEffect();
		
	}
	// Apply changes
	if (nSample > sSample)
	{
		for (int i = sSample; i <= nSample; i++)
			changes[i]->Effect();
	}

	sampleSelected = newSample;
}
