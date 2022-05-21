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
		LuaScript(const std::string& filename);
		~LuaScript() = default;

	private:
		lua_State* L;
	};

}