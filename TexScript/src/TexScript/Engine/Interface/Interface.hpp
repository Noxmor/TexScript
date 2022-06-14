#pragma once

#include <vector>

#include "TexScript/Engine/Core/Command.hpp"

namespace TexScript {

	struct Interface
	{
		std::vector<std::string> Messages;
		std::vector<Command> Commands;
		Command CustomCommand;
	};

}