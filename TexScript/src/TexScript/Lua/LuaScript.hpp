#pragma once

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "TexScript/Engine/Core/Interface.hpp"

namespace TexScript {

	class LuaScript
	{
	public:
		LuaScript(const std::string& filepath);
		~LuaScript();

		bool Execute();
		bool PushVarOnStack(const std::string& var);

		void ClearStack();
		size_t StackSize() const { return lua_gettop(L); }

		bool Call(const std::string& luaFuncName, Interface& inf);

		template<typename T>
		void PushGlobalOnStack(const std::string& name, const T& global)
		{
			static_assert(false);
		}

		template<>
		void PushGlobalOnStack(const std::string& name, const CommandActionFlag& global)
		{
			lua_pushinteger(L, global);
			lua_setglobal(L, name.c_str());
		}

		template<typename T>
		T Var(const std::string& var)
		{
			static_assert(false);
		}

		template<>
		std::string LuaScript::Var<std::string>(const std::string& var)
		{
			std::string result = "null";
			if (!PushVarOnStack(var)) return result;
			if (lua_isstring(L, -1))
			{
				result = (std::string)lua_tostring(L, -1);
				lua_pop(L, 1);
			}

			return result;
		}

		template<>
		int LuaScript::Var<int>(const std::string& var)
		{
			int result = 0;
			if (!PushVarOnStack(var)) return result;
			if (lua_isinteger(L, -1))
			{
				result = (int)lua_tointeger(L, -1);
				lua_pop(L, 1);
			}

			return result;
		}

		template<>
		size_t LuaScript::Var<size_t>(const std::string& var)
		{
			size_t result = 0;
			if (!PushVarOnStack(var)) return result;
			if (lua_isinteger(L, -1))
			{
				result = (size_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
			}

			return result;
		}

		template<>
		uint8_t LuaScript::Var<uint8_t>(const std::string& var)
		{
			uint8_t result = 0;
			if (!PushVarOnStack(var)) return result;
			if (lua_isinteger(L, -1))
			{
				result = (uint8_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
			}

			return result;
		}

		template<>
		uint16_t LuaScript::Var<uint16_t>(const std::string& var)
		{
			uint16_t result = 0;
			if (!PushVarOnStack(var)) return result;
			if (lua_isinteger(L, -1))
			{
				result = (uint16_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
			}

			return result;
		}

		template<>
		uint32_t LuaScript::Var<uint32_t>(const std::string& var)
		{
			uint32_t result = 0;
			if (!PushVarOnStack(var)) return result;
			if (lua_isinteger(L, -1))
			{
				result = (uint32_t)lua_tointeger(L, -1);
				lua_pop(L, 1);
			}

			return result;
		}

		template<>
		float LuaScript::Var<float>(const std::string& var)
		{
			float result = 0.0f;
			if (!PushVarOnStack(var)) return result;
			if (lua_isnumber(L, -1))
			{
				result = (float)lua_tonumber(L, -1);
				lua_pop(L, 1);
			}

			return result;
		}

		template<>
		bool LuaScript::Var<bool>(const std::string& var)
		{
			bool result = false;
			if (!PushVarOnStack(var)) return result;
			if (lua_isboolean(L, -1))
			{
				result = (bool)lua_toboolean(L, -1);
				lua_pop(L, 1);
			}

			return result;
		}

	private:
		lua_State* L;
		std::string m_Filepath;
	};

}