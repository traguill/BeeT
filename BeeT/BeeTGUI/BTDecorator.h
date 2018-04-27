#ifndef __BTDECORATOR_H__
#define __BTDECORATOR_H__

#include "ThirdParty/NodeEditor/Source/Shared/Math2D.h"
#include "Data.h"
#include <boost/any.hpp>

// TODO: Different types of decorators. For now only Blackboard comparisons

class Blackboard;
struct BBVar;

enum DecBoolOpt
{
	IS_FALSE = 0,
	IS_TRUE = 1
};

enum DecNumberOpt
{
	IS_EQUAL = 0,
	IS_NOT_EQUAL = 1,
	IS_GREATER = 2,
	IS_LESS = 3,
	IS_EQUAL_OR_GREATER = 4,
	IS_EQUAL_OR_LESS = 5
};

enum DecStringOpt
{
	S_IS_EQUAL = 0,
	S_IS_NOT_EQUAL = 1,
	CONTAINS = 2,
	NOT_CONTAINS = 3
};

class BTDecorator
{
public:
	BTDecorator(Blackboard* bb, BBVar* var);
	BTDecorator(Blackboard* bb, Data& data);
	~BTDecorator();

	void PrepareToDraw();
	void InspectorInfo();

	void Save(Data& file);

private:
	void PrintType()const;

	// Prepare to draw by type
	void PrepDrawBool();
	void PrepDrawInt();
	void PrepDrawFloat();
	void PrepDrawString();

	// Type Options
	void TypeBoolOptions();
	void TypeNumberOptions(int numType); // 0-int 1-float
	void TypeStringOptions();

public:
	ax::rect contentRect;

private:
	int uid;
	Blackboard* bb = nullptr;
	BBVar* var = nullptr;
	
	boost::any var2;

	int option = 0;

};
#endif // !__BTDECORATOR_H__
