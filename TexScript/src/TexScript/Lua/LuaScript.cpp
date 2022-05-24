#include "tspch.hpp"

#include "LuaScript.hpp"

#include "TexScript/Engine/Core/Console.hpp"

namespace TexScript {

	static int LuaAddMessage(lua_State* L)
	{
		if (lua_gettop(L) != 2 || !lua_isuserdata(L, 1) || !lua_isstring(L, 2)) return -1;

		Interface* const inf = static_cast<Interface* const>(lua_touserdata(L, 1));
		const std::string msg = (std::string)lua_tostring(L, 2);

		inf->Messages.emplace_back(msg);

		return 0;
	}

	static int LuaAddCommand(lua_State* L)
	{
		if (lua_gettop(L) < 3 || !lua_isuserdata(L, 1) || !lua_isstring(L, 2) || !lua_isinteger(L, 3)) return -1;

		Interface* const inf = static_cast<Interface* const>(lua_touserdata(L, 1));
		const std::string displayNameID = (std::string)lua_tostring(L, 2);
		const int flags = (int)lua_tointeger(L, 3);

		Command cmd(displayNameID, flags);

		switch (flags)
		{
			case CommandActionFlag::None: break;
			case CommandActionFlag::PushInf:
			{
				if (lua_gettop(L) != 4 || !lua_isstring(L, 4)) return -1;
				cmd.PushInfID = (std::string)lua_tostring(L, 4);
				break;
			}

			case CommandActionFlag::PopInf: break;
			case CommandActionFlag::Quit: break;
		}

		inf->Commands.emplace_back(cmd);

		return 0;
	}

	static int LuaLoadLocale(lua_State* L)
	{
		if (lua_gettop(L) != 2 || !lua_isstring(L, 1) || !lua_isstring(L, 2)) return -1;

		const std::string key = (std::string)lua_tostring(L, 1);
		const std::string value = (std::string)lua_tostring(L, 2);
		Console::Get().LoadLocale(key, value);

		return 0;
	}

	static int LuaPrintln(lua_State* L)
	{
		if (lua_gettop(L) != 1 || !lua_isstring(L, 1)) return -1;

		const std::string  text = (std::string)lua_tostring(L, 1);
		Console::Get().LuaPrintln(text);

		return 0;
	}

	static int LuaStop(lua_State* L)
	{
		if (lua_gettop(L) != 0) return -1;
		Console::Get().Stop();
		return 0;
	}

	LuaScript::LuaScript(const std::string& filepath)
		: m_Filepath(filepath)
	{
		L = luaL_newstate();
		luaL_openlibs(L);

		lua_register(L, "ts_message", LuaAddMessage);
		lua_register(L, "ts_command", LuaAddCommand);
		lua_register(L, "ts_locale", LuaLoadLocale);
		lua_register(L, "ts_println", LuaPrintln);
		lua_register(L, "ts_stop", LuaStop);

		if (luaL_loadfile(L, filepath.c_str()))
		{
			TS_ERROR("[LuaScript]: Could not load script ({0})!", filepath);
			lua_close(L);
			L = nullptr;
		}
	}

	LuaScript::~LuaScript()
	{
		if (L) lua_close(L);
	}

	bool LuaScript::Execute()
	{
		if (!L)
		{
			TS_WARN("[LuaScript]: Tried to execute invalid script ({0})!", m_Filepath);
			return false;
		}

		return luaL_dofile(L, m_Filepath.c_str()) == LUA_OK;
	}

	bool LuaScript::PushVarOnStack(const std::string& var)
	{
		const bool containsFields = var.find(".") != std::string::npos;
		const std::string global = containsFields ? var.substr(0, var.find(".")) : var;

		lua_getglobal(L, global.c_str());
		if (lua_isnil(L, -1))
		{
			TS_WARN("[LuaScript]: Var '{0}' is not defined!", global);
			lua_pop(L, 1);
			return false;
		}

		if (!containsFields) return true;

		const std::string fields = var.substr(var.find(".") + 1) + ".";
		std::string field = "";

		for (const char c : fields)
		{
			if (c != '.')
			{
				field += c;
				continue;
			}

			if (field.empty())
			{
				continue;
			}

			lua_getfield(L, -1, field.c_str());
			if (lua_isnil(L, -1))
			{
				TS_WARN("[LuaScript]: Field '{0}' (in {1}) is not defined!", field, var);
				lua_pop(L, 2);
				return false;
			}

			lua_remove(L, -2);
			field = "";
		}
		
		return true;
	}

	void LuaScript::ClearStack()
	{
		lua_pop(L, lua_gettop(L));
	}

	bool LuaScript::Call(const std::string& luaFuncName, Interface& inf)
	{
		if (!L)
		{
			TS_WARN("[LuaScript]: Tried to execute invalid script ({0})!", m_Filepath);
			return false;
		}

		lua_getglobal(L, luaFuncName.c_str());
		if (lua_isnil(L, -1))
		{
			TS_WARN("[LuaScript]: Function '{0}' (in {1}) is not defined!", luaFuncName, m_Filepath);
			lua_pop(L, 1);
			return false;
		}

		if (!lua_isfunction(L, -1))
		{
			TS_WARN("[LuaScript]: '{0}' (in {1}) is not of type function!", luaFuncName, m_Filepath);
			lua_pop(L, 1);
			return false;
		}

		lua_pushlightuserdata(L, &inf);
		lua_pcall(L, 1, 0, 0);

		if (lua_isnil(L, -1))
		{
			TS_WARN("[LuaScript]: Error while calling function '{0}' (in {1})!", luaFuncName, m_Filepath);
			lua_pop(L, 1);
			return false;
		}

		return true;
	}

}