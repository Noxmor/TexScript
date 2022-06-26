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
		for (size_t i = m_Items.size(); i > 0; i--)
		{
			const ItemStack& is = m_Items.at(i - 1);
			if (is.ItemStackID != item.ItemStackID || is.Type != item.Type)
				continue;

			if (item.Count >= is.Count)
			{
				m_Items.erase(m_Items.end() - i);
				return;
			}
		}
	}

}