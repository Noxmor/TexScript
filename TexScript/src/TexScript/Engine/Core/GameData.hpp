#pragma once

#include <unordered_map>

namespace TexScript {

	struct GameData
	{
		std::unordered_map<std::string, std::string> CommandData;
	};

}