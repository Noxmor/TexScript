#include "tspch.hpp"

#include "Console.hpp"

#include "TexScript/Base/Application.hpp"

#include "TexScript/Lua/LuaScript.hpp"

#include <fstream>

namespace TexScript {

	Console* Console::s_Instance = nullptr;

	Console::Console(const std::string& title)
	{
		TS_ASSERT(!s_Instance, "Console already exists!");
		s_Instance = this;

		SetTitle(title);

		LoadConfig();
		
		LuaScript script("base/data.lua");
		TS_ASSERT(script, "Script is not valid!");

		bool hasQuitCmd = false;
		for (const Command& cmd : m_Interfaces.at(m_InterfaceStack.top()).Commands)
		{
			if (cmd.CommandActionFlags & CommandActionFlag::Quit)
			{
				hasQuitCmd = true;
			}
		}

		TS_ASSERT(hasQuitCmd, "The main interface needs to have a quit command!");
	}

	void Console::Shutdown()
	{
		SaveConfig();
	}

	void Console::OnUpdate()
	{
		const std::string& infID = m_InterfaceStack.top();
		const Interface& inf = m_Interfaces.at(infID);
		PrintInterface(inf);

		GetInput();
		GetCommandFromInput();

		if (m_Input.size() && m_InputCommand)
		{
			const Command& cmd = inf.Commands.at(m_InputCommand - 1);

			if (cmd.CommandActionFlags & CommandActionFlag::Quit)
			{
				Application::Get().Stop();
			}

			if (cmd.CommandActionFlags & CommandActionFlag::PushInf)
			{
				if (!cmd.PushInfID.empty())
				{
					m_InterfaceStack.push(cmd.PushInfID);
				}
				else
				{
					TS_ERROR("Interface ID to push was not specified!");
				}
			}

			if (cmd.CommandActionFlags & CommandActionFlag::PopInf)
			{
				if (m_InterfaceStack.size() > 1)
				{
					m_InterfaceStack.pop();
				}
				else
				{
					TS_ERROR("Interface Stack size is not big enough to pop!");
				}
			}
		}

		Clear();
	}

	void Console::RegisterLuaTable(const LuaTable& table)
	{
		table.Print();
		
		if (!table.HasStringData("type")) return;

		const std::string& type = table.StringData().at("type");

		if (type == "interface")
		{
			const std::string& infID = table.StringData().at("id");
			Interface inf;

			if (table.HasBoolData("main"))
			{
				const bool isMainInf = table.BoolData().at("main");
				if (isMainInf)
					m_InterfaceStack.push(infID);
			}

			const LuaTable& commands = table.Children().at("commands");
			for (auto& it = commands.Children().begin(); it != commands.Children().end(); ++it)
			{
				const std::string& cmdID = it->second.StringData().at("id");
				std::string pushID;
				if (it->second.HasStringData("push_id"))
				{
					pushID = it->second.StringData().at("push_id");
				}

				const LuaTable& flagsTable = it->second.Children().at("flags");
				int flags = 0;
				for (const auto& [key, flag] : flagsTable.StringData())
				{
					flags |= StringToCommandActionFlag(flag);
				}

				Command cmd(cmdID, flags);
				cmd.PushInfID = pushID;
				inf.Commands.emplace_back(cmd);
			}

			RegisterInterface(infID, inf);
		}
	}

	void Console::RegisterInterface(const std::string& id, const Interface& inf)
	{
		m_Interfaces[id] = inf;
	}

	void Console::Stop()
	{
		Application::Get().Stop();
	}

	void Console::Clear()
	{
		system("cls");
	}

	void Console::SetTitle(const std::string& title)
	{
		const std::string cmd = "title " + title;
		system(cmd.c_str());
	}

	void Console::GetInput()
	{
		std::getline(std::cin, m_Input);
	}

	void Console::GetCommandFromInput()
	{
		if (m_Input.empty()) return;

		for (const char c : m_Input)
		{
			if (!std::isdigit(c))
			{
				m_InputCommand = 0;
				return;
			}
		}

		m_InputCommand = std::stoi(m_Input);

		if (m_InputCommand > m_Interfaces.at(m_InterfaceStack.top()).Commands.size())
		{
			m_InputCommand = 0;
		}
	}

	void Console::LoadConfig()
	{
		//TODO: Load config
	}

	void Console::SaveConfig()
	{
		const std::string content = "CHARACTERS_PER_LINE = " + std::to_string(m_GameConfig.CharactersPerLine) + '\n'
			+ "LANGUAGE = \"" + m_GameConfig.Language + "\"";

		std::ofstream of("config.cfg");
		of << content;
		of.close();
	}

	void Console::Print(const std::string& text)
	{
		std::vector<std::string> words;
		std::string currentWord = "";

		for (const char c : text + ' ')
		{
			if (c != ' ')
			{
				currentWord += c;
				continue;
			}

			if (currentWord.empty()) continue;

			words.emplace_back(currentWord);
			currentWord = "";
		}

		if (words.empty()) return;

		uint8_t charactersOnLine = 0;

		std::cout << words.at(0);
		charactersOnLine += (uint8_t)words.at(0).size() + 1;
		
		for (size_t i = 1; i < words.size(); i++)
		{
			const std::string& word = words.at(i);

			if (charactersOnLine + word.size() +1 <= m_GameConfig.CharactersPerLine)
			{
				std::cout << ' ' + word;
				charactersOnLine += (uint8_t)word.size() + 1;
				continue;
			}

			std::cout << std::endl;
			std::cout << word;
			charactersOnLine = (uint8_t)word.size();
		}
	}

	void Console::Println(const std::string& text)
	{
		Print(text);
		std::cout << std::endl;
	}

	void Console::PrintInterface(const Interface& inf)
	{
		const auto& commands = inf.Commands;

		Println("-----");

		for (const std::string& msg : inf.Messages)
		{
			Println(msg);
		}

		Println("-----");

		for (size_t i = 0; i < commands.size(); i++)
		{
			const Command& cmd = commands.at(i);
			Println("[" + std::to_string(i + 1) + "] " + cmd.DisplayNameID);
		}

		Println("-----");
		Print("-> ");
	}

}