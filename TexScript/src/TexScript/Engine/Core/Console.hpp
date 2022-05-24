#pragma once

#include <stack>

#include "TexScript/Engine/Core/GameConfig.hpp"
#include "TexScript/Engine/Core/GameData.hpp"
#include "TexScript/Engine/Core/Command.hpp"
#include "TexScript/Engine/Core/Interface.hpp"

namespace TexScript {

	class Console
	{
	public:
		Console(const std::string& title);
		~Console() = default;

		void Shutdown();

		void OnUpdate();

		void RegisterInterface(const std::string& id);
		void LoadLocale(const std::string& key, const std::string& value);

		void LuaPrintln(const std::string& text);

		void Stop();

	public:
		static Console& Get() { return *s_Instance; }

	private:
		void Clear();
		void SetTitle(const std::string& title);

		void GetInput();
		void GetCommandFromInput();

		void LoadConfig();
		void SaveConfig();

		void LoadGameData();

		void Print(const std::string& text);
		void Println(const std::string& text);
		void PrintInterface(const Interface& inf);

		void InterfaceFunctionCall(const std::string& infID, const std::string& func, const bool globals);

	private:
		std::string m_Input;
		uint8_t m_InputCommand = 0;

		GameConfig m_GameConfig;
		GameData m_GameData;

		std::unordered_map<std::string, Interface> m_Interfaces;
		std::stack<std::string> m_InterfaceStack;

	private:
		static Console* s_Instance;
	};

}