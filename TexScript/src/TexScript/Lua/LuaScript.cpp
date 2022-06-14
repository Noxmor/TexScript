#include "tspch.hpp"

#include "LuaScript.hpp"

#include "TexScript/Engine/Core/Console.hpp"

#include "TexScript/Lua/LuaTable.hpp"

namespace TexScript {

	static LuaTable DecodeLuaTable(lua_State* const L)
	{
		LuaTable table;

		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			if (lua_type(L, -2) != LUA_TSTRING && lua_type(L, -2) != LUA_TNUMBER)
			{
				TS_ERROR("[LuaScript]: Key needs to be either of type string or of type integer!");
				lua_pop(L, 1);
				continue;
			}

			if (lua_type(L, -2) == LUA_TNUMBER && !lua_isinteger(L, -2))
			{
				TS_ERROR("[LuaScript]: Key needs to be an integer, not a number!");
				lua_pop(L, 1);
				continue;
			}

			const std::string key = lua_type(L, -2) == LUA_TSTRING ? lua_tostring(L, -2) : std::to_string(lua_tointeger(L, -2));

			if (lua_type(L, -1) == LUA_TSTRING)
			{
				const std::string value = lua_tostring(L, -1);
				table.Insert(key, value);
			}

			if (lua_type(L, -1) == LUA_TNUMBER && lua_isinteger(L, -1))
			{
				const int32_t value = (int32_t)lua_tointeger(L, -1);
				table.Insert(key, value);
			}

			if (lua_type(L, -1) == LUA_TNUMBER && !lua_isinteger(L, -1))
			{
				const float value = (float)lua_tonumber(L, -1);
				//table.Insert(key, value);
				TS_ERROR("[LuaScript]: Floats are currently unhandled!");
			}

			if (lua_type(L, -1) == LUA_TBOOLEAN)
			{
				const bool value = lua_toboolean(L, -1);
				table.Insert(key, value);
			}

			if (lua_type(L, -1) == LUA_TFUNCTION)
				TS_ERROR("[LuaScript]: Functions are currently unhandled!");

			if (lua_type(L, -1) == LUA_TTABLE)
			{
				LuaTable subTable = DecodeLuaTable(L);
				table.Insert(key, subTable);
			}

			lua_pop(L, 1);
		}

