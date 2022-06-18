#include "tspch.hpp"

#include "Console.hpp"

#include "TexScript/Base/Application.hpp"

#include <fstream>
#include <filesystem>

namespace TexScript {

	Console* Console::s_Instance = nullptr;

	static Command DecodeLuaCommand(const LuaTable& cmdTable)
	{
		Command cmd;

		if (cmdTable.HasStringData("id"))
			cmd.DisplayNameID = cmdTable.StringData("id");

		if (cmdTable.HasStringData("push_id"))
			cmd.PushInfID = cmdTable.StringData("push_id");

		if (cmdTable.HasIntData("loc_id"))
			cmd.LocationID = cmdTable.IntData("loc_id");

		if (cmdTable.HasStringData("language_id"))
			cmd.LanguageID = cmdTable.StringData("language_id");

		if (cmdTable.HasTableData("flags"))
		{
			const LuaTable& flagsTable = cmdTable.TableData("flags");

			for (const auto& [key, flag] : flagsTable.StringData())
				cmd.CommandActionFlags |= StringToCommandActionFlag(flag);
		}

		return cmd;
	}

	static Interface& DecodeLuaInterface(const LuaTable& infTable, InterfaceHandler& infHandler)
	{
		Interface inf;

		const std::string& infID = infTable.StringData("id");

		if (infTable.HasBoolData("main"))
		{
			const bool isMainInf = infTable.BoolData("main");
			if (isMainInf)
				infHandler.Push(infID);
		}

		if (infTable.HasTableData("messages"))
		{
			const LuaTable& messages = infTable.TableData("messages");
			for (auto it = messages.StringData().begin(); it != messages.StringData().end(); ++it)
			{
				const std::string& msg = it->second;
				inf.Messages.emplace_back(msg);
			}
		}

		if (infTable.HasTableData("commands") && !infTable.HasTableData("custom_command"))
		{
			const LuaTable& commands = infTable.TableData("commands");

			for (auto it = commands.TableData().begin(); it != commands.TableData().end(); ++it)
			{
				const Command cmd = DecodeLuaCommand(it->second);
				inf.Commands.emplace_back(cmd);
			}
		}

		if (infTable.HasTableData("custom_command"))
		{
			const LuaTable& customCommand = infTable.TableData("custom_command");
			inf.CustomCommand = DecodeLuaCommand(customCommand);
		}

		infHandler.RegisterInterface(infID, inf);
		return infHandler.GetInterface(infID);
	}

	static Location& DecodeLuaLocation(const LuaTable& locTable, LocationHandler& locHandler)
	{
		Location loc;
		loc.LocationID = locTable.IntData("id");

		if (locTable.HasStringData("region"))
			loc.RegionID = locTable.StringData("region");

		if (locTable.HasBoolData("locked"))
			loc.Locked = locTable.BoolData("locked");

		if (locTable.HasBoolData("visible"))
			loc.Visible = locTable.BoolData("visible");

		const LuaTable& paths = locTable.TableData("paths");
		for (auto it = paths.IntData().begin(); it != paths.IntData().end(); ++it)
			loc.Paths.emplace_back(it->second);

		locHandler.RegisterLocation(loc);
		return locHandler.GetLocation(loc.LocationID);
	}

