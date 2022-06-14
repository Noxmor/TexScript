#pragma once

#include <vector>

namespace TexScript {

	struct Location
	{
		size_t LocationID = 0;
		bool Locked = false;
		bool Visible = true;
		std::vector<size_t> Paths;
	};

}