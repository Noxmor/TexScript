#pragma once

namespace TexScript {

	enum CommandActionFlag
	{
		None = 0,

		Quit = BIT(0),
		PushInf = BIT(1),
		PopInf = BIT(2)
	};

	static CommandActionFlag StringToCommandActionFlag(const std::string& flag)
	{
		static std::unordered_map<std::string, CommandActionFlag> flags;
		flags["None"] = CommandActionFlag::None;
		flags["Quit"] = CommandActionFlag::Quit;
		flags["PushInf"] = CommandActionFlag::PushInf;
		flags["PopInf"] = CommandActionFlag::PopInf;

		if (flags.find(flag) != flags.end())
		{
			return flags.at(flag);
		}

		return CommandActionFlag::None;
	}

}