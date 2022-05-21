#pragma once

#include "TexScript/Lua/LuaScript.hpp"

namespace TexScript {

	class Application
	{
	public:
		Application();
		~Application() = default;

		void Run();
		void Stop();
		void Shutdown();

	public:
		static Application& Get() { return *s_Instance; }

	private:
		bool m_Running = false;
		Ref<LuaScript> m_LuaScript;

	private:
		static Application* s_Instance;
	};

}