		return table;
	}

	static int LuaAddCommand(lua_State* const L)
	{
		if (lua_gettop(L) != 2)
		{
			TS_ERROR("[LuaScript]: Function (addCommand): Invalid number of arguments ({0}) specified!", lua_gettop(L));
			return -1;
		}
		
		if (lua_type(L, -1) != LUA_TTABLE || lua_type(L, -2) != LUA_TSTRING)
		{
			TS_ERROR("[LuaScript]: Function (addCommand): Attempted to call, but arguments were of type {0} and {1}!", lua_typename(L, lua_type(L, -1)), lua_typename(L, lua_type(L, -2)));
			return -1;
		}

		const std::string infID = lua_tostring(L, -2);
		const LuaTable cmdTable = DecodeLuaTable(L);
		Console::Get().LuaAddCommand(infID, cmdTable);

		return 0;
	}

	static int LuaClearCommands(lua_State* const L)
	{
		if (lua_gettop(L) != 1)
		{
			TS_ERROR("[LuaScript]: Function (clearCommands): Invalid number of arguments ({0}) specified!", lua_gettop(L));
			return -1;
		}

		if (lua_type(L, -1) != LUA_TSTRING)
		{
			TS_ERROR("[LuaScript]: Function (clearCommands): Attempted to call, but argument were of type {0}!", lua_typename(L, lua_type(L, -1)));
			return -1;
		}

		const std::string infID = lua_tostring(L, -1);
		Console::Get().LuaClearCommands(infID);

		return 0;
	}

	static int LuaExtend(lua_State* const L)
	{
		if (lua_gettop(L) != 1)
		{
			TS_ERROR("[LuaScript]: Function (extend): Invalid number of arguments ({0}) specified!", lua_gettop(L));
			return -1;
		}

		if (lua_type(L, -1) != LUA_TTABLE)
		{
			TS_ERROR("[LuaScript]: Function (extend): Expected argument to be of type table!");
			return -1;
		}

		//Iteration over table passed in data.extend()
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			if (lua_type(L, -1) != LUA_TTABLE)
			{
				TS_ERROR("[LuaScript:] Function (extend): Expected argument to be of type table!");
				lua_pop(L, 2);
				return -1;
			}

			const LuaTable root = DecodeLuaTable(L);
			Console::Get().RegisterLuaTable(root);

			lua_pop(L, 1);
		}

		return 0;
	}

	static int LuaPrint(lua_State* const L)
	{
		if (!lua_isstring(L, -1))
		{
			TS_ERROR("[LuaScript]: Attempted to call print, but argument was of type {0}!", lua_typename(L, lua_type(L, -1)));
			return -1;
		}
		
		const std::string text = lua_tostring(L, -1);
		TS_TRACE(text);
		return 0;
	}

	LuaScript::LuaScript(const std::string& filepath)
		: m_Filepath(filepath)
	{
		L = luaL_newstate();
		luaL_openlibs(L);

		lua_register(L, "print", LuaPrint);

		lua_newtable(L);

		if (filepath == "base/data.lua")
		{
			lua_pushstring(L, "extend");
			lua_pushcfunction(L, LuaExtend);
			lua_settable(L, -3);
		}

		if (filepath == "base/control.lua")
		{
			lua_pushstring(L, "addCommand");
			lua_pushcfunction(L, LuaAddCommand);
			lua_settable(L, -3);
			lua_pushstring(L, "clearCommands");
			lua_pushcfunction(L, LuaClearCommands);
			lua_settable(L, -3);
		}

		lua_setglobal(L, "data");

		luaL_dostring(L, "package.path = \"base/?.lua\"");

		if (luaL_dofile(L, filepath.c_str()))
		{
			const std::string error = lua_tostring(L, -1);
			TS_ERROR("Error during lua script execution: {0}", error);
			lua_close(L);
			L = nullptr;
		}
	}

	LuaScript::~LuaScript()
	{
		if (L)
			lua_close(L);
	}

	bool LuaScript::PushVarOnStack(const std::string& var) const
	{
		const bool containsFields = var.find(".") != std::string::npos;
		const std::string global = containsFields ? var.substr(0, var.find(".")) : var;

		lua_getglobal(L, global.c_str());
		if (lua_isnil(L, -1))
		{
			TS_WARN("[LuaScript]: Var '{0}' is not defined!", global);
			Pop(1);
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
				Pop(2);
				return false;
			}

			lua_remove(L, -2);
			field = "";
		}
		
		return true;
	}

	void LuaScript::Pop(const int n) const
	{
		lua_pop(L, n);
	}

	void LuaScript::ClearStack() const
	{
		lua_pop(L, lua_gettop(L));
	}

	int LuaScript::StackSize() const
	{
		return lua_gettop(L);
	}

	void LuaScript::RegisterFunction(const lua_CFunction& func) const
	{
		lua_register(L, nullptr, func);
	}

	void LuaScript::RegisterGlobalFunction(const std::string& name, const lua_CFunction& func) const
	{
		lua_register(L, name.c_str(), func);
	}

	bool LuaScript::CheckInteger(const int index) const
	{
		return lua_isinteger(L, index);
	}

	bool LuaScript::CheckNumber(const int index) const
	{
		return lua_isnumber(L, index);
	}

	bool LuaScript::CheckBool(const int index) const
	{
		return lua_isboolean(L, index);
	}

	bool LuaScript::CheckString(const int index) const
	{
		return lua_isstring(L, index);
	}

	int LuaScript::ToInteger(const int index) const
	{
		return (int)lua_tointeger(L, index);
	}

	float LuaScript::ToNumber(const int index) const
	{
		return (float)lua_tonumber(L, index);
	}

	bool LuaScript::ToBool(const int index) const
	{
		return lua_toboolean(L, index);
	}

	std::string LuaScript::ToString(const int index) const
	{
		return lua_tostring(L, index);
	}

	bool LuaScript::Call(const std::string& luaFuncName, const size_t numArgs, const size_t numResults) const
	{
		lua_getglobal(L, luaFuncName.c_str());

		if (lua_type(L, -1) == LUA_TNIL)
		{
			TS_ERROR("[LuaScript]: Function '{0}' (in {1}) is not defined!", luaFuncName, m_Filepath);
			Pop(1);
			return false;
		}

		if (lua_type(L, -1) != LUA_TFUNCTION)
		{
			TS_ERROR("[LuaScript]: '{0}' (in {1}) is not of type function!", luaFuncName, m_Filepath);
			Pop(1);
			return false;
		}

		for (size_t i = 0; i < numArgs; i++)
		{
			lua_pushvalue(L, -2 - i);
		}

		if (lua_pcall(L, (int)numArgs, (int)numResults, 0))
		{
			const std::string error = lua_tostring(L, -1);
			TS_ERROR("Error during lua script execution: {0}", error);
		}

		return true;
	}

	void LuaScript::CreateTable() const
	{
		lua_newtable(L);
	}

	void LuaScript::PushTableInteger(const std::string& key, const int value, const int index) const
	{
		lua_pushstring(L, key.c_str());
		lua_pushinteger(L, value);
		lua_settable(L, index);
	}

	void LuaScript::PushTableNumber(const std::string& key, const float value, const int index) const
	{
		lua_pushstring(L, key.c_str());
		lua_pushnumber(L, value);
		lua_settable(L, index);
	}

	void LuaScript::PushTableBool(const std::string& key, const bool value, const int index) const
	{
		lua_pushstring(L, key.c_str());
		lua_pushboolean(L, value);
		lua_settable(L, index);
	}

	void LuaScript::PushTableString(const std::string& key, const std::string& value, const int index) const
	{
		lua_pushstring(L, key.c_str());
		lua_pushstring(L, value.c_str());
		lua_settable(L, index);
	}

	void LuaScript::PushTableSubTable(const std::string& key, const int index) const
	{
		lua_pushstring(L, key.c_str());
		lua_newtable(L);
		lua_settable(L, index);
	}

	void LuaScript::GetGlobal(const std::string& name) const
	{
		lua_getglobal(L, name.c_str());
	}

	void LuaScript::SetGlobal(const std::string& name) const
	{
		lua_setglobal(L, name.c_str());
	}

	void LuaScript::PushGlobalInteger(const std::string& name, const int global) const
	{
		lua_pushinteger(L, global);
		lua_setglobal(L, name.c_str());
	}

	void LuaScript::PushGlobalNumber(const std::string& name, const float global) const
	{
		lua_pushnumber(L, global);
		lua_setglobal(L, name.c_str());
	}

	void LuaScript::PushGlobalBool(const std::string& name, const bool global) const
	{
		lua_pushboolean(L, global);
		lua_setglobal(L, name.c_str());
	}

	void LuaScript::PushGlobalString(const std::string& name, const std::string& global) const
	{
		lua_pushstring(L, global.c_str());
		lua_setglobal(L, name.c_str());
	}

	std::vector<std::string> LuaScript::GetStringsFromTable(const std::string& table) const
	{
		std::vector<std::string> strings;
		//GetGlobal(table);
		PushVarOnStack(table);
		const int tableIndex = StackSize();

		if (!lua_istable(L, tableIndex))
		{
			TS_ERROR("[LuaScript]: Specified name does not map to a lua table!");
			Pop(1);
			return strings;
		}

		lua_pushnil(L);
		while (lua_next(L, tableIndex) != 0)
		{
			const std::string key = ToString(-2);
			if (!CheckString(-1))
			{
				TS_WARN("[LuaScript]: Skipped non string element in table!");
				continue;
			}

			strings.emplace_back(ToString(-1));
			Pop(1);
		}

		Pop(2);

		return strings;
	}

}