	Console::Console(const std::string& title)
	{
		TS_ASSERT(!s_Instance, "Console already exists!");
		s_Instance = this;

		SetTitle(title);

		Load();

		LuaScript script;
		script.Load("data/" + m_GameConfig.GameDir + "/data.lua");
		TS_ASSERT(script, "Script is not valid!");

		m_ControlScript.Load("data/" + m_GameConfig.GameDir + "/control.lua");
		TS_ASSERT(m_ControlScript, "Script is not valid!");

		bool hasQuitCmd = false;
		for (const Command& cmd : m_InterfaceHandler.CurrentInterface().Commands)
		{
			if (cmd.CommandActionFlags & CommandActionFlag::Quit)
				hasQuitCmd = true;
		}

		TS_ASSERT(hasQuitCmd, "The main interface needs to have a quit command!");

		//Enables reading special characters e.g. umlaute
		std::locale::global(std::locale(""));

		m_LocaleHandler.Load("data/" + m_GameConfig.GameDir + "/locale/" + m_GameConfig.Language + "/" + m_GameConfig.GameDir + ".cfg");

		lua_State* const L = m_ControlScript.LuaState();
		lua_getglobal(L, "data");
		lua_pushstring(L, "languages");
		lua_newtable(L);
		size_t dirCount = 0;
		for (const auto dir : std::filesystem::directory_iterator("data/" + m_GameConfig.GameDir + "/locale"))
		{
			if (!dir.is_directory())
				continue;

			std::string language = dir.path().filename().u8string();
			for (char& c : language)
				c = std::toupper(c);

			lua_pushinteger(L, ++dirCount);
			lua_pushstring(L, language.c_str());
			lua_settable(L, -3);
		}
		lua_settable(L, -3);
		lua_pushstring(L, "current_loc");
		lua_newtable(L);
		lua_pushstring(L, "id");
		lua_pushinteger(L, m_LocationHandler.CurrentLocationID());
		lua_settable(L, -3);
		lua_pushstring(L, "paths");
		lua_newtable(L);
		for (size_t i = 0; i < m_LocationHandler.CurrentLocation().Paths.size(); i++)
		{
			lua_pushinteger(L, i + 1);
			lua_pushinteger(L, m_LocationHandler.CurrentLocation().Paths.at(i));
			lua_settable(L, -3);
		}
		lua_settable(L, -3);
		lua_settable(L, -3);
	}

	void Console::Shutdown()
	{
		Save();
	}

	void Console::OnUpdate()
	{
		const Interface& inf = m_InterfaceHandler.CurrentInterface();
		PrintInterface(inf);

		GetInput();
		GetCommandFromInput();

		if (m_Input.size() && (m_InputCommand || m_InterfaceHandler.CurrentInterface().CustomCommand.CommandActionFlags))
		{
			const Command& cmd = m_InputCommand ? inf.Commands.at(m_InputCommand - 1) : m_InterfaceHandler.CurrentInterface().CustomCommand;

			if (cmd.CommandActionFlags & CommandActionFlag::Quit)
			{
				Application::Get().Stop();
			}

			if (cmd.CommandActionFlags & CommandActionFlag::PushInf)
			{
				if (m_InterfaceHandler.HasInterfaceID(cmd.PushInfID))
				{
					m_InterfaceHandler.Push(cmd.PushInfID);

					lua_State* L = m_ControlScript.LuaState();
					lua_newtable(L);
					lua_pushstring(L, "inf");
					lua_newtable(L);
					lua_pushstring(L, "id");
					lua_pushstring(L, cmd.PushInfID.c_str());
					lua_settable(L, -3);
					lua_settable(L, -3);
					m_ControlScript.Call("onPushInterface", 1, 0);
				}
				else
					TS_ERROR("[Console]: (PushInf) Invalid interface ID!");
			}

			if (cmd.CommandActionFlags & CommandActionFlag::NewGame)
			{
				m_SaveGameName = m_CustomString;
				SaveGame();
			}

			if (cmd.CommandActionFlags & CommandActionFlag::LoadGame)
			{
				m_SaveGameName = m_CustomString;
				LoadSaveGame();
			}

			if (cmd.CommandActionFlags & CommandActionFlag::Move)
			{
				m_LocationHandler.Travel(cmd.LocationID);

				lua_State* const L = m_ControlScript.LuaState();
				lua_getglobal(L, "data");
				lua_pushstring(L, "current_loc");
				lua_newtable(L);
				lua_pushstring(L, "id");
				lua_pushinteger(L, m_LocationHandler.CurrentLocationID());
				lua_settable(L, -3);
				lua_pushstring(L, "paths");
				lua_newtable(L);
				for (size_t i = 0; i < m_LocationHandler.CurrentLocation().Paths.size(); i++)
				{
					lua_pushinteger(L, i + 1);
					lua_pushinteger(L, m_LocationHandler.CurrentLocation().Paths.at(i));
					lua_settable(L, -3);
				}
				lua_settable(L, -3);
				lua_settable(L, -3);
			}

			if (cmd.CommandActionFlags & CommandActionFlag::Language)
			{
				std::string newLanguage = cmd.LanguageID;
				for (char& c : newLanguage)
					c = std::tolower(c);

				if (m_GameConfig.Language != newLanguage)
				{
					m_GameConfig.Language = newLanguage;
					m_LocaleHandler.Clear();
					m_LocaleHandler.Load("data/" + m_GameConfig.GameDir + "/locale/" + m_GameConfig.Language + "/" + m_GameConfig.GameDir + ".cfg");
				}
			}

			if (cmd.CommandActionFlags & CommandActionFlag::Mod)
				m_GameConfig.GameDir = cmd.DisplayNameID;

			if (cmd.CommandActionFlags & CommandActionFlag::PopInf)
			{
				const std::string poppedInfID = m_InterfaceHandler.CurrentInterfaceID();
				m_InterfaceHandler.Pop();

				lua_State* L = m_ControlScript.LuaState();
				lua_newtable(L);
				lua_pushstring(L, "inf");
				lua_newtable(L);
				lua_pushstring(L, "id");
				lua_pushstring(L, poppedInfID.c_str());
				lua_settable(L, -3);
				lua_settable(L, -3);
				m_ControlScript.Call("onPopInterface", 1, 0);
			}
		}

		Clear();
	}

