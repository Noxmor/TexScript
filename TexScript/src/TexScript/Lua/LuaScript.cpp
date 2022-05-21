#include "tspch.hpp"

#include "LuaScript.hpp"

namespace TexScript {

	LuaScript::LuaScript(const std::string& filepath)
		: m_Filepath(filepath)
	{
		L = luaL_newstate();
		luaL_openlibs(L);

		if (luaL_loadfile(L, filepath.c_str()) || lua_pcall(L, 0, 0, 0))
		{
			TS_ERROR("[LuaScript]: Could not load script ({0})!", filepath);
			L = nullptr;
		}
	}

	LuaScript::~LuaScript()
	{
		if (L) lua_close(L);
	}

	bool LuaScript::Execute()
	{
		if (!L) return false;
		return luaL_dofile(L, m_Filepath.c_str()) == LUA_OK;
	}

	void LuaScript::Clean()
	{
		lua_pop(L, lua_gettop(L));
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

			lua_getfield(L, -1, field.c_str());
			if (lua_isnil(L, -1))
			{
				TS_WARN("[LuaScript]: Var '{0}' (in {1}) is not defined!", field, var);
				lua_pop(L, 2);
				return false;
			}

			lua_remove(L, -2);
			field = "";
		}
		
		return true;
	}

}