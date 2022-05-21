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
			if (m_LuaScript->Execute())
			{
				const size_t random = m_LuaScript->Get<size_t>("random");
				TS_TRACE("Random: {0}", random);
				
				const bool condition = m_LuaScript->Get<bool>("condition");
				TS_TRACE("Condition: {0}", condition);

				const size_t age = m_LuaScript->Get<size_t>("person.age");
				TS_TRACE("Age: {0}", age);

				const float x = m_LuaScript->Get<float>("person.position.x");
				const float y = m_LuaScript->Get<float>("person.position.y");
				TS_TRACE("Position: x: {0}, y: {1}", x, y);

				const std::string name = m_LuaScript->Get<std::string>("person.name");
				TS_TRACE("Name: {0}", name);

				const size_t lower = m_LuaScript->Get<size_t>("LOWER");
				const size_t upper = m_LuaScript->Get<size_t>("UPPER");
				TS_TRACE("Lower: {0}, Upper: {1}", lower, upper);

				TS_WARN("Elements on the stack: {0}", m_LuaScript->GetStackCount());
			}
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