	void Console::RegisterLuaTable(const LuaTable& table)
	{
		table.Print();

		if (!table.HasStringData("type")) return;

		const std::string& type = table.StringData("type");

		if (type == "interface")
			DecodeLuaInterface(table, m_InterfaceHandler);

		if (type == "location")
			DecodeLuaLocation(table, m_LocationHandler);

		if (type == "item")
		{
			ItemStack item;
			item.ItemStackID = table.StringData("id");
			item.StackSize = table.IntData("stack_size");
			m_ItemStacks.emplace_back(item);
		}
	}

	void Console::LuaAddCommand(const std::string& infID, const LuaTable& cmdTable)
	{
		if (!m_InterfaceHandler.HasInterfaceID(infID))
		{
			TS_ERROR("[Console]: Invalid interface ID ({0}) specified!", infID);
			return;
		}

		const Command cmd = DecodeLuaCommand(cmdTable);
		m_InterfaceHandler.GetInterface(infID).Commands.emplace_back(cmd);
	}

	void Console::LuaClearCommands(const std::string& infID)
	{
		if (!m_InterfaceHandler.HasInterfaceID(infID))
		{
			TS_ERROR("[Console]: Invalid interface ID ({0}) specified!", infID);
			return;
		}

		Interface& inf = m_InterfaceHandler.GetInterface(infID);
		inf.Commands.clear();
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
		if (m_Input.empty())
			return;

		if (m_InterfaceHandler.CurrentInterface().CustomCommand.CommandActionFlags)
		{
			m_CustomString = m_Input;
			m_InputCommand = 0;
			return;
		}

		for (const char c : m_Input)
		{
			if (!std::isdigit(c))
			{
				m_InputCommand = 0;
				return;
			}
		}

		m_InputCommand = std::stoi(m_Input);

		if (m_InputCommand > m_InterfaceHandler.CurrentInterface().Commands.size())
		{
			m_InputCommand = 0;
		}
	}

	void Console::Load()
	{
		LoadConfig();
	}

	void Console::LoadConfig()
	{
		const std::string filepath = "config.cfg";
		std::ifstream file(filepath);

		if (file.fail())
		{
			TS_WARN("[Console]: No config found, using default settings!");
			file.close();
			return;
		}

		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty())
				continue;

			if (line.find('=') == std::string::npos)
			{
				TS_WARN("[Console]: Error while reading line ({0}): Key and value are not seperated by '='!", filepath);
				continue;
			}

			if (line.at(0) == '=')
			{
				TS_WARN("[Console]: Error while reading line ({0}): Line cannot start with '='!", filepath);
				continue;
			}

