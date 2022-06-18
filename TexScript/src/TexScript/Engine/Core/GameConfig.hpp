#pragma once

#include <stdint.h>
#include <string>

namespace TexScript {

	struct GameConfig
	{
		uint8_t CharactersPerLine = 64;
		std::string Language = "english";
		std::string GameDir = "base";
	};

}