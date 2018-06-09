#ifndef __BTNODETYPES_H__
#define __BTNODETYPES_H__

#include <string>
#include <vector>

/* -----------------------------------------
   - Quick Acess class of Node Types. Pointer located in BeeTGui.h
   -----------------------------------------*/
struct NodeType
{
	int typeId;
	int maxOutputs; // -1 equals to undefined number of outputs

	std::string icon;
	std::string name;
	std::string category;

	NodeType();
	NodeType(int typeId,const std::string& category, const std::string& icon, const std::string& name, int maxOutputs);
};

struct ListObject; // Forward declaration of ItemList object

class BTNodeTypes
{
public:
	BTNodeTypes();
	~BTNodeTypes();

	void Init();

	NodeType* GetTypeById(int id);
	int GetNodeTypeId(const std::string& category, const std::string& name)const;
	ListObject* GetListObjectPtr()const;

private:
	void InsertType(const std::string& category, const std::string& icon, const std::string& name, int maxOutputs);

private:
	std::vector<NodeType> typesList;
	ListObject* listObject = nullptr;
};
#endif