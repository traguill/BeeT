#include "ItemList.h"
#include "Log.h"

#include <algorithm>

using namespace std;

// --------------------------------------------------------------------------------------

struct MatchFilterItems
{
	bool open;
	string category;
	vector<string> items;
};
// --------------------------------------------------------------------------------------

ItemList::ItemList()
{
}

ItemList::~ItemList()
{
}

void ItemList::Draw()
{
	if (listPtr == nullptr)
		return;

	if (ImGui::IsKeyReleased(ImGui::GetKeyIndex(ImGuiKey_Enter)))
	{
		if (!itemSelected.empty() && !categorySelected.empty())
		{
			ExecuteSelFunc(categorySelected, itemSelected);
			return;
		}
	}

	if (ImGui::IsKeyReleased(ImGui::GetKeyIndex(ImGuiKey_Escape)))
	{
		SetVisible(false);
		return;
	}

	bool open = true;
	ImGui::SetNextWindowSize(ImVec2(250.0f, 350.0f));
	if (visibleFlag)
		ImGui::SetNextWindowPos(ImVec2(posX, posY));

	ImGui::Begin("ItemList", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	if (visibleFlag)
	{
		ImGui::SetKeyboardFocusHere();
		visibleFlag = false;
	}

	if (!ImGui::IsWindowFocused())
	{
		SetVisible(false);
		ImGui::End();
		return;
	}

	if (filter.Draw(" search(?)"))
	{
		FilterMatchItems();
	}

	bool firstSelected = true;
	itemSelected.clear();
	categorySelected.clear();
	for (auto category : matchItems)
	{
		if (category.open)
		{
			// Display category name open and all items that match the search
			ImGui::SetNextTreeNodeOpen(true);
			if(ImGui::TreeNodeEx(category.category.data(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
			{ 
				for (auto item : category.items)
				{
					if (ImGui::Selectable(item.data(), firstSelected))
					{
						ExecuteSelFunc(category.category, item);
					}
					if (firstSelected)
					{
						firstSelected = false;
						itemSelected = item;
						categorySelected = category.category;
					}
				}
				ImGui::TreePop();
			}
		}
		else
		{
			// Display only category name
			// If the user opens, display ALL the items
			if (ImGui::TreeNode(category.category.data()))
			{
				map<string, vector<string>>::iterator catIterator = listPtr->list.find(category.category);
				if (catIterator != listPtr->list.end())
				{
					for (auto item : catIterator->second)
					{
						if (ImGui::Selectable(item.data()))
						{
							ExecuteSelFunc(category.category, item);
						}
					}
				}
				ImGui::TreePop();
			}	
		}
	}

	ImGui::End();
}

void ItemList::SetVisible(bool visible, ListObject * lObject)
{
	if (visible)
	{
		if (listPtr == nullptr)
		{
			listPtr = lObject;
			InitializeMatchItems();
			filter.Clear();
			visibleFlag = true;
		}
		else
			LOGW("Unable to set a new ListObject in ItemList widget. There is already a list in the widget. Use PopListObject first to remove the current ListObject before inserting another one.")
	}
	else
	{
		listPtr = nullptr;
		matchItems.clear();
		object = nullptr;
		selFunc = nullptr;
		filter.Clear();
		visibleFlag = false;
	}
	this->visible = visible;
}

bool ItemList::IsVisible() const
{
	return visible;
}

void ItemList::SetSelFunctionCallback(void(*fc)(void *, const std::string &, const std::string &), void * obj)
{
	selFunc = fc;
	object = obj;
}

void ItemList::SetWidgetPosition(float x, float y)
{
	posX = x;
	posY = y;
}

void ItemList::FilterMatchItems()
{
	matchItems.clear();

	for (auto category : listPtr->list)
	{
		bool isEmpty = true;
		MatchFilterItems categoryMatch;
		categoryMatch.category = category.first;
		categoryMatch.open = false;
		if (filter.InputBuf[0] != '\0')
		{
			for (auto item : category.second)
				if (filter.PassFilter(item.data()))
					categoryMatch.items.push_back(item.data());
		}
		if (categoryMatch.items.size() != 0)
		{
			categoryMatch.open = true;
			isEmpty = false;
		}
		else if (filter.PassFilter(categoryMatch.category.data()))
			isEmpty = false;

		if (isEmpty == false)
			matchItems.push_back(categoryMatch);
	}
}

void ItemList::InitializeMatchItems()
{
	matchItems.clear();
	if (listPtr)
	{
		for (auto categoryIt : listPtr->list)
		{
			MatchFilterItems tmp;
			tmp.category = categoryIt.first;
			tmp.open = false;
			matchItems.push_back(tmp);
		}
	}
}

void ItemList::ExecuteSelFunc(const string& category, const string& item)
{
	if (object && selFunc)
	{
		selFunc(object, category, item);
	}
	// Auto-closes
	SetVisible(false);
}

map<string, vector<string>>::iterator ListObject::CreateCategory(std::string category)
{
	map<string, vector<string>>::iterator found = list.find(category);
	if (found == list.end())
	{
		vector<string> emptyList;
		auto ret = list.insert(pair<string, vector<string>>(category, emptyList));
		found = ret.first;
	}

	return found;
}

void ListObject::AddItemInCategory(std::string category, std::string item)
{
	map<string, vector<string>>::iterator categoryFound = list.find(category);
	if (categoryFound == list.end())
		categoryFound = CreateCategory(category);

	bool sameItemFound = false;
	for (auto matchItem : (*categoryFound).second)
	{
		if (matchItem.compare(item) == 0)
		{
			sameItemFound = true;
			break;
		}
	}
	if(sameItemFound == false)
		(*categoryFound).second.push_back(item);	
}

void ListObject::SortAll()
{
	for (auto category : list)
		sort(category.second.begin(), category.second.end());
}

void ListObject::SortCategory(std::string category)
{
	map<string, vector<string>>::iterator categoryFound = list.find(category);
	if (categoryFound != list.end())
		sort(categoryFound->second.begin(), categoryFound->second.end());
}