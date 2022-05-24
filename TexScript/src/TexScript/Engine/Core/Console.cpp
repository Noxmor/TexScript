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
		LoadGameData();

		//TODO: For test purpose, refactor into lua scripts!
		m_InterfaceStack.push("INF_MAIN");

		RegisterInterface("INF_MAIN");
		RegisterInterface("INF_MODS");

		InterfaceFunctionCall(m_InterfaceStack.top(), "onPushInterface", true);
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
					InterfaceFunctionCall(m_InterfaceStack.top(), "onPopInterface", false);
					m_InterfaceStack.push(cmd.PushInfID);
					InterfaceFunctionCall(m_InterfaceStack.top(), "onPushInterface", true);
				}
				else
				{
					TS_WARN("Interface ID to push was not specified!");
				}
			}

			if (cmd.CommandActionFlags & CommandActionFlag::PopInf)
			{
				if (m_InterfaceStack.size() > 1)
				{
					InterfaceFunctionCall(m_InterfaceStack.top(), "onPopInterface", false);
					m_InterfaceStack.pop();
					InterfaceFunctionCall(m_InterfaceStack.top(), "onPushInterface", true);
				}
				else
				{
					TS_WARN("Interface Stack size is not big enough to pop!");
				}
			}
		}

		Clear();
	}

	void Console::RegisterInterface(const std::string& id)
	{
		Interface inf;
		m_Interfaces[id] = inf;

		LuaScript script("base/interface/" + id + ".lua");
		
		if (script.Execute())
		{
			script.Call("onRegisterInterface", m_Interfaces.at(id));
		}
	}

	void Console::LoadLocale(const std::string& key, const std::string& value)
	{
		if (key._Starts_with("CMD"))
		{
			m_GameData.CommandData[key] = value;
			return;
		}
	}

	void Console::LuaPrintln(const std::string& text)
	{
		Println(text);
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
		LuaScript script("config.lua");
		if (!script.Execute())
		{
			TS_ERROR("Could not load config!");
			return;
		}

		m_GameConfig.CharactersPerLine = script.Var<uint8_t>("CHARACTERS_PER_LINE");
		m_GameConfig.CharactersPerLine = m_GameConfig.CharactersPerLine ? m_GameConfig.CharactersPerLine : 50;
		m_GameConfig.Language = script.Var<std::string>("LANGUAGE");

		TS_INFO("Successfully loaded config!");
	}

	void Console::SaveConfig()
	{
		const std::string content = "CHARACTERS_PER_LINE = " + std::to_string(m_GameConfig.CharactersPerLine) + '\n'
			+ "LANGUAGE = \"" + m_GameConfig.Language + "\"";

		std::ofstream of("config.lua");
		of << content;
		of.close();
	}

	void Console::LoadGameData()
	{
		LuaScript commands("base/commands.lua");
		if (!commands.Execute())
		{
			TS_ERROR("Could not load command data!");
		}

		LuaScript commandLocale("base/locale/" + m_GameConfig.Language + "/commands.lua");
		if (!commandLocale.Execute())
		{
			TS_ERROR("Could not load command locale data!");
		}

		TS_INFO("Successfully loaded game data!");
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

	void Console::InterfaceFunctionCall(const std::string& infID, const std::string& func, const bool globals)
	{

		Interface& inf = m_Interfaces.at(infID);

		LuaScript script("base/interface/" + infID + ".lua");
		
		if (globals)
		{
			script.PushGlobalOnStack("None", CommandActionFlag::None);
			script.PushGlobalOnStack("PushInf", CommandActionFlag::PushInf);
			script.PushGlobalOnStack("PopInf", CommandActionFlag::PopInf);
			script.PushGlobalOnStack("Quit", CommandActionFlag::Quit);
		}

		if (func == "onPushInterface")
		{
			inf.Messages.clear();
			inf.Commands.clear();
		}

		if (script.Execute())
		{
			script.Call(func, inf);
		}
	}

}