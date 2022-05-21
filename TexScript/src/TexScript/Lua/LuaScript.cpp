#include "tspch.hpp"

#include "LuaScript.hpp"

namespace TexScript {

	LuaScript::LuaScript(const std::string& filename)
	{
		L = luaL_newstate();

		if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0))
		{
			TS_ASSERT(false, "[LuaScript]: Could not load script (" + filename + "!)");
			std::cout << "Error: script not loaded (" << filename << ")" << std::endl;
			L = nullptr;
		}
	}

}