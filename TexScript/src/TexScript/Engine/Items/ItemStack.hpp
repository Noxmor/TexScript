#pragma once

#include <string>

namespace TexScript {

	enum class ItemStackType
	{
		None = 0,
		RecoverHP,
		RecoverMP
	};

	struct ItemStack
	{
		std::string ItemStackID;
		ItemStackType Type = ItemStackType::None;
		size_t Count = 1;
	};

}