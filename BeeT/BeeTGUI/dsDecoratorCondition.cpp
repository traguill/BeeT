#include "dsDecoratorCondition.h"
#include "dBehaviorTree.h"
#include "BTNode.h"
#include "BTDecorator.h"

dsDecoratorCondition::dsDecoratorCondition(dBehaviorTree * bt, const Data & data) : dSample(bt, DECORATOR_CONDITION)
{
	timestamp = data.GetFloat("timestamp");
	int decID = data.GetInt("decoratorId");
	int nodeID = data.GetInt("nodeId");
	BTNode* n = bt->FindNode(nodeID);
	if (n)
	{
		for (auto dec : n->decorators)
		{
			if (dec->GetUID() == decID)
			{
				decorator = dec;
				break;
			}
		}
	}
	pass = data.GetBool("pass");
}

dsDecoratorCondition::~dsDecoratorCondition()
{
}

void dsDecoratorCondition::Print() const
{}

void dsDecoratorCondition::Effect()
{
	if (decorator)
	{
		decorator->bgColor = pass ? DEC_PASS_COLOR : DEC_NO_PASS_COLOR;
	}
}

void dsDecoratorCondition::CounterEffect()
{
	if (decorator)
	{
		decorator->bgColor = decorator->IsCheckedEveryFrame() ? DEC_TICK_ALWAYS_COLOR : DEC_TICK_ONCE_COLOR;
	}
}
