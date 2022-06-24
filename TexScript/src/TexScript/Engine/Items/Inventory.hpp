#pragma once

#include "TexScript/Engine/Items/ItemStack.hpp"

namespace TexScript {

	class Inventory
	{
	public:
		Inventory() = default;
		~Inventory() = default;

		void AddItem(const ItemStack& item);
		void RemoveItem(const ItemStack& item);

		const std::vector<ItemStack>& Items() const { return m_Items; }

	private:
		std::vector<ItemStack> m_Items;
	};

}