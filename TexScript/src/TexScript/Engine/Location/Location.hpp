#pragma once

#include <vector>
#include <string>

#include "TexScript/Engine/Items/ItemStack.hpp"

namespace TexScript {

	struct Location
	{
		size_t LocationID = 0;
		std::string RegionID;
		bool Locked = false;
		bool Visible = true;
		std::vector<size_t> Paths;
		std::vector<ItemStack> Items;
	};

}