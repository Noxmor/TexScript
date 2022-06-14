#pragma once

#include "TexScript/Engine/Core/GameConfig.hpp"
#include "TexScript/Engine/Core/GameData.hpp"
#include "TexScript/Engine/Core/Command.hpp"
#include "TexScript/Engine/Core/LocaleHandler.hpp"

#include "TexScript/Engine/Interface/InterfaceHandler.hpp"

#include "TexScript/Engine/Location/LocationHandler.hpp"

#include "TexScript/Engine/Items/ItemStack.hpp"

#include "TexScript/Lua/LuaScript.hpp"
#include "TexScript/Lua/LuaTable.hpp"

namespace TexScript {

	class Console
	{
	public:
		Console(const std::string& title);
		~Console() = default;

		void Shutdown();

		void OnUpdate();

		void RegisterLuaTable(const LuaTable& table);
		void LuaAddCommand(const std::string& infID, const LuaTable& cmdTable);
		void LuaClearCommands(const std::string& infID);

		void Stop();

	public:
		static Console& Get() { return *s_Instance; }

	private:
		void Clear();
		void SetTitle(const std::string& title);

		void GetInput();
		void GetCommandFromInput();

		void Load();
		void LoadConfig();
		void LoadSaveGame();

		void Save();
		void SaveConfig();
		void SaveGame();

		void Print(const std::string& text);
		void Println(const std::string& text);
		void PrintPartingLine();
		void PrintInterface(const Interface& inf);

	private:
		std::string m_Input;
		uint8_t m_InputCommand = 0;
		std::string m_CustomString;

		GameConfig m_GameConfig;
		GameData m_GameData;

		InterfaceHandler m_InterfaceHandler;
		LocationHandler m_LocationHandler;
		LocaleHandler m_LocaleHandler;

		std::vector<ItemStack> m_ItemStacks;

		std::string m_SaveGameName;

		LuaScript m_ControlScript;

	private:
		static Console* s_Instance;
	};

}