			const std::string value = line.substr(line.find('=') + 1);
			if (value.empty())
			{
				TS_WARN("[Console]: Error while reading line ({0}): Value was not specified!", filepath);
				continue;
			}

			const std::string key = line.substr(0, line.find('='));

			if (key == "CHARACTERS_PER_LINE")
			{
				bool isInteger = true;
				for (const char c : value)
				{
					if (!std::isdigit(c))
					{
						isInteger = false;
						TS_WARN("[Console]: Config: CHARACTERS_PER_LINE value can not be converted to int!");
						break;
					}
				}

				if(isInteger)
					m_GameConfig.CharactersPerLine = std::stoi(value);
			}

			if (key == "LANGUAGE")
				m_GameConfig.Language = value;

			if (key == "GAME_DIR")
				m_GameConfig.GameDir = value;
		}

		file.close();
	}

	void Console::LoadSaveGame()
	{
		//TODO: Load save game
	}

	void Console::Save()
	{
		SaveConfig();
		SaveGame();
	}

	void Console::SaveConfig()
	{
		std::ofstream of("config.cfg");

		of << "CHARACTERS_PER_LINE=" << std::to_string(m_GameConfig.CharactersPerLine) << '\n';
		of << "LANGUAGE=" << m_GameConfig.Language << '\n';
		of << "GAME_DIR=" << m_GameConfig.GameDir;

		of.close();
	}

	void Console::SaveGame()
	{
		if (m_SaveGameName.empty())
			return;

		std::ofstream file("saves/" + m_SaveGameName + ".save");

		if (file.fail())
		{
			TS_ERROR("[Console]: Failed to serialize save game: {0}", m_SaveGameName);
			return;
		}

		file << "CURRENT_LOC=" << std::to_string(m_LocationHandler.CurrentLocationID()) << '\n';

		for (const Location& loc : m_LocationHandler.GetLocations())
		{
			file << "[LocationData]" << '\n';
			file << "ID=" << loc.LocationID << '\n';
			file << "LOCKED=" << loc.Locked << '\n';
			file << "VISIBLE=" << loc.Visible << '\n';
			file << "REG_ID=" << loc.RegionID << '\n';

			file << "[PathData]" << '\n';
			for (const size_t path : loc.Paths)
			{
				file << "PATH=" << path << '\n';
			}
		}

		file.close();
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

			if (charactersOnLine + word.size() + 1 <= m_GameConfig.CharactersPerLine)
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

	void Console::PrintPartingLine()
	{
		std::string partingLine;
		for (size_t i = 0; i < m_GameConfig.CharactersPerLine; i++)
		{
			partingLine += '-';
		}

		Println(partingLine);
	}

	void Console::PrintInterface(const Interface& inf)
	{
		const auto& commands = inf.Commands;

		PrintPartingLine();

		for (const std::string& msg : inf.Messages)
		{
			std::string locale = m_LocaleHandler.Locale(msg);
			size_t start = locale.find('[');
			size_t end = locale.find(']');

			while (start != std::string::npos && end != std::string::npos)
			{
				const std::string expression = locale.substr(start + 1, end - start - 1);

				if (expression == "Loc.GetName")
				{
					const std::string locationLocale = "LOC_" + std::to_string(m_LocationHandler.CurrentLocationID());
					locale.replace(start, end + 1, m_LocaleHandler.Locale(locationLocale));
				}

				if (expression == "Loc.GetRegionName")
				{
					locale.replace(start, end + 1, m_LocaleHandler.Locale(m_LocationHandler.CurrentLocation().RegionID));
				}

				start = locale.find('[');
				end = locale.find(']');
			}
			
			Println(locale);
		}

		PrintPartingLine();

		for (size_t i = 0; i < commands.size(); i++)
		{
			const Command& cmd = commands.at(i);
			Println("[" + std::to_string(i + 1) + "] " + m_LocaleHandler.Locale(cmd.DisplayNameID));
		}

		PrintPartingLine();
		Print("-> ");
	}

}