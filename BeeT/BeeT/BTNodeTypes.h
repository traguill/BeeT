#ifndef __BTNODETYPES_H__
#define __BTNODETYPES_H__

#include <string>
#include <vector>

struct NodeType
{
	int typeId;
	std::string name;
	int maxOutputs; // -1 equals to undefined number of outputs

	NodeType();
	NodeType(int typeId, std::string name, int maxOutputs);
};

class BTNodeTypes
{
public:
	BTNodeTypes();
	~BTNodeTypes();

	void Init();

	NodeType* GetTypeById(int id);
	std::vector<NodeType> GetTypeList()const;

private:
	std::vector<NodeType> typesList;
};
#endif