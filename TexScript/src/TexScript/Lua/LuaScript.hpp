#pragma once

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "TexScript/Engine/Interface/Interface.hpp"

namespace TexScript {

	class LuaScript
	{
	public:
		LuaScript(const std::string& filepath);
		~LuaScript();

		bool PushVarOnStack(const std::string& var) const;

		void Pop(const int n) const;
		void ClearStack() const;
		int StackSize() const;

		void RegisterFunction(const lua_CFunction& func) const;
		void RegisterGlobalFunction(const std::string& name, const lua_CFunction& func) const;

		bool CheckInteger(const int index) const;
		bool CheckNumber(const int index) const;
		bool CheckBool(const int index) const;
		bool CheckString(const int index) const;

		int ToInteger(const int index) const;
		float ToNumber(const int index) const;
		bool ToBool(const int index) const;
		std::string ToString(const int index) const;

		bool Call(const std::string& luaFuncName, const size_t numArgs, const size_t numResults) const;

		void CreateTable() const;

		void PushTableInteger(const std::string& key, const int value, const int index) const;
		void PushTableNumber(const std::string& key, const float value, const int index) const;
		void PushTableBool(const std::string& key, const bool value, const int index) const;
		void PushTableString(const std::string& key, const std::string& value, const int index) const;
		void PushTableSubTable(const std::string& key, const int index) const;

		void GetGlobal(const std::string& name) const;
		void SetGlobal(const std::string& name) const;

		void PushGlobalInteger(const std::string& name, const int global) const;
		void PushGlobalNumber(const std::string& name, const float global) const;
		void PushGlobalBool(const std::string& name, const bool global) const;
		void PushGlobalString(const std::string& name, const std::string& global) const;

		std::vector<std::string> GetStringsFromTable(const std::string& table) const;

		template<typename T>
		T Var(const std::string& var) const
		{
			static_assert(false);
		}

		template<>
		std::string LuaScript::Var<std::string>(const std::string& var) const
		{
			std::string result = "null";
			if (!PushVarOnStack(var)) return result;
			if (CheckString(-1))
			{
				result = ToString(-1);
				Pop(1);
			}

			return result;
		}

		template<>
		int LuaScript::Var<int>(const std::string& var) const
		{
			int result = 0;
			if (!PushVarOnStack(var)) return result;
			if (CheckInteger(-1))
			{
				result = ToInteger(-1);
				Pop(1);
			}

			return result;
		}

		template<>
		size_t LuaScript::Var<size_t>(const std::string& var) const
		{
			size_t result = 0;
			if (!PushVarOnStack(var)) return result;
			if (CheckInteger(-1))
			{
				result = ToInteger(-1);
				Pop(1);
			}

			return result;
		}

		template<>
		uint8_t LuaScript::Var<uint8_t>(const std::string& var) const
		{
			uint8_t result = 0;
			if (!PushVarOnStack(var)) return result;
			if (CheckInteger(-1))
			{
				result = ToInteger(-1);
				Pop(1);
			}

			return result;
		}

		template<>
		uint16_t LuaScript::Var<uint16_t>(const std::string& var) const
		{
			uint16_t result = 0;
			if (!PushVarOnStack(var)) return result;
			if (CheckInteger(-1))
			{
				result = ToInteger(-1);
				Pop(1);
			}

			return result;
		}

		template<>
		uint32_t LuaScript::Var<uint32_t>(const std::string& var) const
		{
			uint32_t result = 0;
			if (!PushVarOnStack(var)) return result;
			if (CheckInteger(-1))
			{
				result = ToInteger(-1);
				Pop(1);
			}

			return result;
		}

		template<>
		float LuaScript::Var<float>(const std::string& var) const
		{
			float result = 0.0f;
			if (!PushVarOnStack(var)) return result;
			if (CheckNumber(-1))
			{
				result = ToNumber(-1);
				Pop(1);
			}

			return result;
		}

		template<>
		bool LuaScript::Var<bool>(const std::string& var) const
		{
			bool result = false;
			if (!PushVarOnStack(var)) return result;
			if (CheckBool(-1))
			{
				result = ToBool(-1);
				Pop(1);
			}

			return result;
		}

		lua_State* const LuaState() const { return L; }

		operator bool() const { return L; }

	private:
		lua_State* L;
		std::string m_Filepath;
	};

}