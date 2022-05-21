#include "tspch.hpp"

#include "Application.hpp"

namespace TexScript {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		TS_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_LuaScript = CreateRef<LuaScript>("base/test.lua");
	}

	void Application::Run()
	{
		m_Running = true;

		while (m_Running)
		{

		}

		Shutdown();
	}

	void Application::Stop()
	{
		m_Running = false;
	}

	void Application::Shutdown()
	{
	}

}