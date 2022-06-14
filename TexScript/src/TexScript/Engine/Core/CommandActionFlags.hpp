#pragma once

namespace TexScript {

	enum CommandActionFlag
	{
		None = 0,

		Quit = BIT(0),
		PushInf = BIT(1),
		PopInf = BIT(2),
		NewGame = BIT(4),
		LoadGame = BIT(5)
	};

	static CommandActionFlag StringToCommandActionFlag(const std::string& flag)
	{
		static std::unordered_map<std::string, CommandActionFlag> flags;
		flags["Quit"] = CommandActionFlag::Quit;
		flags["PushInf"] = CommandActionFlag::PushInf;
		flags["PopInf"] = CommandActionFlag::PopInf;
		flags["NewGame"] = CommandActionFlag::NewGame;
		flags["LoadGame"] = CommandActionFlag::LoadGame;

		if (flags.find(flag) != flags.end())
		{
			return flags.at(flag);
		}

		return CommandActionFlag::None;
	}

}