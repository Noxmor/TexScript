#pragma once

#include <string>

#include "TexScript/Engine/Core/CommandActionFlags.hpp"

namespace TexScript {

	struct Command
	{
		std::string DisplayNameID;
		int CommandActionFlags = CommandActionFlag::None;

		std::string PushInfID;
		size_t LocationID = 0;

		Command() = default;
		Command(const std::string& displayNameID, const int commandActionFlags)
			: DisplayNameID(displayNameID), CommandActionFlags(commandActionFlags)
		{
		}
	};

}