#pragma once

#include "plugin.h"

#include "item/Item.h"
#include "item/CheckBox.h"
#include "item/Options.h"
#include "item/NumberRange.h"
#include "item/ButtonKey.h"

class Window {
public:
	CVector2D m_Position;
	std::vector<Item*> m_Items;
	int m_SelectedIndex = 0;

	std::string m_Title = "Title";
	std::string m_Description = "Desc";

	CVector2D m_Size = CVector2D(400, 0);

	int m_ListStartIndex = 0; //remove
	int m_ListIndex = 0;
	int m_MaxItems = 15;

	void Draw();
	void GoUp();
	void GoDown();
	void Update();
	void Destroy();
	void CheckSelected(int add);

	int GetViewTopItemIndex();
	int GetViewBottomItemIndex();

	Item* GetSelectedItem();
	int GetNextSelectableItemIndex(int by);
	Item* AddItem(Item* item);
	Item* AddItem(std::string text);
	Item* AddButton(std::string text);

	CheckBox* AddCheckBox(std::string text, bool* value);
	Options* AddOptions(std::string text, int* value);

	template<class T>
	NumberRange<T>* AddNumberRange(std::string text, T* value, T min, T max);
	Item* AddDivider();
	Item* AddDivider(float height);

	ButtonKey* AddButtonKey(std::string text, int* keys);
};