#include "tspch.hpp"

#include "Inventory.hpp"

namespace TexScript {

	void Inventory::AddItem(const ItemStack& item)
	{
		for (ItemStack& i : m_Items)
		{
			if (i.ItemStackID != item.ItemStackID || i.Type != item.Type)
				continue;

			i.Count = i.Count + item.Count > 99 ? 99 : i.Count + item.Count;
			return;
		}

		m_Items.emplace_back(item);
	}

	void Inventory::RemoveItem(const ItemStack& item)
	{
		//TODO: Implementation
	}

}