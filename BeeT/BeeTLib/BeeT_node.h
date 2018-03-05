#ifndef __BEET_NODE_H__
#define __BEET_NODE_H__

#include "BeeT_serializer.h"
#include <vector>

class BeeT_Node
{
public:
	BeeT_Node(BeeT_Serializer& data);
	~BeeT_Node();

private:
	int id = -1;
	std::vector<BeeT_Node*> childs;
	int type = -1; // Change for a real type
};
#endif // !__BEET_NODE_H__

