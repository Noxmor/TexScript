#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace TexScript {

	class LuaScript
	{
	public:
		LuaScript(const std::string& filepath);
		~LuaScript();

		bool Execute();

		void Clean();

		size_t GetStackCount() const { return lua_gettop(L); }

		bool PushVarOnStack(const std::string& var);

		template<typename T>
		T Get(const std::string& var)
		{
			static_assert(false);
		}

		template<>
		std::string LuaScript::Get<std::string>(const std::string& var)
		{
			std::string result;
			if (!PushVarOnStack(var)) return result;
			if (lua_isstring(L, -1))
			{
				result = (std::string)lua_tostring(L, -1);
				lua_pop(L, 1);
			}

			return result;
		}

		template<>
		size_t LuaScript::Get<size_t>(const std::string& var)
		{
			size_t result = 0;
			if (!PushVarOnStack(var)) return result;
			if (lua_isnumber(L, -1))
			{
				result = (size_t)lua_tonumber(L, -1);
				lua_pop(L, 1);
			}

			return result;
		}

		template<>
		int LuaScript::Get<int>(const std::string& var)
		{
			int result = 0;
			if (!PushVarOnStack(var)) return result;
			if (lua_isnumber(L, -1))
			{
				result = (int)lua_tonumber(L, -1);
				lua_pop(L, 1);
			}

			return result;
		}

		template<>
		float LuaScript::Get<float>(const std::string& var)
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
		bool LuaScript::Get<bool>(const std::string& var)
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