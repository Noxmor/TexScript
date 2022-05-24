#include "tspch.hpp"

#include "Application.hpp"

namespace TexScript {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& title)
	{
		TS_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Console = CreateScope<Console>(title);
	}

	void Application::Run()
	{
		m_Running = true;

		while (m_Running)
		{
			m_Console->OnUpdate();
		}

		Shutdown();
	}

	void Application::Stop()
	{
		m_Running = false;
	}

	void Application::Shutdown()
	{
		m_Console->Shutdown();
	}

}