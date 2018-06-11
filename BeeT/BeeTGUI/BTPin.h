#ifndef __BTPIN_H__
#define __BTPIN_H__

#include <vector>
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "Data.h"

class BTNode;
class BTLink;

class BTPin
{
public:
	BTPin();
	BTPin(int id, ax::NodeEditor::PinKind kind, BTNode* node, bool isSimpleParallel);
	BTPin(BTNode* node, Data& data);
	~BTPin();

	bool IsLinkAvailable()const;

	void Save(Data& file)const;

public:
	int id = -1;
	BTNode* node = nullptr;
	ax::NodeEditor::PinKind kind;
	std::vector<BTLink*> links;

	bool isSimpleParallel = false;
private:
};
#endif // !__BTPIN_H__
