#ifndef __ITEM_LIST_H__
#define __ITEM_LIST_H__

#include <map>
#include <vector>
#include <string>
#include "ThirdParty/ImGui/imgui.h"

struct ListObject
{
	std::map<std::string, std::vector<std::string>> list; // map<Category, vector<ItemsInCategory>>

	std::map<std::string, std::vector<std::string>>::iterator CreateCategory(std::string category);
	void AddItemInCategory(std::string category, std::string item); // Inserts an item in the specified category. If the category doesn't exist, creates one.
	void SortAll(); // Sorts all the ListObject alphabetically A-Z
	void SortCategory(std::string category); // Sorts only a specific category alphabetically A-Z
};

struct MatchFilterItems;

class ItemList
{
public:
	ItemList();
	~ItemList();

	void Draw();

	void SetVisible(bool visible, ListObject* lObject = nullptr);
	bool IsVisible()const;
	void SetSelFunctionCallback(void(*fc)(void*, const std::string&, const std::string&), void* obj);
	void SetWidgetPosition(float x, float y);

private:
	void FilterMatchItems();
	void InitializeMatchItems();

	void ExecuteSelFunc(const std::string& category, const std::string& item);

private:
	ListObject* listPtr = nullptr;
	std::vector<MatchFilterItems> matchItems;
	ImGuiTextFilter filter;
	bool visible = false;

	bool visibleFlag = false; // Set to true the first tick the window is visible

	void* object; // Class to call the selFunc
	void(*selFunc)(void* obj, const std::string& category, const std::string& item); // Function to call when an item is selected

	float posX, posY;
};
#endif // !__ITEM_LIST_H__

