#pragma once

#include "TexScript/Engine/Core/Console.hpp"

namespace TexScript {

	class Application
	{
	public:
		Application(const std::string& title);
		~Application() = default;

		void Run();
		void Stop();
		void Shutdown();

	public:
		static Application& Get() { return *s_Instance; }

	private:
		bool m_Running = false;
		Scope<Console> m_Console;

	private:
		static Application* s_Instance;
	};

}